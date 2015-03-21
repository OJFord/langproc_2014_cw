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
	SyntaxTree(std::string raw);
	SyntaxTree(SyntaxTreePtrInitList sts);
	
	static std::string computeRaw(SyntaxTreePtrInitList il);
	
	virtual std::string what(void)	const{
		return "SyntaxTree";
	}
	virtual std::string raw(void)	const{
		return _raw;
	}

protected:
	const std::string _raw;
	SyntaxTreePtrVector children;
	
private:
};

class Terminal: public SyntaxTree{
public:
	Terminal(const Token2& tk); 
	
	// Returns terminal name
	std::string what(void) const;

protected:
	const std::string _what;

private:
};

class NonTerminal: public SyntaxTree{
public:
	// Copy-construct
	NonTerminal(const SyntaxTree& stp)
	: SyntaxTree(stp){
	}

	NonTerminal(const SyntaxTreePtrInitList& il)
	: SyntaxTree(il){
	}

private:
};

class TypeSpecifier: public NonTerminal{
public:
	// One branch - copy-construct
	TypeSpecifier(const Terminal* t): NonTerminal(*t){
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
			throw InvalidTokenException(t->raw(), what());
		}
	}

	// structunionspec
	// enumspec
	// typedefname
	
	std::string what(void) const{
		return "TypeSpecifier";
	}
};

class DeclarationSpecifiers: public NonTerminal{
public:
	// Single ast branch is pointless, just copy-construct to produce
	DeclarationSpecifiers(TypeSpecifier* ts)
	: NonTerminal(*ts){
	}
	DeclarationSpecifiers(TypeSpecifier* ts, DeclarationSpecifiers* ds)
	: NonTerminal( SyntaxTreePtrInitList({ts, ds}) ){
	}
	// tq
	// tq ds
	// sc
	// sc ds
	
	std::string what(void) const{
		return "DeclarationSpecifiers";
	}
};

class Identifier: public Terminal{
public:
	Identifier(Token2* tk)
	: Terminal(*tk){
	}
};

class Declarator;
class DirectDeclarator: public NonTerminal{
public:
	DirectDeclarator(Identifier* i);
	// This doesn't create a loop - *d must be enclosed in paren
	//	whereas Declarator(DirectDeclarator*) needs no paren
	DirectDeclarator(Declarator* d);
	//d[ce?]
	//d(ptl)
	//d(il?)

	std::string what(void) const;
};

class Declarator: public NonTerminal{
public:
	Declarator(DirectDeclarator* dd)
	: NonTerminal(*dd){
	}

	/*// Not the worst naming ever, just incredibly meta..
	Declarator(Pointer* p, DirectDeclarator* dd)
	: NonTerminal( SyntaxTreePtrInitList({[, dd]}) ){
	}*/

	std::string what(void) const{
		return "Declarator";
	}
};

class InitialiserDeclarator: public NonTerminal{
public:
	InitialiserDeclarator(Declarator* d)
	: NonTerminal(*d){
	}
	/*InitialiserDeclarator(Declarator* d, Initialiser* i)
	: NonTerminal( SyntaxTreePtrInitList({d, i}) ){
	}*/

	std::string what(void) const{
		return "Initialiser";
	}
};

class InitialiserDeclaratorList: public NonTerminal{
public:
	InitialiserDeclaratorList(InitialiserDeclarator* idl)
	: NonTerminal(*idl){
	}
	InitialiserDeclaratorList(InitialiserDeclarator* id,
		InitialiserDeclaratorList* idl)
	: NonTerminal( SyntaxTreePtrInitList({id, idl}) ){
	}

	std::string what(void) const{
		return "InitialiserDeclaratorList";
	}
};

class Declaration: public NonTerminal{
public:
	Declaration(DeclarationSpecifiers* ds)
	: NonTerminal(*ds){
	}
	Declaration(DeclarationSpecifiers* ds, InitialiserDeclaratorList *idl)
	: NonTerminal( SyntaxTreePtrInitList({ds, idl}) ){
	}

	std::string what(void) const{
		return "Declaration";
	}
};

class ExternalDeclaration: public NonTerminal{
public:
	// One branch - copy-construct
	ExternalDeclaration(Declaration* d)
	: NonTerminal(*d){
	}
	//ExternalDeclaration(FunctionDefinition);
	
	std::string what(void) const{
		return "ExternalDeclaration";
	}
};

class TranslationUnit: public NonTerminal{
public:
	TranslationUnit(TranslationUnit* tu, ExternalDeclaration* ed)
	: NonTerminal( SyntaxTreePtrInitList({tu, ed}) ){
	}

	// One branch - copy-construct
	TranslationUnit(ExternalDeclaration* ed)
	: NonTerminal(*ed){
	}
	
	std::string what(void) const{
		return "TranslationUnit";
	}
};


#endif /* defined(__CARM_Compiler__syntax_tree__) */
