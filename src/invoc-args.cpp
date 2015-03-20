//
//  invoc-args.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 19/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "invoc-args.h"

InvocationArguments::InvocationArguments(const int argc, const char** argv)
: argc(argc), argv(argv){
}

bool InvocationArguments::flagExists(const std::string& option) const{
	const char** end = argv+argc;
	return std::find(argv, end, option) != end;
}
