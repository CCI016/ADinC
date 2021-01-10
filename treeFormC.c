#include <stdio.h>  /* getchar, printf */
#include <stdlib.h> /* NULL */
#include <assert.h>

#include "scanner.h"
#include "treeForm.h"


int acceptCharacter(List *lp, char c) {
  if (*lp != NULL && (*lp)->tt == Symbol && ((*lp)->t).symbol == c ) {
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

FormTree newFormTreeNode(TokenType tt, Token t, FormTree tL, FormTree tR) {
  FormTree new = malloc(sizeof(FormTreeNode));
  assert (new != NULL);
  new->tt = tt;
  new->t = t;
  new->left = tL;
  new->right = tR;
  return new;
}

void freeTree(FormTree t) {
  if (t == NULL) {
    return;
  }
  freeTree(t->left);
  freeTree(t->right);
  free(t);
}

int treeIdentifier(List *lp, FormTree *t) {
  if (*lp != NULL && (*lp)->tt == Identifier ) {
    *t = newFormTreeNode(Identifier, (*lp)->t, NULL, NULL);
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

int treeAtom(List *lp, FormTree *t) {
  if (acceptCharacter(lp,'T')) {
    Token tok;
    tok.symbol = 'T';
    *t = newFormTreeNode(Symbol, tok, NULL, NULL);
    return 1;
  }
  if (acceptCharacter(lp,'F')) {
    Token tok;
    tok.symbol = 'F';
    *t = newFormTreeNode(Symbol, tok, NULL, NULL);
    return 1;
  }
  if (treeIdentifier(lp,t)) {
    return 1;
  }
  if (acceptCharacter(lp,'(') && treeFormula(lp,t) && acceptCharacter(lp,')') ) {
    return 1;
  }
  return 0;
}

int treeLiteral(List *lp, FormTree *t) {
  if (treeAtom(lp,t)) {
    return 1;
  }
  if (acceptCharacter(lp,'~')) {
    FormTree tL = NULL;
    if (treeAtom(lp, &tL)) {
      Token tok;
      tok.symbol = '~';
      *t = newFormTreeNode(Symbol, tok, tL, NULL);
      return 1;
    }
    freeTree(tL);
  }
  return 0;
}

 
int conjunction (List *lp, FormTree *t) {
	if (!treeLiteral(lp,t) ) {
    return 0;
  }
  while (acceptCharacter(lp,'&') ) {
    FormTree tL = *t;
    FormTree tR = NULL;
    if (!treeLiteral(lp,&tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = '&';
    *t = newFormTreeNode(Symbol, tok, tL, tR);
  } 
	return 1;
}

int disjunction (List *lp, FormTree *t) {
  if (!conjunction(lp,t) ) {
    return 0;
  }
  while (acceptCharacter(lp, '|') ) {
    FormTree tL = *t;
    FormTree tR = NULL;
    if (!conjunction(lp, &tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = '|';
    *t = newFormTreeNode(Symbol,tok,tL,tR);
  } 
  return 1;
}

int logicCondition (List *lp, FormTree *t) {
  if (disjuction(lp,t) == 0) {
    return 0;
  }
  if (acceptCharacter(lp, '-') ) {  
    if (!acceptCharacter(lp, '>') ) {
      return 0;
    }
    FormTree tL = *t;
    FormTree tR = NULL;
    if (!disjuction(lp, &tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = 0; 
    *t = newFormTreeNode(Symbol, tok, tL, tR);
  } 
  return 1;
}

int treeFormula(List *lp, FormTree *t) {
  if (!treeConditional(lp,t) ) {
    return 0;
  }
  if (acceptCharacter(lp, '<') ) {  
    if (!acceptCharacter(lp, '-') ) {
      return 0;
    }
    if (!acceptCharacter(lp, '>') ) {
      return 0;
    }
    FormTree tL = *t;
    FormTree tR = NULL;
    if (!logicCondition(lp, &tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = 1;
    *t = newFormTreeNode(Symbol, tok, tL, tR);
  } 
  return 1;
}

int peek(int a, int b) {
  return ( a > b ) ? a : b ;
}

int complexity(FormTree t) { 
  if (t == NULL) {
    return -1;
  }
  return peek(complexity(t->left),complexity(t->right)) +1;
}

void printTree(FormTree t) {
  if (t == NULL) {
    return;
  }
  switch (t->tt) {
  case Symbol:
    switch (t->t.symbol) {
    case 'T':
    case 'F':
      if (t->t.symbol == 1){
        printf(" <-> ");
      }
      else{
        if (t->t.symbol == 0){
        printf(" -> ");
        }
        else
        printf("%c",t->t.symbol);
      }
      break;
    case '~':
      printf("(~");
      printTree(t->left);
      printf(")");
      break;
    default:
      printf("(");
      printTree(t->left);
      if (t->t.symbol == 1){
        printf(" <-> ");
      }
      else{
        if (t->t.symbol == 0){
        printf(" -> ");
        }
        else
        printf(" %c ",t->t.symbol);
      }
      printTree(t->right);
      printf(")");
      break;
    }
    break;
  case Identifier:
    printf("%s", t->t.identifier);
    break;
  }
}
