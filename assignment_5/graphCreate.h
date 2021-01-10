/* scanner.h for assignment 4 on logical formulas */

#ifndef GRAPHCREATE_H
#define GRAPHCREATE_H

#define towns 11 //Number of Towns
int **memoryAlloc(int height, int width);
char **alloc(int heigth, int width);
void createNodes(char **t);
void createEdges(int **t, char **nodes);
int indexFinder(char **nodes, char *town);


#endif
