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

#include "Scanner.h"
#include "parser/tokens.h"
#include "parser/syntax-tree.h"		//this feels dirty.. !TODO restructure stuff..
#include "lexer/exceptions.h"


class SymbolTableEntry{
public:
	SymbolTableEntry(unsigned, Token2);

	// Returns value associated with symbols level of scope
	//	- 0 is global; incremented with each block from there
	unsigned const& scope(void) const;

	// Returns a reference to the symbol in entry
	Token2 const& symbol(void) const;


private:
	const unsigned _scope;
	const Token2 _symbol;
};

class SymbolTable
: private std::vector<SymbolTableEntry*>{
public:
	SymbolTable(void);
	
	// Inserts a new symbol to the table within present scope
	void insert(const Token2&);
	
	// Returns true iff symbol name exists in the table
	bool contains(std::string) const;
	
	// Resolves a symbol name and returns a ptr to its object, which may be null
	//	- If no symbol with name exists, throws UnknownSymbolException
	Terminal* resolve(std::string) const;
	
	// Creates new scope level in table, new symbols will be at this level
	void beginScope(void);
	
	// Ends current scope level, rejects now out of scope symbols
	void closeScope(void);
	
private:
	unsigned scope;
	
	friend std::ostream& operator<<(std::ostream&, const SymbolTable&);
};

class Lexer: public Scanner{
public:
	Lexer(void);
	terminal lexan(void);
	SymbolTable* symtbl;
private:
	terminal last;
};

#endif /* defined(__CARM_Compiler__lexer__) */
