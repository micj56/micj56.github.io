/*
 * absyn.c -- abstract syntax
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "symbol.h"
#include "absyn.h"


Absyn *newList(int line, Absyn *head, Absyn *tail) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_LIST;
  node->line = line;
  node->u.listNode.head = head;
  node->u.listNode.tail = tail;
  return node;
}


Absyn *newNametype(int line, Symbol *name) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_NAMETYPE;
  node->line = line;
  node->u.nametypeNode.name = name;
  return node;
}


Absyn *newArraytype(int line, int size, Absyn *type) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_ARRAYTYPE;
  node->line = line;
  node->u.arraytypeNode.size = size;
  node->u.arraytypeNode.type = type;
  return node;
}


Absyn *newTypedecl(int line, Symbol *name, Absyn *type) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_TYPEDECL;
  node->line = line;
  node->u.typedeclNode.name = name;
  node->u.typedeclNode.type = type;
  return node;
}


Absyn *newVardecl(int line, Symbol *name, Absyn *type) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_VARDECL;
  node->line = line;
  node->u.vardeclNode.name = name;
  node->u.vardeclNode.type = type;
  return node;
}


Absyn *newProcdecl(int line, Symbol *name,
                   Absyn *params, Absyn *decls, Absyn *body) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_PROCDECL;
  node->line = line;
  node->u.procdeclNode.name = name;
  node->u.procdeclNode.params = params;
  node->u.procdeclNode.decls = decls;
  node->u.procdeclNode.body = body;
  return node;
}


Absyn *newParam(int line, Symbol *name, Absyn *type, boolean isRef) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_PARAM;
  node->line = line;
  node->u.paramNode.name = name;
  node->u.paramNode.type = type;
  node->u.paramNode.isRef = isRef;
  return node;
}


Absyn *newAssign(int line, Absyn *var, Absyn *expr) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_ASSIGN;
  node->line = line;
  node->u.assignNode.var = var;
  node->u.assignNode.expr = expr;
  return node;
}


Absyn *newIf(int line, Absyn *test, Absyn *thenPart, Absyn *elsePart) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_IF;
  node->line = line;
  node->u.ifNode.test = test;
  node->u.ifNode.thenPart = thenPart;
  node->u.ifNode.elsePart = elsePart;
  return node;
}


Absyn *newWhile(int line, Absyn *test, Absyn *body) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_WHILE;
  node->line = line;
  node->u.whileNode.test = test;
  node->u.whileNode.body = body;
  return node;
}


Absyn *newCall(int line, Symbol *name, Absyn *args) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_CALL;
  node->line = line;
  node->u.callNode.name = name;
  node->u.callNode.args = args;
  return node;
}


Absyn *newOp(int line, int op, Absyn *left, Absyn *right) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_OP;
  node->line = line;
  node->u.opNode.op = op;
  node->u.opNode.left = left;
  node->u.opNode.right = right;
  return node;
}


Absyn *newVar(int line, Absyn *var) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_VAR;
  node->line = line;
  node->u.varNode.var = var;
  return node;
}


Absyn *newInt(int line, int val) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_INT;
  node->line = line;
  node->u.intNode.val = val;
  return node;
}


Absyn *newSimplevar(int line, Symbol *name) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_SIMPLEVAR;
  node->line = line;
  node->u.simplevarNode.name = name;
  return node;
}


Absyn *newArrayvar(int line, Absyn *var, Absyn *index) {
  Absyn *node;

  node = (Absyn *) allocate(sizeof(Absyn));
  node->type = ABSYN_ARRAYVAR;
  node->line = line;
  node->u.arrayvarNode.var = var;
  node->u.arrayvarNode.index = index;
  return node;
}


/**************************************************************/


static void indent(int n) {
  int i;

  for (i = 0; i < n; i++) {
    printf("  ");
  }
}


static void say(char *s) {
  printf("%s", s);
}


static void sayInt(int i) {
  printf("%d", i);
}


static void sayBoolean(boolean b) {
  if (b) {
    printf("TRUE");
  } else {
    printf("FALSE");
  }
}


static void showNode(Absyn *node, int indent);


static void showList(Absyn *node, int n) {
  indent(n);
  if (node == NULL) {
    say("--empty--");
  } else {
    say("List(\n");
    do {
      showNode(node->u.listNode.head, n + 1);
      node = node->u.listNode.tail;
      if (node != NULL) {
        say(",\n");
      } else {
        say(")");
      }
    } while (node != NULL);
  }
}


static void showNametype(Absyn *node, int n) {
  indent(n);
  say("Nametype(");
  say(symbolString(node->u.nametypeNode.name));
  say(")");
}


static void showArraytype(Absyn *node, int n) {
  indent(n);
  say("Arraytype(\n");
  indent(n + 1);
  sayInt(node->u.arraytypeNode.size);
  say(",\n");
  showNode(node->u.arraytypeNode.type, n + 1);
  say(")");
}


static void showTypedecl(Absyn *node, int n) {
  indent(n);
  say("Typedecl(\n");
  indent(n + 1);
  say(symbolString(node->u.typedeclNode.name));
  say(",\n");
  showNode(node->u.typedeclNode.type, n + 1);
  say(")");
}


static void showVardecl(Absyn *node, int n) {
  indent(n);
  say("Vardecl(\n");
  indent(n + 1);
  say(symbolString(node->u.vardeclNode.name));
  say(",\n");
  showNode(node->u.vardeclNode.type, n + 1);
  say(")");
}


