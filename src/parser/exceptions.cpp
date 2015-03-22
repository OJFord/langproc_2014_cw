//
//  exceptions.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 13/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/exceptions.h"


InvalidTokenException::InvalidTokenException(std::string expect, const Token& input)
: expect(expect), input(input){
}

InvalidTokenException::~InvalidTokenException(void) throw(){
}

const char* InvalidTokenException::what() const throw(){
	return ( input.matched+" is not a valid "+expect
			+" ("+std::to_string( input.pos.lineNo() )
			+":"+std::to_string( input.pos.colNo() )+")" ).c_str();
}

UnexpectedEOFException::UnexpectedEOFException(void){
}

const char* UnexpectedEOFException::what() const throw(){
	return "Reached EOF unexpectedly.";
}
