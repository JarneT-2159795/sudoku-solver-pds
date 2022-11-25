#!/bin/bash
module load GCCcore/8.3.0
g++ -fopenmp -std=c++2a -o main.out *.cpp
