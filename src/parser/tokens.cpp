//
//  tokens.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 13/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include <iostream>
#include "tokens.h"
#include "../lexer/Scanner.h"

const std::map<std::string, Keyword*> Keyword::keywords = {
	{	"auto",		new Keyword("AUTO")			},
	{	"double",	new Keyword("DOUBLE")		},
	{	"int",		new Keyword("INT")			},
	{	"struct",	new Keyword("STRUCT")		},
	{	"break",	new Keyword("BREAK")		},
	{	"else",		new Keyword("ELSE")			},
	{	"long",		new Keyword("LONG")			},
	{	"switch",	new Keyword("SWITCH")		},
	{	"case",		new Keyword("CASE")			},
	{	"enum",		new Keyword("ENUM")			},
	{	"register",	new Keyword("REGISTER")		},
	{	"typedef",	new Keyword("TYPEDEF")		},
	{	"char",		new Keyword("CHAR")			},
	{	"extern",	new Keyword("EXTERN")		},
	{	"return",	new Keyword("RETURN")		},
	{	"union",	new Keyword("UNION")		},
	{	"const",	new Keyword("CONST")		},
	{	"float",	new Keyword("FLOAT")		},
	{	"short",	new Keyword("SHORT")		},
	{	"unsigned",	new Keyword("UNSIGNED")		},
	{	"continue",	new Keyword("CONTINUE")		},
	{	"for",		new Keyword("FOR")			},
	{	"signed",	new Keyword("SIGNED")		},
	{	"void",		new Keyword("VOID")			},
	{	"default",	new Keyword("DEFAULT")		},
	{	"goto",		new Keyword("GOTO")			},
	{	"sizeof",	new Keyword("SIZEOF")		},
	{	"volatile",	new Keyword("VOLATILE")		},
	{	"do",		new Keyword("DO")			},
	{	"if",		new Keyword("IF")			},
	{	"static",	new Keyword("STATIC")		},
	{	"while",	new Keyword("WHILE")		},
};
/*
int main(int argc, char* argv[]){
	
	 auto it = Keyword::keywords.find("auto");
	 std::cout << it->second->what() << std::endl;
	 
	 it = Keyword::keywords.find("sizeof");
	 std::cout << it->second->what() << std::endl;
	 
	 it = Keyword::keywords.find("int");
	 std::cout << it->second->what() << std::endl;
	 
	 
	 
	 Identifier* id = new Identifier("ojford");
	 std::cout << id->what() << std::endl << id->raw() << std::endl;
	 
	 id = new Identifier("test");
	 std::cout << id->what() << std::endl << id->raw() << std::endl;
	 
	 id = new Identifier("im_an_identifier");
	 std::cout << id->what() << std::endl << id->raw() << std::endl;
	 
	 
	 
	 FloatingConstant *fc = new FloatingConstant(".4539e+2f");
	 std::cout << fc->what() << std::endl << fc->val << std::endl << fc->raw() << std::endl;
	 
	 fc = new FloatingConstant("31467.4539e-1L");
	 std::cout << fc->what() << std::endl << fc->val << std::endl << fc->raw() << std::endl;
	 
	 fc = new FloatingConstant(".0");
	 std::cout << fc->what() << std::endl << fc->val << std::endl << fc->raw() << std::endl;
	 
	 
	 IntegerConstant *ic = new IntegerConstant("0xf");
	 std::cout << ic->what() << std::endl << ic->val << std::endl << IntegerConstant::base(ic->raw()) << std::endl << ic->raw() << std::endl;
	 
	 ic = new IntegerConstant("0xfu");
	 std::cout << ic->what() << std::endl << ic->val << std::endl << IntegerConstant::base(ic->raw()) << std::endl << ic->raw() << std::endl;
	 
	 ic = new IntegerConstant("0xfl");
	 std::cout << ic->what() << std::endl << ic->val << std::endl << IntegerConstant::base(ic->raw()) << std::endl << ic->raw() << std::endl;
	 
	 
	 CharacterConstant *cc = new CharacterConstant("'c'");
	 std::cout << cc->what() << std::endl << cc->val << std::endl << cc->raw() << std::endl;
	 
	 cc = new CharacterConstant("'\013'");
	 std::cout << cc->what() << std::endl << cc->val << std::endl << cc->raw() << std::endl;
	 
	 cc = new CharacterConstant("'\n'");
	 std::cout << cc->what() << std::endl << cc->val << std::endl << cc->raw() << std::endl;
	 
	 
	 auto it1 = PunctuatorOperator::puncops.find("?");
	 std::cout << it1->second->what() << std::endl << it1->second->raw() << std::endl;
	 
	 auto it2 = PunctuatorOperator::puncops.find("+=");
	 std::cout << it2->second->what() << std::endl << it2->second->raw() << std::endl;
	 
	 auto it3 = PunctuatorOperator::puncops.find("sizeof");
	 std::cout << it3->second->what() << std::endl << it3->second->raw() << std::endl;

}
 */
