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
#include "exceptions.h"
#include "../parser/tokens.h"

typedef std::pair<std::string, Terminal*> Symbol;
typedef std::pair<unsigned, Symbol> ScopeSymbolPair;

class SymbolTable
: private std::vector<ScopeSymbolPair>{
public:
	SymbolTable(void);
	
	// Inserts a new symbol to the table within present scope
	void insert(Symbol);
	
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
