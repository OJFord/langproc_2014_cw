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
	{	LEX_EOF,							"EOF character"					},
	{	KW_AUTO,							"`auto`"						},
	{	KW_DOUBLE,							"`double`"						},
	{	KW_INT,								"`int`"							},
	{	KW_STRUCT,							"`struct`"						},
	{	KW_BREAK,							"`break`"						},
	{	KW_ELSE,							"`else`"						},
	{	KW_LONG,							"`long`"						},
	{	KW_SWITCH,							"`switch`"						},
	{	KW_CASE,							"`case`"						},
	{	KW_ENUM,							"`enum`"						},
	{	KW_REGISTER,						"`register`"					},
	{	KW_TYPEDEF,							"`typedef`"						},
	{	KW_CHAR,							"`char`"						},
	{	KW_EXTERN,							"`extern`"						},
	{	KW_RETURN,							"`return`"						},
	{	KW_UNION,							"`union`"						},
	{	KW_CONST,							"`const`"						},
	{	KW_FLOAT,							"`float`"						},
	{	KW_SHORT,							"`short`"						},
	{	KW_UNSIGNED,						"`unsigned`"					},
	{	KW_CONTINUE,						"`continue`"					},
	{	KW_FOR,								"`for`"							},
	{	KW_SIGNED,							"`signed`"						},
	{	KW_VOID,							"`void`"						},
	{	KW_DEFAULT,							"`default`"						},
	{	KW_GOTO,							"`goto`"						},
	{	KW_SIZEOF,							"`sizeof`"						},
	{	KW_VOLATILE,						"`volatile`"					},
	{	KW_DO,								"`do`"							},
	{	KW_IF,								"`if`"							},
	{	KW_STATIC,							"`static`"						},
	{	KW_WHILE,							"`while`"						},
	{	IDENTIFIER,							"an identifier"					},
	{	CONSTANT_FLOATING,					"a constant float"				},
	{	CONSTANT_INTEGER,					"a constant integer"			},
	{	CONSTANT_CHAR,						"a constant character"			},
	{	CONSTANT_CHAR_WIDE,					"a constant wide character"		},
	{	CONSTANT_STRINGLITERAL,				"a string literal"				},
	{	CONSTANT_STRINGLITERAL_WIDE,		"a wide string literal"			},
	{	PUNCOP_BRACKET_LEFT,				"`[`"							},
	{	PUNCOP_BRACKET_RIGHT,				"`]`"							},
	{	PUNCOP_PAREN_LEFT,					"`(`"							},
	{	PUNCOP_PAREN_RIGHT,					"`)`"							},
	{	PUNCOP_BRACE_LEFT,					"`{`"							},
	{	PUNCOP_BRACE_RIGHT,					"`}`"							},
	{	PUNCOP_COMMA,						"`,`"							},
	{	PUNCOP_COLON,						"`:`"							},
	{	PUNCOP_EQUALS,						"`=`"							},
	{	PUNCOP_SEMICOLON,					"`;`"							},
	{	PUNCOP_DOT,							"`.`"							},
	{	PUNCOP_ARROW,						"`->`"							},
	{	PUNCOP_INCREMENT,					"`++`"							},
	{	PUNCOP_DECREMENT,					"`--`"							},
	{	PUNCOP_BIT_AND,						"`&`"							},
	{	PUNCOP_PLUS,						"`+`"							},
	{	PUNCOP_MINUS,						"`-`"							},
	{	PUNCOP_BIT_NEG,						"`~`"							},
	{	PUNCOP_BOOL_NEG,					"`!`"							},
	{	PUNCOP_MULT,						"`*`"							},
	{	PUNCOP_DIV,							"`/`"							},
	{	PUNCOP_MOD,							"`%`"							},
	{	PUNCOP_SHIFT_LEFT,					"`<<`"							},
	{	PUNCOP_SHIFT_RIGHT,					"`>>`"							},
	{	PUNCOP_LT,							"`<`"							},
	{	PUNCOP_GT,							"`>`"							},
	{	PUNCOP_LT_EQUALS,					"`<=`"							},
	{	PUNCOP_GT_EQUALS,					"`>=`"							},
	{	PUNCOP_EQUALITY,					"`==`"							},
	{	PUNCOP_NOTEQUALS,					"`!="							},
	{	PUNCOP_BIT_XOR,						"`^`"							},
	{	PUNCOP_BIT_OR,						"`|`"							},
	{	PUNCOP_BOOL_AND,					"`&&`"							},
	{	PUNCOP_BOOL_OR,						"`||`"							},
	{	PUNCOP_TERNARY,						"`?`"							},
	{	PUNCOP_MULT_EQUALS,					"`*=`"							},			
	{	PUNCOP_DIV_EQUALS,					"`/=`"							},
	{	PUNCOP_MOD_EQUALS,					"`%`"							},
	{	PUNCOP_PLUS_EQUALS,					"`+=`"							},
	{	PUNCOP_MINUS_EQUALS,				"`-=`"							},
	{	PUNCOP_BSL_EQUALS,					"`<<=`"							},
	{	PUNCOP_BSR_EQUALS,					"`>>=`"							},
	{	PUNCOP_AND_EQUALS,					"`&=`"							},
	{	PUNCOP_XOR_EQUALS,					"`^=`"							},
	{	PUNCOP_OR_EQUALS,					"`|`"							},
	{	PUNCOP_ELLIPSIS,					"`...`"							},
	{	PUNCOP_UNARY_MINUS,					"`-`"							},
	{	PUNCOP_UNARY_PLUS,					"`-`"							}
};
