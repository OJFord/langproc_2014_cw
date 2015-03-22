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
	Declaration* dtree = declaration();
	if(dtree){
		if(verbose)
			std::cout << "Matched " << dtree->what() << std::endl;
		return new ExternalDeclaration(dtree);
	}
	else try{
		FunctionDefinition* fdtree = function_definition();
		if(verbose)
			std::cout << "Matched " << fdtree->what() << std::endl;
		return new ExternalDeclaration(fdtree);
	}
	catch(InvalidTokenException& e){
		Token tk = lexer->lookahead();
		errors->report( Error(e.what(), tk.srcfile, tk.pos) );
		return nullptr;
	}
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

	// Expect semicolon from either rule
	lexer->consume(PUNCOP_SEMICOLON);
	
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
	throw "Not implemented.";
}
SelectionStatement* Parser::selections_tatement(void){
	throw "Not implemented.";
}
ExpressionStatement* Parser::expression_statement(void){
	throw "Not implemented.";
}
CompoundStatement* Parser::compound_statement(void){
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
	throw "Not implemented.";
}
Statement* Parser::statement(void){
	throw "Not implemented.";
}
StatementList* Parser::statement_list(void){
	throw "Not implemented.";
}


Initialiser* Parser::initialiser(void){
	if( lexer->lookahead(1).lexID == PUNCOP_BRACE_LEFT ){
		lexer->consume(PUNCOP_BRACE_LEFT);
		
		InitialiserList* iltree = initialiser_list();
		if(!iltree)
			throw InvalidTokenException(iltree->what(), lexer->lookahead(1));
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
		Token tk( lexer->consume(IDENTIFIER) );
		return new Identifier( new Terminal(tk) );
	} catch(InvalidTokenException& e){
		return nullptr;
	}
}
IdentifierList* Parser::identifier_list(void){
	throw "Not implemented.";
}


ParameterDeclaration* Parser::parameter_declaration(void){
	throw "Not implemented.";
}
ParameterList* Parser::parameter_list(void){
	throw "Not implemented.";
}
ParameterTypeList* Parser::paramater_type_list(void){
	throw "Not implemented.";
}


TypeQualifier* Parser::type_qualifier(void){
	throw "Not implemented.";
}
TypeQualifierList* Parser::type_qualifier_list(void){
	throw "Not implemented.";
}


Pointer* Parser::pointer(void){
	throw "Not implemented.";
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
	throw "Not implemented.";
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


Expression* Parser::expression(void){
	throw "Not implemented.";
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
			return nullptr;

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
	throw "Not implemented.";
}
ConditionalExpression* Parser::conditional_expression(void){
	LogicalORExpression* loetree = logical_or_expression();
	if(!loetree)
		return nullptr;
	else if(verbose)
		std::cout << "Matched " << loetree->what() << std::endl;
		
	if( lexer->lookahead().lexID == PUNCOP_TERNARY ){
		
		lexer->consume(PUNCOP_TERNARY);
		Expression* etree = expression();
		if(!etree)
			throw InvalidTokenException(etree->what(), lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << etree->what() << std::endl;
		
		lexer->consume(PUNCOP_COLON);
		ConditionalExpression* cetree = conditional_expression();
		if(!cetree)
			throw InvalidTokenException(cetree->what(), lexer->lookahead());
		else if(verbose)
			std::cout << "Matched " << cetree->what() << std::endl;
		
		return new ConditionalExpression(loetree, etree, cetree);
	}
	else
		return new ConditionalExpression(loetree);
}


LogicalORExpression* Parser::logical_or_expression(void){
	throw "Not implemented.";
}
LogicalANDExpression* Parser::logical_and_expression(void){
	throw "Not implemented.";
}
InclusiveORExpression* Parser::inclusive_or_expression(void){
	throw "Not implemented.";
}
ExclusiveORExpression* Parser::exclusive_or_expression(void){
	throw "Not implemented.";
}
ANDExpression* Parser::and_expression(void){
	throw "Not implemented.";
}
EqualityExpression* Parser::equality_expression(void){
	throw "Not implemented.";
}
RelationalExpression* Parser::relational_expression(void){
	throw "Not implemented.";
}


ShiftExpression* Parser::shift_expression(void){
	throw "Not implemented.";
}
AdditiveExpression* Parser::additive_expression(void){
	throw "Not implemented.";
}
MultiplicativeExpression* Parser::multiplicative_expression(void){
	throw "Not implemented.";
}


CastExpression* Parser::cast_expression(void){
	throw "Not implemented.";
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
				throw InvalidTokenException(cetree->what(), lexer->lookahead(1));
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
					throw InvalidTokenException(etree->what(), lexer->lookahead());
				else if(verbose)
					std::cout << "Matched " << etree->what() << std::endl;
				
				lexer->consume(PUNCOP_BRACKET_RIGHT);
				pfetree = new PostfixExpression(pfetree, etree);
				break;
			}
			
			case PUNCOP_PAREN_LEFT: {
				ArgumentExpressionList* aeltree = argument_expression_list();
				
				if(!aeltree)
					throw InvalidTokenException(aeltree->what(), lexer->lookahead());
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
					throw InvalidTokenException(id->what(), lexer->lookahead());
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
	Identifier* id = identifier();
	if(id){
		if(verbose)
			std::cout << "Matched " << id->what() << std::endl;
		return new PrimaryExpression(id);
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
	
	lexer->consume(PUNCOP_PAREN_LEFT);
	Expression* e = expression();
	if(sl){
		if(verbose)
			std::cout << "Matched " << sl->what() << std::endl;
		lexer->consume(PUNCOP_PAREN_RIGHT);
		return new PrimaryExpression(sl);
	}
	else
		throw InvalidTokenException(e->what(), lexer->lookahead());
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
	throw "Not implemented.";
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
