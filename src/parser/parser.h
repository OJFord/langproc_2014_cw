//
//  parser.h
//  CARM-Compiler
//
//  Created by Ollie Ford on 15/03/2015.
//  Copyright (c) 2015 OJFord. All rights reserved.
//

#include "parser/tokens.h"
#include "parser/syntax-tree.h"
#include "lexer/lexer.h"

class Parser{
public:
	Parser(bool);
	Parser(bool, const char* filename);
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
	//void function_definition(SyntaxTree*, lexeme);
	Declaration* declaration(void);
	DeclarationSpecifiers* declaration_specifiers(void);
	//void storage_class_specifier(SyntaxTree*, lexeme);
	TypeSpecifier* type_specifier(void);
	//void type_qualifier(SyntaxTree*, lexeme);
	
	Lexer* lexer;
	SymbolTable& symtbl;
};

