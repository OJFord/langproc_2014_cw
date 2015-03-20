//
//  tokens.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 13/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

//#include <iostream>
#include "parser/tokens.h"

Token2::Token2(lexeme l, std::string m)
: lexID(l), lexed( lexname.find(l)->second ), matched(m){
}

std::string Token2::name(const lexeme& l){
	return lexname.find(l)->second;
}
/*
lexeme Token2::id(const std::string& s){
	return lexname.right.find(s)->second;
}
*/
const std::map<lexeme, std::string> Token2::lexname={
	{	LEX_EOF,							"EOF"							},
	{	KW_AUTO,							"KW_AUTO"						},
	{	KW_DOUBLE,							"KW_DOUBLE"						},
	{	KW_INT,								"KW_INT"						},
	{	KW_STRUCT,							"KW_STRUCT"						},
	{	KW_BREAK,							"KW_BREAK"						},
	{	KW_ELSE,							"KW_ELSE"						},
	{	KW_LONG,							"KW_LONG"						},
	{	KW_SWITCH,							"KW_SWITCH"						},
	{	KW_CASE,							"KW_CASE"						},
	{	KW_ENUM,							"KW_ENUM"						},
	{	KW_REGISTER,						"KW_REGISTER"					},
	{	KW_TYPEDEF,							"KW_TYPEDEF"					},
	{	KW_CHAR,							"KW_CHAR"						},
	{	KW_EXTERN,							"KW_EXTERN"						},
	{	KW_RETURN,							"KW_RETURN"						},
	{	KW_UNION,							"KW_UNION"						},
	{	KW_CONST,							"KW_CONST"						},
	{	KW_FLOAT,							"KW_FLOAT"						},
	{	KW_SHORT,							"KW_SHORT"						},
	{	KW_UNSIGNED,						"KW_UNSIGNED"					},
	{	KW_CONTINUE,						"KW_CONTINUE"					},
	{	KW_FOR,								"KW_FOR"						},
	{	KW_SIGNED,							"KW_SIGNED"						},
	{	KW_VOID,							"KW_VOID"						},
	{	KW_DEFAULT,							"KW_DEFAULT"					},
	{	KW_GOTO,							"KW_GOTO"						},
	{	KW_SIZEOF,							"KW_SIZEOF"						},
	{	KW_VOLATILE,						"KW_VOLATILE"					},
	{	KW_DO,								"KW_DO"							},
	{	KW_IF,								"KW_IF"							},
	{	KW_STATIC,							"KW_STATIC"						},
	{	KW_WHILE,							"KW_WHILE"						},
	{	IDENTIFIER,							"IDENTIFIER"					},
	{	CONSTANT_FLOATING,					"CONSTANT_FLOATING"				},
	{	CONSTANT_INTEGER,					"CONSTANT_INTEGER"				},
	{	CONSTANT_CHAR,						"CONSTANT_CHAR"					},
	{	CONSTANT_CHAR_WIDE,					"CONSTANT_CHAR_WIDE"			},
	{	CONSTANT_STRINGLITERAL,				"CONSTANT_STRINGLITERAL"		},
	{	CONSTANT_STRINGLITERAL_WIDE,		"CONSTANT_STRINGLITERAL_WIDE"	},
	{	PUNCOP_BRACKET_LEFT,				"PUNCOP_BRACKET_LEFT"			},
	{	PUNCOP_BRACKET_RIGHT,				"PUNCOP_BRACKET_RIGHT"			},
	{	PUNCOP_PAREN_LEFT,					"PUNCOP_PAREN_LEFT"				},
	{	PUNCOP_PAREN_RIGHT,					"PUNCOP_PAREN_RIGHT"			},
	{	PUNCOP_BRACE_LEFT,					"PUNCOP_BRACE_LEFT"				},
	{	PUNCOP_BRACE_RIGHT,					"PUNCOP_BRACE_RIGHT"			},
	{	PUNCOP_COMMA,						"PUNCOP_COMMA"					},
	{	PUNCOP_COLON,						"PUNCOP_COLON"					},
	{	PUNCOP_EQUALS,						"PUNCOP_EQUALS"					},
	{	PUNCOP_SEMICOLON,					"PUNCOP_SEMICOLON"				},
	{	PUNCOP_DOT,							"PUNCOP_DOT"					},
	{	PUNCOP_ARROW,						"PUNCOP_ARROW"					},
	{	PUNCOP_INCREMENT,					"PUNCOP_INCREMENT"				},
	{	PUNCOP_DECREMENT,					"PUNCOP_DECREMENT"				},
	{	PUNCOP_BIT_AND,						"PUNCOP_BIT_AND"				},
	{	PUNCOP_PLUS,						"PUNCOP_PLUS"					},
	{	PUNCOP_MINUS,						"PUNCOP_MINUS"					},
	{	PUNCOP_BIT_NEG,						"PUNCOP_BIT_NEG"				},
	{	PUNCOP_BOOL_NEG,					"PUNCOP_BOOL_NEG"				},
	{	PUNCOP_MULT,						"PUNCOP_MULT"					},
	{	PUNCOP_DIV,							"PUNCOP_DIV"					},
	{	PUNCOP_MOD,							"PUNCOP_MOD"					},
	{	PUNCOP_SHIFT_LEFT,					"PUNCOP_SHIFT_LEFT"				},
	{	PUNCOP_SHIFT_RIGHT,					"PUNCOP_SHIFT_RIGHT"			},
	{	PUNCOP_LT,							"PUNCOP_LT"						},
	{	PUNCOP_GT,							"PUNCOP_GT"						},
	{	PUNCOP_LT_EQUALS,					"PUNCOP_LT_EQUALS"				},
	{	PUNCOP_GT_EQUALS,					"PUNCOP_GT_EQUALS"				},
	{	PUNCOP_EQUALITY,					"PUNCOP_EQUALITY"				},
	{	PUNCOP_NOTEQUALS,					"PUNCOP_NOTEQUALS"				},
	{	PUNCOP_BIT_XOR,						"PUNCOP_BIT_XOR"				},
	{	PUNCOP_BIT_OR,						"PUNCOP_BIT_OR"					},
	{	PUNCOP_BOOL_AND,					"PUNCOP_BOOL_AND"				},
	{	PUNCOP_BOOL_OR,						"PUNCOP_BOOL_OR"				},
	{	PUNCOP_TERNARY,						"PUNCOP_TERNARY"				},
	{	PUNCOP_MULT_EQUALS,					"PUNCOP_MULT_EQUALS"			},
	{	PUNCOP_DIV_EQUALS,					"PUNCOP_DIV_EQUALS"				},
	{	PUNCOP_MOD_EQUALS,					"PUNCOP_MOD_EQUALS"				},
	{	PUNCOP_PLUS_EQUALS,					"PUNCOP_PLUS_EQUALS"			},
	{	PUNCOP_MINUS_EQUALS,				"PUNCOP_MINUS_EQUALS"			},
	{	PUNCOP_BSL_EQUALS,					"PUNCOP_BSL_EQUALS"				},
	{	PUNCOP_BSR_EQUALS,					"PUNCOP_BSR_EQUALS"				},
	{	PUNCOP_AND_EQUALS,					"PUNCOP_AND_EQUALS"				},
	{	PUNCOP_XOR_EQUALS,					"PUNCOP_XOR_EQUALS"				},
	{	PUNCOP_OR_EQUALS,					"PUNCOP_OR_EQUALS"				},
	{	PUNCOP_ELLIPSIS,					"PUNCOP_ELLIPSIS"				},
	{	PUNCOP_UNARY_MINUS,					"PUNCOP_UNARY_MINUS"			},
	{	PUNCOP_UNARY_PLUS,					"PUNCOP_UNARY_PLUS"				}
};

/*
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
};*/
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
