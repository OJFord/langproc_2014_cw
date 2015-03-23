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
	SyntaxTree(SyntaxTree*);
	SyntaxTree(SyntaxTreePtrInitList);
	virtual ~SyntaxTree();

	static std::string computeRaw(SyntaxTreePtrInitList);
	
	virtual std::string what(void) const;
	virtual std::string raw(void) const;

	// The n-branch tree beneath *this
	SyntaxTreePtrVector subtree;

protected:
	const std::string _raw;
	
private:
	friend std::ostream& operator<<(std::ostream&, const SyntaxTree&);
};

class Terminal: public SyntaxTree{
public:
	Terminal(const Token&);
	Terminal(const SyntaxTree&);
	virtual ~Terminal();
	
	Token token(void) const;
	// Returns terminal name
	virtual std::string what(void) const;

protected:
	const Token* _token;
	const std::string _what;

private:
};

class NonTerminal: public SyntaxTree{
public:
	// Copy-construct
	NonTerminal(const NonTerminal&) =default;
	NonTerminal(const SyntaxTree&);
	NonTerminal(const SyntaxTreePtrInitList&);
	virtual ~NonTerminal();

	// Returns name of production
	std::string what(void) const;

private:
};

class Identifier: public Terminal{
public:
	Identifier(Token*);
	Identifier(Terminal*);
	
	std::string what(void) const;

private:
};

class FloatingConstant;
class IntegerConstant;
class EnumerationConstant;
class CharacterConstant;
class WideCharacterConstant;
class Constant: public Terminal{
public:
	Constant(FloatingConstant*);
	Constant(IntegerConstant*);
	Constant(EnumerationConstant*);
	Constant(CharacterConstant*);
	Constant(WideCharacterConstant*);
	
	std::string what(void) const;
	
private:
};


enum floatingtype{
	DOUBLE,
	FLOAT,
	LONGDOUBLE
};
class FloatingConstant: public Terminal{
public:
	FloatingConstant(Token*);

	static bool isFloat(std::string);
	static bool isLongDouble(std::string);
	static bool isDouble(std::string);
	
	std::string what(void) const;
	const floatingtype type;
	const long double val;
	
private:
};

enum integertype{
	INT,
	UINT,
	LINT,
	ULINT
};
class IntegerConstant: public Terminal{
public:	
	IntegerConstant(Token*);
	
	static bool isDecimal(std::string);
	static bool isOctal(std::string);
	static bool isHexadecimal(std::string);
	
	static int base(std::string);
	
	static unsigned long int toInt(std::string);
	static bool isInt(std::string);
	
	static unsigned long int toUnsignedInt(std::string);
	static bool isUnsignedInt(std::string);
	
	static unsigned long int toLongInt(std::string);
	static bool isLongInt(std::string);
	
	static unsigned long int toUnsignedLongInt(std::string);
	static bool isUnsignedLongInt(std::string);
	
	std::string what(void) const;
	const integertype type;
	const unsigned long int val;

private:
};

class EnumerationConstant: public Terminal{
public:
	EnumerationConstant(Identifier*);
	
	std::string what(void) const;

private:
};

class CharacterConstant: public Terminal{
public:
	CharacterConstant(Token*);
	
	std::string what(void) const;
	static int toChar(std::string);
	const int val;

private:
};

class WideCharacterConstant: public Terminal{
public:
	WideCharacterConstant(Token*);

	std::string what(void) const;
	static wchar_t toWChar(std::string);
	const wchar_t val;

private:
};

class StringLiteral: public Terminal{
public:
	StringLiteral(Token*);
	
	struct string{
		const char* data;
		const unsigned len;
	};
	
	std::string what(void) const;
	static string toStr(std::string);

	const char* val;
	const unsigned size;

private:
};

class WideStringLiteral: public Terminal{
public:
	WideStringLiteral(Token*);
	
	struct string{
		const wchar_t* data;
		const unsigned len;
	};
	
	std::string what(void) const;
	static string toWStr(std::string);

	const wchar_t* val;
	const unsigned size;

private:
};

