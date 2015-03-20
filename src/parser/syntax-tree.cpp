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
}

std::string Terminal::what(void) const{
	return _what;
}

TranslationUnit* Parser::translation_unit(void){
	switch( lexer->lookahead().lexID ){
		case LEX_EOF:
			return nullptr;
		default:
			TranslationUnit* tutree = translation_unit();
			ExternalDeclaration* edtree = external_declaration();
			if( tutree!=nullptr )
				return new TranslationUnit(tutree, edtree);
			else if( edtree!=nullptr )
				return new TranslationUnit(edtree);
			else
				return nullptr;
	}
}

ExternalDeclaration* Parser::external_declaration(void){
	//SyntaxTree* fdtree = function_definition();
	Declaration* dtree = declaration();
	//if( fdtree!=nullptr )
	//	return new FunctionDefinition(fdtree);
	//else
	if( dtree!=nullptr )
		return new ExternalDeclaration(dtree);
	else
		return nullptr;
}

//void Parser::function_definition(void){}
Declaration* Parser::declaration(void){
	DeclarationSpecifiers* dstree = declaration_specifiers();
	if( dstree==nullptr )
		return nullptr;
	//SyntaxTree* idltree= initialiser_declarator_list();
	Terminal* semi = new Terminal( lexer->consume(PUNCOP_SEMICOLON) );
	//if( idltree!=nullptr )
	//	return new Declaration(dstree, idltree);
	//else
	return new Declaration(dstree, semi);
}

DeclarationSpecifiers* Parser::declaration_specifiers(){
	//SyntaxTree* scstree= storage_class_specifier();
	TypeSpecifier* tstree = type_specifier();
	//SyntaxTree* tqtree = type_qualifier();
	DeclarationSpecifiers* dstree = declaration_specifiers();

	/*if( scstree!=nullptr )
		return dstree!=nullptr
		? new DeclarationSpecifiers(scstree, dstree)
		: new DeclarationSpecifiers(scstree);
	
	else*/ if( tstree!=nullptr )
		return dstree!=nullptr
		? new DeclarationSpecifiers(tstree, dstree)
		: new DeclarationSpecifiers(tstree);
	
	/*else if( tqtree!=nullptr )
		return dstree!=nullptr
		? new DeclarationSpecifiers(tqtree, dstree)
		: new DeclarationSpecifiers(tqtree);
	*/ 
	else
		return nullptr;
}

//SyntaxTree* Parser::storage_class_specifier(void){}

TypeSpecifier* Parser::type_specifier(void){
	Token2 tk = lexer->lookahead();

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
