//
//  invoc-args.h
//  CARM-Compiler
//
//  Created by Ollie Ford on 19/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#ifndef __CARM_Compiler__invoc_args__
#define __CARM_Compiler__invoc_args__

#include <string>

class InvocationArguments{
public:
	InvocationArguments(const int, const char**);

	bool flagExists(const std::string&) const;
	const char* getOption(const std::string&) const;

private:
	const int argc;
	const char** argv;
};

#endif /* defined(__CARM_Compiler__invoc_args__) */
