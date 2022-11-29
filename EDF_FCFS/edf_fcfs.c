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

typedef struct task 
{
  /* Initial elements */
  char name[MAX_TASK_NAME_SIZE];
  unsigned int computational_time;  
  unsigned int arrivalDate;      
  unsigned int deadline; 

  /* Used by scheduler */
  unsigned int state;             
  unsigned int computationsDone; 
  unsigned int number_of_executions ;   

} task;

char *states[] = {"admission", "runnable", "running", "terminated"};

char *get_state_string(int state) 
{
  return states[state];
}
void print_tasks(task tasks[], int nbOfTasks, int cur_time) 
{  
if (verbose) 
{
  for(int i = 1; i < nbOfTasks; i++) 
  {
    printf("Task: %s  \tarrivalDate:%d \tstate:%s  \tcomputations:%d/%d \tdeadline_time:%d\n",
	   tasks[i].name,
	   tasks[i].arrivalDate,
	   get_state_string(tasks[i].state),
	   tasks[i].computationsDone,
	   tasks[i].computational_time,
	   (tasks[i].arrivalDate + (tasks[i].number_of_executions + 1) * tasks[i].deadline) - cur_time);
	   
  }
}
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
int EDF(task tasks[], int nbOfTasks, int cur_time) 
{
  int val = 0;
  unsigned int min_deadline;
  unsigned int next;
  unsigned int deadline_time;
  unsigned int scheduled_task = -1;
  for (int i = 1; i < nbOfTasks; i++) 
  {
    if (tasks[i].state == ADMISSION) 
	{
      if (tasks[i].arrivalDate == cur_time) 
	  {
        tasks[i].state = RUNNABLE;
      }
    }
     next = tasks[i].arrivalDate + (tasks[i].number_of_executions + 1) * tasks[i].deadline;
     if (cur_time == next && tasks[i].deadline>0) 
	 {
        if (tasks[i].state == RUNNABLE || tasks[i].state == RUNNING) 
		{
 	        fprintf(stderr, "Task %d has not met its deadline\n", i);
        }
        tasks[i].state = RUNNABLE;
        tasks[i].computationsDone = 0;
        tasks[i].number_of_executions=tasks[i].number_of_executions+1;
     }
      if (tasks[i].state == RUNNING) 
	  {
        if (tasks[i].computationsDone == tasks[i].computational_time) 
		{
  	      tasks[i].state = TERMINATED;
        }
      }
      if (tasks[i].state == RUNNING) {
        tasks[i].state = RUNNABLE;
      }
      if (tasks[i].state == RUNNABLE && val == 0) 
	  {
        min_deadline = (tasks[i].arrivalDate + (tasks[i].number_of_executions + 1) * tasks[i].deadline) - cur_time;
		val = 1;
        scheduled_task = i;
      }
      if (tasks[i].state == RUNNABLE) 
	  {
        deadline_time = (tasks[i].arrivalDate + (tasks[i].number_of_executions + 1) * tasks[i].deadline) - cur_time;
		if (deadline_time < min_deadline) 
		{
		scheduled_task = i;
  	    min_deadline = deadline_time;
        }
      }
  }

  if ( scheduled_task != -1 ) 
  {
    tasks[scheduled_task].computationsDone=tasks[scheduled_task].computationsDone+1;
    tasks[scheduled_task].state = RUNNING;
    return scheduled_task;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  char line [MAX_LINE_SIZE]; 
  task tasks[MAX_NB_OF_TASKS];
  int nbOfTasks = 1;
  int time = 0;
  int task_index;
  int temp_val=0;
  /**** Reading the task file, and storing into a struct ****/
  FILE *file = fopen (argv[1], "r" );
  if (file == NULL) 
  {
    perror(argv[1]);
    return -1;
  }
  printf("Loading file of tasks\n");
  /* Reading the file line by line */
  while ((fgets(line, sizeof line, file) != NULL ))
  {
    sscanf(line, "%s %d %d %d",
	   tasks[nbOfTasks].name,
	   &tasks[nbOfTasks].computational_time,
	   &tasks[nbOfTasks].arrivalDate,
	   &tasks[nbOfTasks].deadline);
    tasks[nbOfTasks].state = ADMISSION;
    tasks[nbOfTasks].computationsDone = 0;
    tasks[nbOfTasks].number_of_executions = 0;
    nbOfTasks++;

  }

  fclose(file);
  if (verbose) 
  {
  printf("%d tasks loaded\n\n", nbOfTasks - 1);
  }
  /**** Scheduling the set of tasks ****/
  if(verbose)
  {
	  printf("Scheduling the set of tasks\n");
  }
  temp_val=50;
  while(hasTasksToSchedule(tasks, nbOfTasks) > 0) 
  {
    for (int i = 0; i < temp_val; i++) 
	{
      task_index = EDF(tasks, nbOfTasks, time);
      if (task_index > 0) 
	  {
	printf("******** Time %d: %s\n\n", time,  tasks[task_index].name);
      }
      else if (task_index == 0) 
	  {
	printf("******** Time %d: no task to schedule\n\n", time);
      }
      time ++;
    }
	return 0;
	printf("\n");
  }

}
