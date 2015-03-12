//
//  lexer.lex
//  CARM-Compiler
//
//  Created by Ollie Ford on 26/02/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

NONDIGIT						[_a-zA-Z]
DIGIT							[0-9]

DIGITSEQ						{DIGIT}+

SIGN							[+-]
EXP								[eE]{SIGN}?{DIGITSEQ}
FLOATSFX						[flFL]
FRACTION						{DIGITSEQ}?\.{DIGITSEQ}|{DIGITSEQ}\.

NZDIGIT							[1-9]
ODIGIT							[0-7]
HDIGIT							[0-9a-zA-Z]
UNSGNDSFX						[uU]
LONGSFX							[lL]
INTSFX							{UNSGNDSFX}{LONGSFX}?|{LONGSFX}{UNSGNDSFX}?

SESCSEQ							[\'\"\?\\\a\b\f\n\r\t\v]
OESCSEQ							\\{ODIGIT}{1,3}
HESCSEQ							\\{HDIGIT}+
ESCSEQ							{SESCSEQ}|{OESCSEQ}|{HESCSEQ}

CCHAR							[^'\n\\]|{ESCSEQ}
CCHARSEQ						{CCHAR}+

SCHAR							[^"\n\\]|{ESCSEQ}
SCHARSEQ						{SCHAR}+

HCHAR							[^>\n]
HCHARSEQ						{HCHAR}+

QCHAR							[^"\n]
QCHARSEQ						{QCHAR}+

PPNUM							{DIGIT}|{NONDIGIT}|[eE]{SIGN}|\.
PPNUMSEQ						{PPNUM}+

%%

/*
*	Skip
*/
[ \t\n]


/*
*	Keyword
*/
auto							return Parser::KW_AUTO;
double							return Parser::KW_DOUBLE;
int								return Parser::KW_INT;
struct							return Parser::KW_STRUCT;
break							return Parser::KW_BREAK;
else							return Parser::KW_ELSE;
long							return Parser::KW_LONG;
switch							return Parser::KW_SWITCH;
case							return Parser::KW_CASE;
enum							return Parser::KW_ENUM;
register						return Parser::KW_REGISTER;
typedef							return Parser::KW_TYPEDEF;
char							return Parser::KW_CHAR;
extern							return Parser::KW_EXTERN;
return							return Parser::KW_RETURN;
union							return Parser::KW_UNION;
const							return Parser::KW_CONST;
float							return Parser::KW_FLOAT;
short							return Parser::KW_SHORT;
unsigned						return Parser::KW_UNSIGNED;
continue						return Parser::KW_CONTINUE;
for								return Parser::KW_FOR;
signed							return Parser::KW_SIGNED;
void							return Parser::KW_VOID;
default							return Parser::KW_DEFAULT;
goto							return Parser::KW_GOTO;
sizeof							return Parser::KW_SIZEOF;
volatile						return Parser::KW_VOLATILE;
do								return Parser::KW_DO;
if								return Parser::KW_IF;
static							return Parser::KW_STATIC;
while							return Parser::KW_WHILE;


/*
*	Identifier
*/
{NONDIGIT}{NONDIGIT}+{DIGIT}*	return Parser::IDENTIFIER;


/*
*	Constant
*/
{FRACTION}{EXP}?{FLOATSFX}?		|
{DIGITSEQ}{EXP}{FLOATSFX}?		return Parser::FLOATCONSTANT;
{NZDIGIT}{DIGIT}*{INTSFX}?		return Parser::DECCONSTANT;
0{ODIGIT}+{INTSFX}?				return Parser::OCTCONSTANT;
0[xX]{HDIGIT}+{INTSFX}?			return Parser::HEXCONSTANT;

\'{CCHARSEQ}\'					return Parser::CHARCONSTANT;
L\'{CCHARSEQ}\'					return Parser::WCHARCONSTANT;

\"{SCHARSEQ}?\"					return Parser::STRINGLITERAL;
L\"{SCHARSEQ}?\"				return Parser::WSTRINGLITERAL;


/*
*	Operator or punctuator
*/
\[								return Parser::LSQUBRACKET;
\]								return Parser::RSQUBRACKET;
\(								return Parser::LBRACKET;
\)								return Parser::RBRACKET;
\{								return Parser::LBRACE;
\}								return Parser::RBRACE;
,								return Parser::COMMA;
:								return Parser::COLON;
=								return Parser::EQUALS;
;								return Parser::SCOLON;


/*
*	Operator
*/
\.								return Parser::OPERATOR_DOT;
-\>								return Parser::OPERATOR_PTR;
\+\+							return Parser::OPERATOR_INCR;
--								return Parser::OPERATOR_DECR;
&								return Parser::OPERATOR_BITAND;
\+								return Parser::OPERATOR_PLUS;
-								return Parser::OPERATOR_MINUS;
~								return Parser::OPERATOR_BITNEG;
!								return Parser::OPERATOR_BOOLNEG;
\*								return Parser::OPERATOR_MULT;
\/								return Parser::OPERATOR_DIV;
%								return Parser::OPERATOR_MOD;
\<\<							return Parser::OPERATOR_BSL;
\>\>							return Parser::OPERATOR_BSR;
\<								return Parser::OPERATOR_LT;
\>								return Parser::OPERATOR_GT;
\<=								return Parser::OPERATOR_LTEQU;
\>=								return Parser::OPERATOR_GTEQU;
==								return Parser::OPERATOR_EQUALITY;
!=								return Parser::OPERATOR_NEQU;
\^								return Parser::OPERATOR_BITXOR;
\|								return Parser::OPERATOR_BITOR;
&&								return Parser::OPERATOR_BOOLAND;
\|\|							return Parser::OPERATOR_BOOLOR;
\?								return Parser::OPERATOR_TERNARY;
\*=								return Parser::OPERATOR_MULTEQU;
\/=								return Parser::OPERATOR_DIVEQU;
%=								return Parser::OPERATOR_MODEQU;
\+=								return Parser::OPERATOR_PLUSEQU;
-=								return Parser::OPERATOR_MINUSEQU;
\<\<=							return Parser::OPERATOR_BSLEQU;
\>\>=							return Parser::OPERATOR_BSREQU;
&=								return Parser::OPERATOR_ANDEQU;
\^=								return Parser::OPERATOR_XOREQU;
\|=								return Parser::OPERATOR_OREQU;


/*
*	Punctuator
*/
\.\.\.							return Parser::PUNCTUATOR_TRIPLEDOT;
