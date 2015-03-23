//
//  syntax-tree.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 16/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include <climits>
#include <cerrno>
#include <cstdlib>
#include <boost/regex.hpp>
#include "parser/parser.h"
#include "parser/syntax-tree.h"

SyntaxTree::SyntaxTree(std::string raw)
: _raw(raw){
}
SyntaxTree::SyntaxTree(SyntaxTree* st)
: _raw( st->raw() ){
}
SyntaxTree::SyntaxTree(SyntaxTreePtrInitList sts)
: subtree(sts), _raw( /*computeRaw(sts)*/ ){
}
SyntaxTree::~SyntaxTree(){
}

/*
std::string SyntaxTree::computeRaw(SyntaxTreePtrInitList il){
	std::string ret;
	for(auto i:il){
		if(i && i->raw().length()>0){
			ret += ""+i->raw();
			//ret += i->raw()+"";		// all differebt and w/wout space.. wtf
			//ret += i->raw();
		}
	}
	return ret;
}
*/

std::string SyntaxTree::raw(void) const{
	return _raw;
}

std::string SyntaxTree::what(void) const{
	return "syntax tree";
}

void treePrinterHelper(std::ostream& os,
	std::string level, const SyntaxTree& st){

	for(size_t i=0; i<st.subtree.size(); ++i){
		if(st.subtree.at(i)){
			std::string sublevel = level; sublevel += "."+std::to_string(i+1);
			os << sublevel << std::string( 50-sublevel.length(), ' ');
			
			os << "| " << st.subtree.at(i)->what();
			os << std::string( 33-st.subtree.at(i)->what().length(), ' ' );
			os << "| " << st.subtree.at(i)->raw() << std::endl;

			treePrinterHelper(os, sublevel, *st.subtree.at(i));
		}
	}
}
std::ostream& operator<<(std::ostream& os, const SyntaxTree& st){
	os << std::string(20, ' ') << "Tree level" << std::string(20, ' ');
	os << "|" << std::string(15, ' ') << "What" << std::string(15, ' ');
	os << "|" << std::string(15, ' ') << "Raw" << std::string(15, ' ') << std::endl;
	os << std::string(50, '-') << "+" << std::string(34, '-');
	os << "+" << std::string(34, '-') << std::endl;
	
	os << "." << std::string(49, ' ');
	os << "| " << st.what() << std::string( 33-st.what().length(), ' ' );
	os << "| " << st.raw() << std::endl;

	treePrinterHelper(os, std::string(), st);
	os << std::endl;

	return os;
}

Terminal::Terminal(const Token& tk)
: SyntaxTree(tk.matched), _token(&tk), _what(tk.lexed){
	if(what()=="EOF")
		throw UnexpectedEOFException();
}
Terminal::Terminal(const SyntaxTree& st)
: SyntaxTree(st){
}
Terminal::~Terminal(){
	delete _token;
}

Token Terminal::token(void) const{
	return *_token;
}

std::string Terminal::what(void) const{
	return "terminal";
}

NonTerminal::NonTerminal(const SyntaxTree& st)
: SyntaxTree(st){
}
NonTerminal::NonTerminal(const SyntaxTreePtrInitList& il)
: SyntaxTree(il){
}
NonTerminal::~NonTerminal(){
}

std::string NonTerminal::what(void) const{
	return "non-terminal";
}

Identifier::Identifier(Token* tk)
: Terminal(*tk){
	if(tk->lexID != IDENTIFIER)
		throw InvalidTokenException(what(), "previous symbol", *tk);
}
Identifier::Identifier(Terminal* t)
: Terminal( SyntaxTreePtrInitList({t}) ){
}

std::string Identifier::what(void) const{
	return "identifier";
}

Constant::Constant(FloatingConstant* fc)
: Terminal(fc){
}
Constant::Constant(IntegerConstant* ic)
: Terminal(ic){
}
Constant::Constant(EnumerationConstant* ec)
: Terminal(ec){
}
Constant::Constant(CharacterConstant* cc)
: Terminal(cc){
}
Constant::Constant(WideCharacterConstant* wcc)
: Terminal(wcc){
}

std::string Constant::what(void) const{
	return "constant";
}

FloatingConstant::FloatingConstant(Token* tk)
:	Terminal(*tk),
	type(isFloat(tk->matched)
	?	FLOAT
	:	isLongDouble(tk->matched)
		?	LONGDOUBLE
		:	DOUBLE
	),
	val(isFloat(tk->matched)
	?	strtof(tk->matched.c_str(), nullptr)
	:	isLongDouble(tk->matched)
		?	strtold(tk->matched.c_str(), nullptr)
		:	strtod(tk->matched.c_str(), nullptr)
	){
	if(	!boost::regex_match(
			tk->matched, boost::regex("[0-9]*.[0-9]+([eE][+-]?[0-9]+)?[flFL]?"))
	&&	!boost::regex_match(
			tk->matched, boost::regex("[0-9]+.([eE][+-]?[0-9]+)?[flFL]?"))
	&&	!boost::regex_match(
			tk->matched, boost::regex("[0-9]+[eE][+-]?[0-9]+[flFL]?"))
	){
		throw InvalidTokenException(what(), "previous symbol", *tk);
	}
}
bool FloatingConstant::isFloat(std::string fc){
	return boost::regex_match(fc, boost::regex("[0-9.eE+-]+[fF]"));
}
bool FloatingConstant::isLongDouble(std::string fc){
	return boost::regex_match(fc, boost::regex("[0-9.eE+-]+[lL]"));
}
bool FloatingConstant::isDouble(std::string fc){
	return boost::regex_match(fc, boost::regex("[0-9.eE+-]+"));
}

