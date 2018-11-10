/*
 * varalloc.c -- variable allocation
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "symbol.h"
#include "types.h"
#include "absyn.h"
#include "table.h"
#include "varalloc.h"


int intByteSize(void) {
  return 4;
}


int boolByteSize(void) {
  return 4;
}


int refByteSize(void) {
  return 4;
}


/**************************************************************/


static void varallocArgs(Entry *procEntry) {
  ParamTypes *paramTypes;
  int offset;
  int byteSize;

  /* just to be sure */
  if (procEntry == NULL || procEntry->kind != ENTRY_KIND_PROC) {
    error("procedure declaration vanished from symbol table");
  }
  /* store access information for arguments */
  paramTypes = procEntry->u.procEntry.paramTypes;
  offset = 0;
  while (paramTypes != NULL) {
    paramTypes->offset = offset;
    if (paramTypes->isRef) {
      byteSize = refByteSize();
    } else {
      byteSize = paramTypes->type->byteSize;
    }
    offset += byteSize;
    paramTypes = paramTypes->next;
  }
  /* store size of argument area */
  procEntry->u.procEntry.argumentArea = offset;
}


static void varallocProcdecl1(Absyn *node, Table *symTable) {
  Entry *procEntry;
  Table *localTable;
  ParamTypes *paramTypes;
  Absyn *params;
  Absyn *param;
  Entry *paramEntry;
  Absyn *decls;
  Absyn *decl;
  Entry *varEntry;
  int offset;
  int byteSize;

  /* store access information for arguments */
  procEntry = lookup(symTable, node->u.procdeclNode.name);
  varallocArgs(procEntry);
  /* store access information for parameters */
  localTable = procEntry->u.procEntry.localTable;
  paramTypes = procEntry->u.procEntry.paramTypes;
  params = node->u.procdeclNode.params;
  while (params != NULL) {
    param = params->u.listNode.head;
    params = params->u.listNode.tail;
    paramEntry = lookup(localTable, param->u.paramNode.name);
    if (paramEntry == NULL || paramEntry->kind != ENTRY_KIND_VAR) {
      error("parameter vanished from symbol table");
    }
    paramEntry->u.varEntry.offset = paramTypes->offset;
    paramTypes = paramTypes->next;
  }
  /* store access information for local variables */
  decls = node->u.procdeclNode.decls;
  offset = 0;
  while (decls != NULL) {
    decl = decls->u.listNode.head;
    decls = decls->u.listNode.tail;
    if (decl->type == ABSYN_VARDECL) {
      varEntry = lookup(localTable, decl->u.vardeclNode.name);
      if (varEntry == NULL || varEntry->kind != ENTRY_KIND_VAR) {
        error("variable vanished from symbol table");
      }
      if (varEntry->u.varEntry.isRef) {
        byteSize = refByteSize();
      } else {
        byteSize = varEntry->u.varEntry.type->byteSize;
      }
      offset -= byteSize;
      varEntry->u.varEntry.offset = offset;
    }
  }
  /* store size of localvar area */
  procEntry->u.procEntry.localvarArea = -offset;
}


/**************************************************************/


static int argsize(Absyn *node, Table *symTable, int currMax);


static int argsizeList(Absyn *node, Table *symTable, int currMax) {
  while (node != NULL) {
    currMax = argsize(node->u.listNode.head, symTable, currMax);
    node = node->u.listNode.tail;
  }
  return currMax;
}


static int argsizeAssign(Absyn *node, Table *symTable, int currMax) {
  currMax = argsize(node->u.assignNode.var, symTable, currMax);
  currMax = argsize(node->u.assignNode.expr, symTable, currMax);
  return currMax;
}


static int argsizeIf(Absyn *node, Table *symTable, int currMax) {
  currMax = argsize(node->u.ifNode.test, symTable, currMax);
  currMax = argsize(node->u.ifNode.thenPart, symTable, currMax);
  currMax = argsize(node->u.ifNode.elsePart, symTable, currMax);
  return currMax;
}


