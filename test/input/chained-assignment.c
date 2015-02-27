//
//  chained-assignment.c
//  CARM-Compiler
//
//  Created by Ollie Ford on 26/02/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include <stdio.h>

int main(){
	int a, b;
	int c = 1;
	a = b = c;
	printf("%d %d %d", a, b, c);
}