std::string FloatingConstant::what(void) const{
	return "floating constant";
}

IntegerConstant::IntegerConstant(Token* tk)
: Terminal(*tk),
	type(isInt(tk->matched)
	?	INT
	:	isUnsignedInt(tk->matched)
		?	UINT
		:	isLongInt(tk->matched)
			? LINT
			:	ULINT
	),
	val(isInt(tk->matched)
	?	toInt(tk->matched)
	:	isUnsignedInt(tk->matched)
		? toUnsignedInt(tk->matched)
		:	isLongInt(tk->matched)
			? toLongInt(tk->matched)
			:	toUnsignedLongInt(tk->matched)
	){
}
	
bool IntegerConstant::isDecimal(std::string ic){
	return boost::regex_match(ic, boost::regex("[1-9][0-9]+[uUlL]*"));
}
bool IntegerConstant::isOctal(std::string ic){
	return boost::regex_match(ic, boost::regex("0[0-7]+[uUlL]*"));
}
bool IntegerConstant::isHexadecimal(std::string ic){
	return boost::regex_match(ic, boost::regex("0[xX][0-9A-Fa-f]+[uUlL]*"));
}

int IntegerConstant::base(std::string ic){
	return isDecimal(ic) ? 10 :	isOctal(ic) ? 8 : isHexadecimal(ic) ? 16 : 0;
}

unsigned long int IntegerConstant::toInt(std::string ic){
	return strtol(ic.c_str(), nullptr, base(ic));
}
bool IntegerConstant::isInt(std::string ic){
	// int if doesn't overflow, and has no u/l suffix
	long int t = toInt(ic);
	return t <= INT_MAX
	&& t >= INT_MIN
	&& boost::regex_match(ic, boost::regex("[0-9a-fA-FxX]+"));
}

