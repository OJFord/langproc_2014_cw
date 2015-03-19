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

class Declaration: public NonTerminal{
public:
	Declaration(DeclarationSpecifiers* ds, Terminal* semi)
	: NonTerminal( SyntaxTreePtrInitList({ds, semi}) ){
		
		if( semi->what()!="PUNCOP_SEMICOLON" )
			throw InvalidTokenException(";", semi->raw());
	}
	//Declaration(DeclarationSpecifiers, InitialiserDeclaratorList, TPUNCOP_SEMICOLON);
	
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
