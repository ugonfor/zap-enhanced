all: zap

zap: 
	gcc -c -o ./include/util.o ./include/util.c
	gcc -c -o ./include/zap.o ./include/zap.c
	gcc -c -o ./include/utmp/utmp_util.o ./include/utmp/utmp_util.c
	gcc -c -o ./main.o ./main.c
	gcc -o zap ./main.o ./include/util.o ./include/utmp/utmp_util.o ./include/zap.o

clean: 
	rm *.o
	rm ./include/*.o
	rm ./include/utmp/*.o
	rm zap
	rm peda*
	rm ./utmp
	rm ./wtmp