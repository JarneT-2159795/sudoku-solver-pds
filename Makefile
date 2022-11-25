all: create compile run

create:
	python3 generator.py

compile:
	g++ -fopenmp -std=c++2a -o main.out *.cpp

compile-vsc:
	module load GCCcore/8.3.0
	g++ -fopenmp -std=c++2a -o main.out *.cpp

run:
	./main.out
