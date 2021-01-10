// a SLOW spell checker Denis G

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
  int counter = 0, index = 0, c = EOF;

  // step 1: read in the dictionary
  dict *dictionary = newEmptyDict();
  while (scanf("%45s",word) && word[0] != '!') {
    trimWord(word);
    addWord(word,dictionary);
  }
  getchar();
  // step 2: read in text
  while ((c = getchar()) && c != EOF) {
    if (isalpha(c)) {
      word[index] = c;
      index++;
    } else {
      trimWord(word); 
      word[index] = '\0';
      if (word[0] != '\0' && !check(word, dictionary)) {
        counter++;
        printf("%s\n", word);
      }
      index = 0;
    }
  }
  if (word[0] != '\0' && !check(word, dictionary)) {
    counter++;
    printf("%s\n", word);
  }
  printf("%d\n", counter);
  freeDict(dictionary);
  return 0;
}
