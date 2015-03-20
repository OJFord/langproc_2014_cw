LEX		= flexc++
CXX		= /usr/local/bin/g++-4.6
CXXFLAGS= -std=gnu++0x -Wall -Wextra -pedantic -g #-Wfatal-errors

all	: bin build test

bin :
	mkdir bin; mkdir bin/lexer; mkdir bin/parser

bin/CARM-Compiler	: src/compiler.cpp bin/parser/* bin/lexer/*
	$(CXX) src/*.cpp $(CXXFLAGS) bin/lexer/*.o bin/parser/*.o -Isrc -o bin/CARM-Compiler

bin/parser/*	: bin/lexer/* src/parser/*
	cd bin/parser; $(CXX) $(CXXFLAGS) -I../../src ../../src/parser/*.cpp -c

bin/lexer/*	: src/lexer/*
	cd bin/lexer; $(CXX) $(CXXFLAGS) -I../../src ../../src/lexer/*.cpp -c

# fell down a giant hole trying to get flexc++ running on mac
# flexit.sh compiles on emulated arm/linux and pulls back flex output
src/lexer/lex.cpp	: src/lexer/C89.lex
	./flexit.sh

build	: bin/CARM-Compiler

clean	:
	rm -r bin; make all

test	: build
	./bin/CARM-Compiler test/input/A_sample1.c -v
