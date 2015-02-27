//
//  silly-increments.c
//  CARM-Compiler
//
//  Created by Ollie Ford on 26/02/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include <stdio.h>

int main(int, char**){
	int x=1;
	int y=2;
	int silly = x+++++y;
	printf("%d %d %d", x, y, silly);
}
