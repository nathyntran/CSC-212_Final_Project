output: main.o boyer.o rabin.o
	g++ main.o boyer.o rabin.o -o output

main.o: main.cpp
	g++ -c main.cpp

boyer.o: boyer.cpp boyer.h
	g++ -c boyer.cpp

rabin.o: rabin.cpp rabin.h
	g++ -c rabin.cpp

remove:
	rm *.o output

run:
	make

# This is where you can change stuff to fit your code
# ./output <text name> <pattern file name> <size of search>
#										   don't choose a number that is lower than 6, but if you type 0, it searches the entire pattern
# TYPE "make run" PLEASE
	./output text_1.txt text_2.txt 15

	make remove
