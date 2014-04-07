#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Program {
  int arrival;
  int burst;
  struct Program * next;
} Program;

FILE * getFile() {
	printf("RR simulation$ ");
	char input[20];
	scanf("%s", input);
	printf("Input file is %s\n", input);
	return fopen(input, "r");
}

void validateFile(FILE * file) {
  if (file == NULL) {
    exit(0);
  }
}

Program * newProgram(int arrival, int burst) {
  Program * program = (Program *) malloc(sizeof(Program));
  program->arrival = arrival;
  program->burst = burst;
  program->next = 0;
  return program;
}

Program * createProgramList(FILE * inputFile) {
  char line[20];
	Program * home = 0, * current;
	while (fgets(line, sizeof(line), inputFile) != NULL) {
	  int arrival, burst;
	  sscanf(line, "%d\t%d", &arrival, &burst);
	  Program * temp = newProgram(arrival, burst);
	  if (home != 0) {
	    current->next = temp;
	  } else {
	    home = temp;
	  }
	  current = temp;
	}
	return home;
}

int main() {
	FILE * file = getFile();
  validateFile(file);
  
  Program * initialList = createProgramList(file);
	
	Program * current = initialList;
	while (current != 0) {
	  printf("%d %d\n", current->arrival, current->burst);
	  current = current->next;
	}
	
	Program * runningList = 0;
	Program * runningTail = 0; // Tail of runningList
	
	int clockTick = 0;
	while (initialList != 0 || runningList != 0) { // If we have programs in initialList or runningList
	
	  // Simulating a program coming in
	  while (initialList != 0) {
      if (initialList->arrival == clockTick) { // Program is comming in
        // Seperate the program from initialList
        Program * programComingIn = initialList;
        initialList = initialList->next;
        programComingIn->next = 0;
        
        // Add the program to the runningList
        if (runningTail != 0) { // runningList has a tail, means it is not empty
          runningTail->next = programComingIn;
          runningTail = runningTail->next;
        } else { // runningList has no tail, means it is empty
          runningList = programComingIn;
          runningTail = runningList;
        }
        
        printf("New program comes at %d\n", clockTick);
      } else { // Program is not coming in this clockTick
        break;
      }
	  }
	  
	  // increment clockTick
	  clockTick++;
	  // pausing 1sec. per clockTick
	  // (just to make the simulation attractive)
	  // NOTE: In Linux, sleep takes the time in seconds while in Windows, it takes in milliseconds
	  sleep(1);
	}
}