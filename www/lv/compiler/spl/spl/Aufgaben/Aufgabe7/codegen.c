/*
 * codegen.c -- ECO32 code generator
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
#include "codegen.h"


static FILE *asmFile;


/**************************************************************/


static int getReg(int numRegs) {
  if (numRegs <= 0) {
    error("expression too complicated, running out of registers");
  }
  return 24 - numRegs;
}


static int newLabel(void) {
  static int numLabels = 0;

  return numLabels++;
}


/**************************************************************/


static void genCodeNode(Absyn *node, Table *symTable, int numRegs);


static void genCodeList(Absyn *node, Table *symTable, int numRegs) {
  while (node != NULL) {
    genCodeNode(node->u.listNode.head, symTable, numRegs);
    node = node->u.listNode.tail;
  }
}


static void genCodeProcdecl(Absyn *node, Table *symTable, int numRegs) {
  Entry *procEntry;
  Table *localTable;
  int localvarArea;
  int outgoingArea;
  int framesize;

  /* get symbol table entry for this procedure */
  ...

  /* compute frame size */
  ...

  /* generate procedure prolog */
  fprintf(asmFile, "\n");
  fprintf(asmFile, "\t.export\t%s\n",
          symbolString(node->u.procdeclNode.name));
  fprintf(asmFile, "%s:\n",
          symbolString(node->u.procdeclNode.name));

  ... usw.

  /* generate code for body of procedure */
  genCodeNode(node->u.procdeclNode.body, localTable, numRegs);

  /* generate procedure epilog */
  ...
}


static void genCodeAssign(Absyn *node, Table *symTable, int numRegs) {
  genCodeNode(node->u.assignNode.var, symTable, numRegs);
  genCodeNode(node->u.assignNode.expr, symTable, numRegs - 1);
  fprintf(asmFile, "\tstw\t$%d,$%d,0\n",
          getReg(numRegs - 1), getReg(numRegs));
}


static void genCodeCondition(Absyn *node, Table *symTable, int numRegs,
                             int falseLabel) {
  genCodeNode(node->u.opNode.left, symTable, numRegs);
  genCodeNode(node->u.opNode.right, symTable, numRegs - 1);
  switch (node->u.opNode.op) {
    case ABSYN_OP_EQ:
      fprintf(asmFile, "\tbne\t$%d,$%d,L%d\n",
              getReg(numRegs), getReg(numRegs - 1), falseLabel);
      break;
    case ABSYN_OP_NE:
      fprintf(asmFile, "\tbeq\t$%d,$%d,L%d\n",
              getReg(numRegs), getReg(numRegs - 1), falseLabel);
      break;
    case ABSYN_OP_LT:
      fprintf(asmFile, "\tbge\t$%d,$%d,L%d\n",
              getReg(numRegs), getReg(numRegs - 1), falseLabel);
      break;
    case ABSYN_OP_LE:
      fprintf(asmFile, "\tbgt\t$%d,$%d,L%d\n",
              getReg(numRegs), getReg(numRegs - 1), falseLabel);
      break;
    case ABSYN_OP_GT:
      fprintf(asmFile, "\tble\t$%d,$%d,L%d\n",
              getReg(numRegs), getReg(numRegs - 1), falseLabel);
      break;
    case ABSYN_OP_GE:
      fprintf(asmFile, "\tblt\t$%d,$%d,L%d\n",
              getReg(numRegs), getReg(numRegs - 1), falseLabel);
      break;
    default:
      error("unknown operator %d in genCodeCondition", node->u.opNode.op);
  }
}


static void genCodeIf(Absyn *node, Table *symTable, int numRegs) {
  int falseLabel;
  int joinLabel;

  if (node->u.ifNode.elsePart == NULL) {
    /* if without else */
    ...
  } else {
    /* if with else */
    ...
  }
}


static void genCodeWhile(Absyn *node, Table *symTable, int numRegs) {
  int againLabel;
  int breakLabel;

  againLabel = newLabel();
  breakLabel = newLabel();
  fprintf(asmFile, "L%d:\n", againLabel);
  genCodeCondition(node->u.whileNode.test, symTable, numRegs, breakLabel);
  genCodeNode(node->u.whileNode.body, symTable, numRegs);
  fprintf(asmFile, "\tj\tL%d\n", againLabel);
  fprintf(asmFile, "L%d:\n", breakLabel);
}


static void genCodeCall(Absyn *node, Table *symTable, int numRegs) {
  Entry *procEntry;
  Absyn *args;
  Absyn *arg;
  ParamTypes *params;
  int i;

  /* get symbol table entry of called procedure */
  /* "isref" parameter attributes needed !      */

  ...
  /* store (values or addresses of) outgoing args in frame */
  i = 0;
  ...

  /* jal */
  fprintf(asmFile, "\tjal\t%s\n",
          symbolString(node->u.callNode.name));
}


