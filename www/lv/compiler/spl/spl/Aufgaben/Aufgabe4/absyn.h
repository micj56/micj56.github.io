/*
 * absyn.h -- abstract syntax
 */


#ifndef _ABSYN_H_
#define _ABSYN_H_


#define ABSYN_LIST		0
#define ABSYN_NAMETYPE		1
#define ABSYN_ARRAYTYPE		2
#define ABSYN_TYPEDECL		3
#define ABSYN_VARDECL		4
#define ABSYN_PROCDECL		5
#define ABSYN_PARAM		6
#define ABSYN_ASSIGN		7
#define ABSYN_IF		8
#define ABSYN_WHILE		9
#define ABSYN_CALL		10
#define ABSYN_OP		11
#define ABSYN_VAR		12
#define ABSYN_INT		13
#define ABSYN_SIMPLEVAR		14
#define ABSYN_ARRAYVAR		15

#define ABSYN_OP_EQ		0
#define ABSYN_OP_NE		1
#define ABSYN_OP_LT		2
#define ABSYN_OP_LE		3
#define ABSYN_OP_GT		4
#define ABSYN_OP_GE		5
#define ABSYN_OP_ADD		6
#define ABSYN_OP_SUB		7
#define ABSYN_OP_MUL		8
#define ABSYN_OP_DIV		9


typedef struct absyn {
  int type;
  int line;
  union {
    struct {
      struct absyn *head;
      struct absyn *tail;
    } listNode;
    struct {
      Symbol *name;
    } nametypeNode;
    struct {
      int size;
      struct absyn *type;
    } arraytypeNode;
    struct {
      Symbol *name;
      struct absyn *type;
    } typedeclNode;
    struct {
      Symbol *name;
      struct absyn *type;
    } vardeclNode;
    struct {
      Symbol *name;
      struct absyn *params;
      struct absyn *decls;
      struct absyn *body;
    } procdeclNode;
    struct {
      Symbol *name;
      struct absyn *type;
      boolean isRef;
    } paramNode;
    struct {
      struct absyn *var;
      struct absyn *expr;
    } assignNode;
    struct {
      struct absyn *test;
      struct absyn *thenPart;
      struct absyn *elsePart;
    } ifNode;
    struct {
      struct absyn *test;
      struct absyn *body;
    } whileNode;
    struct {
      Symbol *name;
      struct absyn *args;
    } callNode;
    struct {
      int op;
      struct absyn *left;
      struct absyn *right;
    } opNode;
    struct {
      struct absyn *var;
    } varNode;
    struct {
      int val;
    } intNode;
    struct {
      Symbol *name;
    } simplevarNode;
    struct {
      struct absyn *var;
      struct absyn *index;
    } arrayvarNode;
  } u;
} Absyn;


Absyn *newList(int line, Absyn *head, Absyn *tail);
Absyn *newNametype(int line, Symbol *name);
Absyn *newArraytype(int line, int size, Absyn *type);
Absyn *newTypedecl(int line, Symbol *name, Absyn *type);
Absyn *newVardecl(int line, Symbol *name, Absyn *type);
Absyn *newProcdecl(int line, Symbol *name,
                   Absyn *params, Absyn *decls, Absyn *body);
Absyn *newParam(int line, Symbol *name, Absyn *type, boolean isRef);
Absyn *newAssign(int line, Absyn *var, Absyn *expr);
Absyn *newIf(int line, Absyn *test, Absyn *thenPart, Absyn *elsePart);
Absyn *newWhile(int line, Absyn *test, Absyn *body);
Absyn *newCall(int line, Symbol *name, Absyn *args);
Absyn *newOp(int line, int op, Absyn *left, Absyn *right);
Absyn *newVar(int line, Absyn *var);
Absyn *newInt(int line, int val);
Absyn *newSimplevar(int line, Symbol *name);
Absyn *newArrayvar(int line, Absyn *var, Absyn *index);

void showAbsyn(Absyn *node);


#endif /* _ABSYN_H_ */