class Expression;
class PrimaryExpression: public NonTerminal{
public:
	PrimaryExpression(Identifier*);
	PrimaryExpression(Constant*);
	PrimaryExpression(StringLiteral*);
	PrimaryExpression(Expression*);

	std::string what(void) const;
	
private:
};

class ArgumentExpressionList;
class PostfixExpression: public NonTerminal{
public:
	PostfixExpression(PrimaryExpression*);
	PostfixExpression(PostfixExpression*, Expression*);
	PostfixExpression(PostfixExpression*, ArgumentExpressionList*);
	PostfixExpression(PostfixExpression*);
	PostfixExpression(PostfixExpression*, Terminal*, Identifier*);
	PostfixExpression(PostfixExpression*, Terminal*);
	
	std::string what(void) const;
	
private:
};

class AssignmentExpression;
class ArgumentExpressionList: public NonTerminal{
public:
	ArgumentExpressionList(AssignmentExpression*);
	ArgumentExpressionList(ArgumentExpressionList*, AssignmentExpression*);
	
	std::string what(void) const;
	
private:
};

class CastExpression;
class TypeName;
class UnaryExpression: public NonTerminal{
public:
	UnaryExpression(PostfixExpression*);
	UnaryExpression(Terminal*, UnaryExpression*);
	UnaryExpression(Terminal*, CastExpression*);
	UnaryExpression(Terminal*, TypeName*);
	
	std::string what(void) const;
	
private:
};

class CastExpression: public NonTerminal{
public:
	CastExpression(UnaryExpression*);
	CastExpression(TypeName*, CastExpression*);
	
	std::string what(void) const;
	
private:
};

class MultiplicativeExpression: public NonTerminal{
public:
	MultiplicativeExpression(CastExpression*);
	MultiplicativeExpression(MultiplicativeExpression*, Terminal*, CastExpression*);
	
	std::string what(void) const;
	
private:
};

class AdditiveExpression: public NonTerminal{
public:
	AdditiveExpression(MultiplicativeExpression*);
	AdditiveExpression(AdditiveExpression*, Terminal*, MultiplicativeExpression*);
	
	std::string what(void) const;
	
private:
};

class ShiftExpression: public NonTerminal{
public:
	ShiftExpression(AdditiveExpression*);
	ShiftExpression(ShiftExpression*, Terminal*, AdditiveExpression*);
	
	std::string what(void) const;
	
private:
};

class RelationalExpression: public NonTerminal{
public:
	RelationalExpression(ShiftExpression*);
	RelationalExpression(RelationalExpression*, Terminal*, ShiftExpression*);
	
	std::string what(void) const;
	
private:
};

class EqualityExpression: public NonTerminal{
public:
	EqualityExpression(RelationalExpression*);
	EqualityExpression(EqualityExpression*, Terminal*, RelationalExpression*);
	
	std::string what(void) const;
	
private:
};

class ANDExpression: public NonTerminal{
public:
	ANDExpression(EqualityExpression*);
	ANDExpression(ANDExpression*, EqualityExpression*);
	
	std::string what(void) const;
	
private:
};

class ExclusiveORExpression: public NonTerminal{
public:
	ExclusiveORExpression(ANDExpression*);
	ExclusiveORExpression(ExclusiveORExpression*, ANDExpression*);
	
	std::string what(void) const;
	
private:
};

class InclusiveORExpression: public NonTerminal{
public:
	InclusiveORExpression(ExclusiveORExpression*);
	InclusiveORExpression(InclusiveORExpression*, ExclusiveORExpression*);
	
	std::string what(void) const;
	
private:
};

class LogicalANDExpression: public NonTerminal{
public:
	LogicalANDExpression(InclusiveORExpression*);
	LogicalANDExpression(LogicalANDExpression*, InclusiveORExpression*);
	
	std::string what(void) const;
	
private:
};

class LogicalORExpression: public NonTerminal{
public:
	LogicalORExpression(LogicalANDExpression*);
	LogicalORExpression(LogicalORExpression*, LogicalANDExpression*);
	
	std::string what(void) const;
	
private:
};

class ConditionalExpression: public NonTerminal{
public:
	ConditionalExpression(LogicalORExpression*);
	ConditionalExpression(LogicalORExpression*, Expression*, ConditionalExpression*);
	
