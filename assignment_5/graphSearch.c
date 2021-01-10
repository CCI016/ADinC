/* Students : Cainarean Constantin S4142152; c.cainarean@sudent.rug.nl
							Garabajiu Denis S4142551; d.garabajiu@student.rug.nl
   Date: 29.03.2020 */

int cycleCheck(int *path, int city) {
	int index = 0;
	while (path[index] != -1) {
		if (path[index] == city) {
			return 1;
		}
		index++;	
	}
	return 0;
}

void pathSearch(int **t, int currentTown, int goalTown, int *bestPath, int *currentPath, int *bestTime, int currentTime, int index) {
	if (t[currentTown][goalTown] != -1 || currentTown == goalTown) {
		if (*bestTime > currentTime + t[currentTown][goalTown]) {
			*bestTime = currentTime + t[currentTown][goalTown];
			for (int i = 0; i < 12; i++) {
				bestPath[i] = currentPath[i];
			}
			//currentPath[index] = -1;
		}
		return;
	}
	for (int i = 0; i < 11; i++) {
		if (t[currentTown][i] != -1) {
			if (!cycleCheck(currentPath, i)) {
				currentPath[index] = i;
				currentPath[index + 1] = -1;
				pathSearch(t, i, goalTown, bestPath, currentPath, bestTime, currentTime + t[currentTown][i], index + 1);
				currentPath[index] = -1;
			}
		}
	}
}
