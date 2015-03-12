LEX	= flexc++
PGEN= bisonc++

lexer.o			:
	cd lexer; $(LEX) C89.lex

parser.o		: lexer.o
	cd parser; $(PGEN) C89.gr -V

CARM-Compiler.o	: parser.o
	g++ -std=c++0x compiler.cpp parser/parse.cc lexer/lex.cc