//
//  compiler.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 01/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "invoc-args.h"
#include "parser/parser.h"

int main(const int argc, const char* argv[]){
	InvocationArguments ia(argc, argv);
	Parser* parser;

	bool verbose = ia.flagExists("-v")
		? true
		: false;

	if(argc>1)
		parser = new Parser(verbose, argv[1]);
	else
		parser = new Parser(verbose);
	
	parser->parse();
}
