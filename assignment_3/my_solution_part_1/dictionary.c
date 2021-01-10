// implements a dictionary

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

dict *newEmptyDict() {
  dict *d = malloc(sizeof(dict));
  if (d == NULL) {
    return NULL;
  }
  d->numWords = 0;
  d->maxWords = 1;
  d->words = malloc(sizeof(char*) * d->maxWords);
  return d;
}

// add word to the dictionary if it is is not already known
void addWord(char word[LENGTH + 1], dict *d) {
  if (!check(word, d)) {
    // if we need more space before adding the word, double the size
    if (d->numWords == d->maxWords) {
      d->words = realloc(d->words,(sizeof(char*)) * (d->maxWords * 2));
      if (d->words == NULL) {
        printf("Out of memory.\n");
        exit(-1);
      }
      d->maxWords = d->maxWords * 2;
    }
    // now we actually add the word
    d->words[d->numWords] = malloc(sizeof(char) * (LENGTH + 1));
    strcpy(d->words[d->numWords],word);
    d->numWords++;
  }
}

// check whether word is in dictionary
bool check(const char *word, dict *d) {
  for (int i = 0; i < d->numWords; i++) {
    if (strcmp(d->words[i],word) == 0) {
      return true;
    }
  }
  return false;
}

void freeDict(dict *d) {
  for (int i = 0; i < d->numWords; i++) {
    free(d->words[i]);
  }
  free(d->words);
  free(d);
}
