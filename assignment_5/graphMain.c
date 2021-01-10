/* Students : Cainarean Constantin S4142152; c.cainarean@sudent.rug.nl
							Garabajiu Denis S4142551; d.garabajiu@student.rug.nl
   Date: 29.03.2020 */

#include <stdio.h>
#include <stdlib.h> 
#include "graphCreate.h"
#include "graphSearch.h"

//2 functions for freeing the Nodes array and Edges array
void freeNodes(char **tab, int height) {
  for (int row = 0; row < height; row++) {
    free(tab[row]);
  }
  free(tab);
}

void freeEdges(int **tab, int height) { //
  for (int row = 0; row < height; row++) {
    free(tab[row]);
  }
  free(tab);
}

//Prints a town corresponding to a certain index
void townPrint(int index) {
	if (index == 0) {
		printf("Leeuwarden\n");
	} else if (index == 1) {
		printf("Groningen\n");
	} else if (index == 2) {
		printf("Meppel\n");
	} else if (index == 3) {
		printf("Zwolle\n");
	} else if (index == 4) {
		printf("Enschede\n");
	} else if (index == 5) {
		printf("Utrecht\n");
	} else if (index == 6) {
		printf("Amsterdam\n");
	} else if (index == 7) {
		printf("Den Haag\n");
	} else if (index == 8) {
		printf("Nijmegen\n");
	} else if (index == 9) {
		printf("Eindhoven\n");
	} else if (index == 10) {
		printf("Maastricht\n");
	}
}

int main(int argc, char **argv) {
	int **edges, path[12], currentPath[12], time, goalTownIndex;
	char **nodes, firstTown[4], goalTown[4], stop;
	edges = memoryAlloc(towns, towns);
	nodes = alloc(towns, 4);
	//creating the nodes and the edges
	createNodes(nodes);
	createEdges(edges, nodes);
	//scanning the towns
	getchar();
	stop = getchar();
	while (stop != '!') {
		time = 9999;
		//initializing the path and currentPath with -1 in all cells in order to avoid any errors
		for (int z = 0; z < 12; z++) {
			path[z] = -1;
			currentPath[z] = -1;
		}
		//scanning the first and the goal town
		firstTown[0] = stop;
		scanf("%c%c%*s", &firstTown[1], &firstTown[2]);
		firstTown[3] = '\0';
		scanf("%3s%*s", goalTown);
		currentPath[0] = indexFinder(nodes, firstTown);
		goalTownIndex = indexFinder(nodes, goalTown); // saving the last town's index
		pathSearch(edges, currentPath[0], goalTownIndex, path, currentPath, &time, 0, 1); //finds the best route from first city to final town 
		//printing the path
		for (int i = 0; i < 12; i++) {
			if (path[0] == -1) {
				printf("UNREACHABLE\n");
				break;
			} else if (path[i] == -1) {
				townPrint(goalTownIndex);
				printf("%d\n", time);
				break;
			}
				townPrint(path[i]);
		}
		getchar();
		stop = getchar();
	}
	freeEdges(edges, 11); //free
	freeNodes(nodes, 11); //free
}