	std::string what(void) const;
	
private:
};

class AssignmentExpression: public NonTerminal{
public:
	AssignmentExpression(ConditionalExpression*);
	AssignmentExpression(UnaryExpression*, Terminal*, AssignmentExpression*);
	
	std::string what(void) const;
	
private:
};

class Expression: public NonTerminal{
public:
	Expression(AssignmentExpression*);
	Expression(Expression*, AssignmentExpression*);
	
	std::string what(void) const;
	
private:
};

class ConstantExpression: public NonTerminal{
public:
	ConstantExpression(ConditionalExpression*);
	
	std::string what(void) const;
	
private:
};

class DeclarationSpecifiers;
class InitialiserDeclaratorList;
class Declaration: public NonTerminal{
public:
	Declaration(DeclarationSpecifiers*);
	Declaration(DeclarationSpecifiers*, InitialiserDeclaratorList*);
	
	std::string what(void) const;
	
private:
};

class StorageClassSpecifier;
class TypeSpecifier;
class TypeQualifier;
class DeclarationSpecifiers: public NonTerminal{
public:
	DeclarationSpecifiers(StorageClassSpecifier*);
	DeclarationSpecifiers(StorageClassSpecifier*, DeclarationSpecifiers*);
	DeclarationSpecifiers(TypeSpecifier*);
	DeclarationSpecifiers(TypeSpecifier*, DeclarationSpecifiers*);
	DeclarationSpecifiers(TypeQualifier*);
	DeclarationSpecifiers(TypeQualifier*, DeclarationSpecifiers*);
	
	std::string what(void) const;
	
private:
};

class InitialiserDeclarator;
class InitialiserDeclaratorList: public NonTerminal{
public:
	InitialiserDeclaratorList(InitialiserDeclarator*);
	InitialiserDeclaratorList(InitialiserDeclarator*, InitialiserDeclaratorList*);
	
	std::string what(void) const;
	
private:
};

class Declarator;
class Initialiser;
class InitialiserDeclarator: public NonTerminal{
public:
	InitialiserDeclarator(Declarator*);
	InitialiserDeclarator(Declarator*, Initialiser*);
	
	std::string what(void) const;
	
private:
};

class StorageClassSpecifier: public NonTerminal{
public:
	StorageClassSpecifier(Terminal*);
	
	std::string what(void) const;
	
private:
};

class StructOrUnionSpecifier;
class EnumerationSpecifier;
class TypeDefName;
class TypeSpecifier: public NonTerminal{
public:
	TypeSpecifier(Terminal*);
	TypeSpecifier(StructOrUnionSpecifier*);
	TypeSpecifier(EnumerationSpecifier*);
	TypeSpecifier(TypeDefName*);
	
	std::string what(void) const;
	
private:
};

class StructOrUnion;
class StructDeclarationList;
class StructOrUnionSpecifier: public NonTerminal{
public:
	StructOrUnionSpecifier(StructOrUnion*, Identifier*, StructDeclarationList*);
	StructOrUnionSpecifier(StructOrUnion*, StructDeclarationList*);
	
	std::string what(void) const;
	
private:
};

class StructOrUnion: public NonTerminal{
public:
	StructOrUnion(Terminal*);
	
	std::string what(void) const;
	
private:
};

class StructDeclaration;
class StructDeclarationList: public NonTerminal{
public:
	StructDeclarationList(StructDeclarationList*);
	StructDeclarationList(StructDeclarationList*, StructDeclaration*);
	
	std::string what(void) const;
	
private:
};

class StructQualifierList;
class StructDeclaratorList;
class StructDeclaration: public NonTerminal{
public:
	StructDeclaration(StructQualifierList*, StructDeclaratorList*);

	std::string what(void) const;
	
private:
};

class SpecifierQualifierList: public NonTerminal{
public:
	SpecifierQualifierList(TypeSpecifier*);
	SpecifierQualifierList(TypeSpecifier*, SpecifierQualifierList*);
	SpecifierQualifierList(TypeQualifier*);
	SpecifierQualifierList(TypeQualifier*, SpecifierQualifierList*);

	std::string what(void) const;
	
private:
};

