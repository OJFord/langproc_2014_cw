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
		for(size_t i=14-name.length(); i!=0; --i)
			os << " ";
		os << "|     " << k << std::endl;
	}
	return os;
}

Lexer::Lexer(bool verbose)
: Scanner(), symtbl(new SymbolTable), verbose(verbose),
	srcfile("stdio"), labuf(new LookAheadBuffer){
	if(verbose)
		std::cout << "Being verbose." << std::endl;
	moreBuffer();	// init la buffer
}

Lexer::Lexer(bool verbose, const char* fname)
: Scanner(), symtbl(new SymbolTable), verbose(verbose),
	srcfile(fname), labuf(new LookAheadBuffer){
	if(verbose)
		std::cout << "Being verbose." << std::endl;
	switchIstream(fname);
	moreBuffer();	// ctor delegation needs gcc47, labs have 44..
}

Lexer::~Lexer(){
	delete symtbl;
	delete labuf;
}

Token& Lexer::lexan(void){
	Token& ret = lookahead(1);
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
	Token tk(t, matched(), srcfile, SrcPos(lineNr(), 0) );	// eeh.. no built in `colNr()`
	labuf->push_back(tk);
	std::cout << "Lexed a " << tk.lexed << " on " << matched() << std::endl;
}

Token& Lexer::lookahead(unsigned k){
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

Token& Lexer::consume(const lexeme& m){
	if(verbose)
		std::cout << "Eating a " << Token::name(m) << std::endl;

	Token tk = Token(m);

	Token& la = lookahead(1);
	if( la.lexID == tk.lexID ){
		// Ensure buffer is never empty
		labuf->pop_front();
		if(labuf->empty())
			moreBuffer();

		switch(tk.lexID){
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
	else{
		throw InvalidTokenException(Token::name(m), "previous symbol", la);
	}
	if(verbose)
		std::cout << *labuf << std::endl;
	return la;
}

SymbolTableEntry::SymbolTableEntry(unsigned scope, Token sym)
: _scope(scope), _symbol(sym){
}

unsigned const& SymbolTableEntry::scope(void) const{
	return _scope;
}

Token const& SymbolTableEntry::symbol(void) const{
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

void SymbolTable::insert(Token symbol){
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

Token const& SymbolTable::resolve(const std::string& name) const{
	// probably a fair assumption that symbol is more likely
	//	in an inner scope, so more effecient to iterate 'backward'
	for(auto it=rbegin(); it!=rend(); ++it){
		const Token& s = (*it)->symbol();
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
		for(size_t i=14-(*it)->symbol().matched.length(); i!=0; --i)
			os << " ";
		os << "|     " << (*it)->scope() << std::endl;
	}
	
	return os;
}
