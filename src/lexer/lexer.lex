//
//  lexer.lex
//  CARM-Compiler
//
//  Created by Ollie Ford on 26/02/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

class-name				C89Lexer
%target-directory		"./gen"

%%

NONDIGIT				[_[:alnum:]]
DIGIT					[:digit:]

DIGITSEQ				{DIGIT}+

SIGN					[+-]
EXP						[eE]{SIGN}?{DIGITSEQ}
FLOATSFX				[flFL]
FRACTION				({DIGITSEQ}?\.{DIGITSEQ})
|{DIGITSEQ}\.

NZDIGIT					[1-9]
ODIGIT					[0-7]
HDIGIT					[:xdigit:]
UNSGNDSFX				[uU]
LONGSFX					[lL]
INTSFX					{UNSGNDSFX}
|{LONGSFX}
|{UNSGNDSFX}{LONGSFX}
|{LONGSFX}{UNSGNDSFX}

SESCSEQ					[\'\"\?\\\a\b\f\n\r\t\v]
OESCSEQ					\\{ODIGIT}{1-3}
HESCSEQ					\\{HDIGIT}+
ESCSEQ					{SESCSEQ}|{OESCSEQ}|{HESCSEQ}

CCHAR					[^'\n\\]
|{ESCSEQ}
CCHARSEQ				{CCHAR}+

SCHAR					[^"\n\\]
|{ESCSEQ}
SCHARSEQ				{SCHAR}+

%%

[ \t\n]					//skip


/*
*	Keyword
*/
auto					|
double					|
int						|
struct					|
break					|
else					|
long					|
switch					|
case					|
enum					|
register				|
typedef					|
char					|
extern					|
return					|
union					|
const					|
float					|
short					|
unsigned				|
continue				|
for						|
signed					|
void					|
default					|
goto					|
sizeof					|
volatile				|
do						|
if						|
static					|
while					C89Parser::KEYWORD


/*
*	Identifier
*/
{NONDIGIT}\
[{NONDIGIT}{DIGIT}]*	C89Parser::IDENTIFIER


/*
*	Constant
*/
{FRACTION}				C89Parser::FRACCONSTANT

{FRACTION}\
{EXP}?{FLOATSFX}?		|

{DIGITSEQ}{EXP}\
{FLOATSFX}?				C89Parser::FLOATCONSTANT

{NZDIGIT}\
{DIGIT}*{INTSFX}?		C89Parser::DECCONSTANT

0{ODIGIT}+{INTSFX}?		C89Parser::OCTCONSTANT

0[xX]{HDIGIT}+{INTSFX}?	C89Parser::HEXCONSTANT

\'{CCHARSEQ}\'			C89Parser::CHARCONSTANT

L\'{CCHARSEQ}\'			C89Parser::WCHARCONSTANT

\"{SCHARSEQ}?\"			C89Parser::STRINGLITERAL

L\"{SCHARSEQ}?\"		C89Parser::WSTRINGLITERAL


/*
*	Operator or punctuator
*/
\[						C89Parser::LSQBRACKET
\]						C89Parser::RSQBRACKET
\(						C89Parser::LBRACKET
\)						C89Parser::RBRACKET
\{						C89Parser::LBRACE
\}						C89Parser::RBRACE
\*						C89Parser::ASTERISK
,						C89Parser::COMMA
:						C89Parser::COLON
=						C89Parser::EQUALS
;						C89Parser::SCOLON
#						C89Parser::OCTOTHORPE

/*
*	Operator
*/
\.						C89Parser::OPERATOR_DOT
->						C89Parser::OPERATOR_PTR
++						C89Parser::OPERATOR_INCR
--						C89Parser::OPERATOR_DECR
&						C89Parser::OPERATOR_BITAND
+						C89Parser::OPERATOR_PLUS
-						C89Parser::OPERATOR_MINUS
~						C89Parser::OPERATOR_BITNEG
!						C89Parser::OPERATOR_BOOLNEG
\/						C89Parser::OPERATOR_DIV
%						C89Parser::OPERATOR_MOD
<<						C89Parser::OPERATOR_BSL
>>						C89Parser::OPERATOR_BSR
<						C89Parser::OPERATOR_LT
>						C89Parser::OPERATOR_GT
<=						C89Parser::OPERATOR_LTEQU
>=						C89Parser::OPERATOR_GTEQU
==						C89Parser::OPERATOR_EQUALITY
!=						C89Parser::OPERATOR_NOEQU
\^						C89Parser::OPERATOR_BITXOR
\|						C89Parser::OPERATOR_BITOR
&&						C89Parser::OPERATOR_BOOLAND
\|\|					C89Parser::OPERATOR_BOOLOR
\?						C89Parser::OPERATOR_TERNARY
\*=						C89Parser::OPERATOR_MULTEQU
\/=						C89Parser::OPERATOR_DIVEQU
%=						C89Parser::OPERATOR_MODEQU
+=						C89Parser::OPERATOR_PLUSEQU
-=						C89Parser::OPERATOR_MINUSEQU
<<=						C89Parser::OPERATOR_BSLEQU
>>=						C89Parser::OPERATOR_BSREQU
&=						C89Parser::OPERATOR_ANDEQU
\^=						C89Parser::OPERATOR_XOREQU
\|=						C89Parser::OPERATOR_OREQU


/*
*	Punctuator
*/
\.\.\.					C89Parser::TRIPLEDOTPUNC



/*
*	PREPROCESSOR TOKENS
*/
##						C89Parser::PPOPERATOR_CONCAT
#						C89Parser::PPDIRECTIVE

/*token:

preprocessing-token:
header-name

identifier

pp-number

character-constant

string-literal

operator

punctuator
*/