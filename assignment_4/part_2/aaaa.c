/* Students : Cainarean Constantin S4142152; c.cainarean@sudent.rug.nl
							Garabajiu Denis S4142551; d.garabajiu@student.rug.nl
   Date: 09.03.2020 */

/* This recognizes and makes syntax trees for the following grammar:
 *
 * <atom>     ::=  'T' | 'F' | <identifier> | '(' <formula> ')'
 * <literal>  ::=  <atom> | '~' <atom>
 * <formula>  ::=  <literal> { '&' <literal> | '|' <literal> } ['<->' | '->' <literal> { '&' <literal> | '|' <literal> }]
 *
 */

#include <stdio.h>	/* getchar, printf */
#include <stdlib.h> /* NULL */
#include <assert.h>

#include "scanner.h"
#include "treeForm.h"

/* The acceptCharacter function takes a pointer to a token list and a character.
 * It checks whether the first token in the list is a Symbol with the given character.
 * When that is the case, it returns 1 and moves the pointer to the rest of the token list.
 * Otherwise it yields 0 and the pointer remains unchanged.
 */

/* Alghorithm : The program is based on the idea that the strength can be deconstructed like a 
 * inversed Matryoshka doll. So we start from the "weakest" and go to the strongest. We start 
 * from biconditional and suppose that it is created from implication. Implication is supposed to
 * be from | and so on. */

int acceptCharacter(List *lp, char c) {
	if (*lp != NULL && (*lp)->tt == Symbol && ((*lp)->t).symbol == c)
	{
		*lp = (*lp)->next;
		return 1;
	}
	return 0;
}

FormTree newFormTreeNode(TokenType tt, Token t, FormTree tL, FormTree tR) {
	FormTree new = malloc(sizeof(FormTreeNode));
	assert(new != NULL);
	new->tt = tt;
	new->t = t;
	new->left = tL;
	new->right = tR;
	return new;
}

void freeTree(FormTree t) {
	if (t == NULL)
	{
		return;
	}
	freeTree(t->left);
	freeTree(t->right);
	free(t);
}

int treeIdentifier(List *lp, FormTree *t) {
	if (*lp != NULL && (*lp)->tt == Identifier) {
		*t = newFormTreeNode(Identifier, (*lp)->t, NULL, NULL);
		*lp = (*lp)->next;
		return 1;
	}
	return 0;
}

// <atom>  ::=  'T' | 'F' | <identifier> | '(' <formula> ')'
int treeAtom(List *lp, FormTree *t) {
	if (acceptCharacter(lp, 'T')) {
		Token tok;
		tok.symbol = 'T';
		*t = newFormTreeNode(Symbol, tok, NULL, NULL);
		return 1;
	}
	if (acceptCharacter(lp, 'F')) {
		Token tok;
		tok.symbol = 'F';
		*t = newFormTreeNode(Symbol, tok, NULL, NULL);
		return 1;
	}
	if (treeIdentifier(lp, t)) {
		return 1;
	}
	if (acceptCharacter(lp, '(') && treeFormula(lp, t) && acceptCharacter(lp, ')')) {
		return 1;
	}
	return 0;
}

