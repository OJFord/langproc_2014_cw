// Generated by Flexc++ V0.98.00 on Fri, 27 Feb 2015 11:37:20 +0000

#ifndef C89Lexer_H_INCLUDED_
#define C89Lexer_H_INCLUDED_

// $insert baseclass_h
#include "C89Lexerbase.h"


// $insert classHead
class C89Lexer: public C89LexerBase
{
    public:
        explicit C89Lexer(std::istream &in = std::cin,
                                std::ostream &out = std::cout);

        C89Lexer(std::string const &infile, std::string const &outfile);
        
        // $insert lexFunctionDecl
        int lex();

    private:
        int lex__();
        int executeAction__(size_t ruleNr);

        void print();
        void preCode();     // re-implement this function for code that must 
                            // be exec'ed before the patternmatching starts
};

// $insert scannerConstructors
inline C89Lexer::C89Lexer(std::istream &in, std::ostream &out)
:
    C89LexerBase(in, out)
{}

inline C89Lexer::C89Lexer(std::string const &infile, std::string const &outfile)
:
    C89LexerBase(infile, outfile)
{}

// $insert inlineLexFunction
inline int C89Lexer::lex()
{
    return lex__();
}

inline void C89Lexer::preCode() 
{
    // optionally replace by your own code
}

inline void C89Lexer::print() 
{
    print__();
}


#endif // C89Lexer_H_INCLUDED_