class StructDeclarator;
class StructDeclaratorList: public NonTerminal{
public:
	StructDeclaratorList(StructDeclarator*);
	StructDeclaratorList(StructDeclaratorList*, StructDeclarator*);

	std::string what(void) const;
	
private:
};

class Declarator;
class StructDeclarator: public NonTerminal{
public:
	StructDeclarator(Declarator*);
	StructDeclarator(Declarator*, ConstantExpression*);
	StructDeclarator(ConstantExpression*);

	std::string what(void) const;
	
private:
};

class EnumeratorList;
class EnumerationSpecifier: public NonTerminal{
public:
	EnumerationSpecifier(Identifier*);
	EnumerationSpecifier(Identifier*, EnumeratorList*);
	EnumerationSpecifier(EnumeratorList*);
	
	std::string what(void) const;
	
private:
};

class Enumerator;
class EnumeratorList: public NonTerminal{
public:
	EnumeratorList(Enumerator*);
	EnumeratorList(EnumeratorList*, Enumerator*);
	
	std::string what(void) const;
	
private:
};

class Enumerator: public NonTerminal{
public:
	Enumerator(EnumerationConstant*);
	Enumerator(EnumerationConstant*, ConstantExpression*);
	
	std::string what(void) const;
	
private:
};

class TypeQualifier: public Terminal{
public:
	TypeQualifier(Terminal*);
	
	std::string what(void) const;
	
private:
};

class DirectDeclarator;
class Pointer;
class Declarator: public NonTerminal{
public:
	Declarator(DirectDeclarator*);
	Declarator(Pointer*, DirectDeclarator*);

	std::string what(void) const;

private:
};

class ParameterTypeList;
class IdentifierList;
class DirectDeclarator: public NonTerminal{
public:
	DirectDeclarator(Identifier*);
	DirectDeclarator(Declarator*);
	DirectDeclarator(DirectDeclarator*, ConstantExpression*);
	DirectDeclarator(DirectDeclarator*, ParameterTypeList*);
	DirectDeclarator(DirectDeclarator*, IdentifierList*);

	std::string what(void) const;

private:
};

class TypeQualifierList;
class Pointer: public NonTerminal{
public:
	Pointer(TypeQualifierList*);
	Pointer(TypeQualifierList*, Pointer*);
	
	std::string what(void) const;
	
private:
};

class TypeQualifierList: public NonTerminal{
public:
	TypeQualifierList(TypeQualifier*);
	TypeQualifierList(TypeQualifierList*, TypeQualifier*);
	
	std::string what(void) const;
	
private:
};

class ParameterList;
class ParameterTypeList: public NonTerminal{
public:
	ParameterTypeList(ParameterList*, bool=false);
	
	std::string what(void) const;
	
private:
};

class ParameterDeclaration;
class ParameterList: public NonTerminal{
public:
	ParameterList(ParameterDeclaration*);
	ParameterList(ParameterList*, ParameterDeclaration*);
	
	std::string what(void) const;
	
private:
};

class DeclarationSpecifiers;
class Declarator;
class AbstractDeclarator;
class ParameterDeclaration: public NonTerminal{
public:
	ParameterDeclaration(DeclarationSpecifiers*, Declarator*);
	ParameterDeclaration(DeclarationSpecifiers*);
	ParameterDeclaration(DeclarationSpecifiers*, AbstractDeclarator*);
	
	std::string what(void) const;
	
private:
};

class IdentifierList: public NonTerminal{
public:
	IdentifierList(Identifier*);
	IdentifierList(IdentifierList*, Identifier*);
	
	std::string what(void) const;
	
private:
};

class TypeName: public NonTerminal{
public:
	TypeName(SpecifierQualifierList*);
	TypeName(SpecifierQualifierList*, AbstractDeclarator*);
	
	std::string what(void) const;
	
private:
};

class DirectAbstractDeclarator;
class AbstractDeclarator: public NonTerminal{
public:
	AbstractDeclarator(Pointer*);
	AbstractDeclarator(DirectAbstractDeclarator*);
	AbstractDeclarator(Pointer*, DirectAbstractDeclarator*);
	
	std::string what(void) const;
	
private:
};

