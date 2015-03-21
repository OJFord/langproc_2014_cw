//
//  syntax-tree.h
//  CARM-Compiler
//
//  Created by Ollie Ford on 16/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#ifndef __CARM_Compiler__syntax_tree__
#define __CARM_Compiler__syntax_tree__

#include <vector>
#include <initializer_list>
#include "parser/tokens.h"
#include "parser/exceptions.h"
#include "lexer/lexer.h"

class SyntaxTree;	// forward
typedef std::vector<SyntaxTree*> SyntaxTreePtrVector;
typedef std::initializer_list<SyntaxTree*> SyntaxTreePtrInitList;

class SyntaxTree{
public:
	SyntaxTree(std::string);
	SyntaxTree(SyntaxTreePtrInitList);
	
	static std::string computeRaw(SyntaxTreePtrInitList);
	
	virtual std::string what(void)	const;
	virtual std::string raw(void)	const;

protected:
	const std::string _raw;
	SyntaxTreePtrVector children;
	
private:
};

class Terminal: public SyntaxTree{
public:
	Terminal(const Token2&); 
	
	Token2 token(void) const;
	// Returns terminal name
	std::string what(void) const;

protected:
	const Token2 _token;
	const std::string _what;

private:
};

class NonTerminal: public SyntaxTree{
public:
	// Copy-construct
	NonTerminal(const NonTerminal&) =default;
	NonTerminal(const SyntaxTree&);
	NonTerminal(const SyntaxTreePtrInitList&);

	// Returns name of production
	virtual std::string what(void) const =0;

private:
};

class TypeSpecifier: public NonTerminal{
public:
	TypeSpecifier(const Terminal*);
	// structunionspec
	// enumspec
	// typedefname
	
	std::string what(void) const;

private:
};

class DeclarationSpecifiers: public NonTerminal{
public:
	DeclarationSpecifiers(TypeSpecifier*);
	DeclarationSpecifiers(TypeSpecifier*, DeclarationSpecifiers*);
	
	// tq
	// tq ds
	// sc
	// sc ds
	
	std::string what(void) const;

private:
};

class Identifier: public Terminal{
public:
	Identifier(Token2*);

private:
};

class Declarator;
class DirectDeclarator: public NonTerminal{
public:
	DirectDeclarator(Identifier*);
	// This doesn't create a loop - *d must be enclosed in paren
	//	whereas Declarator(DirectDeclarator*) needs no paren
	DirectDeclarator(Declarator*);
	//d[ce?]
	//d(ptl)
	//d(il?)

	std::string what(void) const;

private:
};

class Declarator: public NonTerminal{
public:
	Declarator(DirectDeclarator*);
	//Declarator(Pointer*, DirectDeclarator*);

	std::string what(void) const;

private:
};

class InitialiserDeclarator: public NonTerminal{
public:
	InitialiserDeclarator(Declarator*);
	//InitialiserDeclarator(Declarator*, Initialiser*);

	std::string what(void) const;

private:
};

class InitialiserDeclaratorList: public NonTerminal{
public:
	InitialiserDeclaratorList(InitialiserDeclarator*);
	InitialiserDeclaratorList(
		InitialiserDeclarator*, InitialiserDeclaratorList*);

	std::string what(void) const;

private:
};

class Declaration: public NonTerminal{
public:
	Declaration(DeclarationSpecifiers*);
	Declaration(DeclarationSpecifiers*, InitialiserDeclaratorList*);

	std::string what(void) const;

private:
};

class ExternalDeclaration: public NonTerminal{
public:
	ExternalDeclaration(Declaration*);
	//ExternalDeclaration(FunctionDefinition*);
	
	std::string what(void) const;

private:
};

class TranslationUnit: public NonTerminal{
public:
	TranslationUnit(TranslationUnit* tu, ExternalDeclaration*);
	TranslationUnit(ExternalDeclaration*);
	
	std::string what(void) const;

private:
};

#endif /* defined(__CARM_Compiler__syntax_tree__) */
