//
//  syntax-tree.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 16/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/parser.h"
#include "parser/syntax-tree.h"

SyntaxTree::SyntaxTree(std::string raw)
: _raw(raw){
}

SyntaxTree::SyntaxTree(SyntaxTreePtrInitList sts)
: _raw( computeRaw(sts) ), children(sts){
}
	
std::string SyntaxTree::computeRaw(SyntaxTreePtrInitList il){
	std::string ret;
	for(auto i:il)
		ret += " "+i->raw();
	return ret;
}

std::string SyntaxTree::what(void)	const{
	return "SyntaxTree";
}

std::string SyntaxTree::raw(void)	const{
	return _raw;
}

Terminal::Terminal(const Token2& tk)
: SyntaxTree(tk.matched), _token(tk), _what(tk.lexed){
	if(what()=="EOF")
		throw UnexpectedEOFException();
}

Token2 Terminal::token(void) const{
	return _token;
}

std::string Terminal::what(void) const{
	return _what;
}

NonTerminal::NonTerminal(const SyntaxTree& stp)
: SyntaxTree(stp){
}

NonTerminal::NonTerminal(const SyntaxTreePtrInitList& il)
: SyntaxTree(il){
}

TypeSpecifier::TypeSpecifier(const Terminal* t): NonTerminal(*t){
	std::string ts = t->what();

	if(	ts != "KW_VOID"
	&&	ts != "KW_CHAR"
	&&	ts != "KW_SHORT"
	&&	ts != "KW_INT"
	&&	ts != "KW_LONG"
	&&	ts != "KW_FLOAT"
	&&	ts != "KW_DOUBLE"
	&&	ts != "KW_SIGNED"
	&&	ts != "KW_UNSIGNED"
	){	// Not the Terminal we're looking for
		throw InvalidTokenException("type specifier", t->token());
	}
}

// structunionspec
// enumspec
// typedefname

std::string TypeSpecifier::what(void) const{
	return "TypeSpecifier";
}

DeclarationSpecifiers::DeclarationSpecifiers(TypeSpecifier* ts)
: NonTerminal(*ts){
}
DeclarationSpecifiers::DeclarationSpecifiers(TypeSpecifier* ts,
	DeclarationSpecifiers* ds)
: NonTerminal( SyntaxTreePtrInitList({ts, ds}) ){
}
// tq
// tq ds
// sc
// sc ds

std::string DeclarationSpecifiers::what(void) const{
	return "DeclarationSpecifiers";
}

Identifier::Identifier(Token2* tk)
: Terminal(*tk){
	if(tk->lexID != IDENTIFIER)
		throw InvalidTokenException("Identifier", *tk);
}

DirectDeclarator::DirectDeclarator(Identifier* i)
: NonTerminal(*i){
}

// This doesn't create a loop - *d must be enclosed in paren
//	whereas Declarator(DirectDeclarator*) needs no paren
DirectDeclarator::DirectDeclarator(Declarator* d)
: NonTerminal(*d){
}

//d[ce?]
//d(ptl)
//d(il?)

std::string DirectDeclarator::what(void) const{
	return "DirectDeclarator";
}

Declarator::Declarator(DirectDeclarator* dd)
: NonTerminal(*dd){
}

/*
Declarator::Declarator(Pointer* p, DirectDeclarator* dd)
: NonTerminal( SyntaxTreePtrInitList({[, dd]}) ){
}
*/

std::string Declarator::what(void) const{
	return "Declarator";
}

InitialiserDeclarator::InitialiserDeclarator(Declarator* d)
: NonTerminal(*d){
}

/*
InitialiserDeclarator::InitialiserDeclarator(Declarator* d,
	Initialiser* i)
: NonTerminal( SyntaxTreePtrInitList({d, i}) ){
}
*/

std::string InitialiserDeclarator::what(void) const{
	return "Initialiser";
}

InitialiserDeclaratorList::InitialiserDeclaratorList(
	InitialiserDeclarator* idl)
: NonTerminal(*idl){
}

InitialiserDeclaratorList::InitialiserDeclaratorList(
	InitialiserDeclarator* id, InitialiserDeclaratorList* idl)
: NonTerminal( SyntaxTreePtrInitList({id, idl}) ){
}

std::string InitialiserDeclaratorList::what(void) const{
	return "InitialiserDeclaratorList";
}

Declaration::Declaration(DeclarationSpecifiers* ds)
: NonTerminal(*ds){
}

Declaration::Declaration(DeclarationSpecifiers* ds,
	InitialiserDeclaratorList *idl)
: NonTerminal( SyntaxTreePtrInitList({ds, idl}) ){
}

std::string Declaration::what(void) const{
	return "Declaration";
}

ExternalDeclaration::ExternalDeclaration(Declaration* d)
: NonTerminal(*d){
}

//ExternalDeclaration::ExternalDeclaration(FunctionDefinition);

std::string ExternalDeclaration::what(void) const{
	return "ExternalDeclaration";
}

TranslationUnit::TranslationUnit(TranslationUnit* tu, ExternalDeclaration* ed)
: NonTerminal( SyntaxTreePtrInitList({tu, ed}) ){
}

TranslationUnit::TranslationUnit(ExternalDeclaration* ed)
: NonTerminal(*ed){
}

std::string TranslationUnit::what(void) const{
	return "TranslationUnit";
}
