//
//  invoc-args.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 19/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include <algorithm>
#include "invoc-args.h"

InvocationArguments::InvocationArguments(const int argc, const char** argv)
: argc(argc), argv(argv){
}

bool InvocationArguments::flagExists(const std::string& flag) const{
	const char** end = argv+argc;
	return std::find(argv, end, flag) != end;
}

const char* InvocationArguments::getOption(const std::string& opt) const{
	const char** end = argv+argc;
	const char** it = std::find(argv, end, opt);
	
	if (it != end && ++it != end)
		return *it;

	return nullptr;
}
