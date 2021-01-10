#include <stdio.h>
#include <stdlib.h>

/* This imports the type and function declarations for the linear data
 * structures presented in the first lecture: stacks, queues and lists.
 *
 * NOTE You only need ONE of the three.
 * Please remove the other two lines.
 */
 #include "LibStack.h"

// short description of alghorithm//
/* We have a stack in which we register every change of the level by saving the time at */
/* which that change of the level happened. As long as the changes of the leveles are   */
/* increasing, we just save them in the stack, but as soon as the change is decresing we*/
/* start poping from the stack the previous change and time until we achive the same lvl*/

int main(int argc, char *argv[]) {
  int nmbrSig, currHeigth, nextHeigth, length, time;
  Stack st;
  scanf("%d", &nmbrSig);
  for (int sigCounter = 0; sigCounter < nmbrSig; sigCounter++) {
		// a loop that implements the algh for n-signals
    scanf("%d", &length); //length of signal
    st = newStack(length);
		currHeigth = 0;
    for (time = 0; time < length; time++) { // goes through the signal
      scanf("%d", &nextHeigth);
      if (nextHeigth > currHeigth) { // compares the lvls of signal
        for (int sigLvl = currHeigth; sigLvl < nextHeigth; sigLvl++) {
          push(time, &st); // change of lvl is positive? -> push it on stack
        }
      } else if (nextHeigth < currHeigth){
        for (int sigLvl = currHeigth; sigLvl > nextHeigth; --sigLvl) {
          printf("[%d,%d)@%d ", pop(&st), time, sigLvl);
					//change of lvl is negative? pop from stack until the previuos lvl is = to current
        }
      }
			currHeigth = nextHeigth; // saving for the next comparation
    }
		while (!isEmptyStack(st)) { // if we read thorugh the signal, and the stack is not empty, pop the reamining data
			printf("[%d,%d)@%d ", pop(&st), time, currHeigth);
			currHeigth--;
		}
		printf("\n");
		freeStack(st);
  }
  return 0;
}
