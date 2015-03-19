//
//  parser.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/parser.h"

Parser::Parser(void)
: lexer(new Lexer), symtbl( *(lexer->symtbl) ){
}

//constructor delegation needs gcc47.. fml
Parser::Parser(const char* fname)
: lexer(new Lexer(fname)), symtbl( *(lexer->symtbl) ){
}

Parser::~Parser(void){
	delete lexer;
}

void Parser::parse(){ 
	std::cout << "Parsing started." << std::endl;
	
	ast = translation_unit();
	
	std::cout << "Parsing complete." << std::endl;
}
