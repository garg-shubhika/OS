Implementation of EDF (Earliest Deadline First) and FCFS (First-come and First-served) in C:

tasksEDF_FCFS Format:
Task name; computationalTime; arrivalTime; absolute deadline
Example:
T1 2 1 4
where computationalTime=2, arrivalTime=1 and absolute deadline=4

In case, there is no deadline:
T1 2 0
where computationalTime=2 and arrivalTime=0

COMMANDS:
1. To view the contents of the task file
$ cat test1/tasksEDF_FCFS
$ cat test2/tasksEDF_FCFS
$ cat test3/tasksEDF_FCFS
$ cat test4/tasksEDF_FCFS

2. Compile and run the edf_fcfs.c code
$ make
$ make run

3. Create a file to contain the output of the code
$ ./schedule tasksEDF_FCFS > mytest
$ make mytest1 > test1/mytest1

4. To run the test cases of different task files:
$ make mytest1
$ make mytest2
$ make mytest3
$ make mytest4