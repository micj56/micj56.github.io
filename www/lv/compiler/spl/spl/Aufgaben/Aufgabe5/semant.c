/*
 * semant.c -- semantic analysis
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
#include "semant.h"


static boolean showLocalTables;

static Type *intType;
static Type *boolType;


static Type *checkNode(Absyn *node, Table *symTable, int pass);


static Type *checkList(Absyn *node, Table *symTable, int pass) {
  while (node != NULL) {
    checkNode(node->u.listNode.head, symTable, pass);
    node = node->u.listNode.tail;
  }
  return NULL;
}


static Type *checkNametype(Absyn *node, Table *symTable, int pass) {
  Entry *entry;
  Type *type;

  entry = lookup(symTable, node->u.nametypeNode.name);
  if (entry == NULL) {
    error("undefined type '%s' in line %d",
          symbolString(node->u.nametypeNode.name), node->line);
  }
  if (entry->kind != ENTRY_KIND_TYPE) {
    error("'%s' is not a type in line %d",
          symbolString(node->u.nametypeNode.name), node->line);
  }
  type = entry->u.typeEntry.type;
  return type;
}


static Type *checkArraytype(Absyn *node, Table *symTable, int pass) {
  Type *baseType;
  Type *type;

  baseType = ...
  type = ...
  return type;
}


static Type *checkTypedecl(Absyn *node, Table *symTable, int pass) {
  Type *type;
  Entry *entry;

  if (pass != 1) {
    return NULL;
  }
  type = checkNode(node->u.typedeclNode.type, symTable, pass);
  entry = newTypeEntry(type);
  if (enter(symTable, node->u.typedeclNode.name, entry) == NULL) {
    error("redeclaration of '%s' in line %d",
          symbolString(node->u.typedeclNode.name), node->line);
  }
  return NULL;
}


static Type *checkVardecl(Absyn *node, Table *symTable, int pass) {
  ...
  return NULL;
}


static ParamTypes *checkParams(Absyn *node, Table *symTable, int pass) {
  ...
}


static Type *checkProcdecl(Absyn *node, Table *symTable, int pass) {
  Entry *procEntry;
  Table *localTable;
  Absyn *params;
  Absyn *param;
  ParamTypes *paramTypes;
  Type *paramType;
  boolean paramIsRef;
  Entry *paramEntry;

  if (pass == 1) {
    /* build and enter proc declaration into symbol table */
    ...
  } else {
    /* get the proc declaration entered in pass 1 again */
    /* enter parameters into local symbol table */
    /* enter local declarations into local symbol table */
    /* do semantic check on procedure body */
    /* show symbol table if requested */
  }
}


static Type *checkParam(Absyn *node, Table *symTable, int pass) {
  Type *type;

  type = checkNode(node->u.paramNode.type, symTable, pass);
  return type;
}


static Type *checkAssign(Absyn *node, Table *symTable, int pass) {
...
}


static Type *checkIf(Absyn *node, Table *symTable, int pass) {
...
}


static Type *checkWhile(Absyn *node, Table *symTable, int pass) {
...
}


static Type *checkCall(Absyn *node, Table *symTable, int pass) {
...
}


static Type *checkOp(Absyn *node, Table *symTable, int pass) {
  Type *leftType;
  Type *rightType;
  Type *type;

  leftType = checkNode(node->u.opNode.left, symTable, pass);
  rightType = checkNode(node->u.opNode.right, symTable, pass);
  if (leftType != rightType) {
    error("expression combines different types in line %d",
          node->line);
  }
  switch (node->u.opNode.op) {
    case ABSYN_OP_EQ:
    case ABSYN_OP_NE:
    case ABSYN_OP_LT:
    case ABSYN_OP_LE:
    case ABSYN_OP_GT:
    case ABSYN_OP_GE:
      if (leftType != intType) {
        error("comparison requires integer operands in line %d",
              node->line);
      }
      type = boolType;
      break;
    case ABSYN_OP_ADD:
    case ABSYN_OP_SUB:
    case ABSYN_OP_MUL:
    case ABSYN_OP_DIV:
      if (leftType != intType) {
        error("arithmetic operation requires integer operands in line %d",
              node->line);
      }
      type = intType;
      break;
    default:
      error("unknown op %d in checkOp", node->u.opNode.op);
  }
  node->dataType = type;
  return type;
}


static Type *checkVar(Absyn *node, Table *symTable, int pass) {
  Type *type;

  type = checkNode(node->u.varNode.var, symTable, pass);
  node->dataType = type;
  return type;
}


static Type *checkInt(Absyn *node, Table *symTable, int pass) {
  Type *type;

  type = intType;
  node->dataType = type;
  return type;
}


static Type *checkSimplevar(Absyn *node, Table *symTable, int pass) {
  Entry *entry;
  Type *type;

  entry = lookup(symTable, node->u.simplevarNode.name);
  if (entry == NULL) {
    error("undefined variable '%s' in line %d",
          symbolString(node->u.simplevarNode.name), node->line);
  }
  if (entry->kind != ENTRY_KIND_VAR) {
    error("'%s' is not a variable in line %d",
          symbolString(node->u.simplevarNode.name), node->line);
  }
  type = entry->u.varEntry.type;
  node->dataType = type;
  return type;
}


static Type *checkArrayvar(Absyn *node, Table *symTable, int pass) {
...
}


