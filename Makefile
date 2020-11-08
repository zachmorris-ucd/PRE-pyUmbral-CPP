pre.out: main.o

main.o: main.cpp main.h
	g++ -g -std=c++14 -Wall -Werror -c main.cpp
