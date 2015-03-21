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

Terminal::Terminal(const Token2& tk)
: SyntaxTree(tk.matched), _what(tk.lexed){
	if(what()=="EOF")
		throw UnexpectedEOFException();
}

std::string Terminal::what(void) const{
	return _what;
}

TranslationUnit* Parser::translation_unit(void){
	switch( lexer->lookahead().lexID ){
		case LEX_EOF:
			return nullptr;
		
		default:
			ExternalDeclaration* edtree = external_declaration();
			if( !edtree )
				return nullptr;
			else{
				TranslationUnit* tutree = translation_unit();
				return (tutree)
				? new TranslationUnit(tutree, edtree)
				: new TranslationUnit(edtree);
			}
	}
}

ExternalDeclaration* Parser::external_declaration(void){
	//SyntaxTree* fdtree = function_definition();
	Declaration* dtree = declaration();
	//if( fdtree!=nullptr )
	//	return new FunctionDefinition(fdtree);
	//else
	return (dtree)
	? new ExternalDeclaration(dtree)
	: nullptr;
}

//void Parser::function_definition(void){}
Declaration* Parser::declaration(void){
	DeclarationSpecifiers* dstree = declaration_specifiers();
	if( dstree==nullptr )
		return nullptr;		// Both rules need dec spec
	
	SyntaxTree* idltree= initialiser_declarator_list();

	// Expect semicolon from either rule
	lexer->consume(PUNCOP_SEMICOLON);
	return (idltree)
	? new Declaration(dstree, idltree)
	: new Declaration(dstree);
}

DeclarationSpecifiers* Parser::declaration_specifiers(){
	//SyntaxTree* scstree= storage_class_specifier();
	TypeSpecifier* tstree = type_specifier();
	if(tstree){
		DeclarationSpecifiers* dstree = declaration_specifiers();
		return (dstree)
		? new DeclarationSpecifiers(tstree, dstree)
		: new DeclarationSpecifiers(tstree);
	}
	else
		return nullptr;
	//SyntaxTree* tqtree = type_qualifier();
	

	/*if( scstree!=nullptr )
		return dstree!=nullptr
		? new DeclarationSpecifiers(scstree, dstree)
		: new DeclarationSpecifiers(scstree);
	
	else*/
		
	
	/*else if( tqtree!=nullptr )
		return dstree!=nullptr
		? new DeclarationSpecifiers(tqtree, dstree)
		: new DeclarationSpecifiers(tqtree);
	*/
}

//SyntaxTree* Parser::storage_class_specifier(void){}

TypeSpecifier* Parser::type_specifier(void){
	Token2 tk = lexer->lookahead(1);

	switch(tk.lexID){
		case KW_VOID:
		case KW_CHAR:
		case KW_SHORT:
		case KW_INT:
		case KW_LONG:
		case KW_FLOAT:
		case KW_DOUBLE:
		case KW_SIGNED:
		case KW_UNSIGNED:
			lexer->consume(tk.lexID);
			return new TypeSpecifier( new Terminal(tk) );

		default:
			/*SyntaxTree* sustree= struct_union_specifier();
			SyntaxTree* estree = enum_specifier();
			SyntaxTree* tdntree= typedef_name();
			return	sustree!=nullptr? new TypeSpecifier(sustree)
			:		estree!=nullptr	? new TypeSpecifier(estree)
			:		tdntree!=nullptr? new TypeSpecifier(tdntree)
			: nullptr
			*/return nullptr;
	}
}

//SyntaxTree* Parser::type_qualifier(void){}
