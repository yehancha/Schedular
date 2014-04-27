#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Program {
  int arrival;
  int burst;
  int id;
  struct Program * next;
} Program;

int programCount = 0; // Number of programs in the initial list
int programsIn = 0; // number of programs came into tunning list. used when drawing the chart.
int totalTime;  // Total time processor runs

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
  programCount = 0;
	while (fgets(line, sizeof(line), inputFile) != NULL) {
	  int arrival, burst;
	  sscanf(line, "%d\t%d", &arrival, &burst);
	  Program * temp = newProgram(arrival, burst);
	  temp->id = programCount++;
	  if (home != 0) {
	    current->next = temp;
	  } else {
	    home = temp;
	  }
	  current = temp;
	}
	return home;
}

int calTotalTime(Program * programList) {
  totalTime = 0;
  int lastArrival = 0;
  int lastBurst = 0;
  
  while (programList != 0) {
    totalTime += programList->burst;
    lastArrival = programList->arrival;
    lastBurst = programList->burst;
    programList = programList->next;
  }
  
  if (totalTime < lastArrival + lastBurst - 1) {
    totalTime = lastArrival + lastBurst - 1;
  }
  
  return totalTime;
}

void printChart(int chart[]) {
  int i = 0;
	while (i < programsIn) {
	  printf("#%d:\t", i);
	  int j = 0;
	  int end = totalTime + 1;
	  while (j < end) {
	    if (chart[j] == i) {
	      printf("*");
	    } else {
	      printf(" ");
	    }
	    j++;
	  }
	  printf("\n");
	  i++;
	}
}

int main() {
	FILE * file = getFile();
  validateFile(file);
  
  Program * initialList = createProgramList(file);
  calTotalTime(initialList);
  
  // Stores chart information.
  // chart tells what program was working at each clockTick
  // Ex: chart[13] = 4
  // This tells that in 13th clockTick, #4 program was working. "4" is program id.
  int chart[totalTime];
  // Assign all clockTicks to -1, so they will not represent any program id when starts.
  int i = 0;
  while (i < totalTime) {
    chart[i] = -1;
    i++;
  }
  
  int arrivalTimes[programCount];
  int burstTimes[programCount];
  int finishTimes[programCount];
  int counter = 0;
  // Initializing
  while (counter < programCount) {
    arrivalTimes[counter] = 0;
    burstTimes[counter] = 0;
    finishTimes[counter++] = 0;
  }
	
	Program * runningList = 0; // Will contain running programs at that particular time
	Program * runningTail = 0; // Tail of runningList
	
	int timeQuantum = 3;
	int timeSpent = 0; // time spent by the current program
	
	int clockTick = 0;
	while (initialList != 0 || runningList != 0) { // If we have programs in initialList or runningList
	  
  	printf("\033[2J\033[1;1H"); // clears terminal
  	printf("ClockTick: %d\n", clockTick);
	
	  // Simulating a program coming in
	  int programsComing = 0; // 0=program didn't come in, 1=program came in
	  while (initialList != 0) { // If initialList is not empty
      if (initialList->arrival == clockTick) { // Program is comming in
        // Recording these things for later calculations
        arrivalTimes[initialList->id] = initialList->arrival;
        burstTimes[initialList->id] = initialList->burst;
      
        printf("#%d ", initialList->id);
        programsComing = 1;
        
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
        
        programsIn++;
      } else { // Program is not coming in this clockTick
        break;
      }
	  }
	  
	  if (programsComing == 1) {
	    printf("coming in.\n");
	  } else {
	    printf("\n"); // just adding an empty line to maintain the same number of lines above the chart
	  }
	  
	  if (runningList != 0) { // If the runningList is not empty
	    runningList->burst--; // program spent 1 clockTick
      printf("#%d is working (%d/%d, %d remain)\n", runningList->id, timeSpent + 1, timeQuantum, runningList->burst);
      timeSpent++;
      chart[clockTick] = runningList->id; // This program worked in this clockTick
      
	    if (runningList->burst == 0) { // Program has finished work
	      printf("#%d finished work\n", runningList->id);
	      // Recording finish time
	      finishTimes[runningList->id] = clockTick;
	      // Removing finished program from runningList
	      if (runningTail == runningList) { // If tail == head
	        runningTail = runningList->next;
	      }
	      runningList = runningList->next;
	      // Resetting timeSpent for the next program
	      timeSpent = 0;
	    } else if (timeSpent == timeQuantum) { // Time to change the program
	      // Removing head of runningList
	      if (runningList->next != 0) { // only if there's more than one program
  	      Program * toTail = runningList;
  	      runningList = runningList->next;
  	      toTail->next = 0;
  	      
  	      // Adding it to the tail
  	      runningTail->next = toTail;
  	      runningTail = toTail;
  	      printf("Switched from #%d to #%d\n", toTail->id, runningList->id);
	      } else {
	        printf("\n"); // just adding an empty line to maintain the same number of lines above the chart
	      }
	      
	      // Resetting timeSpent for the next program
	      timeSpent = 0;
	    } else {
	      printf("\n"); // just adding an empty line to maintain the same number of lines above the chart
	    }
	  } else {
	    printf("\n\n"); // just adding an empty line to maintain the same number of lines above the chart
	  }
	  
	  // increment clockTick
	  clockTick++;
	  
	  printf("\n");
	  printChart(chart);
  	
	  // pausing Xsec. per clockTick
	  // (just to make the simulation attractive)
	  // NOTE: In Linux, sleep takes the time in seconds while in Windows, it takes in milliseconds
	  sleep(1);
	}
	
	// Calculate average turnaround time and average waiting time
	double totalTAT = 0;
	double totalWT = 0;
	counter = 0;
	while(counter < programCount) {
	  int turnAroundTime = finishTimes[counter] - arrivalTimes[counter] + 1; // Arrival clock tick is also calculated. So +1.
	  totalTAT += turnAroundTime;
	  totalWT += turnAroundTime - burstTimes[counter++]; // waiting = turnAround - burst;
	}
	
	double averageTAT = totalTAT / programCount;
	double averageWT = totalWT / programCount;
	printf("\nAverage turnaround time=%f\nAverage waiting time=%f\n", averageTAT, averageWT);
}