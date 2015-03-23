//
//  reductions.cpp
//  CARM-Compiler
//
//  Created by Ollie Ford on 23/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include <sstream>
#include <string>
#include "parser/reductions.h"

std::string SyntaxTree::reduce(void){
	return "";
}

std::string Terminal::reduce(void){
	return "";
}

std::string NonTerminal::reduce(void){
	return "";
}

std::string Identifier::reduce(void){
	return "";
}

std::string FloatingConstant::reduce(void){
	return "";
}

std::string IntegerConstant::reduce(void){
	return "";
}

std::string EnumerationConstant::reduce(void){
	return "";
}

std::string CharacterConstant::reduce(void){
	return "";
}

std::string WideCharacterConstant::reduce(void){
	return "";
}

std::string StringLiteral::reduce(void){
	return "";
}

std::string WideStringLiteral::reduce(void){
	return "";
}

std::string Constant::reduce(void){
	return "";
}

std::string PrimaryExpression::reduce(void){
	return "";
}

std::string PostfixExpression::reduce(void){
	return "";
}

std::string ArgumentExpressionList::reduce(void){
	return "";
}

std::string UnaryExpression::reduce(void){
	return "";
}

std::string CastExpression::reduce(void){
	return "";
}

std::string MultiplicativeExpression::reduce(void){
	return "";
}

std::string AdditiveExpression::reduce(void){
	return "";
}

std::string ShiftExpression::reduce(void){
	return "";
}

std::string RelationalExpression::reduce(void){
	return "";
}

std::string EqualityExpression::reduce(void){
	return "";
}

std::string ANDExpression::reduce(void){
	return "";
}

std::string ExclusiveORExpression::reduce(void){
	return "";
}

std::string InclusiveORExpression::reduce(void){
	return "";
}

std::string LogicalANDExpression::reduce(void){
	return "";
}

std::string LogicalORExpression::reduce(void){
	return "";
}

std::string ConditionalExpression::reduce(void){
	return "";
}

std::string AssignmentExpression::reduce(void){
	return "";
}
std::string Expression::reduce(void){
	return "";
}

std::string ConstantExpression::reduce(void){
	return "";
}

std::string Declaration::reduce(void){
	return "";
}

std::string DeclarationSpecifiers::reduce(void){
	return "";
}

std::string InitialiserDeclaratorList::reduce(void){
	return "";
}

std::string InitialiserDeclarator::reduce(void){
	return "";
}

std::string StorageClassSpecifier::reduce(void){
	return "";
}

std::string TypeSpecifier::reduce(void){
	return "";
}

std::string StructOrUnionSpecifier::reduce(void){
	return "";
}

std::string StructOrUnion::reduce(void){
	return "";
}

std::string StructDeclarationList::reduce(void){
	return "";
}


std::string StructDeclaration::reduce(void){
	return "";
}

std::string SpecifierQualifierList::reduce(void){
	return "";
}

std::string StructDeclaratorList::reduce(void){
	return "";
}

std::string StructDeclarator::reduce(void){
	return "";
}

std::string EnumerationSpecifier::reduce(void){
	return "";
}

std::string EnumeratorList::reduce(void){
	return "";
}

std::string Enumerator::reduce(void){
	return "";
}

std::string TypeQualifier::reduce(void){
	return "";
}

std::string Declarator::reduce(void){
	return "";
}

std::string DirectDeclarator::reduce(void){
	return "";
}

std::string Pointer::reduce(void){
	return "";
}

std::string TypeQualifierList::reduce(void){
	return "";
}

std::string ParameterTypeList::reduce(void){
	return "";
}

std::string ParameterList::reduce(void){
	return "";
}

std::string ParameterDeclaration::reduce(void){
	return "";
}

std::string IdentifierList::reduce(void){
	return "";
}

std::string TypeName::reduce(void){
	return "";
}

std::string AbstractDeclarator::reduce(void){
	return "";
}

std::string DirectAbstractDeclarator::reduce(void){
	return "";
}

std::string TypeDefName::reduce(void){
	return "";
}

std::string Initialiser::reduce(void){
	return "";
}

std::string InitialiserList::reduce(void){
	return "";
}

std::string Statement::reduce(void){
	return "";
}

std::string LabeledStatement::reduce(void){
	return "";
}

std::string CompoundStatement::reduce(void){
	return "";
}

std::string DeclarationList::reduce(void){
	return "";
}

std::string StatementList::reduce(void){
	return "";
}

std::string ExpressionStatement::reduce(void){
	return "";
}

std::string SelectionStatement::reduce(void){
	return "";
}

std::string IterationStatement::reduce(void){
	return "";
}

std::string JumpStatement::reduce(void){
	return "";
}

std::string ExternalDeclaration::reduce(void){
	std::stringstream ret;
	ret << ".global " << subtree.at(0) << std::endl;
	ret << subtree.at(0) << std::endl;
	return ret.str();
}

std::string TranslationUnit::reduce(void){
	std::stringstream ret;
	for(auto i:subtree){
		i->reduce();
		ret << std::endl << std::endl;
	}
	return ret.str();
}

std::string FunctionDefinition::reduce(void){
	return "";
}
