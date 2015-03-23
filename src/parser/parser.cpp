//
//  parser.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/parser.h"

Error::Error(std::string what, std::string where, SrcPos when)
: what(what), where(where), when(when){
}

ErrorStack::ErrorStack(void)
: impl(new std::vector<Error*>()){
}

ErrorStack::~ErrorStack(){
	delete impl;
}

size_t ErrorStack::num(void) const{
	return impl->size();
}

void ErrorStack::report(const Error& e){
	impl->push_back( new Error(e) );
}

std::ostream& operator<<(std::ostream& os, const ErrorStack& es){
	os << "Compiler exited with " << es.num() << " errors:" << std::endl;
	for(auto i:*(es.impl) )
		os << *i;
	os << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Error& e){
	os << e.where << ":" << e.when.lineNo() << ":" << e.when.colNo();
	os << ": error: " << e.what;
	return os;
}

Parser::Parser(bool verbose)
: verbose(verbose), lexer(new Lexer(verbose)),
	symtbl( *(lexer->symtbl) ), errors( new ErrorStack ){
}

//constructor delegation needs gcc47.. fml
Parser::Parser(bool verbose, const char* fname)
: verbose(verbose), lexer(new Lexer(verbose, fname)),
	symtbl( *(lexer->symtbl) ), errors( new ErrorStack ){
}

Parser::~Parser(void){
	delete lexer;
	delete ast;
}

void Parser::parse(){
	if(verbose)
		std::cout << "Parsing started." << std::endl;

	ast = translation_unit();
	
	if(!ast){
		std::cerr << "Compilation failed." << std::endl;
		std::cerr << *errors << std::endl;
		exit(EXIT_FAILURE);
	}
	
	if(verbose){
		std::cout << "Matched " << ast->what() << std::endl;
		std::cout << "Parsing complete." << std::endl;
		std::cout << *ast << std::endl;
	}
}

void Parser::reportInvalidToken(const InvalidTokenException& e){
	errors->report( Error(e.what(), e.where(), e.when()) );
}
