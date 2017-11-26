test: main.o Schedulerread.o Thread.o
	gcc -o test main.o read.o write.o
 
main.o : Init.h Scheduler.h Thread.h main.c
	gcc -c main.c
 
Schedulerread.o : Init.h Scheduler.h Thread.h Scheduler.c
	gcc -c Scheduler.c
 
Thread.o :  Init.h Thread.h Thread.c
	gcc -c Thread.c
