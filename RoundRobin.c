#include <stdio.h>

int main() {
	printf("RR simulation$ ");
	char input[20];
	scanf("%s", input);
	printf("Input file is %s\n", input);
	FILE* file = fopen(input, "r");

	char line[20];
	int count = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
	 // printf("#%d: %s", count++, line);
	  int arrival, burst;
	  sscanf(line, "%d\t%d", &arrival, &burst);
	  printf("%d %d", arrival, burst);
	}
}
