//
//  parser.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser.h"

Parser::Parser(void): lexer(new Lexer), symtbl( *(lexer->symtbl) ){
}

//constructor delegation needs gcc47.. fml
Parser::Parser(const char* fname) : lexer(new Lexer), symtbl( *(lexer->symtbl) ){
	lexer->switchIstream(fname);
}

Parser::~Parser(void){
	delete lexer;
}

void Parser::parse() const{
	while(lexer->lexan()){
		std::cout << "Matched: " << lexer->matched()
		<< " on line " << lexer->lineNr() << std::endl;
	}
	
	std::cout << symtbl << std::endl;
}
