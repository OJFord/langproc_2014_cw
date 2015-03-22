//
//  tokens.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 13/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/tokens.h"

SrcPos::SrcPos(size_t line, size_t col)
: std::pair<size_t,size_t>(line,col){
}

size_t SrcPos::lineNo(void) const{
	return first;
}
size_t SrcPos::colNo(void) const{
	return second;
}

Token::Token(lexeme l, std::string fname, SrcPos pos)
: lexID(l), lexed( lexname.find(l)->second ),
	matched("N/A"), srcfile(fname), pos(pos){
}
Token::Token(lexeme l, std::string m, std::string fname, SrcPos pos)
: lexID(l), lexed( lexname.find(l)->second ),
	matched(m), srcfile(fname), pos(pos){
}

std::string Token::name(const lexeme& l){
	return lexname.find(l)->second;
}

const std::map<lexeme, std::string> Token::lexname={
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
