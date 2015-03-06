LEX     = flexc++
PGEN    = bisonc++

lexer			:
	cd lexer; $(LEX) C89.lex

parser			: lexer
	cd parser; $(PGEN) C89.gr

CARM-Compiler.o	: parser
	g++ -std=c++0x compiler.cpp parser/parse.cc lexer/lex.cc