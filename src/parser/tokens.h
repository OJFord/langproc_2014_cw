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
#include <boost/bimap.hpp>
//#include <climits>
//#include <cerrno>
//#include <cstdlib>
#include "lexer/lex.h"

class Token2{
public:
	Token2(lexeme, std::string);

	static std::string name(const lexeme&);
	//static lexeme id(const std::string&);

	const lexeme lexID;
	const std::string lexed;
	const std::string matched;

private:
	// Maps enumerated lexemes to their names, pretty-printing & errors ftw
	//	- an std::map is pretty overkill, but has some maybe useful stuff, order doesn't matter etc.
	//	- and maybe hopefully constant maps of constant things are optimised out completely
	static const std::map<lexeme, std::string> lexname;
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
