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
	
	int clockTick = 0;
	while (initialList != 0 || runningList != 0) {
	  // First the initialList will be empty
	  // Then the runningList
	  // So we check initialList first and then runningList
	  Program * current = initialList;
	  while (current != 0) {
	    if (current->arrival == clockTick) { // current Program is comming in
	      // Removing current Program from initialList
	      initialList = current->next;
	      printf("New program comes at %d\n", clockTick);
	    }
	   current = current->next;
	  }
	  
	  // increment clockTick
	  clockTick++;
	  // pausing 1sec. per clockTick
	  // (just to make the simulation attractive)
	  // NOTE: In Linux, sleep takes the time in seconds while in Windows, it takes in milliseconds
	  sleep(1);
	}
}