static void genCodeOp(Absyn *node, Table *symTable, int numRegs) {
  genCodeNode(node->u.opNode.left, symTable, numRegs);
  genCodeNode(node->u.opNode.right, symTable, numRegs - 1);
  switch (node->u.opNode.op) {
    case ABSYN_OP_ADD:
      fprintf(asmFile, "\tadd\t$%d,$%d,$%d\n",
              getReg(numRegs), getReg(numRegs), getReg(numRegs - 1));
      break;
    case ABSYN_OP_SUB:
      fprintf(asmFile, "\tsub\t$%d,$%d,$%d\n",
              getReg(numRegs), getReg(numRegs), getReg(numRegs - 1));
      break;
    case ABSYN_OP_MUL:
      fprintf(asmFile, "\tmul\t$%d,$%d,$%d\n",
              getReg(numRegs), getReg(numRegs), getReg(numRegs - 1));
      break;
    case ABSYN_OP_DIV:
      fprintf(asmFile, "\tdiv\t$%d,$%d,$%d\n",
              getReg(numRegs), getReg(numRegs), getReg(numRegs - 1));
      break;
    default:
      error("unknown operator %d in genCodeOp", node->u.opNode.op);
  }
}


static void genCodeVar(Absyn *node, Table *symTable, int numRegs) {
   ...
}


static void genCodeInt(Absyn *node, Table *symTable, int numRegs) {
   ...
}


static void genCodeSimplevar(Absyn *node, Table *symTable, int numRegs) {
  /* don't forget additional "ldw" for reference params */
}


static void genCodeArrayvar(Absyn *node, Table *symTable, int numRegs) {
  genCodeNode(node->u.arrayvarNode.var, symTable, numRegs);
  genCodeNode(node->u.arrayvarNode.index, symTable, numRegs - 1);
  fprintf(asmFile, "\tadd\t$%d,$0,%d\n",
          getReg(numRegs - 2),
          node->u.arrayvarNode.var->dataType->u.arrayType.size);
  fprintf(asmFile, "\tbgeu\t$%d,$%d,_indexError\n",
          getReg(numRegs - 1), getReg(numRegs - 2));
  fprintf(asmFile, "\tmul\t$%d,$%d,%d\n",
          getReg(numRegs - 1), getReg(numRegs - 1),
          node->u.arrayvarNode.var->dataType->u.arrayType.baseType->byteSize);
  fprintf(asmFile, "\tadd\t$%d,$%d,$%d\n",
          getReg(numRegs), getReg(numRegs), getReg(numRegs - 1));
}


static void genCodeNode(Absyn *node, Table *symTable, int numRegs) {
  if (node == NULL) {
    /* this can only happen with an empty list */
    genCodeList(node, symTable, numRegs);
  } else
  switch (node->type) {
    case ABSYN_LIST:
      genCodeList(node, symTable, numRegs);
      break;
    case ABSYN_PROCDECL:
      genCodeProcdecl(node, symTable, numRegs);
      break;
    case ABSYN_ASSIGN:
      genCodeAssign(node, symTable, numRegs);
      break;
    case ABSYN_IF:
      genCodeIf(node, symTable, numRegs);
      break;
    case ABSYN_WHILE:
      genCodeWhile(node, symTable, numRegs);
      break;
    case ABSYN_CALL:
      genCodeCall(node, symTable, numRegs);
      break;
    case ABSYN_OP:
      genCodeOp(node, symTable, numRegs);
      break;
    case ABSYN_VAR:
      genCodeVar(node, symTable, numRegs);
      break;
    case ABSYN_INT:
      genCodeInt(node, symTable, numRegs);
      break;
    case ABSYN_SIMPLEVAR:
      genCodeSimplevar(node, symTable, numRegs);
      break;
    case ABSYN_ARRAYVAR:
      genCodeArrayvar(node, symTable, numRegs);
      break;
    default:
      error("unknown node type %d in genCodeNode", node->type);
  }
}


/**************************************************************/


void assemblerProlog(void) {
  fprintf(asmFile, "\t.import\tprinti\n");
  fprintf(asmFile, "\t.import\tprintc\n");
  fprintf(asmFile, "\t.import\treadi\n");
  fprintf(asmFile, "\t.import\treadc\n");
  fprintf(asmFile, "\t.import\texit\n");
  fprintf(asmFile, "\t.import\ttime\n");
  fprintf(asmFile, "\t.import\tclearAll\n");
  fprintf(asmFile, "\t.import\tsetPixel\n");
  fprintf(asmFile, "\t.import\tdrawLine\n");
  fprintf(asmFile, "\t.import\tdrawCircle\n");
  fprintf(asmFile, "\t.import\t_indexError\n");
  fprintf(asmFile, "\n");
  fprintf(asmFile, "\t.code\n");
  fprintf(asmFile, "\t.align\t4\n");
}


void genCode(Absyn *program, Table *globalTable, FILE *outFile) {
  Absyn *nodes;
  Absyn *node;

  asmFile = outFile;
  assemblerProlog();
  nodes = program;
  while (nodes != NULL) {
    if (nodes->type != ABSYN_LIST) {
      error("illegal prog tree in genCode");
    }
    node = nodes->u.listNode.head;
    nodes = nodes->u.listNode.tail;
    if (node->type == ABSYN_PROCDECL) {
      genCodeNode(node, globalTable, 16);
    }
  }
}
