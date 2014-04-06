#include <stdio.h>
#include <stdlib.h>

typedef struct Program {
  int arrival;
  int burst;
  struct Program * next;
} Program;

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
	printf("RR simulation$ ");
	char input[20];
	scanf("%s", input);
	printf("Input file is %s\n", input);
	FILE * file = fopen(input, "r");

  Program * home;
  if (file != NULL) {
	  home = createProgramList(file);
  } else {
    exit(1);
  }
	
	Program * current = home;
	while (current != 0) {
	  printf("%d %d\n", current->arrival, current->burst);
	  current = current->next;
	}
}
