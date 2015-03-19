//
//  exceptions.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "lexer/exceptions.h"

UnknownSymbolException::UnknownSymbolException(std::string sym) : sym(sym){
}

UnknownSymbolException::~UnknownSymbolException(void) throw(){
}
	
const char* UnknownSymbolException::what(void) const throw(){
	return ("Symbol "+sym+" was not declared in this scope.").c_str();
}
