all: compile run

compile:
	g++ -g -std=c++20 -o main.out *.cpp

run:
	./main.out
