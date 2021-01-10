/* Students : Cainarean Constantin S4142152; c.cainarean@sudent.rug.nl
							Garabajiu Denis S4142551; d.garabajiu@student.rug.nl
   Date: 02.03.2020 */

// declares a dictionary using tries;

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
#define LENGTH 45
#define CASE 'a'
#define ALPHABET 26

// a dictionary is an array
typedef struct dict {
  bool isWord;
  struct dict *children[26];
} dict;

dict *newEmptyNode();
void addWord(char word[LENGTH + 1], dict *d);
bool check(const char *word, dict *d);
void freeDict(dict *n);

#endif // DICTIONARY_H
