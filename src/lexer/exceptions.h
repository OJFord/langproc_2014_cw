//
//  exceptions.h
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#ifndef __CARM_Compiler__lex_exceptions__
#define __CARM_Compiler__lex_exceptions__

#include <exception>
#include <string>

class UnknownSymbolException: public std::exception{
public:
	UnknownSymbolException(std::string);
	~UnknownSymbolException(void) throw();
	
	virtual const char* what(void) const throw();

protected:
	const std::string sym;
private:
};

#endif /* defined(__CARM_Compiler__exceptions__) */
