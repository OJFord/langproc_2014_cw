LEX     = flexc++
PGEN    = bisonc++
objects = lexer.o parser.o CARM-Compiler.o

CARM-Compiler	: $(objects)

lexer.o			: #parser/parser.cpp
	cd lexer; $(LEX) lexer.lex

parser.o		: lexer.o
	cd parser; $(PGEN) C89.gr --target-directory gen

CARM-Compiler.o	: compiler.cpp
