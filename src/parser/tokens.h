//
//  tokens.h
//  CARM-Compiler
//
//  Created by Ollie Ford on 13/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#ifndef __CARM_Compiler__tokens__
#define __CARM_Compiler__tokens__

#include <string>
#include <map>
#include "lexer/lex.h"

class SrcPos: std::pair<size_t, size_t>{
public:
	SrcPos(size_t, size_t);
	
	size_t lineNo(void) const;
	size_t colNo(void) const;
};

class Token{
public:
	Token(lexeme, std::string="main.c", SrcPos=SrcPos(0,0));
	Token(lexeme, std::string, std::string="main.c", SrcPos=SrcPos(0,0));

	static std::string name(const lexeme&);

	const lexeme		lexID;
	const std::string	lexed;
	const std::string	matched;
	
	const std::string	srcfile;
	const SrcPos		pos;

private:
	// Maps enumerated lexemes to their names, pretty-printing & errors ftw
	//	- an std::map is pretty overkill, but has some maybe useful stuff, order doesn't matter etc.
	//		and maybe-hopefully constant maps of constant things are optimised out completely
	static const std::map<lexeme, std::string> lexname;
};

/*
const std::map<std::string, PunctuatorOperator*> PunctuatorOperator::puncops{
	{	"[",		PunctuatorOperator()("[",		"BRACKET_LEFT")		},
	{	"]",		PunctuatorOperator()("]",		"BRACKET_RIGHT")	},
	{	"(",		PunctuatorOperator()("(", 	"PAREN_LEFT")		},
	{	")",		PunctuatorOperator()(")", 	"PAREN_RIGHT")		},
	{	".",		PunctuatorOperator()(".",		"DOT")				},
	{	"->",		PunctuatorOperator()("->",	"ARROW")			},
	{	"++",		PunctuatorOperator()("++",	"INCREMENT")		},
	{	"--",		PunctuatorOperator()("--",	"DECREMENT")		},
	{	"&",		PunctuatorOperator()("&",		"AMPERSAND")		},
	{	"*",		PunctuatorOperator()("*",		"MULT")				},
	{	"+",		PunctuatorOperator()("+",		"PLUS")				},
	{	"-",		PunctuatorOperator()("-",		"MINUS")			},
	{	"~",		PunctuatorOperator()("~",		"BIT_NEG")			},
	{	"!",		PunctuatorOperator()("!",		"BOOL_NEG")			},
	{	"sizeof",	PunctuatorOperator()("sizeof","SIZEOF")			},
	{	"/",		PunctuatorOperator()("/",		"DIV")				},
	{	"%",		PunctuatorOperator()("%",		"MOD")				},
	{	"<<",		PunctuatorOperator()("<<",	"SHIFT_LEFT")		},
	{	">>",		PunctuatorOperator()(">>",	"SHIFT_RIGHT")		},
	{	"<",		PunctuatorOperator()("<",		"LT")				},
	{	">",		PunctuatorOperator()(">",		"GT")				},
	{	"<=",		PunctuatorOperator()("<=",	"LTEQUALS")			},
	{	">=",		PunctuatorOperator()(">=",	"GTEQUALS")			},
	{	"==",		PunctuatorOperator()("==",	"EQUALITY")			},
	{	"!=",		PunctuatorOperator()("!=",	"NOTEQUALITY")		},
	{	"^",		PunctuatorOperator()("^",		"BIT_XOR")			},
	{	"|",		PunctuatorOperator()("|",		"BIT_OR")			},
	{	"&&",		PunctuatorOperator()("&&",	"BOOL_AND")			},
	{	"||",		PunctuatorOperator()("||",	"BOOL_OR")			},
	{	"?",		PunctuatorOperator()("?",		"TERNARY")			},
	{	":",		PunctuatorOperator()(":",		"COLON")			},
	{	"=",		PunctuatorOperator()("=",		"EQUALS")			},
	{	"*=",		PunctuatorOperator()("*=",	"MULT_EQUALS")		},
	{	"/=",		PunctuatorOperator()("/=",	"DIV_EQUALS")		},
	{	"%=",		PunctuatorOperator()("%=",	"MOD_EQUALS")		},
	{	"+=",		PunctuatorOperator()("+=",	"PLUS_EQUALS")		},
	{	"-=",		PunctuatorOperator()("-=",	"MINUS_EQUALS")		},
	{	"<<=",		PunctuatorOperator()("<<=",	"BSL_EQUALS")		},
	{	">>=",		PunctuatorOperator()(">>=",	"BSR_EQUALS")		},
	{	"&=",		PunctuatorOperator()("&=",	"AND_EQUALS")		},
	{	"^=",		PunctuatorOperator()("^=",	"XOR_EQUALS")		},
	{	"|=",		PunctuatorOperator()("|=",	"OR_EQUALS")		},
	{	",",		PunctuatorOperator()(",",		"COMMA")			},
	{	"{",		PunctuatorOperator()("{",		"BRACE_LEFT")		},
	{	"}",		PunctuatorOperator()("}",		"BRACE_RIGHT")		},
	{	";",		PunctuatorOperator()(";",		"SEMICOLON")		},
	{	"...",		PunctuatorOperator()("...",	"ELLIPSIS")			}
};

}// namespace Terminals
*/
#endif /* defined(__CARM_Compiler__tokens__) */
