//
//  exceptions.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 13/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/exceptions.h"


InvalidTokenException::InvalidTokenException(std::string tk, std::string raw)
: token(tk), input(raw){
}

InvalidTokenException::~InvalidTokenException(void) throw(){
}

const char* InvalidTokenException::what() const throw(){
	return (input+" is not a valid "+token).c_str();
}

UnexpectedEOFException::UnexpectedEOFException(void)
: InvalidTokenException("<<EOF>>", ""){
}

const char* UnexpectedEOFException::what() const throw(){
	return "File ended unexpectedly.";
}
