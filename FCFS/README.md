We will use the following commands for FCFS Algorithm implementation in C:

FORMAT OF THE tasks file:

Task name; computationalTime; arrivalDate;
 
Example:
T1 2 10

COMMANDS:
1. To view the contents of the task file
$ cat test1/tasks2
$ cat test2/tasks2

2. Compile and run the edf.c code
$ make
$ make run

3. Create a file to contain the output of the code
$ ./schedule tasks > mytest1
$ make mytest1 > test1/mytest1

4. To run the test cases of different task files:
$ make mytest1
$ make mytest2