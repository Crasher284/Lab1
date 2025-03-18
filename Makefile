all: test.exe

run: test.exe
	.\test.exe

test.exe: main.o dynamic.o dynamic_int.o dynamic_double.o
	gcc -g main.o dynamic.o dynamic_int.o dynamic_double.o -o test.exe --std=c99

main.o: main.c
	gcc -g -c main.c -o main.o --std=c99

dynamic.o: dynamic.c
	gcc -g -c dynamic.c -o dynamic.o --std=c99

dynamic_int.o: dynamic_int.c
	gcc -g -c dynamic_int.c -o dynamic_int.o --std=c99

dynamic_double.o: dynamic_double.c
	gcc -g -c dynamic_double.c -o dynamic_double.o --std=c99

.PHONY: clean all

clean:
	del *.o

fullclean: clean
	del *.exe