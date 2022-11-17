all: create compile run

create:
	python3 generator.py

compile:
	g++ -g -pthread -fopenmp -std=c++2a -o main.out *.cpp

run:
	./main.out
