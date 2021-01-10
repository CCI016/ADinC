/* Students : Cainarean Constantin S4142152; c.cainarean@sudent.rug.nl
							Garabajiu Denis S4142551; d.garabajiu@student.rug.nl
   Date: 02.03.2020 */

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

  // step 1: read in the dictionary
  dict *dictionary = newEmptyNode();
  while (scanf("%45s",word) && word[0] != '!') {
    trimWord(word);
    addWord(word,dictionary);
  }

  // step 2: read in text
  int counter = 0; // number of unknown words

  int index = 0;
  int c = EOF;
  getchar();
	while ((c = getchar()) && c != EOF) {
		if (isalpha(c)) { // If the char is a letter write it in the array
			word[index] = c;
			index++;
		} else { // finish the current-word array
			trimWord(word);
			word[index] = '\0';
			if (!check(word,dictionary) && word[0] != '\0')  { // If the word is not in the dictionary
					counter++;
					printf ("%s\n",word);
			}
			index = 0;
		}
	}
  // step 3: print number of unknown words
  printf("%d\n", counter);
  freeDict(dictionary);
  return 0;
}
