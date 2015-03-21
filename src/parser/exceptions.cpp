//
//  exceptions.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 13/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/exceptions.h"


InvalidTokenException::InvalidTokenException(std::string expect, const Token2& raw)
: expect(expect), input(raw){
}

InvalidTokenException::~InvalidTokenException(void) throw(){
}

const char* InvalidTokenException::what() const throw(){
	return (input.matched+" is not a valid "+expect).c_str();
}

UnexpectedEOFException::UnexpectedEOFException(void){
}

const char* UnexpectedEOFException::what() const throw(){
	return "Reached EOF unexpectedly.";
}
