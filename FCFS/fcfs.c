#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE 256
#define MAX_NB_OF_TASKS 32
#define MAX_TASK_NAME_SIZE 32

#define ADMISSION 0
#define RUNNABLE 1
#define RUNNING 2
#define TERMINATED 3
#define VERBOSE_OPTION "-verbose"

int verbose = 0;

typedef struct task {
  /* Initial elements */
  char* name;
  unsigned int totalOfComputations;
  unsigned int arrivalDate;
  /* Used by scheduler */
  unsigned int state;
  unsigned int computationsDone;
} task;

char * states[] = {"admission", "runnable", "running", "terminated"};

char * getStateString(int state) {
  return states[state];
}

/* Returns the number of tasks that still have to be run, */
/* that is, all tasks which still have computations to perform */
int hasTasksToSchedule(task tasks[], int nbOfTasks) {
  int total = 0;
  int i;

  for(i=0; i<nbOfTasks; i++) {
    if (tasks[i].state != TERMINATED) {
      total ++;
    }
  }
  return total;
}


/* Returns the index of the selected task */
/* Return -1 if no task could be selected */
int  FCFS(task tasks[], int nbOfTasks, int currentTime) {
  int i;

  // First admit task if currentTime >= arrivalTime
  for(i=0; i<nbOfTasks; i++) {
    if (tasks[i].state == ADMISSION) {
      if (tasks[i].arrivalDate == currentTime) {
	tasks[i].state = RUNNABLE;
      }
    }
  }

  // Is a task running? Has that running tasks finished its computations? 
  //   If yes, put it in terminated state
  //   If no, continue this task
  for(i=0; i<nbOfTasks; i++) {
    if (tasks[i].state == RUNNING) {
      if (tasks[i].computationsDone == tasks[i].totalOfComputations) {
	tasks[i].state = TERMINATED;
	break;
      } else {
	/* Select this task */
	tasks[i].computationsDone ++;
	return i;
      }
    }
  }

  // Otherwise, select a new runnable task
  for(i=0; i<nbOfTasks; i++) {
    if (tasks[i].state == RUNNABLE) {
      if (tasks[i].computationsDone < tasks[i].totalOfComputations) {
	tasks[i].computationsDone ++;
	tasks[i].state = RUNNING;
	return i;
      }
    }
  }

  // No task could be selected
  return -1;
}


void printTasks(task tasks[], int nbOfTasks) {
  int i;
  if (verbose) {
    for(i=0; i<nbOfTasks; i++) {
      printf("Task: %s  arrivalDate:%d state:%s  computations:%d/%d\n", tasks[i].name, tasks[i].arrivalDate, getStateString(tasks[i].state), tasks[i].computationsDone, tasks[i].totalOfComputations);
    }
  }
}
// We will be sorting all tasks by the arrivalTime
void sorting_Tasks(task tasks[], int n) 
{
	task k;
	for(int i=0; i<n-1; i++) {
		for(int j=0; j<n-i-1; j++) {
			
				if(tasks[j].arrivalDate>tasks[j+1].arrivalDate) {
					k = tasks[j];
					tasks[j] = tasks[j+1];
					tasks[j+1] = k;
				}
			
		}
	}
}

int main(int argc, char *argv[]){
  char line [MAX_LINE_SIZE]; /* or other suitable maximum line size */
  task tasks[MAX_NB_OF_TASKS];
  int nbOfTasks = 0;
  int time = 0;
  int taskIndex;

  /* reading arguments */
  if (argc < 2) {
    printf("Missing arguments: schedule <input file> [-verbose]\n");
    exit(1);
  }

  /* Checking for the presence of the verbose option */
  if (argc > 2) {
    if (strcmp(argv[2], VERBOSE_OPTION) == 0) {
      verbose = 1;
    }
  }

 
  /**** Reading the task file, and storing into a struct ****/
  FILE *file = fopen (argv[1], "r" );
  if (file == NULL) {
    perror(argv[1]);
    return -1;
  }

  /* Reading the file line by line */
  if (verbose) {printf("Loading file of tasks\n");}
  
  while (fgets(line, sizeof line, file) != NULL ) {
    char *str=(char*)(malloc(sizeof(char)*MAX_TASK_NAME_SIZE));
		tasks[nbOfTasks].name=str;

    sscanf(line, "%s %d %d", tasks[nbOfTasks].name, &tasks[nbOfTasks].totalOfComputations, &tasks[nbOfTasks].arrivalDate);
    tasks[nbOfTasks].state = ADMISSION;
    tasks[nbOfTasks].computationsDone = 0; 
    nbOfTasks ++;
  }
  fclose(file);

  if (verbose) {
    printf("%d tasks loaded\n\n", nbOfTasks);
  }
  
  /**** Scheduling the set of tasks ****/
  if (verbose) {
    printf("Scheduling the set of tasks\n");
  }
  /*Calling the function to do sorting based on Arrival Time*/
  sorting_Tasks(tasks,nbOfTasks);

  while(hasTasksToSchedule(tasks, nbOfTasks) > 0) {
    printTasks(tasks, nbOfTasks);
    taskIndex = FCFS(tasks, nbOfTasks, time);
    if (taskIndex >= 0) {
      printf("Time %d: %s\n", time,  tasks[taskIndex].name);
    } else {
      if (hasTasksToSchedule(tasks, nbOfTasks) > 0) {
	printf("Time %d: no task to schedule\n", time);
      }
    }
    time ++;
  }
  
  
    /**** That's all folks ****/
  printTasks(tasks, nbOfTasks);
  time --;
  if (verbose) {
    printf("All done after %d units of time\n", time);
  }
  return 0;
}
  

