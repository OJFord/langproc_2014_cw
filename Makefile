LEX			= flexc++
CXX			= /usr/local/bin/g++-4.6
CXXFLAGS	= -std=gnu++0x -Wall -Wextra -pedantic -g #-Wfatal-errors
LDFLAGS		= -L/usr/local/lib -lboost_regex 

INC			= -Isrc -I/usr/local/include

SRC_C		:= $(wildcard src/*.cpp)
HED_C		:= $(wildcard src/*.h)
SRC_P		:= $(wildcard src/parser/*.cpp)
HED_P		:= $(wildcard src/parser/*.h)
SRC_L		:= $(wildcard src/lexer/*.cpp)
HED_L		:= $(wildcard src/lexer/*.h)

# for simplicity, assume parser.o's depend only on parser.h's
#	in reality, there's a small cross-over;
#	- will require occasional make clean, but much simpler here
OBJ_C		:= $(patsubst src/%.cpp, bin/%.o, $(SRC_C)) $(HED_C)
OBJ_P		:= $(patsubst src/%.cpp, bin/%.o, $(SRC_P)) $(HED_P)
OBJ_L		:= $(patsubst src/%.cpp, bin/%.o, $(SRC_L)) $(HED_L)

build				: bin/compiler

bin/compiler		: $(OBJ_C) $(OBJ_P)	$(OBJ_L)			| bin/
	$(CXX)    $(CXXFLAGS) $(INC) -o bin/compiler $^ $(LDFLAGS)

bin/parser/%.o		: src/parser/%.cpp $(HED_P)				| bin/
	$(CXX) -c $(CXXFLAGS) $(INC) -o $@ $<

bin/lexer/%.o		: src/lexer/%.cpp $(HED_L)				| bin/
	$(CXX) -c $(CXXFLAGS) $(INC) -o $@ $<

bin/%.o				: src/%.cpp $(HED_C)					| bin/
	$(CXX) -c $(CXXFLAGS) $(INC) -o $@ $<

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
