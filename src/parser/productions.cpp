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


FunctionDefinition* Parser::function_definition(void){
	throw "Not implemented.";
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
	throw "Not implemented.";
}


JumpStatement* Parser::jump_statement(void){
	throw "Not implemented.";
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
	throw "Not implemented.";
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
		Token2 tk( lexer->consume(IDENTIFIER) );
		return new Identifier( new Terminal(tk) );	//! Identifier( new Terminal(tk) );
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
	Token2& op = lexer->lookahead(1);
	
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
	throw "Not implemented.";
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
	
	Token2& tk = lexer->lookahead(1);
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
	throw "Not implemented.";
}
PrimaryExpression* Parser::primary_expression(void){
	throw "Not implemented.";
}


StringLiteral* Parser::string_literal(void){
	throw "Not implemented.";
}
CharacterConstant* Parser::character_constant(void){
	throw "Not implemented.";
}
EnumerationConstant* Parser::enumeration_constant(void){
	throw "Not implemented.";
}
IntegerConstant* Parser::integer_constant(void){
	throw "Not implemented.";
}
FloatingConstant* Parser::floating_constant(void){
	throw "Not implemented.";
}
Constant* Parser::constant(void){
	throw "Not implemented.";
}
