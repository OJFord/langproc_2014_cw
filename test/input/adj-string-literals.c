//
//  adj-string-literals.c
//  CARM-Compiler
//
//  Created by Ollie Ford on 27/02/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include <stdio.h>

int main(){
	/*
	This pair of adjacent character string literals
	
	"\x12" "3"
	
	produces a single character string literal containing the two characters whose values are \x12 and '3', because escape sequences are converted into single members of the execution character set just prior to adjacent string literal concatenation.
	 */
	if( ("\x12" "3") == "\x123" )
		printf("Bugger.");
	else
		printf("Good job.");
}
