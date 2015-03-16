//
//  compiler.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 01/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/parser.h"

int main(int argc, char* argv[]){
	Parser* parser;
	
	if(argc>1)
		parser = new Parser(argv[1]);
	else
		parser = new Parser();
	
	parser->parse();
}
