LEX		= flexc++
PGEN	= bisonc++
CXX		= /usr/local/bin/g++-4.6
CXXFLAGS= -std=gnu++0x -Wall -g #-Wfatal-errors

all	: build test

bin/CARM-Compiler	: src/compiler.cpp bin/parser/* bin/lexer/*
	$(CXX) src/*.cpp $(CXXFLAGS) bin/lexer/*.o bin/parser/*.o -Isrc -o bin/CARM-Compiler

bin/parser/*	: bin/lexer src/parser/*
	cd bin/parser; $(CXX) $(CXXFLAGS) -I../../src ../../src/parser/*.cpp -c

bin/lexer/*	: src/lexer/*
	cd bin/lexer; $(CXX) $(CXXFLAGS) -I../../src ../../src/lexer/*.cpp -c

#src/lexer/lex.cpp	: src/lexer/C89.lex
#	cd src; $(LEX) C89.lex

build	: bin/CARM-Compiler

clean	:
	rm -r bin/parser/*; rm -r bin/lexer/*; rm bin/*; make all

test	: build
	./bin/CARM-Compiler test/input/Asample_1.c;
