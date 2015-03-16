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


%x IN_COMMENT
%%

/*
*	Skip
*/

// Comment mini-scanner
<INITIAL>{
	\/\*						begin(StartCondition__::IN_COMMENT);
}
<IN_COMMENT>{
	\*\/						begin(StartCondition__::INITIAL);
	[^*\n]+
	\*
	\n
}
#.*
[ \t\n]


/*
*	Keyword
*/
auto							return KW_AUTO;
double							return KW_DOUBLE;
int								return KW_INT;
struct							return KW_STRUCT;
break							return KW_BREAK;
else							return KW_ELSE;
long							return KW_LONG;
switch							return KW_SWITCH;
case							return KW_CASE;
enum							return KW_ENUM;
register						return KW_REGISTER;
typedef							return KW_TYPEDEF;
char							return KW_CHAR;
extern							return KW_EXTERN;
return							return KW_RETURN;
union							return KW_UNION;
const							return KW_CONST;
float							return KW_FLOAT;
short							return KW_SHORT;
unsigned						return KW_UNSIGNED;
continue						return KW_CONTINUE;
for								return KW_FOR;
signed							return KW_SIGNED;
void							return KW_VOID;
default							return KW_DEFAULT;
goto							return KW_GOTO;
sizeof							return KW_SIZEOF;
volatile						return KW_VOLATILE;
do								return KW_DO;
if								return KW_IF;
static							return KW_STATIC;
while							return KW_WHILE;


/*
*	Identifier
*/
{NONDIGIT}({NONDIGIT}|{DIGIT})*	return IDENTIFIER;


/*
*	Constant
*/
{FRACTION}{EXP}?{FLOATSFX}?		|
{DIGITSEQ}{EXP}{FLOATSFX}?		return CONSTANT_FLOATING;
{NZDIGIT}{DIGIT}*{INTSFX}?		|
0{ODIGIT}+{INTSFX}?				|
0[xX]{HDIGIT}+{INTSFX}?			return CONSTANT_INTEGER;

\'{CCHARSEQ}\'					return CONSTANT_CHAR;
L\'{CCHARSEQ}\'					return CONSTANT_CHAR_WIDE;

\"{SCHARSEQ}?\"					return CONSTANT_STRINGLITERAL;
L\"{SCHARSEQ}?\"				return CONSTANT_STRINGLITERAL_WIDE;


/*
*	Operator or punctuator
*/
\[								return PUNCOP_BRACKET_LEFT;
\]								return PUNCOP_BRACKET_RIGHT;
\(								return PUNCOP_PAREN_LEFT;
\)								return PUNCOP_PAREN_RIGHT;
\{								return PUNCOP_BRACE_LEFT;
\}								return PUNCOP_BRACE_RIGHT;
,								return PUNCOP_COMMA;
:								return PUNCOP_COLON;
=								return PUNCOP_EQUALS;
;								return PUNCOP_SEMICOLON;


/*
*	Operator
*/
\.								return PUNCOP_DOT;
-\>								return PUNCOP_ARROW;
\+\+							return PUNCOP_INCREMENT;
--								return PUNCOP_DECREMENT;
&								return PUNCOP_BIT_AND;
\+								return PUNCOP_PLUS;
-								return PUNCOP_MINUS;
~								return PUNCOP_BIT_NEG;
!								return PUNCOP_BOOL_NEG;
\*								return PUNCOP_MULT;
\/								return PUNCOP_DIV;
%								return PUNCOP_MOD;
\<\<							return PUNCOP_SHIFT_LEFT;
\>\>							return PUNCOP_SHIFT_LEFT;
\<								return PUNCOP_LT;
\>								return PUNCOP_GT;
\<=								return PUNCOP_LT_EQUALS;
\>=								return PUNCOP_GT_EQUALS;
==								return PUNCOP_EQUALITY;
!=								return PUNCOP_NOTEQUALS;
\^								return PUNCOP_BIT_XOR;
\|								return PUNCOP_BIT_OR;
&&								return PUNCOP_BOOL_AND;
\|\|							return PUNCOP_BOOL_OR;
\?								return PUNCOP_TERNARY;
\*=								return PUNCOP_MULT_EQUALS;
\/=								return PUNCOP_DIV_EQUALS;
%=								return PUNCOP_MOD_EQUALS;
\+=								return PUNCOP_PLUS_EQUALS;
-=								return PUNCOP_MINUS_EQUALS;
\<\<=							return PUNCOP_BSL_EQUALS;
\>\>=							return PUNCOP_BSR_EQUALS;
&=								return PUNCOP_AND_EQUALS;
\^=								return PUNCOP_XOR_EQUALS;
\|=								return PUNCOP_OR_EQUALS;


/*
*	Punctuator
*/
\.\.\.							return PUNCOP_ELLIPSIS;
