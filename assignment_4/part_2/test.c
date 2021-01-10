/* treeForm.c for assignment 4 on logical formulas, February 2019
 *
 * This recognizes and makes syntax trees for the following grammar:
 *
 * <atom>     ::=  'T' | 'F' | <identifier> | '(' <formula> ')'
 * <literal>  ::=  <atom> | '~' <atom>
 * <formula>  ::=  <literal> { '&' <literal> }
 *
 */

#include <stdio.h>  /* getchar, printf */
#include <stdlib.h> /* NULL */
#include <assert.h>

#include "scanner.h"
#include "treeForm.h"

/* The acceptCharacter function takes a pointer to a token list and a character.
 * It checks whether the first token in the list is a Symbol with the given character.
 * When that is the case, it returns 1 and moves the pointer to the rest of the token list.
 * Otherwise it yields 0 and the pointer remains unchanged.
 */

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

FormTree copyTree(FormTree t){
FormTree copy = NULL;
if ( t != NULL ) {
copy = malloc(sizeof(FormTreeNode));
assert(copy != NULL);
copy->tt = t->tt;
copy->t = t->t;
copy->left = copyTree(t->left);
copy->right = copyTree(t->right);
}
return copy;
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

// <atom>  ::=  'T' | 'F' | <identifier> | '(' <formula> ')'
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

// <literal>  ::=  <atom> | '~' <atom>
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

// <conjunct>  ::=  <literal> { '&' <literal>}
int treeConjunct (List *lp, FormTree *t) {
	if ( !treeLiteral(lp,t) ) {
    return 0;
  }
  while ( acceptCharacter(lp,'&') ) {
    FormTree tL = *t;
    FormTree tR = NULL;
    if ( !treeLiteral(lp,&tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = '&';
    *t = newFormTreeNode(Symbol, tok, tL, tR);
  } /* no '&', so we reached the end of conjunction */
	return 1;
}

// <disjunct>  ::=  <conjunct> { '|' <conjunct>}
int treeDisjunct (List *lp, FormTree *t) {
  if ( !treeConjunct(lp,t) ) {
    return 0;
  }
  while ( acceptCharacter(lp, '|') ) {
    FormTree tL = *t;
    FormTree tR = NULL;
    if ( !treeConjunct(lp, &tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = '|';
    *t = newFormTreeNode(Symbol,tok,tL,tR);
  } /* no '|', so we reached the end of conjunction */
  return 1;
}

// <conditional>  ::=  <disjunct> [ '->' <disjunct>]
int treeConditional (List *lp, FormTree *t) {
  if ( !treeDisjunct(lp,t) ) {
    return 0;
  }
  if ( acceptCharacter(lp, '-') ) {  // ->
    if ( !acceptCharacter(lp, '>') ) {
      return 0;
    }
    FormTree tL = *t;
    FormTree tR = NULL;
    if ( !treeDisjunct(lp, &tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = '-';
    *t = newFormTreeNode(Symbol, tok, tL, tR);
  } /* no '->', so we reached the end of conjunction */
  return 1;
}

// <formula>  ::=  <conditional> [ '<->' <conditional> ]
int treeFormula(List *lp, FormTree *t) {
  if ( !treeConditional(lp,t) ) {
    return 0;
  }
  if ( acceptCharacter(lp, '<') ) {  // ->
    if ( !acceptCharacter(lp, '-') ) {
      return 0;
    }
    if ( !acceptCharacter(lp, '>') ) {
      return 0;
    }
    FormTree tL = *t;
    FormTree tR = NULL;
    if ( !treeConditional(lp, &tR) ) {
      freeTree(tR);
      return 0;
    }
    Token tok;
    tok.symbol = '<';
    *t = newFormTreeNode(Symbol, tok, tL, tR);
  } /* no '<->', so we reached the end of conjunction */
  return 1;
}

int max(int m, int n) {
  return ( m > n ) ? m : n ;
}

int height(FormTree t) {
  if (t == NULL) {
    return -1;
  }
  return max(height(t->left),height(t->right)) +1;
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
      printf("%c",t->t.symbol);
      break;
    case '~':
      printf("(~");
      printTree(t->left);
      printf(")");
      break;
    default:
      printf("(");
      printTree(t->left);
      if(t->t.symbol == '-'){
        printf(" -> ");
      }else if(t->t.symbol== '<'){
        printf(" <-> " );
      }else{
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

/*function which converts ~T to F and ~F to T*/
FormTree BooleanSimp(FormTree t){

  if(t->left->t.symbol == 'T' || t->left->t.symbol == 'F'){
  Token tok;
  if(t->left->t.symbol == 'T'){
    tok.symbol = 'F';
  }else if(t->left->t.symbol == 'F'){
    tok.symbol = 'T';
  }
  free(t->left);
  free(t);
  return newFormTreeNode(Symbol, tok, NULL, NULL);
  }
  return t;
}

/* function to simplify conjunction; returns that part of the tree which has a dominant expression*/
FormTree conjunctionSimp(FormTree t){
  FormTree *tp = &t;
  if(t->left->t.symbol == 'T' || t->right->t.symbol == 'F'){
  tp = &(t->right);
  if(t->left->tt != Symbol){
    freeTree(t->left);
  }else{
    free(t->left);
  }
    free(t);
    return *tp;
  }else if(t->right->t.symbol == 'T' ||t->left->t.symbol == 'F'){
    tp = &(t->left);
    if(t->right->tt != Symbol){
      freeTree(t->right);
    }else{
      free(t->right);
    }
      free(t);
      return *tp;
  }
  return *tp;
}


/* function to simplify disjunction; returns that part of the tree which has a dominant expression*/
FormTree disjunctionSimp(FormTree t){
  FormTree *tp = &t;
  if(t->left->t.symbol == 'T' || t->right->t.symbol == 'F'){
    tp = &(t->left);
      freeTree(t->right);
      free(t);
      return *tp;

  }else if(t->right->t.symbol == 'T' ||t->left->t.symbol == 'F'){
    tp = &(t->right);
      freeTree(t->left);
      free(t);
      return *tp;
  }
  return *tp;
}

/*simplifies ->*/
FormTree conditionalSimp(FormTree t){
  Token tok;
  FormTree *tp = &t;
  if(t->left->t.symbol == 'T' || t->right->t.symbol == 'T'){
    tp = &(t->right);
      freeTree(t->left);
      free(t);
      return *tp;
  }else if(t->left->t.symbol == 'F'){
     tok.symbol = 'T';
     freeTree(t);
    return newFormTreeNode(Symbol, tok, NULL, NULL);
  }else if(t->right->t.symbol == 'F'){
     tok.symbol = '~';
       tp = &(t->left);
     free(t->right);
     free(t);
    return simplify(newFormTreeNode(Symbol,tok, *tp, NULL ));
  }
  return *tp;
}

/*simplifies <->*/
FormTree formulaSimp(FormTree t){
  Token tok;
  FormTree *tp = &t;
  if(t->left->t.symbol == 'F'){
    tok.symbol = '~';
    tp = &(t->right);
      freeTree(t->left);
      free(t);
   return (simplify(newFormTreeNode(Symbol,tok, *tp, NULL )));
  }else if(t->right->t.symbol == 'F'){
    tok.symbol = '~';
    tp = &(t->left);
      freeTree(t->right);
      free(t);
   return (simplify(newFormTreeNode(Symbol,tok, *tp, NULL )));
 }else if(t->left->t.symbol == 'T'){
  tp = &(t->right);
    freeTree(t->left);
    free(t);
    return *tp;
 }else if(t->right->t.symbol == 'T'){
   tp = &(t->left);
     freeTree(t->right);
     free(t);
     return *tp;
  }
  return *tp;
}

FormTree simplify(FormTree t){
  if(t == NULL){
    return NULL;
  }

  t->left=simplify(t->left);
  t->right=simplify(t->right);
  if(t->tt == Symbol){
    switch(t->t.symbol){
      case '~':
      /*case with several '~' in a row, just return t->left->left */
        if(t->left->t.symbol == '~'){
        FormTree *tp = &(t->left->left);
          //return t->left->left;
          free(t->left);
          free(t);
          return *tp;
        }else{
      /*with negation consider LEFT subtree, we create new node*/
         return BooleanSimp(t);
        break;
      }
      case '&':
        return conjunctionSimp(t);
        break;
      case '|':
        return disjunctionSimp(t);
        break;
      case '-':
        return conditionalSimp(t);
        break;
      case '<':
        return formulaSimp(t);
        break;
      }
    }

 return t;
}



FormTree translate( FormTree t){
  if(t==NULL){
    return NULL;
  }
  t->left=translate(t->left);
  t->right=translate(t->right);
   Token tok;
  FormTree tL, tR;
    switch (t->t.symbol) {
      case '|':
      tok.symbol = '~';
      tL = newFormTreeNode(Symbol, tok, t->left, NULL);
      tR = newFormTreeNode(Symbol, tok, t->right, NULL);
      tok.symbol = '&';
      FormTree t1 =  newFormTreeNode(Symbol, tok, tL, tR );
      tok.symbol = '~';
      //freeTree(tL);
      //freeTree(tR);
      return newFormTreeNode(Symbol, tok, t1, NULL);
      break;
      case '-':
     tok.symbol = '~';
       tL = newFormTreeNode(Symbol, tok, t->left, NULL);
      tok.symbol = '|';
      return translate(newFormTreeNode(Symbol, tok, tL, t->right));

      break;

      case '<':
      tok.symbol = '&';
      tL = newFormTreeNode(Symbol, tok, t->left, t->right);

      FormTree tcopy = copyTree(tL);
      tok.symbol = '~';
      FormTree tL1 = newFormTreeNode(Symbol, tok, tcopy->left, NULL);
      FormTree tR1 = newFormTreeNode(Symbol, tok, tcopy->right, NULL);
      tok.symbol = '&';
      tR = newFormTreeNode(Symbol, tok, tL1, tR1);
      tok.symbol = '|';
      return translate(newFormTreeNode(Symbol, tok, tL, tR));

      break;
}
 return t;
}
