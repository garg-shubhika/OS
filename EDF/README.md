EDF (Earliest Deadline First) Algorithm Implementation in C:

tasksEDF INPUT FILE FORMAT:
Task name; computationalTime; arrivalTime; absoluteDeadline
Example:
T1 2 1 4

COMMANDS:
1. To view the contents of the task file
$ cat test1/tasksEDF
$ cat test2/tasksEDF
$ cat test3/tasksEDF

2. Compile and run the edf.c code
$ make
$ make run

3. Create a file to contain the output of the code
$ ./schedule tasksEDF > mytest2
$ ./schedule test1/tasksEDF >test1/test1
$ make mytest1 > test1/mytest1

4. To run the test cases of different task files:
$ make mytest1
$ make mytest2
$ make mytest3