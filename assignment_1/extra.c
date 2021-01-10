#include <stdio.h>
#include <stdlib.h>

/* This imports the type and function declarations for the linear data
 * structures presented in the first lecture: stacks, queues and lists.
 *
 * NOTE You only need ONE of the three.
 * Please remove the other two lines.
 */

// short description of alghorithm//
/* We have a stack in which we register every change of the level by saving the time at */
/* which that change of the level happened. As long as the changes of the leveles are   */
/* increasing, we just save them in the stack, but as soon as the change is decresing we*/
/* start poping from the stack the previous change and time until we achive the same lvl*/

void memoryReallocation (int startIndex, int *arr, int reallocSize) {
	arr = realloc(arr, reallocSize * sizeof(int)) ;
	for (int i = startIndex; i < reallocSize; i++)
		arr[i] = 0;
}

void displayArr(int *arr, int finishingIndex, int flag, int numberOfSignals) {
	if (flag == 0)
		printf("%d\n", numberOfSignals);
	printf("%d\n", finishingIndex);
	for (int i = 0; i < finishingIndex - 1; i++) {
		printf("%d ", arr[i]);
	}
	printf("%d\n", arr[finishingIndex - 1]);
}

int main(int argc, char *argv[]) {
  int *array, nmbrSig, upperBound, lowerBound, sigLen, heigth, arrSize = 1;
  scanf("%d\n", &nmbrSig);
  for (int sigs = 0; sigs < nmbrSig; sigs++) {
    scanf("%d\n", &sigLen);
		arrSize = 1;
		array = malloc(arrSize * sizeof(int));
		array[0] = 0;
    for (int len = 0; len < sigLen; len++) {
			scanf("[%d,%d)@%d%*c", &lowerBound, &upperBound, &heigth);
			if (arrSize < upperBound) {
				memoryReallocation(arrSize, array, upperBound);
				arrSize = upperBound;
			}
			for (int index = lowerBound ; index < upperBound; index++) {
				if (array[index] < heigth)
					array[index] = heigth;
			}
		}
		displayArr(array, arrSize, sigs , nmbrSig);
  }
	//free(array);
}
