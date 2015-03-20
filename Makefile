LEX			= flexc++
CXX			= /usr/local/bin/g++-4.6
CXXFLAGS	= -std=gnu++0x -Wall -Wextra -pedantic -g #-Wfatal-errors

SRC_C		:= $(wildcard src/*.cpp)
SRC_P 		:= src/parser/$(wildcard *.cpp)
SRC_L		:= src/lexer/$(wildcard *.cpp)

OBJ_P		:= $(patsubst src/%.cpp, bin/%.o, $(wildcard src/parser/*.cpp))
OBJ_L		:= $(patsubst src/%.cpp, bin/%.o, $(wildcard src/lexer/*.cpp))

build				: bin/compiler

bin/compiler		: $(SRC_C) $(OBJ_P) $(OBJ_L)	| bin/
	$(CXX) $(SRC_C) $(CXXFLAGS) $(OBJ_L) $(OBJ_P) -Isrc -o bin/compiler

bin/parser/%.o		: src/parser/%.cpp $(OBJ_L)	| bin/
	$(CXX) -c $(CXXFLAGS) -Isrc -o $@ $<

bin/lexer/%.o		: src/lexer/%.cpp	| bin/
	$(CXX) -c $(CXXFLAGS) -Isrc -o $@ $<

bin/				:
	mkdir bin; mkdir bin/lexer; mkdir bin/parser

# fell down a giant hole trying to get flexc++ running on mac
#	- flexit.sh compiles on emulated arm/linux and pulls back flex output
src/lexer/lex.cpp	: src/lexer/C89.lex
	./flexit.sh

all					: build test

clean				:
	rm -r bin; make all

test_compile		: build
	cd test; make build

test				: test_compile
	cd test; make test
