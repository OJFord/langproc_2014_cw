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
#include <boost/regex.hpp>
#include <climits>
#include <cerrno>
#include <cstdlib>
#include "exceptions.h"

enum terminal{
	KW_AUTO,
	KW_DOUBLE,
	KW_INT,
	KW_STRUCT,
	KW_BREAK,
	KW_ELSE,
	KW_LONG,
	KW_SWITCH,
	KW_CASE,
	KW_ENUM,
	KW_REGISTER,
	KW_TYPEDEF,
	KW_CHAR,
	KW_EXTERN,
	KW_RETURN,
	KW_UNION,
	KW_CONST,
	KW_FLOAT,
	KW_SHORT,
	KW_UNSIGNED,
	KW_CONTINUE,
	KW_FOR,
	KW_SIGNED,
	KW_VOID,
	KW_DEFAULT,
	KW_GOTO,
	KW_SIZEOF,
	KW_VOLATILE,
	KW_DO,
	KW_IF,
	KW_STATIC,
	KW_WHILE,
	IDENTIFIER,
	CONSTANT_FLOATING,
	CONSTANT_INTEGER,
	CONSTANT_CHAR,
	CONSTANT_CHAR_WIDE,
	CONSTANT_STRINGLITERAL,
	CONSTANT_STRINGLITERAL_WIDE,
	PUNCOP_BRACKET_LEFT,
	PUNCOP_BRACKET_RIGHT,
	PUNCOP_PAREN_LEFT,
	PUNCOP_PAREN_RIGHT,
	PUNCOP_BRACE_LEFT,
	PUNCOP_BRACE_RIGHT,
	PUNCOP_COMMA,
	PUNCOP_COLON,
	PUNCOP_EQUALS,
	PUNCOP_SEMICOLON,
	PUNCOP_DOT,
	PUNCOP_ARROW,
	PUNCOP_INCREMENT,
	PUNCOP_DECREMENT,
	PUNCOP_BIT_AND,
	PUNCOP_PLUS,
	PUNCOP_MINUS,
	PUNCOP_BIT_NEG,
	PUNCOP_BOOL_NEG,
	PUNCOP_MULT,
	PUNCOP_DIV,
	PUNCOP_MOD,
	PUNCOP_SHIFT_LEFT,
	PUNCOP_SHIFT_RIGHT,
	PUNCOP_LT,
	PUNCOP_GT,
	PUNCOP_LT_EQUALS,
	PUNCOP_GT_EQUALS,
	PUNCOP_EQUALITY,
	PUNCOP_NOTEQUALS,
	PUNCOP_BIT_XOR,
	PUNCOP_BIT_OR,
	PUNCOP_BOOL_AND,
	PUNCOP_BOOL_OR,
	PUNCOP_TERNARY,
	PUNCOP_MULT_EQUALS,
	PUNCOP_DIV_EQUALS,
	PUNCOP_MOD_EQUALS,
	PUNCOP_PLUS_EQUALS,
	PUNCOP_MINUS_EQUALS,
	PUNCOP_BSL_EQUALS,
	PUNCOP_BSR_EQUALS,
	PUNCOP_AND_EQUALS,
	PUNCOP_XOR_EQUALS,
	PUNCOP_OR_EQUALS,
	PUNCOP_ELLIPSIS,
	PUNCOP_UNARY_MINUS,
	PUNCOP_UNARY_PLUS
};

class Token{
public:
	virtual std::string what(void)	const =0;
	virtual std::string raw(void)	const =0;
protected:
private:
};

class Terminal: public Token{
public:
	Terminal(std::string raw) : _raw(raw){};
	
	std::string raw(void) const{
		return _raw;
	}
protected:
	const std::string _raw;
private:
};

class NonTerminal: public Token{
public:
protected:
private:
};

