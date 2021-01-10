/* Students : Cainarean Constantin S4142152; c.cainarean@sudent.rug.nl
							Garabajiu Denis S4142551; d.garabajiu@student.rug.nl
   Date: 29.03.2020 */

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "graphCreate.h"

int **memoryAlloc(int height, int width) {
  int **tab;
  tab = malloc(height * sizeof(int *));
  for (int col = 0; col < height; col++) {
    tab[col] = malloc(width * sizeof (int));
  }
  return tab;
}

char **alloc(int heigth, int width) {
	char **tab;
	tab = malloc(sizeof(char *) * heigth);
	for (int col = 0; col < heigth; col++) {
		tab[col] = malloc(sizeof(char) * width);
	}
	return tab;
}

void introduceInNodes(char **t, char *string, int row) {
	for (int index = 0; index < 3; index++) {
		t[row][index] = string[index];
	}
	t[row][3] = '\0';
}

//creates an array wich contains the indexes of the cities
void createNodes(char **t) {
	introduceInNodes(t, "Lee", 0); //Leewarden
	introduceInNodes(t, "Gro", 1); //Groningen
	introduceInNodes(t, "Mep", 2); //Mappel
	introduceInNodes(t, "Zwo", 3); //Zwolle
	introduceInNodes(t, "Ens", 4); //Enschede
	introduceInNodes(t, "Utr", 5); //Utrecht
	introduceInNodes(t, "Ams", 6); //Amsterdam
	introduceInNodes(t, "Den", 7); //Den Haag
	introduceInNodes(t, "Nij", 8); //Nijmegen
	introduceInNodes(t, "Ein", 9); //Eindhoven
	introduceInNodes(t, "Maa", 10); //Maastricht
}

//creates the undirected relation between 2 nodes
void introduceInEdges(int **t, int row, int column, int weigth) {
	t[row][column] = weigth;
	t[column][row] = weigth;
}

//initialization of out Adjacency Matrix
void edgeInitialization(int **t) {
	for (int i = 0; i < towns; i++) {
		for (int z = 0; z < towns; z++) {
			t[i][z] = -1;
		}
	}
}

//finds the index of a town in the nodes array
int indexFinder(char **nodes, char *town) {
	for(int i = 0; i < towns; i++) {
		if (!strcmp(town, nodes[i])) {
			return i;
		}
	}
	return -1;
}

void disruptions(int **t, char **nodes) {
	int disNmbr = 0;
	char firstTown[4], secondTown[4];
	scanf("%d", &disNmbr);
	while (disNmbr != 0) {
		scanf("%3s%*s", firstTown);
		scanf("%3s%*s", secondTown);
		introduceInEdges (t, indexFinder(nodes,firstTown), indexFinder(nodes,secondTown), -1);
		disNmbr--;
	}
}

// creates the initial Adjacency Matrix
void createEdges(int **t, char **nodes) {
	edgeInitialization(t);
	introduceInEdges(t, 0, 1, 34);
	introduceInEdges(t, 1, 2, 50);
	introduceInEdges(t, 0, 2, 40);
	introduceInEdges(t, 2, 3, 15);
	introduceInEdges(t, 3, 4, 50);
	introduceInEdges(t, 3, 5, 51);
	introduceInEdges(t, 3, 8, 77);
	introduceInEdges(t, 5, 6, 26);
	introduceInEdges(t, 5, 9, 47);
	introduceInEdges(t, 6, 7, 51);
	introduceInEdges(t, 7, 9, 89);
	introduceInEdges(t, 9, 10, 63);
	introduceInEdges(t, 9, 8, 55);
	introduceInEdges(t, 8, 10, 111);
	disruptions(t, nodes);
}
