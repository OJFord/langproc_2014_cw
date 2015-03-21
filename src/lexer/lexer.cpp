//
//  lexer.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "lexer/lexer.h"

std::ostream& operator<<(std::ostream& os, const LookAheadBuffer& lab){
	os << "    Lexeme    |    LA(k)    " << std::endl;
	os << "--------------+-------------" << std::endl;
	
	unsigned k=0;
	for(auto it:lab){
		k++;
		std::string name = it.matched;
		os << name;
		for(unsigned i=14-name.length(); i!=0; --i)
			os << " ";
		os << "|     " << k << std::endl;
	}
	return os;
}

Lexer::Lexer(bool verbose)
: Scanner(), symtbl(new SymbolTable), verbose(verbose),
	labuf(new LookAheadBuffer){
	if(verbose)
		std::cout << "Being verbose." << std::endl;
	moreBuffer();	// init la buffer
}

Lexer::Lexer(bool verbose, const char* fname)
: Scanner(), symtbl(new SymbolTable), verbose(verbose),
	labuf(new LookAheadBuffer){
	if(verbose)
		std::cout << "Being verbose." << std::endl;
	switchIstream(fname);
	moreBuffer();	// ctor delegation needs gcc47, labs have 44..
}

Lexer::~Lexer(){
	delete symtbl;
	delete labuf;
}

Token2& Lexer::lexan(void){
	Token2& ret = lookahead(1);
	consume(ret.lexID);
	return ret;
}

void Lexer::moreBuffer(void){
	if(verbose)
		std::cout << "Lexing more input." << std::endl;

	lexeme t = static_cast<lexeme>(lex());
	switch(t){
		// Make parsing easier with unary pseudo-terminals - lol
		case PUNCOP_MINUS:
			// If last lex was an operator or bracket, +/- is unary
			if( last>=PUNCOP_BRACKET_LEFT && last<=PUNCOP_ELLIPSIS )
				t = PUNCOP_UNARY_MINUS;
			break;
		case PUNCOP_PLUS:
			if( last>=PUNCOP_BRACKET_LEFT && last<=PUNCOP_ELLIPSIS )
				t = PUNCOP_UNARY_PLUS;
			break;
			
		default:;
	}
	last = t;
	Token2 tk(t, matched());
	labuf->push_back(tk);
	std::cout << "Lexed a " << tk.lexed << " on " << matched() << std::endl;
}

Token2& Lexer::lookahead(unsigned k){
	if(verbose)
		std::cout << "Looking ahead " << k << std::endl;

	auto it = labuf->begin();
	for(; --k!=0; ++it){
		// Lookahead to k, buffer contains at least k tk after loop
		if( it==labuf->end() )
			moreBuffer();
	}

	if(verbose)
		std::cout << *labuf << std::endl;
	return *it--;
}

Token2& Lexer::consume(const lexeme& m){
	if(verbose)
		std::cout << "Eating a " << Token2::name(m) << std::endl;

	Token2	tk = Token2(m, Token2::name(m));
	Token2& la = lookahead(1);

	if( la.lexID == tk.lexID ){
		labuf->pop_front();
		switch(tk.lexID){
			case IDENTIFIER:
				if( !symtbl->contains( matched() ) )
					symtbl->insert(la);
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
			default:;
		}
	}
	else
		throw InvalidTokenException(tk.matched, la.matched);
	if(verbose)
		std::cout << *labuf << std::endl;
	if(labuf->empty())
		moreBuffer();
	return la;
}

SymbolTableEntry::SymbolTableEntry(unsigned scope, Token2 sym)
: _scope(scope), _symbol(sym){
}

unsigned const& SymbolTableEntry::scope(void) const{
	return _scope;
}

Token2 const& SymbolTableEntry::symbol(void) const{
	return _symbol;
}

SymbolTable::SymbolTable(void){
	scope = 0;
}

void SymbolTable::beginScope(void){
	scope++;
}

void SymbolTable::closeScope(void){
	scope--;
	while( back()->scope()>scope && size()!=0 )
		pop_back();
}

void SymbolTable::insert(const Token2& symbol){
	push_back( new SymbolTableEntry(scope, symbol) );
}
	
bool SymbolTable::contains(const std::string& name) const{
	try{
		resolve(name);
		return true;
	} catch(UnknownSymbolException& e){
		return false;
	}
}

Token2 const& SymbolTable::resolve(const std::string& name) const{
	// probably a fair assumption that symbol is more likely
	//	in an inner scope, so more effecient to iterate 'backward'
	for(auto it=rbegin(); it!=rend(); ++it){
		const Token2& s = (*it)->symbol();
		if( s.matched == name ){
			return s;
		}
	}
	// Symbol not in table
	throw UnknownSymbolException(name);
}

std::ostream& operator<<(std::ostream& os, const SymbolTable& st){
	os << "    Symbol    |    Scope    " << std::endl;
	os << "--------------+-------------" << std::endl;
	
	// GCC 'internal error reporting bug' if this is made range-based
	for(auto it=st.begin(); it!=st.end(); ++it){	
		os << (*it)->symbol().matched;
		for(unsigned i=14-(*it)->symbol().matched.length(); i!=0; --i)
			os << " ";
		os << "|     " << (*it)->scope() << std::endl;
	}
	
	return os;
}
