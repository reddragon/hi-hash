CXX=g++
FLAGS=-Wall

all: test

test: test.cpp hi_hash.h
	$(CXX) $(FLAGS) -o test test.cpp

clean: 
	rm test

