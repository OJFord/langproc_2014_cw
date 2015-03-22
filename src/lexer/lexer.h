//
//  lexer.h
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#ifndef __CARM_Compiler__lexer__
#define __CARM_Compiler__lexer__

#include <vector>
#include <utility>
#include <string>
#include <ostream>
#include <list>

#include "Scanner.h"
#include "parser/tokens.h"
#include "lexer/exceptions.h"
#include "parser/exceptions.h"

class SymbolTableEntry{
public:
	SymbolTableEntry(unsigned, Token);

	// Returns value associated with symbols level of scope
	//	- 0 is global; incremented with each block from there
	unsigned const& scope(void) const;

	// Returns a reference to the symbol in entry
	Token const& symbol(void) const;


private:
	const unsigned _scope;
	const Token _symbol;
};

class SymbolTable
: private std::vector<SymbolTableEntry*>{
public:
	SymbolTable(void);
	
	// Inserts a new symbol to the table within present scope
	void insert(Token);
	
	// Returns true iff symbol name exists in the table
	bool contains(const std::string&) const;
	
	// Resolves a symbol name and returns a copy
	//	- If no symbol with name exists, throws UnknownSymbolException
	Token const& resolve(const std::string&) const;
	
	// Creates new scope level in table, new symbols will be at this level
	void beginScope(void);
	
	// Ends current scope level, rejects now out of scope symbols
	void closeScope(void);
	
private:
	unsigned scope;
	
	friend std::ostream& operator<<(std::ostream&, const SymbolTable&);
};

// Pair containing lexeme and matched string
//typedef std::pair<lexeme, std::string> Token2;


class LookAheadBuffer
: public std::list<Token>{
public:
private:
	friend std::ostream& operator<<(std::ostream&, const LookAheadBuffer&);
};

class Lexer: private Scanner{
public:
	Lexer(bool);
	Lexer(bool, const char*);
	~Lexer();
	
	// Lexes a single token, advancing lookahead buffer
	//	- equivalent to consume( lookahead(1) );
	Token& lexan(void);

	// Looks ahead given number of lexical tokens
	//	- 1..n tokens are buffered
	Token& lookahead(unsigned=1);
	
	// Matches and consumes the lexically next token
	//	- throws UnexpectedTokenException if lexeme does not match
	Token& consume(const lexeme&);
	
	// Ptr to symbol table implementation for symbols in scope
	SymbolTable* symtbl;

protected:
	// Determines of verbosity of debug info etc
	bool verbose;

	// Increases length of lookahead buffer by 1
	void moreBuffer(void);
	
	const std::string srcfile;

private:
	lexeme last;
	LookAheadBuffer* labuf;
	
	//static const std::map<lexeme, Terminal*> terminal;
};

#endif /* defined(__CARM_Compiler__lexer__) */
