//
//  lexer.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "lexer.h"

Lexer::Lexer(void) : Scanner(), symtbl(new SymbolTable){
}

terminal Lexer::lexan(void){
	terminal t = static_cast<terminal>(lex());
	switch(t){
		// Add new identifier to symbol table -- it might not be declared
		//	*properly*, but it's there.
		case IDENTIFIER:
			if( !symtbl->contains(matched()) )
			   symtbl->insert( Symbol( matched(), nullptr ) );
			break;
			
		// Scope changes
		case PUNCOP_BRACE_LEFT:
			std::cout << *symtbl << std::endl;
			symtbl->beginScope();
			break;
		case PUNCOP_BRACE_RIGHT:
			std::cout << *symtbl << std::endl;
			symtbl->closeScope();
			break;
			
		// Make parsing easier with unary pseudo-terminals - lol
		case PUNCOP_MINUS:
			if(last>=PUNCOP_BRACKET_LEFT && last<=PUNCOP_ELLIPSIS)
				t = PUNCOP_UNARY_MINUS;
		case PUNCOP_PLUS:
			if(last>=PUNCOP_BRACKET_LEFT && last<=PUNCOP_ELLIPSIS)
				t = PUNCOP_UNARY_PLUS;

		default:;
	}
	last = t;
	return t;
}

SymbolTable::SymbolTable(void){
	scope = 0;
}

void SymbolTable::beginScope(void){
	scope++;
}

void SymbolTable::closeScope(void){
	scope--;
	while( back().first>scope && size()!=0 )
		pop_back();
}

void SymbolTable::insert(Symbol symbol){
	push_back( ScopeSymbolPair(scope, symbol) );
}
	
bool SymbolTable::contains(std::string name) const{
	try{
		resolve(name);
		return true;
	} catch(UnknownSymbolException){
		return false;
	}
}

Terminal* SymbolTable::resolve(std::string name) const{
	// probably a fair assumption that symbol is more likely
	//	in an inner scope, so more effecient to iterate 'backward'
	for(auto it=rbegin(); it!=rend(); ++it)
		if(it->second.first==name)
			return it->second.second;
	
	throw UnknownSymbolException(name);
}

std::ostream& operator<<(std::ostream& os, const SymbolTable& st){
	os << "    Symbol    |    Scope    " << std::endl;
	os << "--------------+-------------" << std::endl;
	for(auto it=st.begin(); it!=st.end(); ++it){
		os << it->second.first;
		for(unsigned i=14-it->second.first.length(); i!=0; --i)
			os << " ";
		os << "|     " << it->first << std::endl;
	}
	
	return os;
}
