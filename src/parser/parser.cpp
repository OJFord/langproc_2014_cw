//
//  parser.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/parser.h"

Parser::Parser(bool verbose)
: verbose(verbose), lexer(new Lexer(verbose)), symtbl( *(lexer->symtbl) ){
}

//constructor delegation needs gcc47.. fml
Parser::Parser(bool verbose, const char* fname)
: verbose(verbose), lexer(new Lexer(verbose, fname)), symtbl( *(lexer->symtbl) ){
}

Parser::~Parser(void){
	delete lexer;
	delete ast;
}

void Parser::parse(){
	if(verbose)
		std::cout << "Parsing started." << std::endl;

	ast = translation_unit();

	if(verbose)
		std::cout << "Parsing complete." << std::endl;
}
