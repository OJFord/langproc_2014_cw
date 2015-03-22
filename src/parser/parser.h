//
//  parser.h
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#ifndef __CARM_Compiler__parser__
#define __CARM_Compiler__parser__

#include "parser/tokens.h"
#include "parser/syntax-tree.h"
#include "lexer/lexer.h"

class Error{
public:
	Error(std::string, std::string, SrcPos=SrcPos(0,0));
	
private:
	const std::string	what;
	const std::string	where;
	const SrcPos		when;
	
	friend std::ostream& operator<<(std::ostream&, const Error&);
};

class ErrorStack{
public:
	ErrorStack(void);
	~ErrorStack(void);
	
	size_t num(void) const;
	void report(const Error&);

protected:
	std::vector<Error*>* impl;
	
private:
	friend std::ostream& operator<<(std::ostream&, const ErrorStack&);
};


class Parser{
public:
	Parser(bool);
	Parser(bool, const char*);
	~Parser(void);
	
	void parse();

protected:
	// Top of the syntax tree
	SyntaxTree* ast;

	// Determines of verbosity of debug info etc
	bool verbose;

private:
	TranslationUnit* translation_unit(void);
	ExternalDeclaration* external_declaration(void);
	
	FunctionDefinition* function_definition(void);
	Declaration* declaration(void);
	DeclarationList* declaration_list(void);
	
	JumpStatement* jump_statement(void);
	IterationStatement* iteration_statement(void);
	SelectionStatement* selections_tatement(void);
	ExpressionStatement* expression_statement(void);
	CompoundStatement* compound_statement(void);
	LabeledStatement* labeled_statement(void);
	Statement* statement(void);
	StatementList* statement_list(void);

	Initialiser* initialiser(void);
	InitialiserList* initialiser_list(void);

	TypeDefName* typedef_name(void);
	DirectAbstractDeclarator* direct_abstract_declarator(void);
	AbstractDeclarator* abstract_declarator(void);

	TypeName* type_name(void);
	Identifier* identifier(void);
	IdentifierList* identifier_list(void);	

	ParameterDeclaration* parameter_declaration(void);
	ParameterList* parameter_list(void);
	ParameterTypeList* paramater_type_list(void);
	
	TypeQualifier* type_qualifier(void);
	TypeQualifierList* type_qualifier_list(void);

	Pointer* pointer(void);
	DirectDeclarator* direct_declarator(void);
	Declarator* declarator(void);

	Enumerator* enumerator(void);
	EnumeratorList* enumeratorlist(void);
	EnumerationSpecifier* enumeration_specifier(void);

	StructDeclarator* struct_declarator(void);
	StructDeclaratorList* struct_declarator_list(void);
	SpecifierQualifierList* specifier_qualifier_list(void);
	StructDeclaration* struct_declaration(void);
	StructDeclarationList* struct_declaration_list(void);
	StructOrUnion* struct_or_union(void);
	StructOrUnionSpecifier* struct_or_union_specifier(void);

	TypeSpecifier* type_specifier(void);
	StorageClassSpecifier* storage_class_specifier(void);
	InitialiserDeclarator* initialiser_declarator(void);
	InitialiserDeclaratorList* initialiser_declarator_list(void);
	DeclarationSpecifiers* declaration_specifiers(void);
	
	Expression* expression(void);
	AssignmentExpression* assignment_expression(void);
	ConstantExpression* constant_expression(void);
	ConditionalExpression* conditional_expression(void);
	
	LogicalORExpression* logical_or_expression(void);
	LogicalANDExpression* logical_and_expression(void);
	InclusiveORExpression* inclusive_or_expression(void);
	ExclusiveORExpression* exclusive_or_expression(void);
	ANDExpression* and_expression(void);
	EqualityExpression* equality_expression(void);
	RelationalExpression* relational_expression(void);
	
	ShiftExpression* shift_expression(void);
	AdditiveExpression* additive_expression(void);
	MultiplicativeExpression* multiplicative_expression(void);
	
	CastExpression* cast_expression(void);
	UnaryExpression* unary_expression(void);
	ArgumentExpressionList* argument_expression_list(void);
	PostfixExpression* postfix_expression(void);
	PrimaryExpression* primary_expression(void);

	StringLiteral* string_literal(void);
	WideStringLiteral* wide_string_literal(void);
	CharacterConstant* character_constant(void);
	WideCharacterConstant* wide_character_constant(void);
	EnumerationConstant* enumeration_constant(void);
	IntegerConstant* integer_constant(void);
	FloatingConstant* floating_constant(void);
	Constant* constant(void);

	Lexer* lexer;
	SymbolTable& symtbl;
	ErrorStack* errors;
};

#endif /* defined(__CARM_Compiler__parser__) */