static Type *checkNode(Absyn *node, Table *symTable, int pass) {
  if (node == NULL) {
    /* this can only happen with an empty list */
    return checkList(node, symTable, pass);
  } else
  switch (node->type) {
    case ABSYN_LIST:
      return checkList(node, symTable, pass);
    case ABSYN_NAMETYPE:
      return checkNametype(node, symTable, pass);
    case ABSYN_ARRAYTYPE:
      return checkArraytype(node, symTable, pass);
    case ABSYN_TYPEDECL:
      return checkTypedecl(node, symTable, pass);
    case ABSYN_VARDECL:
      return checkVardecl(node, symTable, pass);
    case ABSYN_PROCDECL:
      return checkProcdecl(node, symTable, pass);
    case ABSYN_PARAM:
      return checkParam(node, symTable, pass);
    case ABSYN_ASSIGN:
      return checkAssign(node, symTable, pass);
    case ABSYN_IF:
      return checkIf(node, symTable, pass);
    case ABSYN_WHILE:
      return checkWhile(node, symTable, pass);
    case ABSYN_CALL:
      return checkCall(node, symTable, pass);
    case ABSYN_OP:
      return checkOp(node, symTable, pass);
    case ABSYN_VAR:
      return checkVar(node, symTable, pass);
    case ABSYN_INT:
      return checkInt(node, symTable, pass);
    case ABSYN_SIMPLEVAR:
      return checkSimplevar(node, symTable, pass);
    case ABSYN_ARRAYVAR:
      return checkArrayvar(node, symTable, pass);
    default:
      error("unknown node type %d in checkNode", node->type);
      /* not reached */
      return NULL;
  }
}


static void enterPredefinedTypes(Table *table) {
  enter(table,
        symbol("int"),
        newTypeEntry(intType));
}


static void enterPredefinedProcs(Table *table) {
  Entry *procEntry;

  /* printi(i: int) */
  procEntry = newProcEntry(newParamTypes(intType, FALSE, NULL),
                           table);
  enter(table, symbol("printi"), procEntry);

  /* printc(i: int) */
  procEntry = newProcEntry(newParamTypes(intType, FALSE, NULL),
                           table);
  enter(table, symbol("printc"), procEntry);

  /* readi(ref i: int) */
  procEntry = newProcEntry(newParamTypes(intType, TRUE, NULL),
                           table);
  enter(table, symbol("readi"), procEntry);

  /* readc(ref i: int) */
  procEntry = newProcEntry(newParamTypes(intType, TRUE, NULL),
                           table);
  enter(table, symbol("readc"), procEntry);

  /* exit() */
  procEntry = newProcEntry(NULL,
                           table);
  enter(table, symbol("exit"), procEntry);

  /* time(ref i: int) */
  procEntry = newProcEntry(newParamTypes(intType, TRUE, NULL),
                           table);
  enter(table, symbol("time"), procEntry);

  /* clearAll(color: int) */
  procEntry = newProcEntry(newParamTypes(intType, FALSE, NULL),
                           table);
  enter(table, symbol("clearAll"), procEntry);

  /* setPixel(x: int, y: int, color: int) */
  procEntry = newProcEntry(newParamTypes(intType, FALSE,
                             newParamTypes(intType, FALSE,
                               newParamTypes(intType, FALSE, NULL))),
                           table);
  enter(table, symbol("setPixel"), procEntry);

  /* drawLine(x1: int, y1: int, x2: int, y2: int, color: int) */
  procEntry = newProcEntry(newParamTypes(intType, FALSE,
                             newParamTypes(intType, FALSE,
                               newParamTypes(intType, FALSE,
                                 newParamTypes(intType, FALSE,
                                   newParamTypes(intType, FALSE, NULL))))),
                           table);
  enter(table, symbol("drawLine"), procEntry);

  /* drawCircle(x0: int, y0: int, radius: int, color: int) */
  procEntry = newProcEntry(newParamTypes(intType, FALSE,
                             newParamTypes(intType, FALSE,
                               newParamTypes(intType, FALSE,
                                 newParamTypes(intType, FALSE, NULL)))),
                           table);
  enter(table, symbol("drawCircle"), procEntry);
}


void checkMain(Table *globalTable) {
  Entry *mainEntry;

  mainEntry = lookup(globalTable, symbol("main"));
  if (mainEntry == NULL) {
    error("procedure 'main' is missing");
  }
  if (mainEntry->kind != ENTRY_KIND_PROC) {
    error("'main' is not a procedure");
  }
  if (mainEntry->u.procEntry.paramTypes != NULL) {
    error("procedure 'main' must not have any parameters");
  }
}


Table *check(Absyn *program, boolean showSymbolTables) {
  Table *globalTable;

  /* store global flag to show local symbol tables */
  showLocalTables = showSymbolTables;
  /* generate built-in types */
  intType = newPrimitiveType("int");
  boolType = newPrimitiveType("boolean");
  /* setup global symbol table */
  globalTable = newTable(NULL);
  enterPredefinedTypes(globalTable);
  enterPredefinedProcs(globalTable);
  /* do semantic checks in 2 passes */
  checkNode(program, globalTable, 1);
  checkNode(program, globalTable, 2);
  /* check if "main()" is present */
  checkMain(globalTable);
  /* return global symbol table */
  return globalTable;
}
