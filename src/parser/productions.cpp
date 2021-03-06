//
//  productons.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 22/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/parser.h"

TranslationUnit* Parser::translation_unit(void){
	switch( lexer->lookahead().lexID ){
		case LEX_EOF:
			return nullptr;
		
		default: {
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
}

ExternalDeclaration* Parser::external_declaration(void){
	try{
		FunctionDefinition* fdtree = function_definition();
		if(!fdtree)
			return nullptr;
		else if(verbose)
			std::cout << "Matched " << fdtree->what() << std::endl;
		
		return new ExternalDeclaration(fdtree);
	}
	catch(InvalidTokenException& e){
		reportInvalidToken(e);
	}
	
	Declaration* dtree = declaration();
	if(dtree){
		if(verbose)
			std::cout << "Matched " << dtree->what() << std::endl;
		return new ExternalDeclaration(dtree);
	}
	else
		return nullptr;
}


FunctionDefinition* Parser::function_definition(void){
	DeclarationSpecifiers* dstree = declaration_specifiers();
	Declarator* dtree = declarator();
	if(!dtree)
		return nullptr;
	DeclarationList* dltree = declaration_list();
	CompoundStatement* cstree = compound_statement();
	if(!cstree)
		return nullptr;
	
	if(verbose){
		if(dstree)
			std::cout << "Matched " << dstree->what() << std::endl;
		std::cout << "Matched " << dtree->what() << std::endl;
		if(dltree)
			std::cout << "Matched " << dltree->what() << std::endl;
		std::cout << "Matched " << cstree->what() << std::endl;
	}
	return new FunctionDefinition(dstree, dtree, dltree, cstree);
}
Declaration* Parser::declaration(void){
	DeclarationSpecifiers* dstree = declaration_specifiers();
	if(!dstree)
		return nullptr;		// Both rules need dec spec
	else if(verbose)
		std::cout << "Matched " << dstree->what() << std::endl;

	InitialiserDeclaratorList* idltree = initialiser_declarator_list();
	if(verbose && idltree)
		std::cout << "Matched " << idltree->what() << std::endl;

	try{
		lexer->consume(PUNCOP_SEMICOLON);
	}
	catch(InvalidTokenException& e){
		reportInvalidToken(e);
	}
	
	return (idltree)
	? new Declaration(dstree, idltree)
	: new Declaration(dstree);
}

DeclarationList* Parser::declaration_list(void){
	Declaration* dtree = declaration();
	if(!dtree)
		return nullptr;
	else if (verbose)
		std::cout << "Matched " << dtree->what() << std::endl;
	
	DeclarationList* dltree = declaration_list();
	if(!dltree)
		return new DeclarationList(dtree);
	else if (verbose)
		std::cout << "Matched " << dltree->what() << std::endl;
	
	return new DeclarationList(dltree, dtree);
}


JumpStatement* Parser::jump_statement(void){
	Token tk = lexer->lookahead(1);
	switch(tk.lexID){
		case KW_GOTO: {
			lexer->consume(KW_GOTO);
			Identifier* id = identifier();
			if(!id)
				return nullptr;
			else if(verbose)
				std::cout << "Matched " << id->what() << std::endl;
			lexer->consume(PUNCOP_SEMICOLON);
			return new JumpStatement(id);
		}

		case KW_CONTINUE:
		case KW_BREAK: {
			lexer->consume(tk.lexID);
			lexer->consume(PUNCOP_SEMICOLON);
			return new JumpStatement(new Terminal(tk));
		}

		case KW_RETURN: {
			lexer->consume(KW_RETURN);
			Expression* etree = expression();
			if(etree && verbose)
				std::cout << "Matched " << etree->what() << std::endl;
			return new JumpStatement(etree);	// may be null
		}

		default:
			return nullptr;
	}
}
IterationStatement* Parser::iteration_statement(void){
	Token& tk = lexer->lookahead();
	
	if(tk.lexID==KW_WHILE){
		lexer->consume(KW_WHILE);
		Token& tpl = lexer->consume(PUNCOP_PAREN_LEFT);
		
		Expression* etree = expression();
		if(!etree)
			throw InvalidTokenException(etree->what(), tpl.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << etree->what() << std::endl;
		
		Token& tpr = lexer->consume(PUNCOP_PAREN_RIGHT);
		Statement* st = statement();
		if(!st)
			throw InvalidTokenException(st->what(), tpr.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << st->what() << std::endl;
		
		return new IterationStatement(etree, st);
	}
	else if(tk.lexID==KW_DO){
		Token& td = lexer->consume(KW_DO);
		
		Statement* st = statement();
		if(!st)
			throw InvalidTokenException(st->what(), td.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << st->what() << std::endl;
		
		lexer->consume(KW_WHILE);
		Token& tpl = lexer->consume(PUNCOP_PAREN_LEFT);
		
		Expression* etree = expression();
		if(!etree)
			throw InvalidTokenException(etree->what(), tpl.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << etree->what() << std::endl;
		
		lexer->consume(PUNCOP_PAREN_RIGHT);
		lexer->consume(PUNCOP_SEMICOLON);
		
		return new IterationStatement(st, etree);
		
	}
	else if(tk.lexID==KW_FOR){
		lexer->consume(KW_FOR);
		lexer->consume(PUNCOP_PAREN_LEFT);

		Expression* etree1 = expression();
		if(etree1 && verbose)
			std::cout << "Matched " << etree1->what() << std::endl;
		lexer->consume(PUNCOP_SEMICOLON);
		
		Expression* etree2 = expression();
		if(etree2 && verbose)
			std::cout << "Matched " << etree2->what() << std::endl;
		lexer->consume(PUNCOP_SEMICOLON);
		
		Expression* etree3 = expression();
		if(etree3 && verbose)
			std::cout << "Matched " << etree3->what() << std::endl;
		Token& tpr = lexer->consume(PUNCOP_PAREN_RIGHT);
		
		Statement* st = statement();
		if(!st)
			throw InvalidTokenException(st->what(), tpr.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << st->what() << std::endl;
		
		return new IterationStatement(etree1, etree2, etree3, st);
	}
	else
		return nullptr;
}
SelectionStatement* Parser::selection_tatement(void){
	Token& tk = lexer->lookahead();
	
	if(tk.lexID==KW_IF){
		lexer->consume(KW_IF);
		
		Token& tpl = lexer->consume(PUNCOP_PAREN_LEFT);
		Expression* etree = expression();
		if(!etree)
			throw InvalidTokenException(etree->what(), tpl.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << etree->what() << std::endl;
		Token& tpr = lexer->consume(PUNCOP_PAREN_RIGHT);
		
		Statement* ist = statement();
		if(!ist)
			throw InvalidTokenException(ist->what(), tpr.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << ist->what() << std::endl;
		
		if( lexer->lookahead().lexID==KW_ELSE ){
			Token& te = lexer->consume(KW_ELSE);
			
			Statement* est = statement();
			if(!est)
				throw InvalidTokenException(est->what(), te.matched, lexer->lookahead());
			else if(verbose)
				std::cout << "Matched " << est->what() << std::endl;
			
			return new SelectionStatement(etree, ist, est);
		}
		else
			return new SelectionStatement(new Terminal(tk), etree, ist);
	}
	else if(tk.lexID==KW_SWITCH){
		lexer->consume(KW_SWITCH);
		
		Token& tpl = lexer->consume(PUNCOP_PAREN_LEFT);
		Expression* etree = expression();
		if(!etree)
			throw InvalidTokenException(etree->what(), tpl.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << etree->what() << std::endl;
		Token& tpr = lexer->consume(PUNCOP_PAREN_RIGHT);
		
		Statement* st = statement();
		if(!st)
			throw InvalidTokenException(st->what(), tpr.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << st->what() << std::endl;
		
		return new SelectionStatement(new Terminal(tk), etree, st);
	}
	else
		return nullptr;
}
ExpressionStatement* Parser::expression_statement(void){
	Expression* etree = expression();
	if(!etree && lexer->lookahead().lexID!=PUNCOP_SEMICOLON)
		return nullptr;
	else if(etree && verbose)
		std::cout << "Matched " << etree->what() << std::endl;
	lexer->consume(PUNCOP_SEMICOLON);
	return new ExpressionStatement(etree);
}
CompoundStatement* Parser::compound_statement(void){
	if(lexer->lookahead().lexID!=PUNCOP_BRACE_LEFT)
		return nullptr;
	
	lexer->consume(PUNCOP_BRACE_LEFT);
	DeclarationList* dltree = declaration_list();
	StatementList* sltree = statement_list();
	if(verbose){
		if(dltree)
			std::cout << "Matched " << dltree->what() << std::endl;
		if(sltree)
			std::cout << "Matched " << sltree->what() << std::endl;
	}
	lexer->consume(PUNCOP_BRACE_RIGHT);
	return new CompoundStatement(dltree, sltree);
}
LabeledStatement* Parser::labeled_statement(void){
	Token& tk = lexer->lookahead();
	switch(tk.lexID){
		case IDENTIFIER: {
			Identifier* id = identifier();
			
			if(verbose)
				std::cout << "Matched " << id->what() << std::endl;
			Token& tc = lexer->consume(PUNCOP_COLON);
			
			Statement* st = statement();
			if(!st)
				throw InvalidTokenException(st->what(), tc.matched, lexer->lookahead());
			else if(verbose)
				std::cout << "Matched " << st->what() << std::endl;
			
			return new LabeledStatement(id, st);
		}
			
		case KW_CASE: {	// ...
			lexer->consume(KW_CASE);
			
			ConstantExpression* cetree = constant_expression();
			if(!cetree)
				throw InvalidTokenException(cetree->what(), tk.matched, lexer->lookahead());
			else if(verbose)
				std::cout << "Matched " << cetree->what() << std::endl;
			
			Token& tc = lexer->consume(PUNCOP_COLON);
			Statement* st = statement();
			if(!st)
				throw InvalidTokenException(st->what(), tc.matched, lexer->lookahead());
			else if(verbose)
				std::cout << "Matched " << st->what() << std::endl;
			
			return new LabeledStatement(cetree, st);
		}
			
		case KW_DEFAULT: {
			lexer->consume(KW_DEFAULT);
			
			Token& tc = lexer->consume(PUNCOP_COLON);
			Statement* st = statement();
			if(!st)
				throw InvalidTokenException(st->what(), tc.matched, lexer->lookahead());
			else if(verbose)
				std::cout << "Matched " << st->what() << std::endl;
			
			return new LabeledStatement(st);
		}
		
		default:
			return nullptr;
	}
}
Statement* Parser::statement(void){
	LabeledStatement* lst = labeled_statement();
	if(lst){
		if(verbose)
			std::cout << "Matched " << lst->what() << std::endl;
		return new Statement(lst);
	}
	
	CompoundStatement* cst = compound_statement();
	if(cst){
		if(verbose)
			std::cout << "Matched " << cst->what() << std::endl;
		return new Statement(cst);
	}
	
	ExpressionStatement* est = expression_statement();
	if(est){
		if(verbose)
			std::cout << "Matched " << est->what() << std::endl;
		return new Statement(est);
	}
	
	SelectionStatement* sst = selection_tatement();
	if(sst){
		if(verbose)
			std::cout << "Matched " << sst->what() << std::endl;
		return new Statement(sst);
	}
	
	IterationStatement* ist = iteration_statement();
	if(ist){
		if(verbose)
			std::cout << "Matched " << ist->what() << std::endl;
		return new Statement(ist);
	}
	
	JumpStatement* jst = jump_statement();
	if(jst){
		if(verbose)
			std::cout << "Matched " << jst->what() << std::endl;
		return new Statement(jst);
	}
	
	return nullptr;
}
StatementList* Parser::statement_list(void){
	Statement* st = statement();
	if(!st)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << st->what() << std::endl;
	
	StatementList* stl = new StatementList(st);
	st = statement();
	while(st){
		stl = new StatementList(stl, st);
		st = statement();
	}
	return stl;
}


Initialiser* Parser::initialiser(void){
	Token& tk = lexer->lookahead();
	if( tk.lexID == PUNCOP_BRACE_LEFT ){
		lexer->consume(tk.lexID);
		
		InitialiserList* iltree = initialiser_list();
		if(!iltree)
			throw InvalidTokenException(iltree->what(), tk.matched, lexer->lookahead(1));
		else if(verbose)
			std::cout << "Matched " << iltree->what() << std::endl;

		if( lexer->lookahead(1).lexID == PUNCOP_COMMA
		&&	lexer->lookahead(2).lexID == PUNCOP_BRACE_RIGHT ){
			lexer->consume(PUNCOP_COMMA);	// `,}' is allowed
		}
		lexer->consume(PUNCOP_BRACE_RIGHT);

		return new Initialiser(iltree);
	}
	
	AssignmentExpression* aetree = assignment_expression();
	if(!aetree)
		return nullptr;
	
	if(verbose)
		std::cout << "Matched " << aetree->what() << std::endl;
	
	return new Initialiser(aetree);
}

InitialiserList* Parser::initialiser_list(void){
	Initialiser* itree = initialiser();
	if(!itree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << itree->what() << std::endl;
	
	if( lexer->lookahead(1).lexID == PUNCOP_COMMA ){
		lexer->consume(PUNCOP_COMMA);
		InitialiserList* iltree = initialiser_list();
		if(!iltree)
			return nullptr;
		else if(verbose)
			std::cout << "Matched " << iltree->what() << std::endl;
		
		return new InitialiserList(iltree, itree);
	}
	else
		return new InitialiserList(itree);
}


TypeDefName* Parser::typedef_name(void){
	throw "Not implemented.";
}
DirectAbstractDeclarator* Parser::direct_abstract_declarator(void){
	throw "Not implemented.";
}
AbstractDeclarator* Parser::abstract_declarator(void){
	throw "Not implemented.";
}


TypeName* Parser::type_name(void){
	throw "Not implemented.";
}
Identifier* Parser::identifier(void){
	try{
		Token& tk = lexer->consume(IDENTIFIER);
		return new Identifier( new Terminal(tk) );
	} catch(InvalidTokenException& e){
		return nullptr;
	}
}
IdentifierList* Parser::identifier_list(void){
	throw "Not implemented.";
}


ParameterDeclaration* Parser::parameter_declaration(void){
	DeclarationSpecifiers* dstree = declaration_specifiers();
	if(!dstree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << dstree->what() << std::endl;
	
	Declarator* dtree = declarator();
	if(dtree){
		if(verbose)
			std::cout << "Matched " << dtree->what() << std::endl;
		return new ParameterDeclaration(dstree, dtree);
	}
	else{
		AbstractDeclarator* adtree = abstract_declarator();
		if(adtree && verbose)
			std::cout << "Matched " << adtree->what() << std::endl;
		
		// even if abstract_declarator null
		return new ParameterDeclaration(dstree, adtree);
	}
}
ParameterList* Parser::parameter_list(void){
	ParameterDeclaration* pdtree = parameter_declaration();
	if(!pdtree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << pdtree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	ParameterList* pltree = new ParameterList(pdtree);
	while( next->lexID==PUNCOP_COMMA ){
		lexer->consume(PUNCOP_COMMA);
		
		pdtree = parameter_declaration();
		if(!pdtree)
			throw InvalidTokenException(
				pdtree->what(), next->matched, lexer->lookahead() );
		
		pltree = new ParameterList(pltree, pdtree);
		
		// more?
		next = &lexer->lookahead();
	}
	return pltree;
}
ParameterTypeList* Parser::parameter_type_list(void){
	ParameterList* pltree = parameter_list();
	if(!pltree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << pltree->what() << std::endl;
	
	if( lexer->lookahead(1).lexID==PUNCOP_COMMA
	&&	lexer->lookahead(2).lexID==PUNCOP_ELLIPSIS ){
		lexer->consume(PUNCOP_COMMA);
		lexer->consume(PUNCOP_ELLIPSIS);
		return new ParameterTypeList(pltree, true);
	}
	else
		return new ParameterTypeList(pltree, false);
}


TypeQualifier* Parser::type_qualifier(void){
	Token& tk = lexer->lookahead();
	
	switch(tk.lexID){
		case KW_CONST:
		case KW_VOLATILE:
			return new TypeQualifier(new Terminal(tk));
			
		default:
			return nullptr;
	}
}
TypeQualifierList* Parser::type_qualifier_list(void){
	TypeQualifier* tq = type_qualifier();
	if(!tq)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << tq->what() << std::endl;
	
	TypeQualifierList* tql = new TypeQualifierList(tq);
	if(verbose)
		std::cout << "Matched " << tql->what() << std::endl;
	tq = type_qualifier();
	
	while(tq){
		tql = new TypeQualifierList(tql, tq);
		if(verbose)
			std::cout << "Matched " << tql->what() << std::endl;
		tq = type_qualifier();
	}
	return tql;
}


Pointer* Parser::pointer(void){
	try{
		lexer->consume(PUNCOP_MULT);
	}
	catch(InvalidTokenException& e){
		reportInvalidToken(e);
		return nullptr;
	}
	TypeQualifierList* tqltree = type_qualifier_list();
	if(tqltree && verbose)
		std::cout << "Matched " << tqltree->what() << std::endl;
	if( lexer->lookahead().lexID==PUNCOP_MULT )
		return new Pointer(tqltree, pointer());
	else
		return new Pointer(tqltree);
}
DirectDeclarator* Parser::direct_declarator(void){
	DirectDeclarator* ddtree;

	if( lexer->lookahead(1).lexID == PUNCOP_PAREN_LEFT ){
		lexer->consume(PUNCOP_PAREN_LEFT);
		
		Declarator* dtree = declarator();
		if(verbose && dtree)
			std::cout << "Matched " << dtree->what() << std::endl;
		ddtree = new DirectDeclarator(dtree);
		
		lexer->consume(PUNCOP_PAREN_RIGHT);
	}
	else{
		Token& tid = lexer->lookahead();
		Identifier* id = identifier();
		if(id){
			if(verbose)
				std::cout << "Matched " << id->what() << std::endl;
			
			symtbl.insert(tid);
			
			ddtree = new DirectDeclarator(id);
			if(verbose && ddtree)
				std::cout << "Matched " << ddtree->what() << std::endl;
		}
		else
			return nullptr;
	}
	
	Token* next = &lexer->lookahead();
	while( next->lexID==PUNCOP_BRACKET_LEFT || next->lexID==PUNCOP_PAREN_LEFT ){
		
		if( next->lexID==PUNCOP_BRACKET_LEFT ){
			lexer->consume(PUNCOP_BRACKET_LEFT);
			ddtree = new DirectDeclarator(ddtree, constant_expression());	// even if null
			lexer->consume(PUNCOP_BRACKET_RIGHT);
		}
		
		else{
			lexer->consume(PUNCOP_PAREN_LEFT);
			
			ParameterTypeList* ptltree = parameter_type_list();
			if(ptltree){
				if(verbose)
					std::cout << "Matched " << ptltree->what() << std::endl;
				ddtree = new DirectDeclarator(ddtree, ptltree);
			}
			else{
				IdentifierList* idltree = identifier_list();
				if(idltree && verbose)
					std::cout << "Matched " << idltree->what() << std::endl;
				ddtree = new DirectDeclarator(ddtree, idltree);	// even if null
			}
			
			lexer->consume(PUNCOP_PAREN_RIGHT);
		}
		// more?
		next = &lexer->lookahead();
	}
	return ddtree;
}

Declarator* Parser::declarator(void){
	Pointer* ptree = pointer();				// so meta..
	
	DirectDeclarator* ddtree = direct_declarator();
	if(!ddtree){
		if(!ptree)
			return nullptr;
		else
			throw InvalidTokenException(ddtree->what(), ptree->what(), lexer->lookahead());
	}
	else if(verbose)
		std::cout << "Matched " << ddtree->what() << std::endl;
	
	return new Declarator(ptree, ddtree);	// even if ptree null
}


Enumerator* Parser::enumerator(void){
	throw "Not implemented.";
}
EnumeratorList* Parser::enumeratorlist(void){
	throw "Not implemented.";
}
EnumerationSpecifier* Parser::enumeration_specifier(void){
	throw "Not implemented.";
}


StructDeclarator* Parser::struct_declarator(void){
	throw "Not implemented.";
}
StructDeclaratorList* Parser::struct_declarator_list(void){
	throw "Not implemented.";
}
SpecifierQualifierList* Parser::specifier_qualifier_list(void){
	throw "Not implemented.";
}
StructDeclaration* Parser::struct_declaration(void){
	throw "Not implemented.";
}
StructDeclarationList* Parser::struct_declaration_list(void){
	throw "Not implemented.";
}
StructOrUnion* Parser::struct_or_union(void){
	throw "Not implemented.";
}
StructOrUnionSpecifier* Parser::struct_or_union_specifier(void){
	throw "Not implemented.";
}


TypeSpecifier* Parser::type_specifier(void){
	Token tk = lexer->lookahead(1);

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
			/*StructOrUnionSpecifier* sustree= struct_or_union_specifier();
			EnumerationSpecifier* estree = enumeration_specifier();
			TypeDefName* tdntree = typedef_name();
			return	(sustree)
			?	new TypeSpecifier(sustree)
			:	(estree)
				?	new TypeSpecifier(estree)
				:	(tdntree)
					?	new TypeSpecifier(tdntree)
					:*/return nullptr;
	}
}

StorageClassSpecifier* Parser::storage_class_specifier(void){
	Token& tk = lexer->lookahead();
	switch(tk.lexID){
		case KW_TYPEDEF:
		case KW_EXTERN:
		case KW_STATIC:
		case KW_AUTO:
		case KW_REGISTER:
			return new StorageClassSpecifier(new Terminal(tk));
		
		default:
			return nullptr;
	}
}
InitialiserDeclarator* Parser::initialiser_declarator(void){
	Declarator* dtree = declarator();
	if(dtree){
		if(verbose)
			std::cout << "Matched " << dtree->what() << std::endl;

		if( lexer->lookahead(1).lexID == PUNCOP_EQUALS ){
			
			lexer->consume(PUNCOP_EQUALS);
			Initialiser* itree = initialiser();
			if(itree && verbose)
				std::cout << "Matched " << itree->what() << std::endl;
			
			return (itree)
			?	new InitialiserDeclarator(dtree, itree)
			:	nullptr;
		}
		else
			return new InitialiserDeclarator(dtree);
	}
	else
		return nullptr;
}

InitialiserDeclaratorList* Parser::initialiser_declarator_list(void){
	InitialiserDeclarator* idtree = initialiser_declarator();
	
	if(idtree){
		if(verbose)
			std::cout << "Matched " << idtree->what() << std::endl;
		
		if( lexer->lookahead(1).lexID == PUNCOP_COMMA ){
			
			lexer->consume(PUNCOP_COMMA);
			InitialiserDeclaratorList* idltree = initialiser_declarator_list();
			
			if(!idltree)
				return nullptr;
			else{
				if(verbose)
					std::cout << "Matched " << idltree->what() << std::endl;
				return new InitialiserDeclaratorList(idtree, idltree);
			}
		}	
		else 
			return new InitialiserDeclaratorList(idtree);
	}
	else
		return nullptr;
}

DeclarationSpecifiers* Parser::declaration_specifiers(void){
	
	StorageClassSpecifier* scstree = storage_class_specifier();
	if(scstree){
		
		DeclarationSpecifiers* dstree = declaration_specifiers();
		if(verbose){
			std::cout << "Matched " << scstree->what() << std::endl;
			if(dstree)
				std::cout << "Matched " << dstree->what() << std::endl;
		}
		
		return (dstree)
		?	new DeclarationSpecifiers(scstree, dstree)
		:	new DeclarationSpecifiers(scstree);
	}
	
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
	
	TypeQualifier* tqtree = type_qualifier();
	if(tqtree){
		
		DeclarationSpecifiers* dstree = declaration_specifiers();
		if(verbose){
			std::cout << "Matched " << tqtree->what() << std::endl;
			if(dstree)
				std::cout << "Matched " << dstree->what() << std::endl;
		}
		
		return (dstree)
		?	new DeclarationSpecifiers(tstree, dstree)
		:	new DeclarationSpecifiers(tstree);
	}
	
	else
		return nullptr;
}


Expression* Parser::expression(void){
	AssignmentExpression* aetree = assignment_expression();
	if(!aetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << aetree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	Expression* etree = new Expression(aetree);
	
	while( next->lexID==PUNCOP_BOOL_AND ){
		lexer->consume(next->lexID);
		
		aetree = assignment_expression();
		if(!aetree)
			throw InvalidTokenException(aetree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << aetree->what() << std::endl;
		
		etree = new Expression(etree, aetree);
		
		// advance
		next = &lexer->lookahead();
	}
	return etree;
}
AssignmentExpression* Parser::assignment_expression(void){
	UnaryExpression* uetree = unary_expression();
	Token& op = lexer->lookahead(1);
	
	if(uetree
	&& (	op.lexID == PUNCOP_EQUALS
		|| (op.lexID >= PUNCOP_MULT_EQUALS && op.lexID <= PUNCOP_OR_EQUALS) )){
		
		if(verbose)
			std::cout << "Matched " << uetree->what() << std::endl;
		
		lexer->consume(op.lexID);
		
		AssignmentExpression* aetree = assignment_expression();
		if(!aetree)
			throw InvalidTokenException(aetree->what(), op.matched, lexer->lookahead());

		if(verbose)
			std::cout << "Matched " << aetree->what() << std::endl;

		return new AssignmentExpression(uetree, new Terminal(op), aetree);
	}
	else{
		ConditionalExpression* cetree = conditional_expression();
		if(!cetree)
			return nullptr;

		if(verbose)
			std::cout << "Matched " << cetree->what() << std::endl;

		return new AssignmentExpression(cetree);
	}
}


ConstantExpression* Parser::constant_expression(void){
	ConditionalExpression* cetree = conditional_expression();
	if(!cetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << cetree->what() << std::endl;
	
	return new ConstantExpression(cetree);
}
ConditionalExpression* Parser::conditional_expression(void){
	LogicalORExpression* loetree = logical_or_expression();
	if(!loetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << loetree->what() << std::endl;
		
	if( lexer->lookahead().lexID == PUNCOP_TERNARY ){
		
		Token& tt = lexer->consume(PUNCOP_TERNARY);
		Expression* etree = expression();
		if(!etree)
			throw InvalidTokenException(etree->what(), tt.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << etree->what() << std::endl;
		
		Token& tc = lexer->consume(PUNCOP_COLON);
		ConditionalExpression* cetree = conditional_expression();
		if(!cetree)
			throw InvalidTokenException(cetree->what(), tc.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << cetree->what() << std::endl;
		
		return new ConditionalExpression(loetree, etree, cetree);
	}
	else
		return new ConditionalExpression(loetree);
}


LogicalORExpression* Parser::logical_or_expression(void){
	LogicalANDExpression* laetree = logical_and_expression();
	if(!laetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << laetree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	LogicalORExpression* loetree = new LogicalORExpression(laetree);
	
	while( next->lexID==PUNCOP_BOOL_AND ){
		lexer->consume(next->lexID);
		
		loetree = logical_or_expression();
		if(!loetree)
			throw InvalidTokenException(loetree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << loetree->what() << std::endl;
		
		loetree = new LogicalORExpression(loetree, laetree);
		
		// advance
		next = &lexer->lookahead();
	}
	return loetree;
}
LogicalANDExpression* Parser::logical_and_expression(void){
	InclusiveORExpression* ioetree = inclusive_or_expression();
	if(!ioetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << ioetree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	LogicalANDExpression* laetree = new LogicalANDExpression(ioetree);
	
	while( next->lexID==PUNCOP_BOOL_AND ){
		lexer->consume(next->lexID);
		
		ioetree = inclusive_or_expression();
		if(!ioetree)
			throw InvalidTokenException(ioetree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << ioetree->what() << std::endl;
		
		laetree = new LogicalANDExpression(laetree, ioetree);
		
		// advance
		next = &lexer->lookahead();
	}
	return laetree;
}
InclusiveORExpression* Parser::inclusive_or_expression(void){
	ExclusiveORExpression* xoetree = exclusive_or_expression();
	if(!xoetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << xoetree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	InclusiveORExpression* ioetree = new InclusiveORExpression(xoetree);
	
	while( next->lexID==PUNCOP_BIT_OR ){
		lexer->consume(next->lexID);
		
		xoetree = exclusive_or_expression();
		if(!xoetree)
			throw InvalidTokenException(xoetree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << xoetree->what() << std::endl;
		
		ioetree = new InclusiveORExpression(ioetree, xoetree);
		
		// advance
		next = &lexer->lookahead();
	}
	return ioetree;
}
ExclusiveORExpression* Parser::exclusive_or_expression(void){
	ANDExpression* aetree = and_expression();
	if(!aetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << aetree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	ExclusiveORExpression* xoetree = new ExclusiveORExpression(aetree);
	
	while( next->lexID==PUNCOP_BIT_XOR ){
		lexer->consume(next->lexID);
		
		aetree = and_expression();
		if(!aetree)
			throw InvalidTokenException(aetree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << aetree->what() << std::endl;
		
		xoetree = new ExclusiveORExpression(xoetree, aetree);
		
		// advance
		next = &lexer->lookahead();
	}
	return xoetree;
}
ANDExpression* Parser::and_expression(void){
	EqualityExpression* eetree = equality_expression();
	if(!eetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << eetree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	ANDExpression* aetree = new ANDExpression(eetree);
	
	while( next->lexID==PUNCOP_BIT_AND ){
		lexer->consume(next->lexID);
		
		eetree = equality_expression();
		if(!eetree)
			throw InvalidTokenException(eetree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << eetree->what() << std::endl;
		
		aetree = new ANDExpression(aetree, eetree);
		
		// advance
		next = &lexer->lookahead();
	}
	return aetree;
}
EqualityExpression* Parser::equality_expression(void){
	RelationalExpression* retree = relational_expression();
	if(!retree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << retree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	EqualityExpression* eetree = new EqualityExpression(retree);
	
	while( next->lexID==PUNCOP_EQUALITY || next->lexID==PUNCOP_NOTEQUALS ){
		lexer->consume(next->lexID);
		
		retree = relational_expression();
		if(!retree)
			throw InvalidTokenException(retree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << retree->what() << std::endl;
		
		eetree = new EqualityExpression(eetree, new Terminal(*next), retree);
		
		// advance
		next = &lexer->lookahead();
	}
	return eetree;
}
RelationalExpression* Parser::relational_expression(void){
	ShiftExpression* setree = shift_expression();
	if(!setree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << setree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	RelationalExpression* retree = new RelationalExpression(setree);
	
	while( next->lexID==PUNCOP_LT || next->lexID==PUNCOP_LT_EQUALS
		|| next->lexID==PUNCOP_GT || next->lexID==PUNCOP_GT_EQUALS ){
		lexer->consume(next->lexID);
		
		setree = shift_expression();
		if(!setree)
			throw InvalidTokenException(setree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << setree->what() << std::endl;
		
		retree = new RelationalExpression(retree, new Terminal(*next), setree);
		
		// advance
		next = &lexer->lookahead();
	}
	return retree;
}


ShiftExpression* Parser::shift_expression(void){
	AdditiveExpression* aetree = additive_expression();
	if(!aetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << aetree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	ShiftExpression* setree = new ShiftExpression(aetree);
	
	while( next->lexID==PUNCOP_SHIFT_LEFT || next->lexID==PUNCOP_SHIFT_RIGHT ){
		lexer->consume(next->lexID);
		
		aetree = additive_expression();
		if(!aetree)
			throw InvalidTokenException(aetree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << aetree->what() << std::endl;
		
		setree = new ShiftExpression(setree, new Terminal(*next), aetree);
		
		// advance
		next = &lexer->lookahead();
	}
	return setree;
}
AdditiveExpression* Parser::additive_expression(void){
	MultiplicativeExpression* metree = multiplicative_expression();
	if(!metree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << metree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	AdditiveExpression* aetree = new AdditiveExpression(metree);
	
	while( next->lexID==PUNCOP_PLUS || next->lexID==PUNCOP_MINUS ){
		lexer->consume(next->lexID);
		
		metree = multiplicative_expression();
		if(!metree)
			throw InvalidTokenException(metree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << metree->what() << std::endl;
		
		aetree = new AdditiveExpression(aetree, new Terminal(*next), metree);
		
		// advance
		next = &lexer->lookahead();
	}
	return aetree;
}
MultiplicativeExpression* Parser::multiplicative_expression(void){
	CastExpression* cetree = cast_expression();
	if(!cetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << cetree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	MultiplicativeExpression* metree = new MultiplicativeExpression(cetree);
	
	while( next->lexID==PUNCOP_MULT || next->lexID==PUNCOP_DIV || next->lexID==PUNCOP_MOD ){
		lexer->consume(next->lexID);
		
		cetree = cast_expression();
		if(!cetree)
			throw InvalidTokenException(cetree->what(), next->matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << cetree->what() << std::endl;
		
		metree = new MultiplicativeExpression(metree, new Terminal(*next), cetree);
		
		// advance
		next = &lexer->lookahead();
	}
	return metree;
}


CastExpression* Parser::cast_expression(void){
	UnaryExpression* uetree = unary_expression();
	if(uetree){
		if(verbose)
			std::cout << "Matched " << uetree->what() << std::endl;
		return new CastExpression(uetree);
	}
	else if( lexer->lookahead().lexID==PUNCOP_PAREN_LEFT ){
		Token& tpl = lexer->consume(PUNCOP_PAREN_LEFT);
		TypeName* tn = type_name();
		
		if(!tn)
			throw InvalidTokenException(tn->what(), tpl.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << tn->what() << std::endl;
		
		Token& tpr = lexer->consume(PUNCOP_PAREN_RIGHT);
		CastExpression* cetree = cast_expression();
		if(!cetree)
			throw InvalidTokenException(cetree->what(), tpr.matched, lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << cetree->what() << std::endl;
		
		return new CastExpression(tn, cetree);
	}
	else
		return nullptr;
}
UnaryExpression* Parser::unary_expression(void){
	
	Token& tk = lexer->lookahead(1);
	switch(tk.lexID){

		case PUNCOP_INCREMENT:
		case PUNCOP_DECREMENT: {
			lexer->consume(tk.lexID);
			UnaryExpression* uetree = unary_expression();
			if(!uetree)
				return nullptr;
			else if(verbose)
				std::cout << "Matched " << uetree->what() << std::endl;
			return new UnaryExpression(new Terminal(tk), uetree);
		}

		case PUNCOP_BIT_AND:
		case PUNCOP_MULT:
		case PUNCOP_PLUS:
		case PUNCOP_MINUS:
		case PUNCOP_BIT_NEG:
		case PUNCOP_BOOL_NEG: {
			lexer->consume(tk.lexID);
			CastExpression* cetree = cast_expression();
			if(!cetree)
				throw InvalidTokenException(cetree->what(), tk.matched, lexer->lookahead(1));
			else if(verbose)
				std::cout << "Matched " << cetree->what() << std::endl;
			return new UnaryExpression(new Terminal(tk), cetree);
		}

		case KW_SIZEOF: {
			lexer->consume(tk.lexID);
			if( lexer->lookahead(1).lexID==PUNCOP_PAREN_LEFT ){
				lexer->consume(PUNCOP_PAREN_LEFT);
				TypeName* tntree = type_name();
				if(!tntree)
					return nullptr;
				lexer->consume(PUNCOP_PAREN_RIGHT);
				if(verbose)
					std::cout << "Matched " << tntree->what() << std::endl;
				return new UnaryExpression(new Terminal(tk), tntree);
			}
			else{
				UnaryExpression* uetree = unary_expression();
				if(!uetree)
					return nullptr;
				else if(verbose)
					std::cout << "Matched " << uetree->what() << std::endl;
				return new UnaryExpression(new Terminal(tk), uetree);
			}
		}

		default: {
			PostfixExpression* petree = postfix_expression();
			if(!petree)
				return nullptr;
			else if(verbose)
				std::cout << "Matched " << petree->what() << std::endl;
			return new UnaryExpression(petree);
		}
	}
}

ArgumentExpressionList* Parser::argument_expression_list(void){
	throw "Not implemented.";
}
PostfixExpression* Parser::postfix_expression(void){
	// start with a primary expr; keep trying to postfix
	PrimaryExpression* pretree = primary_expression();
	if(!pretree)
		return nullptr;
	
	if(verbose)
		std::cout << "Matched " << pretree->what() << std::endl;
	
	Token* next = &lexer->lookahead();
	PostfixExpression* pfetree = new PostfixExpression(pretree);
	while(	next->lexID == PUNCOP_BRACKET_LEFT
		  ||next->lexID == PUNCOP_PAREN_LEFT
		  ||next->lexID == PUNCOP_DOT
		  ||next->lexID == PUNCOP_ARROW
		  ||next->lexID == PUNCOP_INCREMENT
		  ||next->lexID == PUNCOP_DECREMENT	){
		
		lexer->consume(next->lexID);
		switch(next->lexID){
			case PUNCOP_BRACKET_LEFT: {
				Expression* etree = expression();
				
				if(!etree)
					throw InvalidTokenException(etree->what(), next->matched, lexer->lookahead());
				else if(verbose)
					std::cout << "Matched " << etree->what() << std::endl;
				
				lexer->consume(PUNCOP_BRACKET_RIGHT);
				pfetree = new PostfixExpression(pfetree, etree);
				break;
			}
			
			case PUNCOP_PAREN_LEFT: {
				ArgumentExpressionList* aeltree = argument_expression_list();
				
				if(!aeltree)
					throw InvalidTokenException(aeltree->what(), next->matched, lexer->lookahead());
				else if(verbose)
					std::cout << "Matched " << aeltree->what() << std::endl;
				
				lexer->consume(PUNCOP_BRACKET_RIGHT);
				pfetree = new PostfixExpression(pfetree, aeltree);
				break;
			}
			
			case PUNCOP_DOT:
			case PUNCOP_ARROW: {
				Identifier* id = identifier();
				
				if(!id)
					throw InvalidTokenException(id->what(), next->matched, lexer->lookahead());
				else if(verbose)
					std::cout << "Matched " << id->what() << std::endl;
				
				pfetree = new PostfixExpression(pfetree, new Terminal(*next), id);
				break;
			}
				
			case PUNCOP_INCREMENT:
			case PUNCOP_DECREMENT: {
				pfetree = new PostfixExpression(pfetree, new Terminal(*next));
			}
			
			default:		// if we get here there's an error in while condition
				throw -1;	//	or missing case
		}
		
		// advance
		next = &lexer->lookahead();
	}
	return pfetree;
}
PrimaryExpression* Parser::primary_expression(void){
	try{
		Identifier* id = identifier();
		if(id){
			if(verbose)
				std::cout << "Matched " << id->what() << std::endl;
			return new PrimaryExpression(id);
		}
	}
	catch(InvalidTokenException& e){
		reportInvalidToken(e);
	}
	
	Constant* ct = constant();
	if(ct){
		if(verbose)
			std::cout << "Matched " << ct->what() << std::endl;
		return new PrimaryExpression(ct);
	}
	
	StringLiteral* sl = string_literal();
	if(sl){
		if(verbose)
			std::cout << "Matched " << sl->what() << std::endl;
		return new PrimaryExpression(sl);
	}

	Token& tpl = lexer->lookahead();
	if( tpl.lexID==PUNCOP_PAREN_LEFT ){
		lexer->consume(PUNCOP_PAREN_LEFT);
		Expression* e = expression();
		if(e){
			if(verbose)
				std::cout << "Matched " << e->what() << std::endl;
			lexer->consume(PUNCOP_PAREN_RIGHT);
			return new PrimaryExpression(e);
		}
		else
			throw InvalidTokenException(e->what(), tpl.matched, lexer->lookahead());
	}
	else
		return nullptr;
}

WideStringLiteral* Parser::wide_string_literal(void){
	Token& tk = lexer->lookahead();
	if( tk.lexID==CONSTANT_STRINGLITERAL_WIDE )
		return new WideStringLiteral(&tk);
	else
		return nullptr;
}
StringLiteral* Parser::string_literal(void){
	Token& tk = lexer->lookahead();
	if( tk.lexID==CONSTANT_STRINGLITERAL )
		return new StringLiteral(&tk);
	else
		return nullptr;
}
WideCharacterConstant* Parser::wide_character_constant(void){
	Token& tk = lexer->lookahead();
	if( tk.lexID==CONSTANT_CHAR_WIDE )
		return new WideCharacterConstant(&tk);
	else
		return nullptr;
}
CharacterConstant* Parser::character_constant(void){
	Token& tk = lexer->lookahead();
	if( tk.lexID==CONSTANT_CHAR )
		return new CharacterConstant(&tk);
	else
		return nullptr;
}
EnumerationConstant* Parser::enumeration_constant(void){
	Identifier* id = identifier();
	if(!id)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << id->what() << std::endl;
	
	return new EnumerationConstant(id);
}
IntegerConstant* Parser::integer_constant(void){
	Token& tk = lexer->lookahead();
	if( tk.lexID==CONSTANT_INTEGER )
		return new IntegerConstant(&tk);
	else
		return nullptr;
}
FloatingConstant* Parser::floating_constant(void){
	Token& tk = lexer->lookahead();
	if( tk.lexID==CONSTANT_FLOATING )
		return new FloatingConstant(&tk);
	else
		return nullptr;
}
Constant* Parser::constant(void){
	FloatingConstant* fc = floating_constant();
	if(fc){
		if(verbose)
			std::cout << "Matched " << fc->what() << std::endl;
		return new Constant(fc);
	}
	
	IntegerConstant* ic = integer_constant();
	if(ic){
		if(verbose)
			std::cout << "Matched " << ic->what() << std::endl;
		return new Constant(ic);
	}
	
	EnumerationConstant* ec = enumeration_constant();
	if(ec){
		if(verbose)
			std::cout << "Matched " << ec->what() << std::endl;
		return new Constant(ec);
	}
	
	CharacterConstant* cc = character_constant();
	if(cc){
		if(verbose)
			std::cout << "Matched " << cc->what() << std::endl;
		return new Constant(cc);
	}
	
	WideCharacterConstant* wcc = wide_character_constant();
	if(wcc){
		if(verbose)
			std::cout << "Matched " << wcc->what() << std::endl;
		return new Constant(wcc);
	}
	
	return nullptr;
}
