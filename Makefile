LEX		= flexc++
PGEN	= bisonc++
CXX		= /usr/local/bin/g++-4.6
CXXFLAGS= -std=gnu++0x -Wall -Wfatal-errors -g
LDFLAGS	= -lboost_regex


bin/CARM-Compiler	: bin/parser bin/lexer
	$(CXX) src/*.cpp $(CXXFLAGS) bin/*.o -o bin/CARM-Compiler

bin/parser	: bin/lexer
	cd bin; $(CXX) ../src/parser/*.cpp $(CXXFLAGS) -c

bin/lexer	: #src/lexer/lex.cpp
	cd bin; $(CXX) ../src/lexer/*.cpp $(CXXFLAGS) -c

src/lexer/lex.cpp	: src/lexer/C89.lex
	cd src; $(LEX) C89.lex

all	: bin/CARM-Compiler

clean	:
	rm bin/*; make all
