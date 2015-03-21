LEX			= flexc++
CXX			= /usr/local/bin/g++-4.6
CXXFLAGS	= -std=gnu++0x -Wall -Wextra -pedantic -g #-Wfatal-errors

SRC_C		:= $(wildcard src/*.{cpp,h})
SRC_P		:= $(wildcard src/parser/*.cpp)
SRC_L		:= $(wildcard src/lexer/*.cpp)

OBJ_P		:= $(patsubst src/%.cpp, bin/%.o, $(SRC_P))
OBJ_L		:= $(patsubst src/%.cpp, bin/%.o, $(SRC_L))

build				: bin/compiler

bin/compiler		: $(SRC_C) $(OBJ_P) $(OBJ_L)		| bin/
	$(CXX) $(SRC_C) $(CXXFLAGS) $(OBJ_L) $(OBJ_P) -Isrc -o bin/compiler

bin/parser/%.o		: src/parser/%.cpp src/parser/%.h	| bin/ $(OBJ_L)
	$(CXX) -c $(CXXFLAGS) -Isrc -o $@ $<

bin/lexer/%.o		: src/lexer/%.cpp src/lexer/%.h		| bin/
	$(CXX) -c $(CXXFLAGS) -Isrc -o $@ $<

bin/				:
	mkdir bin; mkdir bin/lexer; mkdir bin/parser

# fell down a giant hole trying to get flexc++ running on mac
#	- flexit.sh compiles on emulated arm/linux and pulls back flex output
src/lexer/lex.cpp	: src/lexer/C89.lex
	./flexit.sh

all					: build test_compile

clean				:
	rm -r bin; make build

test_compile		: build
	cd test; make build

test				: test_compile
	cd test; make test