class Keyword: public Terminal{
public:
	static const std::map<std::string, Keyword*> keywords;
	std::string what(void) const{
		return "KW_"+raw();
	}
protected:
private:
	Keyword(std::string raw) : Terminal(raw){};
};
/*

namespace Terminals {


class Identifier: public Terminal{
public:
	Identifier(std::string raw) : Terminal(raw){};
	
	std::string what(void) const{
		return "IDENTIFIER";
	}
protected:
private:
};


enum floatingtype{
	DOUBLE,
	FLOAT,
	LONGDOUBLE
};

class FloatingConstant: public Terminal{
public:
	FloatingConstant(std::string raw)
	:	Terminal(raw),
	type(
			isFloat(raw) ? FLOAT
		 :	isLongDouble(raw) ? LONGDOUBLE
		 :	DOUBLE
		 ),
	val(
		isFloat(raw) ? strtof(raw.c_str(), nullptr)
		:	isLongDouble(raw) ? strtold(raw.c_str(), nullptr)
		:	strtod(raw.c_str(), nullptr)
		)
	{
		if(	!boost::regex_match(raw, boost::regex("[0-9]*.[0-9]+([eE][+-]?[0-9]+)?[flFL]?"))
		   &&	!boost::regex_match(raw, boost::regex("[0-9]+.([eE][+-]?[0-9]+)?[flFL]?"))
		   &&	!boost::regex_match(raw, boost::regex("[0-9]+[eE][+-]?[0-9]+[flFL]?"))
		   ){
			throw InvalidTokenException(what(), raw);
		}
	}
	
	static bool isFloat(std::string fc){
		return boost::regex_match(fc, boost::regex("[0-9.eE+-]+[fF]"));
	}
	static bool isLongDouble(std::string fc){
		return boost::regex_match(fc, boost::regex("[0-9.eE+-]+[lL]"));
	}
	static bool isDouble(std::string fc){
		return boost::regex_match(fc, boost::regex("[0-9.eE+-]+"));
	}
	
	std::string what(void) const{
		return "CONSTANT_FLOATING";
	}
	
	const floatingtype type;
	const long double val;
protected:
private:
};

enum integertype{
	INT,
	UINT,
	LINT,
	ULINT
};

class IntegerConstant: public Terminal{
public:
	IntegerConstant(std::string raw)
	:	Terminal(raw),
	type(
			isInt(raw) ? INT
		 :	isUnsignedInt(raw) ? UINT
		 :	isLongInt(raw) ? LINT
		 :	ULINT
		 ),
	val(
		isInt(raw) ? toInt(raw)
		:	isUnsignedInt(raw) ? toUnsignedInt(raw)
		:	isLongInt(raw) ? toLongInt(raw)
		:	toUnsignedLongInt(raw)
		)
	{};
	
	static bool isDecimal(std::string ic){
		return boost::regex_match(ic, boost::regex("[1-9][0-9]+[uUlL]*"));
	}
	static bool isOctal(std::string ic){
		return boost::regex_match(ic, boost::regex("0[0-7]+[uUlL]*"));
	}
	static bool isHexadecimal(std::string ic){
		return boost::regex_match(ic, boost::regex("0[xX][0-9A-Fa-f]+[uUlL]*"));
	}
	
	static int base(std::string ic){
		return isDecimal(ic) ? 10 :	isOctal(ic) ? 8 : isHexadecimal(ic) ? 16 : 0;
	}
	
	static unsigned long int toInt(std::string ic){
		return strtol(ic.c_str(), nullptr, base(ic));
	}
	static bool isInt(std::string ic){
		// int if doesn't overflow, and has no u/l suffix
		long int t = toInt(ic);
		return t <= INT_MAX
		&& t >= INT_MIN
		&& boost::regex_match(ic, boost::regex("[0-9a-fA-FxX]+"));
	}
	
	static unsigned long int toUnsignedInt(std::string ic){
		boost::smatch sub;
		boost::regex_match(ic, sub, boost::regex("(0?[xX]?[0-9a-fA-F]+)[uU]?"));
		return strtoul(sub[1].str().c_str(), nullptr, base(ic));
	}
	static bool isUnsignedInt(std::string ic){
		// uint if not int, doesn't overflow, and is oct/hex/unsigned and not long
		bool match = true;
		unsigned long int t;
		try{
			t = toUnsignedInt(ic);
		}
		catch(const std::out_of_range& e){
			match = false;
		}
		return !isInt(ic)
		&& t <= UINT_MAX
		&& match;
	}
	
	static unsigned long int toLongInt(std::string ic){
		boost::smatch sub;
		boost::regex_match(ic, sub, boost::regex("((0[xX]?)?[0-9a-fA-F]+)[lL]?"));
		return strtol(sub[1].str().c_str(), nullptr, base(ic));
	}
	static bool isLongInt(std::string ic){
		// uint if not int or uint, doesn't overflow, and is not unsigned
		bool match = true;
		long int t;
		try{
			t = toLongInt(ic);
		}
		catch(const std::out_of_range& e){
			match = false;
		}
		return !isInt(ic)
		&& !isUnsignedInt(ic)
		&& !( (t==LONG_MAX || t==LONG_MIN) && errno==ERANGE )
		&& match;
	}
	
	static unsigned long int toUnsignedLongInt(std::string ic){
		boost::smatch sub;
		boost::regex_match(ic, sub, boost::regex("((0[xX]?)?[0-9a-fA-F]+)[uUlL]?[uUlL]?"));
		return strtoul(sub[1].str().c_str(), nullptr, base(ic));
	}
	static bool isUnsignedLongInt(std::string ic){
		return !isInt(ic)		&& !isUnsignedInt(ic)
		&& !isLongInt(ic)	&& !isUnsignedLongInt(ic);
	}
	
	std::string what(void) const{
		return "CONSTANT_INTEGER";
	}
	
	const integertype type;
	const unsigned long int val;
protected:
private:
};

class CharacterConstant: public Terminal{
public:
	CharacterConstant(std::string raw)
	:	Terminal(raw),
	val(toChar(raw))
	{};
	
	static int toChar(std::string cc){
		boost::smatch sub;
		boost::regex_match(cc, sub, boost::regex("'(.+)'"));
		return (int)( *(sub[1].str().c_str()) );
	}
	
	std::string what(void) const{
		return "CONSTANT_CHAR";
	}
	const int val;
protected:
private:
};

class WideCharacterConstant: public Terminal{
public:
	WideCharacterConstant(std::string raw)
	:	Terminal(raw),
	val( (wchar_t)*(raw.c_str()) )
	{};
	
	static wchar_t toWChar(std::string cc){
		boost::smatch sub;
		boost::regex_match(cc, sub, boost::regex("L'(.+)'"));
		return (wchar_t)( *(sub[1].str().c_str()) );
	}
	
	std::string what(void) const{
		return "CONSTANT_CHAR_WIDE";
	}
	const wchar_t val;
protected:
private:
};

class StringLiteral: public Terminal{
public:
	StringLiteral(std::string raw)
	:	Terminal(raw),
	val(toStr(raw).data),
	size(toStr(raw).len)
	{};
	
	struct string{
		const char* data;
		const unsigned len;
	};
	
	static string toStr(std::string cc){
		boost::smatch sub;
		boost::regex_match(cc, sub, boost::regex("\"(.+)\""));
		return {
			sub[1].str().c_str(),
			static_cast<unsigned int>( sub[1].length() )
		};
	}
	
	std::string what(void) const{
		return "CONSTANT_STRINGLITERAL";
	}
	const char* val;
	const unsigned size;
protected:
private:
};

class WideStringLiteral: public Terminal{
public:
	WideStringLiteral(std::string raw)
	:	Terminal(raw),
	val(toWStr(raw).data),
	size(toWStr(raw).len)
	{};
	
	struct string{
		const wchar_t* data;
		const unsigned len;
	};
	
	static string toWStr(std::string cc){
		boost::smatch sub;
		boost::regex_match(cc, sub, boost::regex("L'(.+)'"));
		const std::string& narrow = sub[1].str();
		return {
			std::wstring(narrow.begin(), narrow.end()).c_str(),
			static_cast<unsigned int>( sub[1].length() )
		};
	}
	
	std::string what(void) const{
		return "CONSTANT_STRINGLITERAL_WIDE";
	}
	const wchar_t* val;
	const unsigned size;
protected:
private:
};


class PunctuatorOperator: public Terminal{
public:
	static const std::map<std::string, PunctuatorOperator*> puncops;
	
	std::string what(void) const{
		return "PUNCOP_"+type;
	}
protected:
	const std::string type;
private:
	PunctuatorOperator(std::string raw, std::string type)
	:	Terminal(raw),
		type(type)
	{};
	friend class Keyword;
};

const std::map<std::string, PunctuatorOperator*> PunctuatorOperator::puncops{
	{	"[",		new PunctuatorOperator("[",		"BRACKET_LEFT")		},
	{	"]",		new PunctuatorOperator("]",		"BRACKET_RIGHT")	},
	{	"(",		new PunctuatorOperator("(", 	"PAREN_LEFT")		},
	{	")",		new PunctuatorOperator(")", 	"PAREN_RIGHT")		},
	{	".",		new PunctuatorOperator(".",		"DOT")				},
	{	"->",		new PunctuatorOperator("->",	"ARROW")			},
	{	"++",		new PunctuatorOperator("++",	"INCREMENT")		},
	{	"--",		new PunctuatorOperator("--",	"DECREMENT")		},
	{	"&",		new PunctuatorOperator("&",		"AMPERSAND")		},
	{	"*",		new PunctuatorOperator("*",		"MULT")				},
	{	"+",		new PunctuatorOperator("+",		"PLUS")				},
	{	"-",		new PunctuatorOperator("-",		"MINUS")			},
	{	"~",		new PunctuatorOperator("~",		"BIT_NEG")			},
	{	"!",		new PunctuatorOperator("!",		"BOOL_NEG")			},
	{	"sizeof",	new PunctuatorOperator("sizeof","SIZEOF")			},
	{	"/",		new PunctuatorOperator("/",		"DIV")				},
	{	"%",		new PunctuatorOperator("%",		"MOD")				},
	{	"<<",		new PunctuatorOperator("<<",	"SHIFT_LEFT")		},
	{	">>",		new PunctuatorOperator(">>",	"SHIFT_RIGHT")		},
	{	"<",		new PunctuatorOperator("<",		"LT")				},
	{	">",		new PunctuatorOperator(">",		"GT")				},
	{	"<=",		new PunctuatorOperator("<=",	"LTEQUALS")			},
	{	">=",		new PunctuatorOperator(">=",	"GTEQUALS")			},
	{	"==",		new PunctuatorOperator("==",	"EQUALITY")			},
	{	"!=",		new PunctuatorOperator("!=",	"NOTEQUALITY")		},
	{	"^",		new PunctuatorOperator("^",		"BIT_XOR")			},
	{	"|",		new PunctuatorOperator("|",		"BIT_OR")			},
	{	"&&",		new PunctuatorOperator("&&",	"BOOL_AND")			},
	{	"||",		new PunctuatorOperator("||",	"BOOL_OR")			},
	{	"?",		new PunctuatorOperator("?",		"TERNARY")			},
	{	":",		new PunctuatorOperator(":",		"COLON")			},
	{	"=",		new PunctuatorOperator("=",		"EQUALS")			},
	{	"*=",		new PunctuatorOperator("*=",	"MULT_EQUALS")		},
	{	"/=",		new PunctuatorOperator("/=",	"DIV_EQUALS")		},
	{	"%=",		new PunctuatorOperator("%=",	"MOD_EQUALS")		},
	{	"+=",		new PunctuatorOperator("+=",	"PLUS_EQUALS")		},
	{	"-=",		new PunctuatorOperator("-=",	"MINUS_EQUALS")		},
	{	"<<=",		new PunctuatorOperator("<<=",	"BSL_EQUALS")		},
	{	">>=",		new PunctuatorOperator(">>=",	"BSR_EQUALS")		},
	{	"&=",		new PunctuatorOperator("&=",	"AND_EQUALS")		},
	{	"^=",		new PunctuatorOperator("^=",	"XOR_EQUALS")		},
	{	"|=",		new PunctuatorOperator("|=",	"OR_EQUALS")		},
	{	",",		new PunctuatorOperator(",",		"COMMA")			},
	{	"{",		new PunctuatorOperator("{",		"BRACE_LEFT")		},
	{	"}",		new PunctuatorOperator("}",		"BRACE_RIGHT")		},
	{	";",		new PunctuatorOperator(";",		"SEMICOLON")		},
	{	"...",		new PunctuatorOperator("...",	"ELLIPSIS")			}
};

}// namespace Terminals
*/
#endif /* defined(__CARM_Compiler__tokens__) */