class DirectAbstractDeclarator: public NonTerminal{
public:
	DirectAbstractDeclarator(AbstractDeclarator*);
	DirectAbstractDeclarator(DirectAbstractDeclarator*, ConstantExpression*);
	DirectAbstractDeclarator(ConstantExpression*);
	DirectAbstractDeclarator(DirectAbstractDeclarator*, ParameterTypeList*);
	DirectAbstractDeclarator(ParameterTypeList*);
	
	std::string what(void) const;
	
private:
};

class TypeDefName: public Terminal{
public:
	TypeDefName(Identifier*);
	
	std::string what(void) const;
	
private:
};

class InitialiserList;
class Initialiser: public NonTerminal{
public:
	Initialiser(AssignmentExpression*);
	Initialiser(InitialiserList*);
	
	std::string what(void) const;
	
private:
};

class InitialiserList: public NonTerminal{
public:
	InitialiserList(Initialiser*);
	InitialiserList(InitialiserList*, Initialiser*);
	
	std::string what(void) const;
	
private:
};

class LabeledStatement;
class CompoundStatement;
class ExpressionStatement;
class SelectionStatement;
class IterationStatement;
class JumpStatement;
class Statement: public NonTerminal{
public:
	Statement(LabeledStatement*);
	Statement(CompoundStatement*);
	Statement(ExpressionStatement*);
	Statement(SelectionStatement*);
	Statement(IterationStatement*);
	Statement(JumpStatement*);
	
	std::string what(void) const;
	
private:
};

class LabeledStatement: public NonTerminal{
public:
	LabeledStatement(Identifier*, Statement*);
	LabeledStatement(ConstantExpression*, Statement*);
	LabeledStatement(Statement*);
	
	std::string what(void) const;
	
private:
};

class DeclarationList;
class StatementList;
class CompoundStatement: public NonTerminal{
public:
	CompoundStatement(DeclarationList*, StatementList*);
	
	std::string what(void) const;
	
private:
};

class DeclarationList: public NonTerminal{
public:
	DeclarationList(Declaration*);
	DeclarationList(DeclarationList*, Declaration*);

	std::string what(void) const;
	
private:
};

class StatementList: public NonTerminal{
public:
	StatementList(Statement*);
	StatementList(StatementList*, Statement*);
	
	std::string what(void) const;
	
private:
};

class ExpressionStatement: public NonTerminal{
public:
	ExpressionStatement(Expression*);
	
	std::string what(void) const;
	
private:
};

class SelectionStatement: public NonTerminal{
public:
	SelectionStatement(Terminal*, Expression*, Statement*);
	SelectionStatement(Expression*, Statement*, Statement*);
	
	std::string what(void) const;
	
private:
};

class IterationStatement: public NonTerminal{
public:
	IterationStatement(Terminal*, Statement*);
	IterationStatement(Terminal*, Statement*, Expression*);
	IterationStatement(Terminal*, Expression*, Statement*);
	IterationStatement(Terminal*, Expression*,
					   Expression*, Statement*);
	IterationStatement(Terminal*, Expression*,
					   Expression*, Expression*, Statement*);
	
	std::string what(void) const;
	
private:
};

class JumpStatement: public NonTerminal{
public:
	JumpStatement(Terminal*);
	JumpStatement(Identifier*);
	JumpStatement(Expression*);
	
	std::string what(void) const;
	
private:
};

class ExternalDeclaration;
class TranslationUnit: public NonTerminal{
public:
	TranslationUnit(ExternalDeclaration*);
	TranslationUnit(TranslationUnit*, ExternalDeclaration*);
	
	std::string what(void) const;

private:
};

class FunctionDefinition;
class ExternalDeclaration: public NonTerminal{
public:
	ExternalDeclaration(FunctionDefinition*);
	ExternalDeclaration(Declaration*);
	
	std::string what(void) const;

private:
};

class FunctionDefinition: public NonTerminal{
public:
	FunctionDefinition(DeclarationSpecifiers*, Declarator*,
					   DeclarationList*, CompoundStatement*);
	
	std::string what(void) const;
	
private:
};

#endif /* defined(__CARM_Compiler__syntax_tree__) */
