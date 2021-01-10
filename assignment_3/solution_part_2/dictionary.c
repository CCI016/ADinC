/* Students : Cainarean Constantin S4142152; c.cainarean@sudent.rug.nl
							Garabajiu Denis S4142551; d.garabajiu@student.rug.nl
   Date: 02.03.2020 */


//implementing a dictionary using tries

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Creates a new node
dict *newEmptyNode() {
  dict *d = malloc(sizeof(dict));
  if (d == NULL) {
    return NULL;
  }
  d->isWord = false;
	for (int i = 0; i < ALPHABET; i++) {
		d->children[i] = NULL;
	}
  return d;
}

// add word to the dictionary if it is is not already known
void addWord(char word[LENGTH + 1], dict *d) {
	int index = 0;
	dict *currentNode = d;
	while (word[index] != '\0') {
		// adds a new letter to the path if it is not known
		if (currentNode->children[word[index] - CASE] == NULL) {
			currentNode->children[word[index] - CASE] = newEmptyNode();
		}
		currentNode = currentNode->children[word[index] - CASE];
		index++;
	}
	currentNode->isWord = true; // marking the ending of a word
}

// check whether word is in dictionary
bool check(const char *word, dict *d) {
	int index = 0;
	dict *currentNode = d;
	while (word[index] != '\0') {
		//check if we have such letter in our trie
		if (currentNode->children[word[index] - CASE] == NULL) {
			return false; 
		}
		currentNode = currentNode ->children[word[index] - CASE];
		index++;
	}
	// is the end of a word? -> true 
	// checking for cases as :big and bigger;
	if (currentNode->isWord == true)
		return true;
	return false;
}

// frees our trie
void freeDict(dict *d) {
	for (int i = 0; i < ALPHABET; i++) {
		if (d->children[i] != NULL) {
			freeDict(d->children[i]);
		}
	}
	free(d);
}
