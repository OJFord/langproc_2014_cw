//
//  exceptions.h
//  CARM-Compiler
//
//  Created by Ollie Ford on 13/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#ifndef __CARM_Compiler__parse_exceptions__
#define __CARM_Compiler__parse_exceptions__

#include <exception>
#include <string>
#include "parser/tokens.h"

class InvalidTokenException: public std::exception{
public:
	InvalidTokenException(std::string, const Token&);
	~InvalidTokenException(void) throw();
	
	virtual const char* what() const throw();
	
protected:
	const std::string expect;
	const Token input;
private:
};

class UnexpectedEOFException{
public:
	UnexpectedEOFException(void);

	const char* what() const throw();
};

#endif /* defined(__CARM_Compiler__exceptions__) */