static int argsizeWhile(Absyn *node, Table *symTable, int currMax) {
  currMax = argsize(node->u.whileNode.test, symTable, currMax);
  currMax = argsize(node->u.whileNode.body, symTable, currMax);
  return currMax;
}


static int argsizeCall(Absyn *node, Table *symTable, int currMax) {
  Entry *procEntry;
  int n;

  procEntry = lookup(symTable, node->u.callNode.name);
  if (procEntry == NULL || procEntry->kind != ENTRY_KIND_PROC) {
    error("procedure declaration vanished from symbol table");
  }
  n = procEntry->u.procEntry.argumentArea;
  if (n > currMax) {
    currMax = n;
  }
  currMax = argsize(node->u.callNode.args, symTable, currMax);
  return currMax;
}


static int argsizeOp(Absyn *node, Table *symTable, int currMax) {
  currMax = argsize(node->u.opNode.left, symTable, currMax);
  currMax = argsize(node->u.opNode.right, symTable, currMax);
  return currMax;
}


static int argsizeVar(Absyn *node, Table *symTable, int currMax) {
  currMax = argsize(node->u.varNode.var, symTable, currMax);
  return currMax;
}


static int argsizeInt(Absyn *node, Table *symTable, int currMax) {
  return currMax;
}


static int argsizeSimplevar(Absyn *node, Table *symTable, int currMax) {
  return currMax;
}


static int argsizeArrayvar(Absyn *node, Table *symTable, int currMax) {
  currMax = argsize(node->u.arrayvarNode.var, symTable, currMax);
  currMax = argsize(node->u.arrayvarNode.index, symTable, currMax);
  return currMax;
}


static int argsize(Absyn *node, Table *symTable, int currMax) {
  if (node == NULL) {
    /* this can only happen with an empty list */
    return argsizeList(node, symTable, currMax);
  } else
  switch (node->type) {
    case ABSYN_LIST:
      return argsizeList(node, symTable, currMax);
    case ABSYN_ASSIGN:
      return argsizeAssign(node, symTable, currMax);
    case ABSYN_IF:
      return argsizeIf(node, symTable, currMax);
    case ABSYN_WHILE:
      return argsizeWhile(node, symTable, currMax);
    case ABSYN_CALL:
      return argsizeCall(node, symTable, currMax);
    case ABSYN_OP:
      return argsizeOp(node, symTable, currMax);
    case ABSYN_VAR:
      return argsizeVar(node, symTable, currMax);
    case ABSYN_INT:
      return argsizeInt(node, symTable, currMax);
    case ABSYN_SIMPLEVAR:
      return argsizeSimplevar(node, symTable, currMax);
    case ABSYN_ARRAYVAR:
      return argsizeArrayvar(node, symTable, currMax);
    default:
      error("unknown node type %d in argsize", node->type);
      /* not reached */
      return 0;
  }
}


static void varallocProcdecl2(Absyn *node, Table *symTable) {
  Entry *procEntry;
  Table *localTable;
  int outgoingArea;

  /* compute outgoing area */
  procEntry = lookup(symTable, node->u.procdeclNode.name);
  if (procEntry == NULL || procEntry->kind != ENTRY_KIND_PROC) {
    error("procedure declaration vanished from symbol table");
  }
  localTable = procEntry->u.procEntry.localTable;
  outgoingArea = argsize(node->u.procdeclNode.body, localTable, -1);
  procEntry->u.procEntry.outgoingArea = outgoingArea;
}


/**************************************************************/


