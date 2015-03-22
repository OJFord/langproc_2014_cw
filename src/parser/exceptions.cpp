//
//  exceptions.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 13/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/exceptions.h"


InvalidTokenException::InvalidTokenException(std::string expect, std::string last, const Token& input)
: expect(expect), last(last), input(input){
}

InvalidTokenException::~InvalidTokenException(void) throw(){
}

const char* InvalidTokenException::what(void) const throw(){
	return ( "expected "+expect+" after "+last ).c_str();
}

const char* InvalidTokenException::where(void) const{
	return input.srcfile.c_str();
}

const SrcPos& InvalidTokenException::when(void) const{
	return input.pos;
}

UnexpectedEOFException::UnexpectedEOFException(void){
}

const char* UnexpectedEOFException::what() const throw(){
	return "Reached EOF unexpectedly.";
}