static void showProcdecl(Absyn *node, int n) {
  indent(n);
  say("Procdecl(\n");
  indent(n + 1);
  say(symbolString(node->u.procdeclNode.name));
  say(",\n");
  showNode(node->u.procdeclNode.params, n + 1);
  say(",\n");
  showNode(node->u.procdeclNode.decls, n + 1);
  say(",\n");
  showNode(node->u.procdeclNode.body, n + 1);
  say(")");
}


static void showParam(Absyn *node, int n) {
  indent(n);
  say("Param(\n");
  indent(n + 1);
  say(symbolString(node->u.paramNode.name));
  say(",\n");
  showNode(node->u.paramNode.type, n + 1);
  say(",\n");
  indent(n + 1);
  sayBoolean(node->u.paramNode.isRef);
  say(")");
}


static void showAssign(Absyn *node, int n) {
  indent(n);
  say("Assign(\n");
  showNode(node->u.assignNode.var, n + 1);
  say(",\n");
  showNode(node->u.assignNode.expr, n + 1);
  say(")");
}


static void showIf(Absyn *node, int n) {
  indent(n);
  say("If(\n");
  showNode(node->u.ifNode.test, n + 1);
  say(",\n");
  showNode(node->u.ifNode.thenPart, n + 1);
  say(",\n");
  showNode(node->u.ifNode.elsePart, n + 1);
  say(")");
}


static void showWhile(Absyn *node, int n) {
  indent(n);
  say("While(\n");
  showNode(node->u.whileNode.test, n + 1);
  say(",\n");
  showNode(node->u.whileNode.body, n + 1);
  say(")");
}


static void showCall(Absyn *node, int n) {
  indent(n);
  say("Call(\n");
  indent(n + 1);
  say(symbolString(node->u.callNode.name));
  say(",\n");
  showNode(node->u.callNode.args, n + 1);
  say(")");
}


static void showOp(Absyn *node, int n) {
  indent(n);
  say("Op(\n");
  indent(n + 1);
  switch (node->u.opNode.op) {
    case ABSYN_OP_EQ:
      say("EQ");
      break;
    case ABSYN_OP_NE:
      say("NE");
      break;
    case ABSYN_OP_LT:
      say("LT");
      break;
    case ABSYN_OP_LE:
      say("LE");
      break;
    case ABSYN_OP_GT:
      say("GT");
      break;
    case ABSYN_OP_GE:
      say("GE");
      break;
    case ABSYN_OP_ADD:
      say("ADD");
      break;
    case ABSYN_OP_SUB:
      say("SUB");
      break;
    case ABSYN_OP_MUL:
      say("MUL");
      break;
    case ABSYN_OP_DIV:
      say("DIV");
      break;
    default:
      error("unknown operator %d in showOp", node->u.opNode.op);
  }
  say(",\n");
  showNode(node->u.opNode.left, n + 1);
  say(",\n");
  showNode(node->u.opNode.right, n + 1);
  say(")");
}


static void showVar(Absyn *node, int n) {
  indent(n);
  say("Var(\n");
  showNode(node->u.varNode.var, n + 1);
  say(")");
}


static void showInt(Absyn *node, int n) {
  indent(n);
  say("Int(");
  sayInt(node->u.intNode.val);
  say(")");
}


static void showSimplevar(Absyn *node, int n) {
  indent(n);
  say("Simplevar(");
  say(symbolString(node->u.simplevarNode.name));
  say(")");
}


static void showArrayvar(Absyn *node, int n) {
  indent(n);
  say("Arrayvar(\n");
  showNode(node->u.arrayvarNode.var, n + 1);
  say(",\n");
  showNode(node->u.arrayvarNode.index, n + 1);
  say(")");
}


static void showNode(Absyn *node, int indent) {
  if (node == NULL) {
    /* this can only happen with an empty list */
    showList(node, indent);
  } else
  switch (node->type) {
    case ABSYN_LIST:
      showList(node, indent);
      break;
    case ABSYN_NAMETYPE:
      showNametype(node, indent);
      break;
    case ABSYN_ARRAYTYPE:
      showArraytype(node, indent);
      break;
    case ABSYN_TYPEDECL:
      showTypedecl(node, indent);
      break;
    case ABSYN_VARDECL:
      showVardecl(node, indent);
      break;
    case ABSYN_PROCDECL:
      showProcdecl(node, indent);
      break;
    case ABSYN_PARAM:
      showParam(node, indent);
      break;
    case ABSYN_ASSIGN:
      showAssign(node, indent);
      break;
    case ABSYN_IF:
      showIf(node, indent);
      break;
    case ABSYN_WHILE:
      showWhile(node, indent);
      break;
    case ABSYN_CALL:
      showCall(node, indent);
      break;
    case ABSYN_OP:
      showOp(node, indent);
      break;
    case ABSYN_VAR:
      showVar(node, indent);
      break;
    case ABSYN_INT:
      showInt(node, indent);
      break;
    case ABSYN_SIMPLEVAR:
      showSimplevar(node, indent);
      break;
    case ABSYN_ARRAYVAR:
      showArrayvar(node, indent);
      break;
    default:
      error("unknown node type %d in showAbsyn", node->type);
  }
}


void showAbsyn(Absyn *node) {
  showNode(node, 0);
  printf("\n");
}