unsigned long int IntegerConstant::toUnsignedInt(std::string ic){
	boost::smatch sub;
	boost::regex_match(ic, sub, boost::regex("(0?[xX]?[0-9a-fA-F]+)[uU]?"));
	return strtoul(sub[1].str().c_str(), nullptr, base(ic));
}
bool IntegerConstant::isUnsignedInt(std::string ic){
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

unsigned long int IntegerConstant::toLongInt(std::string ic){
	boost::smatch sub;
	boost::regex_match(ic, sub, boost::regex("((0[xX]?)?[0-9a-fA-F]+)[lL]?"));
	return strtol(sub[1].str().c_str(), nullptr, base(ic));
}
bool IntegerConstant::isLongInt(std::string ic){
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

unsigned long int IntegerConstant::toUnsignedLongInt(std::string ic){
	boost::smatch sub;
	boost::regex_match(ic, sub, boost::regex("((0[xX]?)?[0-9a-fA-F]+)[uUlL]?[uUlL]?"));
	return strtoul(sub[1].str().c_str(), nullptr, base(ic));
}
bool IntegerConstant::isUnsignedLongInt(std::string ic){
	return !isInt(ic)		&& !isUnsignedInt(ic)
	&& !isLongInt(ic)	&& !isUnsignedLongInt(ic);
}

std::string IntegerConstant::what(void) const{
	return "integer constant";
}


EnumerationConstant::EnumerationConstant(Identifier* i)
: Terminal(*i){
}

std::string EnumerationConstant::what(void) const{
	return "enumeration constant";
}

CharacterConstant::CharacterConstant(Token* tk)
: Terminal(*tk), val(toChar(tk->matched)){
}
	
int CharacterConstant::toChar(std::string cc){
	boost::smatch sub;
	boost::regex_match(cc, sub, boost::regex("'(.+)'"));
	return (int)( *(sub[1].str().c_str()) );
}

std::string CharacterConstant::what(void) const{
	return "character constant";
}

WideCharacterConstant::WideCharacterConstant(Token* tk)
: Terminal(tk->matched), val( (wchar_t)*(tk->matched.c_str()) ){
}
	
wchar_t WideCharacterConstant::toWChar(std::string cc){
	boost::smatch sub;
	boost::regex_match(cc, sub, boost::regex("L'(.+)'"));
	return (wchar_t)( *(sub[1].str().c_str()) );
}

std::string WideCharacterConstant::what(void) const{
	return "wide character constant";
}

StringLiteral::StringLiteral(Token* tk)
: Terminal(*tk), val(toStr(tk->matched).data), size(toStr(tk->matched).len){
}

StringLiteral::string StringLiteral::toStr(std::string cc){
	boost::smatch sub;
	boost::regex_match(cc, sub, boost::regex("\"(.+)\""));
	return {
		sub[1].str().c_str(),
		static_cast<unsigned int>( sub[1].length() )
	};
}

std::string StringLiteral::what(void) const{
	return "string literal";
}

WideStringLiteral::WideStringLiteral(Token* tk)
: Terminal(*tk), val(toWStr(tk->matched).data), size(toWStr(tk->matched).len){
}
	
WideStringLiteral::string WideStringLiteral::toWStr(std::string cc){
	boost::smatch sub;
	boost::regex_match(cc, sub, boost::regex("L'(.+)'"));
	const std::string& narrow = sub[1].str();
	return {
		std::wstring(narrow.begin(), narrow.end()).c_str(),
		static_cast<unsigned int>( sub[1].length() )
	};
}

std::string WideStringLiteral::what(void) const{
	return "string literal";
}

PrimaryExpression::PrimaryExpression(Identifier* i)
: NonTerminal( SyntaxTreePtrInitList({i}) ){
}
PrimaryExpression::PrimaryExpression(Constant* c)
: NonTerminal( SyntaxTreePtrInitList({c}) ){
}
PrimaryExpression::PrimaryExpression(StringLiteral* sl)
: NonTerminal( SyntaxTreePtrInitList({sl}) ){
}
PrimaryExpression::PrimaryExpression(Expression* e)
: NonTerminal( SyntaxTreePtrInitList({e}) ){
}

std::string PrimaryExpression::what(void) const{
	return "primary expression";
}

PostfixExpression::PostfixExpression(PrimaryExpression* pe)
: NonTerminal( SyntaxTreePtrInitList({pe}) ){
}
PostfixExpression::PostfixExpression(PostfixExpression* pe, Expression* e)
: NonTerminal( SyntaxTreePtrInitList({pe, e}) ){
}
PostfixExpression::PostfixExpression(PostfixExpression* pe, ArgumentExpressionList* ael)
: NonTerminal( SyntaxTreePtrInitList({pe, ael}) ){
}
PostfixExpression::PostfixExpression(PostfixExpression* pe)
: NonTerminal( SyntaxTreePtrInitList({pe}) ){
}
PostfixExpression::PostfixExpression(PostfixExpression* pe, Terminal* t, Identifier* i)
: NonTerminal( SyntaxTreePtrInitList({pe, t, i}) ){
}
PostfixExpression::PostfixExpression(PostfixExpression* pe, Terminal* t)
: NonTerminal( SyntaxTreePtrInitList({pe, t}) ){
}

std::string PostfixExpression::what(void) const{
	return "postfix expression";
}

ArgumentExpressionList::ArgumentExpressionList(AssignmentExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({ae}) ){
}
ArgumentExpressionList::ArgumentExpressionList(ArgumentExpressionList* ael, AssignmentExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({ae, ael}) ){
}
	
std::string ArgumentExpressionList::what(void) const{
	return "argument expression list";
}

UnaryExpression::UnaryExpression(PostfixExpression* pe)
: NonTerminal( SyntaxTreePtrInitList({pe}) ){
}
UnaryExpression::UnaryExpression(Terminal* t, UnaryExpression* ue)
: NonTerminal( SyntaxTreePtrInitList({t, ue}) ){
}
UnaryExpression::UnaryExpression(Terminal* t, CastExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({t, ce}) ){
}
UnaryExpression::UnaryExpression(Terminal* t, TypeName* tn)
: NonTerminal( SyntaxTreePtrInitList({t, tn}) ){
}

std::string UnaryExpression::what(void) const{
	return "unary expression";
}

CastExpression::CastExpression(UnaryExpression* ue)
: NonTerminal( SyntaxTreePtrInitList({ue}) ){
}
CastExpression::CastExpression(TypeName* tn, CastExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({tn, ce}) ){
}

std::string CastExpression::what(void) const{
	return "cast expression";
}

MultiplicativeExpression::MultiplicativeExpression(CastExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({ce}) ){
}
MultiplicativeExpression::MultiplicativeExpression(
	MultiplicativeExpression* me, Terminal* t, CastExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({me, t, ce}) ){
}

std::string MultiplicativeExpression::what(void) const{
	return "multiplicative expression";
}

AdditiveExpression::AdditiveExpression(MultiplicativeExpression* me)
: NonTerminal( SyntaxTreePtrInitList({me}) ){
}
AdditiveExpression::AdditiveExpression(
	AdditiveExpression* ae, Terminal* t, MultiplicativeExpression* me)
: NonTerminal( SyntaxTreePtrInitList({ae, t, me}) ){
}

std::string AdditiveExpression::what(void) const{
	return "additive expression";
}

ShiftExpression::ShiftExpression(AdditiveExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({ae}) ){
}
ShiftExpression::ShiftExpression(ShiftExpression* se, Terminal* t, AdditiveExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({se, t, ae}) ){
}

std::string ShiftExpression::what(void) const{
	return "shift expression";
}

RelationalExpression::RelationalExpression(ShiftExpression* se)
: NonTerminal( SyntaxTreePtrInitList({se}) ){
}
RelationalExpression::RelationalExpression(
	RelationalExpression* re, Terminal* t, ShiftExpression* se)
: NonTerminal( SyntaxTreePtrInitList({re, t, se}) ){
}

std::string RelationalExpression::what(void) const{
	return "relational expression";
}

EqualityExpression::EqualityExpression(RelationalExpression* re)
: NonTerminal( SyntaxTreePtrInitList({re}) ){
}
EqualityExpression::EqualityExpression(
	EqualityExpression* ee, Terminal* t, RelationalExpression* re)
: NonTerminal( SyntaxTreePtrInitList({ee, t, re}) ){
}

std::string EqualityExpression::what(void) const{
	return "equality expression";
}

ANDExpression::ANDExpression(EqualityExpression* ee)
: NonTerminal( SyntaxTreePtrInitList({ee}) ){
}
ANDExpression::ANDExpression(ANDExpression* ae, EqualityExpression* ee)
: NonTerminal( SyntaxTreePtrInitList({ae, ee}) ){
}

std::string ANDExpression::what(void) const{
	return "AND expression";
}

ExclusiveORExpression::ExclusiveORExpression(ANDExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({ae}) ){
}
ExclusiveORExpression::ExclusiveORExpression(ExclusiveORExpression* xoe, ANDExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({xoe, ae}) ){
}

std::string ExclusiveORExpression::what(void) const{
	return "XOR expression";
}

InclusiveORExpression::InclusiveORExpression(ExclusiveORExpression* xoe)
: NonTerminal( SyntaxTreePtrInitList({xoe}) ){
}
InclusiveORExpression::InclusiveORExpression(
	InclusiveORExpression* ioe, ExclusiveORExpression* xoe)
: NonTerminal( SyntaxTreePtrInitList({ioe, xoe}) ){
}

std::string InclusiveORExpression::what(void) const{
	return "OR expression";
}

LogicalANDExpression::LogicalANDExpression(InclusiveORExpression* ioe)
: NonTerminal( SyntaxTreePtrInitList({ioe}) ){
}
LogicalANDExpression::LogicalANDExpression(LogicalANDExpression* lae, InclusiveORExpression* ioe)
: NonTerminal( SyntaxTreePtrInitList({lae, ioe}) ){
}

std::string LogicalANDExpression::what(void) const{
	return "logical AND expression";
}

LogicalORExpression::LogicalORExpression(LogicalANDExpression* lae)
: NonTerminal( SyntaxTreePtrInitList({lae}) ){
}
LogicalORExpression::LogicalORExpression(LogicalORExpression* loe, LogicalANDExpression* lae)
: NonTerminal( SyntaxTreePtrInitList({loe, lae}) ){
}

std::string LogicalORExpression::what(void) const{
	return "logical OR expression";
}


ConditionalExpression::ConditionalExpression(LogicalORExpression* loe)
: NonTerminal( SyntaxTreePtrInitList({loe}) ){
}
ConditionalExpression::ConditionalExpression(
	LogicalORExpression* loe, Expression* e, ConditionalExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({loe, e, ce}) ){
}

std::string ConditionalExpression::what(void) const{
	return "conditional expression";
}

AssignmentExpression::AssignmentExpression(ConditionalExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({ce}) ){
}
AssignmentExpression::AssignmentExpression(
	UnaryExpression* ue, Terminal* t, AssignmentExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({ue, t, ae}) ){
}

std::string AssignmentExpression::what(void) const{
	return "assignment expression";
}

Expression::Expression(AssignmentExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({ae}) ){
}
Expression::Expression(Expression* e, AssignmentExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({e, ae}) ){
}

std::string Expression::what(void) const{
	return "expression";
}

ConstantExpression::ConstantExpression(ConditionalExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({ce}) ){
}

std::string ConstantExpression::what(void) const{
	return "constant expression";
}

Declaration::Declaration(DeclarationSpecifiers* ds)
: NonTerminal( SyntaxTreePtrInitList({ds}) ){
}
Declaration::Declaration(
	DeclarationSpecifiers* ds, InitialiserDeclaratorList* idl)
: NonTerminal( SyntaxTreePtrInitList({ds, idl}) ){
}

std::string Declaration::what(void) const{
	return "declaration";
}

DeclarationSpecifiers::DeclarationSpecifiers(StorageClassSpecifier* scs)
: NonTerminal( SyntaxTreePtrInitList({scs}) ){
}
DeclarationSpecifiers::DeclarationSpecifiers(
	StorageClassSpecifier* scs, DeclarationSpecifiers* ds)
: NonTerminal( SyntaxTreePtrInitList({scs, ds}) ){
}
DeclarationSpecifiers::DeclarationSpecifiers(TypeSpecifier* ts)
: NonTerminal( SyntaxTreePtrInitList({ts}) ){
}
DeclarationSpecifiers::DeclarationSpecifiers(
	TypeSpecifier* ts, DeclarationSpecifiers* ds)
: NonTerminal( SyntaxTreePtrInitList({ts, ds}) ){
}
DeclarationSpecifiers::DeclarationSpecifiers(TypeQualifier* tq)
: NonTerminal( SyntaxTreePtrInitList({tq}) ){
}
DeclarationSpecifiers::DeclarationSpecifiers(
	TypeQualifier* tq, DeclarationSpecifiers* ds)
: NonTerminal( SyntaxTreePtrInitList({tq, ds}) ){
}

std::string DeclarationSpecifiers::what(void) const{
	return "declaration specifiers";
}

InitialiserDeclaratorList::InitialiserDeclaratorList(
	InitialiserDeclarator* id)
: NonTerminal( SyntaxTreePtrInitList({id}) ){
}
InitialiserDeclaratorList::InitialiserDeclaratorList(
	InitialiserDeclarator* id, InitialiserDeclaratorList* idl)
: NonTerminal( SyntaxTreePtrInitList({id, idl}) ){
}

std::string InitialiserDeclaratorList::what(void) const{
	return "initialiser declarator list";
}

InitialiserDeclarator::InitialiserDeclarator(Declarator* d)
: NonTerminal( SyntaxTreePtrInitList({d}) ){
}
InitialiserDeclarator::InitialiserDeclarator(Declarator* d, Initialiser* i)
: NonTerminal( SyntaxTreePtrInitList({d, i}) ){
}

std::string InitialiserDeclarator::what(void) const{
	return "initialiser declarator";
}

StorageClassSpecifier::StorageClassSpecifier(Terminal* t)
: NonTerminal( SyntaxTreePtrInitList({t}) ){
}

std::string StorageClassSpecifier::what(void) const{
	return "storage class specifier";
}

TypeSpecifier::TypeSpecifier(Terminal* t)
: NonTerminal( SyntaxTreePtrInitList({t}) ){
	lexeme lex = t->token().lexID;

	if(	lex != KW_VOID
	&&	lex != KW_CHAR
	&&	lex != KW_SHORT
	&&	lex != KW_INT
	&&	lex != KW_LONG
	&&	lex != KW_FLOAT
	&&	lex != KW_DOUBLE
	&&	lex != KW_SIGNED
	&&	lex != KW_UNSIGNED
	){	// Not the Terminal we're looking for
		throw InvalidTokenException(what(), "previous symbol", t->token());
	}
}

TypeSpecifier::TypeSpecifier(StructOrUnionSpecifier* sus)
: NonTerminal( SyntaxTreePtrInitList({sus}) ){
}
TypeSpecifier::TypeSpecifier(EnumerationSpecifier* es)
: NonTerminal( SyntaxTreePtrInitList({es}) ){
}
TypeSpecifier::TypeSpecifier(TypeDefName* tdn)
: NonTerminal( SyntaxTreePtrInitList({tdn}) ){
}

std::string TypeSpecifier::what(void) const{
	return "type specifier";
}

StructOrUnionSpecifier::StructOrUnionSpecifier(
	StructOrUnion* su, Identifier* i, StructDeclarationList* sdl)
: NonTerminal( SyntaxTreePtrInitList({su, i, sdl}) ){
}
StructOrUnionSpecifier::StructOrUnionSpecifier(
	StructOrUnion* su, StructDeclarationList* sdl)
: NonTerminal( SyntaxTreePtrInitList({su, sdl}) ){
}

std::string StructOrUnionSpecifier::what(void) const{
	return "struct or union specifier";
}

StructOrUnion::StructOrUnion(Terminal* t)
: NonTerminal( SyntaxTreePtrInitList({t}) ){
}

std::string StructOrUnion::what(void) const{
	return "struct or union";
}

StructDeclarationList::StructDeclarationList(StructDeclarationList* sdl)
: NonTerminal( SyntaxTreePtrInitList({sdl}) ){
}
StructDeclarationList::StructDeclarationList(
	StructDeclarationList* sdl, StructDeclaration* sd)
: NonTerminal( SyntaxTreePtrInitList({sdl, sd}) ){
}

std::string StructDeclarationList::what(void) const{
	return "struct declaration list";
}
/*
StructDeclaration::StructDeclaration(
	StructQualifierList* sql, StructDeclaratorList* sdl)
: NonTerminal( SyntaxTreePtrInitList({sql, sdl}) ){
}
*/
std::string StructDeclaration::what(void) const{
	return "struct declaration";
}

SpecifierQualifierList::SpecifierQualifierList(TypeSpecifier* ts)
: NonTerminal( SyntaxTreePtrInitList({ts}) ){
}
SpecifierQualifierList::SpecifierQualifierList(
	TypeSpecifier* ts, SpecifierQualifierList* sql)
: NonTerminal( SyntaxTreePtrInitList({ts, sql}) ){
}
SpecifierQualifierList::SpecifierQualifierList(TypeQualifier* tq)
: NonTerminal( SyntaxTreePtrInitList({tq}) ){
}
SpecifierQualifierList::SpecifierQualifierList(
	TypeQualifier* tq, SpecifierQualifierList* sql)
: NonTerminal( SyntaxTreePtrInitList({tq, sql}) ){
}

std::string SpecifierQualifierList::what(void) const{
	return "specifier qualifier list";
}

StructDeclaratorList::StructDeclaratorList(StructDeclarator* sd)
: NonTerminal( SyntaxTreePtrInitList({sd}) ){
}
StructDeclaratorList::StructDeclaratorList(
	StructDeclaratorList* sdl, StructDeclarator* sd)
: NonTerminal( SyntaxTreePtrInitList({sdl, sd}) ){
}

std::string StructDeclaratorList::what(void) const{
	return "struct declarator list";
}

StructDeclarator::StructDeclarator(Declarator* d)
: NonTerminal( SyntaxTreePtrInitList({d}) ){
}
StructDeclarator::StructDeclarator(Declarator* d, ConstantExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({d, ce}) ){
}
StructDeclarator::StructDeclarator(ConstantExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({ce}) ){
}

std::string StructDeclarator::what(void) const{
	return "struct declarator";
}

EnumerationSpecifier::EnumerationSpecifier(Identifier* i)
: NonTerminal( SyntaxTreePtrInitList({i}) ){
}
EnumerationSpecifier::EnumerationSpecifier(Identifier* i, EnumeratorList* el)
: NonTerminal( SyntaxTreePtrInitList({i, el}) ){
}
EnumerationSpecifier::EnumerationSpecifier(EnumeratorList* el)
: NonTerminal( SyntaxTreePtrInitList({el}) ){
}

std::string EnumerationSpecifier::what(void) const{
	return "enumeration specifier";
}

EnumeratorList::EnumeratorList(Enumerator* e)
: NonTerminal( SyntaxTreePtrInitList({e}) ){
}
EnumeratorList::EnumeratorList(EnumeratorList* el, Enumerator* e)
: NonTerminal( SyntaxTreePtrInitList({el, e}) ){
}

std::string EnumeratorList::what(void) const{
	return "enumerator list";
}

Enumerator::Enumerator(EnumerationConstant* ec)
: NonTerminal( SyntaxTreePtrInitList({ec}) ){
}
Enumerator::Enumerator(EnumerationConstant* ec, ConstantExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({ec, ce}) ){
}

std::string Enumerator::what(void) const{
	return "enumerator";
}

TypeQualifier::TypeQualifier(Terminal* t)
: Terminal( SyntaxTreePtrInitList({t}) ){
}

std::string TypeQualifier::what(void) const{
	return "type qualifier";
}

Declarator::Declarator(DirectDeclarator* dd)
: NonTerminal( SyntaxTreePtrInitList({dd}) ){
}
Declarator::Declarator(Pointer* p, DirectDeclarator* dd)
: NonTerminal( SyntaxTreePtrInitList({p, dd}) ){
}

std::string Declarator::what(void) const{
	return "declarator";
}

DirectDeclarator::DirectDeclarator(Identifier* i)
: NonTerminal( SyntaxTreePtrInitList({i}) ){
}
DirectDeclarator::DirectDeclarator(Declarator* d)
: NonTerminal( SyntaxTreePtrInitList({d}) ){
}
DirectDeclarator::DirectDeclarator(DirectDeclarator* d, ConstantExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({d, ce}) ){
}
DirectDeclarator::DirectDeclarator(DirectDeclarator* d, ParameterTypeList* ptl)
: NonTerminal( SyntaxTreePtrInitList({d, ptl}) ){
}
DirectDeclarator::DirectDeclarator(DirectDeclarator* d, IdentifierList* il)
: NonTerminal( SyntaxTreePtrInitList({d, il}) ){
}

std::string DirectDeclarator::what(void) const{
	return "direct declarator";
}

Pointer::Pointer(TypeQualifierList* tql)
: NonTerminal( SyntaxTreePtrInitList({tql}) ){
}
Pointer::Pointer(TypeQualifierList* tql, Pointer* p)
: NonTerminal( SyntaxTreePtrInitList({tql, p}) ){
}

std::string Pointer::what(void) const{
	return "pointer";
}

TypeQualifierList::TypeQualifierList(TypeQualifier* tq)
: NonTerminal( SyntaxTreePtrInitList({tq}) ){
}
TypeQualifierList::TypeQualifierList(TypeQualifierList* tql, TypeQualifier* tq)
: NonTerminal( SyntaxTreePtrInitList({tql, tq}) ){
}

std::string TypeQualifierList::what(void) const{
	return "type qualifier list";
}

ParameterTypeList::ParameterTypeList(ParameterList* pl, bool vaList)
: NonTerminal( SyntaxTreePtrInitList({pl}) ){
	if(vaList)
		throw "Not implemented.";
}

std::string ParameterTypeList::what(void) const{
	return "parameter type list";
}

ParameterList::ParameterList(ParameterDeclaration* pd)
: NonTerminal( SyntaxTreePtrInitList({pd}) ){
}
ParameterList::ParameterList(ParameterList* pl, ParameterDeclaration* pd)
: NonTerminal( SyntaxTreePtrInitList({pl, pd}) ){
}

std::string ParameterList::what(void) const{
	return "parameter list";
}

ParameterDeclaration::ParameterDeclaration(DeclarationSpecifiers* ds, Declarator* d)
: NonTerminal( SyntaxTreePtrInitList({ds, d}) ){
}
ParameterDeclaration::ParameterDeclaration(DeclarationSpecifiers* ds)
: NonTerminal( SyntaxTreePtrInitList({ds}) ){
}
ParameterDeclaration::ParameterDeclaration(DeclarationSpecifiers* ds, AbstractDeclarator* ad)
: NonTerminal( SyntaxTreePtrInitList({ds, ad}) ){
}

std::string ParameterDeclaration::what(void) const{
	return "parameter declaration";
}

IdentifierList::IdentifierList(Identifier* i)
: NonTerminal( SyntaxTreePtrInitList({i}) ){
}
IdentifierList::IdentifierList(IdentifierList* il, Identifier* i)
: NonTerminal( SyntaxTreePtrInitList({il, i}) ){
}

std::string IdentifierList::what(void) const{
	return "identifier list";
}

TypeName::TypeName(SpecifierQualifierList* sql)
: NonTerminal( SyntaxTreePtrInitList({sql}) ){
}
TypeName::TypeName(SpecifierQualifierList* sql, AbstractDeclarator* ad)
: NonTerminal( SyntaxTreePtrInitList({sql, ad}) ){
}

std::string TypeName::what(void) const{
	return "type name";
}

AbstractDeclarator::AbstractDeclarator(Pointer* p)
: NonTerminal( SyntaxTreePtrInitList({p}) ){
}
AbstractDeclarator::AbstractDeclarator(DirectAbstractDeclarator* dad)
: NonTerminal( SyntaxTreePtrInitList({dad}) ){
}
AbstractDeclarator::AbstractDeclarator(Pointer* p, DirectAbstractDeclarator* dad)
: NonTerminal( SyntaxTreePtrInitList({p, dad}) ){
}

std::string AbstractDeclarator::what(void) const{
	return "abstract declarator";
}

DirectAbstractDeclarator::DirectAbstractDeclarator(AbstractDeclarator* ad)
: NonTerminal( SyntaxTreePtrInitList({ad}) ){
}
DirectAbstractDeclarator::DirectAbstractDeclarator(DirectAbstractDeclarator* dad, ConstantExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({dad, ce}) ){
}
DirectAbstractDeclarator::DirectAbstractDeclarator(ConstantExpression* ce)
: NonTerminal( SyntaxTreePtrInitList({ce}) ){
}
DirectAbstractDeclarator::DirectAbstractDeclarator(DirectAbstractDeclarator* dad, ParameterTypeList* ptl)
: NonTerminal( SyntaxTreePtrInitList({dad, ptl}) ){
}
DirectAbstractDeclarator::DirectAbstractDeclarator(ParameterTypeList* ptl)
: NonTerminal( SyntaxTreePtrInitList({ptl}) ){
}

std::string DirectAbstractDeclarator::what(void) const{
	return "direct abstract declarator";
}

TypeDefName::TypeDefName(Identifier* i)
: Terminal( SyntaxTreePtrInitList({i}) ){
}

std::string TypeDefName::what(void) const{
	return "typedef name";
}

Initialiser::Initialiser(AssignmentExpression* ae)
: NonTerminal( SyntaxTreePtrInitList({ae}) ){
}

Initialiser::Initialiser(InitialiserList* il)
: NonTerminal( SyntaxTreePtrInitList({il}) ){
}

std::string Initialiser::what(void) const{
	return "initialiser";
}

InitialiserList::InitialiserList(Initialiser* i)
: NonTerminal( SyntaxTreePtrInitList({i}) ){
}
InitialiserList::InitialiserList(InitialiserList* il, Initialiser* i)
: NonTerminal( SyntaxTreePtrInitList({il, i}) ){
}

std::string InitialiserList::what(void) const{
	return "initialiser list";
}

Statement::Statement(LabeledStatement* ls)
: NonTerminal( SyntaxTreePtrInitList({ls}) ){
}
Statement::Statement(CompoundStatement* cs)
: NonTerminal( SyntaxTreePtrInitList({cs}) ){
}
Statement::Statement(ExpressionStatement* es)
: NonTerminal( SyntaxTreePtrInitList({es}) ){
}
Statement::Statement(SelectionStatement* ss)
: NonTerminal( SyntaxTreePtrInitList({ss}) ){
}
Statement::Statement(IterationStatement* is)
: NonTerminal( SyntaxTreePtrInitList({is}) ){
}
Statement::Statement(JumpStatement* js)
: NonTerminal( SyntaxTreePtrInitList({js}) ){
}

std::string Statement::what(void) const{
	return "statement";
}

LabeledStatement::LabeledStatement(Identifier* i, Statement* s)
: NonTerminal( SyntaxTreePtrInitList({i, s}) ){
}
LabeledStatement::LabeledStatement(ConstantExpression* ce, Statement* s)
: NonTerminal( SyntaxTreePtrInitList({ce, s}) ){
}
LabeledStatement::LabeledStatement(Statement* s)
: NonTerminal( SyntaxTreePtrInitList({s}) ){
}

std::string LabeledStatement::what(void) const{
	return "labeled statement";
}

CompoundStatement::CompoundStatement(DeclarationList* dl, StatementList* sl)
: NonTerminal( SyntaxTreePtrInitList({dl, sl}) ){
}

std::string CompoundStatement::what(void) const{
	return "compound statement";
}

DeclarationList::DeclarationList(Declaration* d)
: NonTerminal( SyntaxTreePtrInitList({d}) ){
}
DeclarationList::DeclarationList(DeclarationList* dl, Declaration* d)
: NonTerminal( SyntaxTreePtrInitList({dl, d}) ){
}

std::string DeclarationList::what(void) const{
	return "declaration list";
}

StatementList::StatementList(Statement* s)
: NonTerminal( SyntaxTreePtrInitList({s}) ){
}
StatementList::StatementList(StatementList* sl, Statement* s)
: NonTerminal( SyntaxTreePtrInitList({sl, s}) ){
}

std::string StatementList::what(void) const{
	return "statement list";
}

ExpressionStatement::ExpressionStatement(Expression* e)
: NonTerminal( SyntaxTreePtrInitList({e}) ){
}

std::string ExpressionStatement::what(void) const{
	return "expression statement";
}

SelectionStatement::SelectionStatement(Terminal* t, Expression* e, Statement* s)
: NonTerminal( SyntaxTreePtrInitList({t, e, s}) ){
}
SelectionStatement::SelectionStatement(Expression* e, Statement* s1, Statement* s2)
: NonTerminal( SyntaxTreePtrInitList({e, s1, s2}) ){
}

std::string SelectionStatement::what(void) const{
	return "selection statement";
}

IterationStatement::IterationStatement(Terminal* t, Statement* s)
: NonTerminal( SyntaxTreePtrInitList({t, s}) ){
}
IterationStatement::IterationStatement(Terminal* t, Statement* s, Expression* e)
: NonTerminal( SyntaxTreePtrInitList({t, s, e}) ){
}
IterationStatement::IterationStatement(Terminal* t, Expression* e,
	Statement* s)
: NonTerminal( SyntaxTreePtrInitList({t, e, s}) ){
}
IterationStatement::IterationStatement(Terminal* t, Expression* e1,
	Expression* e2, Statement* s)
: NonTerminal( SyntaxTreePtrInitList({t, e1, e2, s}) ){
}

IterationStatement::IterationStatement(Terminal* t, Expression* e1,
	Expression* e2, Expression* e3, Statement* s)
: NonTerminal( SyntaxTreePtrInitList({t, e1, e2, e3, s}) ){
}

std::string IterationStatement::what(void) const{
	return "iteration statement";
}

JumpStatement::JumpStatement(Terminal* t)
: NonTerminal( SyntaxTreePtrInitList({t}) ){
}
JumpStatement::JumpStatement(Identifier* i)
: NonTerminal( SyntaxTreePtrInitList({i}) ){
}
JumpStatement::JumpStatement(Expression* e)
: NonTerminal( SyntaxTreePtrInitList({e}) ){
}

std::string JumpStatement::what(void) const{
	return "jump statement";
}

ExternalDeclaration::ExternalDeclaration(Declaration* d)
: NonTerminal( SyntaxTreePtrInitList({d}) ){
}

ExternalDeclaration::ExternalDeclaration(FunctionDefinition* fd)
: NonTerminal( SyntaxTreePtrInitList({fd}) ){
}

std::string ExternalDeclaration::what(void) const{
	return "external declaration";
}

TranslationUnit::TranslationUnit(TranslationUnit* tu, ExternalDeclaration* ed)
: NonTerminal( SyntaxTreePtrInitList({tu, ed}) ){
}

TranslationUnit::TranslationUnit(ExternalDeclaration* ed)
: NonTerminal( SyntaxTreePtrInitList({ed}) ){
}

std::string TranslationUnit::what(void) const{
	return "translation unit";
}


FunctionDefinition::FunctionDefinition(DeclarationSpecifiers* ds, Declarator* d,
	DeclarationList* dl, CompoundStatement* cs)
: NonTerminal( SyntaxTreePtrInitList({ds, d, dl, cs}) ){
}

std::string FunctionDefinition::what(void) const{
	return "function definition";
}
