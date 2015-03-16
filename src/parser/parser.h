//
//  parser.h
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "tokens.h"
#include "../lexer/lexer.h"

class Parser{
public:
	Parser();
	Parser(const char* filename);
	~Parser(void);
	
	void parse() const;

private:
	Lexer* lexer;
	SymbolTable& symtbl;
};
