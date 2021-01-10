// a SLOW spell checker

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// remove non-alphabetic characters and convert to lower case
void trimWord(char *word) {
  int k = 0;
  for (int i = 0; i < (int) strlen(word); i++) {
    if (isalpha(word[i])) {
      word[k] = tolower(word[i]);
      k++;
    }
  }
  word[k] = '\0';
}

int main(int argc, char *argv[]) {

  char word[LENGTH + 1] = "";

  // step 1: read in the dictionary
  dict *dictionary = newEmptyDict();
  while (scanf("%45s",word) && word[0] != '!') {
    trimWord(word);
    addWord(word,dictionary);
  }

  // step 2: read in text
  int counter = 0; // number of unknown words

  // BUG: This loop is wrong. It will read "one,twwo" as one word "onetwwo".
  while (scanf("%45s", word) != EOF) {
    trimWord(word);
    if (!check(word,dictionary)) {
      counter++;
      printf("%s\n",word);
    }
  }
  // TODO: Replace the above while loop with a correct solution.
  // Hints:
  // - you should read one character at a time, using getchar()
  // - alphabetical characters should be appended to the current word
  // - any other symbol should terminate the word
  // this code might be useful:
  /*
  int index = 0;
  int c = EOF;
  while ((c = getchar()) && c != EOF) {
    // ...
  }
  */

  // step 3: print number of unknown words
  printf("%d\n", counter);

  freeDict(dictionary);
  return 0;
}
