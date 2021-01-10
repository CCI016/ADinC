// declares a dictionary

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
#define LENGTH 45

// a dictionary is an array
typedef struct dict {
  int numWords;
  int maxWords;
  char **words;
} dict;

dict *newEmptyDict();
void addWord(char word[LENGTH + 1], dict *d);
bool check(const char *word, dict *d);
void freeDict(dict *n);

#endif // DICTIONARY_H