// <literal>  ::=  <atom> | '~' <atom>
int treeLiteral(List *lp, FormTree *t){
	if (treeAtom(lp, t)) {
		return 1;
	}
	if (acceptCharacter(lp, '~')) {
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
int treeConjunct(List *lp, FormTree *t) {
	if (!treeLiteral(lp, t)) {
		return 0;
	}
	while (acceptCharacter(lp, '&')) {
		FormTree tL = *t;
		FormTree tR = NULL;
		if (!treeLiteral(lp, &tR)) {
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
int treeDisjunct(List *lp, FormTree *t) {
	if (!treeConjunct(lp, t)) {
		return 0;
	}
	while (acceptCharacter(lp, '|')) {
		FormTree tL = *t;
		FormTree tR = NULL;
		if (!treeConjunct(lp, &tR)) {
			freeTree(tR);
			return 0;
		}
		Token tok;
		tok.symbol = '|';
		*t = newFormTreeNode(Symbol, tok, tL, tR);
	} /* no '|', so we reached the end of conjunction */
	return 1;
}

// <conditional>  ::=  <disjunct> [ '->' <disjunct>]
int treeConditional(List *lp, FormTree *t) {
	if (!treeDisjunct(lp, t)) {
		return 0;
	}
	if (acceptCharacter(lp, '-')) { // ->
		if (!acceptCharacter(lp, '>')) {
			return 0;
		}
		FormTree tL = *t;
		FormTree tR = NULL;
		if (!treeDisjunct(lp, &tR)) {
			freeTree(tR);
			return 0;
		}
		Token tok;
		tok.symbol = 0; // special symbol from ascii for printing
		*t = newFormTreeNode(Symbol, tok, tL, tR);
	} /* no '->', so we reached the end of conjunction */
	return 1;
}

// <formula>  ::=  <conditional> [ '<->' <conditional> ]
int treeFormula(List *lp, FormTree *t) {
	if (!treeConditional(lp, t)) {
		return 0;
	}
	if (acceptCharacter(lp, '<')) { // ->
		if (!acceptCharacter(lp, '-')) {
			return 0;
		}
		if (!acceptCharacter(lp, '>')) {
			return 0;
		}
		FormTree tL = *t;
		FormTree tR = NULL;
		if (!treeConditional(lp, &tR)) {
			freeTree(tR);
			return 0;
		}
		Token tok;
		tok.symbol = 1; // special symbol from ascii for printing
		*t = newFormTreeNode(Symbol, tok, tL, tR);
	} /* no '<->', so we reached the end of conjunction */
	return 1;
}

int max(int m, int n) {
	return (m > n) ? m : n;
}

int height(FormTree t) { //this functions finds the complexity of the logical fl-a
	if (t == NULL) {
		return -1;
	}
	return max(height(t->left), height(t->right)) + 1;
}

// This functions transforms ~T to F and ~F to T
FormTree boolSimplify(FormTree t) {
	if(t->left->t.symbol == 'T' || t->left->t.symbol == 'F') {
		Token tok;
		tok.symbol = '-';
		if(t->left->t.symbol == 'T') {
			tok.symbol = 'F';
		} else if (t->left->t.symbol == 'F') {
			tok.symbol = 'T';
		}
		free(t->left);
		free(t);
		return newFormTreeNode(Symbol, tok, NULL, NULL);
	}
	return t;
}

// This functions simplifies T | φ and φ | T to T or F | φ and φ | F to φ
FormTree disjunctionSimplify(FormTree t) {
	FormTree *tp = &t;
	if(t->left->t.symbol == 'T' || t->right->t.symbol == 'F'){
		tp = &(t->left);
		freeTree(t->right);
		free(t);
		return *tp;
	} else if (t->right->t.symbol == 'T' ||t->left->t.symbol == 'F') {
		tp = &(t->right);
		freeTree(t->left);
		free(t);
		return *tp;
	}
	return *tp;
}

//This function simplifies (T & φ and φ & T to φ) OR (F & φ and φ & F to F)
FormTree conjunctionSimplify(FormTree t) {
	FormTree *tp = &t;
	if (t->left->t.symbol == 'T' || t->right->t.symbol == 'F') {
		tp = &(t->right);
		if(t->left->tt != Symbol) {
			freeTree(t->left);
		} else {
			free(t->left);
		}
		free(t);
		return *tp;
	} else if (t->right->t.symbol == 'T' ||t->left->t.symbol == 'F') {
		tp = &(t->left);
		if (t->right->tt != Symbol) {
			freeTree(t->right);
		} else {
			free(t->right);
		}
		free(t);
		return *tp;
	}
	return *tp;
}

//Simplifies (T -> φ to φ) And (F -> φ to T) And (φ -> T to T) And (φ -> F to ~φ)
FormTree impliesSimplify(FormTree t) {
	Token tok;
	FormTree *tp = &t;
	if (t->left->t.symbol == 'T' || t->right->t.symbol == 'T') {
		tp = &(t->right);
		freeTree(t->left);
		free(t);
		return *tp;
	} else if (t->left->t.symbol == 'F') {
		tok.symbol = 'T';
		freeTree(t);
		return newFormTreeNode(Symbol, tok, NULL, NULL);
  } else if (t->right->t.symbol == 'F') {
		tok.symbol = '~';
		tp = &(t->left);
		free(t->right);
		free(t);
		return simplify(newFormTreeNode(Symbol,tok, *tp, NULL ));
	}
	return *tp;
}

//Simplifies (T <-> φ and φ <-> T to φ) OR (F <-> φ and φ <-> F to ~φ)
FormTree biconditionalSimplify(FormTree t) {
	Token tok;
	FormTree *tp = &t;
	if (t->left->t.symbol == 'F') {
		tok.symbol = '~';
		tp = &(t->right);
      freeTree(t->left);
      free(t);
			return (simplify(newFormTreeNode(Symbol,tok, *tp, NULL )));
  } else if (t->right->t.symbol == 'F') {
		tok.symbol = '~';
		tp = &(t->left);
		freeTree(t->right);
		free(t);
		return (simplify(newFormTreeNode(Symbol,tok, *tp, NULL )));
	} else if (t->left->t.symbol == 'T') {
		tp = &(t->right);
		freeTree(t->left);
		free(t);
		return *tp;
	} else if (t->right->t.symbol == 'T') {
		tp = &(t->left);
		freeTree(t->right);
		free(t);
		return *tp;
	}
	return *tp;
}

FormTree simplify(FormTree t) {
	if (t == NULL) {
		return t;
	}
	FormTree *cpy;
	t->left = simplify(t->left);
	t->right = simplify(t->right);
	if ((t)->tt == Symbol) {
		switch (t->t.symbol) {
		case '~':
			if ((t->left)->tt == Symbol && (t->left)->t.symbol == '~') {
				// this part of if-statement simplifies the case ~(~Formula) to Formula
				cpy = &(t->left)->left;
				free(t->left);
				free(t);
				return *cpy;
			} else {
				// this part of if-statement simplifies the case ~T to F and ~F to T
				return boolSimplify(t);
			}
		case '|':
			return disjunctionSimplify(t);
		case '&':
			return conjunctionSimplify(t);
		case 0: // 0 is our assigned ascii value for ->
			return impliesSimplify(t);
		case 1: // 1 is our assigned ascii value for <->
			return biconditionalSimplify(t);
		}
	}
	return t;
}

//Translates a | b to ~((~a) & (~b))
FormTree disjunctionTranslate(FormTree t) {
	Token tok;
	t->t.symbol = '&';
	tok.symbol = '~';
	t->left = newFormTreeNode(Symbol,tok, t->left, NULL);
	t->right = newFormTreeNode(Symbol, tok, t->right, NULL);
	return newFormTreeNode(Symbol, tok, t, NULL);
}

FormTree impliesTranslate(FormTree t) {
	Token tok;
	tok.symbol = '~';
	t->t.symbol = '|';
	t->left = newFormTreeNode(Symbol, tok, t->left, NULL);
	return disjunctionTranslate(t);
	return t;
}

FormTree translate(FormTree t) {
	if (t == NULL) {
		return t;
	}
	t->left = translate(t->left);
	t->right = translate(t->right);
	if ((t)->tt == Symbol) {
		switch ((t)->t.symbol) {
			case '|' :
				return disjunctionTranslate(t);
			case 0:
				return impliesTranslate(t);
		}
	}
	return t;
}

void printTree(FormTree t) {
	if (t == NULL) {
		return;
	}
	switch (t->tt) {
	case Symbol:
		switch (t->t.symbol)
		{
		case 'T':
		case 'F':
			printf("%c", t->t.symbol);
			break;
		case '~':
			printf("(~");
			printTree(t->left);
			printf(")");
			break;
		default:
			printf("(");
			printTree(t->left);
			if (t->t.symbol == 0) {
				printf(" -> ");
			} else if (t->t.symbol == 1) {
				printf(" <-> ");
			} else {
				printf(" %c ", t->t.symbol);
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
