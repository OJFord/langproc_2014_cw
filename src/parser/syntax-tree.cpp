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

DirectDeclarator::DirectDeclarator(Identifier* i)
: NonTerminal(*i){
}

// This doesn't create a loop - *d must be enclosed in paren
//	whereas Declarator(DirectDeclarator*) needs no paren
DirectDeclarator::DirectDeclarator(Declarator* d)
: NonTerminal(*d){
}

std::string DirectDeclarator::what(void) const{
	return "DirectDeclarator";
}

TranslationUnit* Parser::translation_unit(void){
	switch( lexer->lookahead().lexID ){
		case LEX_EOF:
			return nullptr;
		
		default:
			ExternalDeclaration* edtree = external_declaration();
			if(edtree){
				
				TranslationUnit* tutree = translation_unit();
				if(verbose){
					std::cout << "Matched " << edtree->what() << std::endl;
					if(tutree)
						std::cout << "Matched " << tutree->what() << std::endl;
				}
				
				return (tutree)
				? new TranslationUnit(tutree, edtree)
				: new TranslationUnit(edtree);
			}
			else{
				return nullptr;
			}
	}
}

ExternalDeclaration* Parser::external_declaration(void){
	//SyntaxTree* fdtree = function_definition();
	Declaration* dtree = declaration();
	if(verbose && dtree)
		std::cout << "Matched " << dtree->what() << std::endl;
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
	if(!dstree)
		return nullptr;		// Both rules need dec spec
	else if(verbose)
		std::cout << "Matched " << dstree->what() << std::endl;

	InitialiserDeclaratorList* idltree = initialiser_declarator_list();
	if(verbose && idltree)
		std::cout << "Matched " << idltree->what() << std::endl;

	// Expect semicolon from either rule
	lexer->consume(PUNCOP_SEMICOLON);
	
	return (idltree)
	? new Declaration(dstree, idltree)
	: new Declaration(dstree);
}

InitialiserDeclaratorList* Parser::initialiser_declarator_list(void){
	InitialiserDeclarator* idtree = initialiser_declarator();
	
	if(idtree){
		if(verbose)
			std::cout << "Matched " << idtree->what() << std::endl;
		
		InitialiserDeclaratorList* idltree = initialiser_declarator_list();
		if(verbose)
			std::cout << "Matched " << idltree->what() << std::endl;

		return (idltree)
		?	new InitialiserDeclaratorList(idtree, idltree)
		:	new InitialiserDeclaratorList(idtree);
	}
	else
		return nullptr;
}

InitialiserDeclarator* Parser::initialiser_declarator(void){
	Declarator* dtree = declarator();
	if(dtree){
		if(verbose)
			std::cout << "Matched " << dtree->what() << std::endl;
		/*Initialiser* i = initialiser();
		if(i){
			lexer->consume(PUNCOP_EQUALS);
			return new InitialiserDeclarator(d, i);
		}
		else*/
			return new InitialiserDeclarator(dtree);
	}
	else
		return nullptr;
}

Declarator* Parser::declarator(void){
	//Pointer* ptree = pointer()	// so meta..
	
	DirectDeclarator* ddtree = direct_declarator();
	if(verbose && ddtree)
		std::cout << "Matched " << ddtree->what() << std::endl;
	
	return (ddtree)
	?	/*(pp)
		? 	new Declarator(ptree, ddtree)
		:*/	new Declarator(ddtree)
	:	nullptr;
}

DirectDeclarator* Parser::direct_declarator(void){
	DirectDeclarator* dd;

	if( lexer->lookahead(1).lexID == PUNCOP_PAREN_LEFT ){
		lexer->consume(PUNCOP_PAREN_LEFT);
		
		Declarator* dtree = declarator();
		if(verbose && dtree)
			std::cout << "Matched " << dtree->what() << std::endl;
		dd = new DirectDeclarator(dtree);
		
		lexer->consume(PUNCOP_PAREN_RIGHT);
	}
	else{
		Identifier* id = identifier();
		if(id){
			if(verbose)
				std::cout << "Matched " << id->what() << std::endl;
			
			dd = new DirectDeclarator(id);
			if(verbose && dd)
				std::cout << "Matched " << id->what() << std::endl;	
		}
		else
			return nullptr;
	}

	//ConstantExpression* ce;
	//ParameterTypeList* ptl;
	//IdentifierList* il;
	// new lookahead, we already matched&consumed whatever above
	switch( lexer->lookahead(1).lexID ){
		case PUNCOP_BRACKET_LEFT:
			/*lexer->consume(PUNCOP_BRACK_LEFT);
			ce = constant_expression();
			lexer->consume(PUNCOP_BRACK_RIGHT);
			// constant expr optional, so pass even if null since carrys
			//	the information " `[]` was matched " implicitly
			return new DirectDeclarator(dd, ce);
			*/throw "Not implemented";

		case PUNCOP_PAREN_LEFT:
			/*lexer->consume(PUNCOP_PAREN_LEFT);
			ptl = parameter_type_list();
			if(!ptl)
				il	= identifier_list();
			lexer->consume(PUNCOP_PAREN_RIGHT);
			// id list optional, so pass even if null since carrys
			//	the information " `()` was matched " implicitly
			return new DirectDeclarator(dd, (ptl)?ptl:il );
			*/throw "Not implemented";

		default:
			return dd;
	}
}

Identifier* Parser::identifier(void){
	try{
		Token2 tk = lexer->consume(IDENTIFIER);
		return new Identifier(&tk);
	} catch(InvalidTokenException& e){
		return nullptr;
	}
}

DeclarationSpecifiers* Parser::declaration_specifiers(void){
	//SyntaxTree* scstree= storage_class_specifier();
	TypeSpecifier* tstree = type_specifier();
	if(tstree){
		
		DeclarationSpecifiers* dstree = declaration_specifiers();
		if(verbose){
			std::cout << "Matched " << tstree->what() << std::endl;
			if(dstree)
				std::cout << "Matched " << dstree->what() << std::endl;
		}

		return (dstree)
		?	new DeclarationSpecifiers(tstree, dstree)
		:	new DeclarationSpecifiers(tstree);
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