static void varallocProcdecl3(Absyn *node, Table *symTable) {
  Entry *procEntry;
  ParamTypes *paramTypes;
  int argNum;
  Table *localTable;
  Absyn *params;
  Absyn *param;
  Entry *paramEntry;
  Absyn *decls;
  Absyn *decl;
  Entry *varEntry;

  /* show results of variable allocation phase */
  procEntry = lookup(symTable, node->u.procdeclNode.name);
  if (procEntry == NULL || procEntry->kind != ENTRY_KIND_PROC) {
    error("procedure declaration vanished from symbol table");
  }
  paramTypes = procEntry->u.procEntry.paramTypes;
  printf("\nVariable allocation for procedure '%s'\n",
         symbolString(node->u.procdeclNode.name));
  paramTypes = procEntry->u.procEntry.paramTypes;
  argNum = 1;
  while (paramTypes != NULL) {
    printf("arg %d: sp + %d\n", argNum, paramTypes->offset);
    paramTypes = paramTypes->next;
    argNum++;
  }
  printf("size of argument area = %d\n",
         procEntry->u.procEntry.argumentArea);
  localTable = procEntry->u.procEntry.localTable;
  params = node->u.procdeclNode.params;
  while (params != NULL) {
    param = params->u.listNode.head;
    params = params->u.listNode.tail;
    paramEntry = lookup(localTable, param->u.paramNode.name);
    if (paramEntry == NULL || paramEntry->kind != ENTRY_KIND_VAR) {
      error("parameter vanished from symbol table");
    }
    printf("param '%s': fp + %d\n",
           symbolString(param->u.paramNode.name),
           paramEntry->u.varEntry.offset);
  }
  decls = node->u.procdeclNode.decls;
  while (decls != NULL) {
    decl = decls->u.listNode.head;
    decls = decls->u.listNode.tail;
    if (decl->type == ABSYN_VARDECL) {
      varEntry = lookup(localTable, decl->u.vardeclNode.name);
      if (varEntry == NULL || varEntry->kind != ENTRY_KIND_VAR) {
        error("variable vanished from symbol table");
      }
      printf("var '%s': fp - %d\n",
             symbolString(decl->u.vardeclNode.name),
             -varEntry->u.varEntry.offset);
    }
  }
  printf("size of localvar area = %d\n",
         procEntry->u.procEntry.localvarArea);
  printf("size of outgoing area = %d\n",
         procEntry->u.procEntry.outgoingArea);
}


/**************************************************************/


void allocVars(Absyn *program, Table *globalTable, boolean showVarAlloc) {
  Absyn *nodes;
  Absyn *node;

  /* compute access information for arguments of predefined procs */
  varallocArgs(lookup(globalTable, symbol("printi")));
  varallocArgs(lookup(globalTable, symbol("printc")));
  varallocArgs(lookup(globalTable, symbol("readi")));
  varallocArgs(lookup(globalTable, symbol("readc")));
  varallocArgs(lookup(globalTable, symbol("exit")));
  varallocArgs(lookup(globalTable, symbol("time")));
  varallocArgs(lookup(globalTable, symbol("clearAll")));
  varallocArgs(lookup(globalTable, symbol("setPixel")));
  varallocArgs(lookup(globalTable, symbol("drawLine")));
  varallocArgs(lookup(globalTable, symbol("drawCircle")));
  /* compute access information for arguments, parameters and local vars */
  nodes = program;
  while (nodes != NULL) {
    if (nodes->type != ABSYN_LIST) {
      error("illegal prog tree in allocVars");
    }
    node = nodes->u.listNode.head;
    nodes = nodes->u.listNode.tail;
    if (node->type == ABSYN_PROCDECL) {
      varallocProcdecl1(node, globalTable);
    }
  }
  /* compute outgoing area sizes */
  nodes = program;
  while (nodes != NULL) {
    if (nodes->type != ABSYN_LIST) {
      error("illegal prog tree in allocVars");
    }
    node = nodes->u.listNode.head;
    nodes = nodes->u.listNode.tail;
    if (node->type == ABSYN_PROCDECL) {
      varallocProcdecl2(node, globalTable);
    }
  }
  /* show variable allocation if requested */
  if (showVarAlloc) {
    nodes = program;
    while (nodes != NULL) {
      if (nodes->type != ABSYN_LIST) {
        error("illegal prog tree in allocVars");
      }
      node = nodes->u.listNode.head;
      nodes = nodes->u.listNode.tail;
      if (node->type == ABSYN_PROCDECL) {
        varallocProcdecl3(node, globalTable);
      }
    }
  }
}
