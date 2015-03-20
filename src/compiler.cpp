//
//  compiler.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 01/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "invoc-args.h"
#include "parser/parser.h"

int main(const int argc, const char* argv[]){
	InvocationArguments ia(argc, argv);
	Parser* parser;

	bool verbose = ia.flagExists("-v")
		? true
		: false;

	if( !ia.flagExists("-S") ){
		std::cerr << "That's not supported." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::streambuf* buf;
	std::ofstream of;

	if( ia.flagExists("-o") ){
		const char* ofname;
		if( (ofname = ia.getOption("-o")) ){
			of.open(ofname);
			buf = of.rdbuf();
		}
		else{
			std::cout << "Argument to '-o' is missing." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	else {
		buf = std::cout.rdbuf();
	}
	std::ostream out(buf);

	try{
		// Naively assume input file is given last
		parser = new Parser(verbose, argv[argc-1]);
	} catch(std::out_of_range& e){
		std::cout << "No input files specified." << std::endl;
		exit(EXIT_FAILURE);
	}

	parser->parse();
}
