//
//  compiler.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 01/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/Parser.h"

int main(int argc, char* argv[]){
	Parser parser;
	parser.d_scanner.switchIstream(argv[1]);		// requires modifying Parser.h for public d_scanner
	parser.parse();
}
