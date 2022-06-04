%option c++ prefix="fir_scanner_" outfile="fir_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <sstream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "fir_parser.tab.h"

// output stream for building string literals
static std::ostringstream strlit;

// don't change this
#define yyerror LexerError
%}
%x X_STRING
%x X_TILDE
%%
                        {set_debug(false); yydebug = 0; }
"!!".*                 ; /* ignore comments */

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE;
"&&"                   return tAND;
"||"                   return tOR; 
"->"                   return tARROW;

  /* ====================================================================== */
  /* ====[                 5.5 - Instrução condicional                ]==== */
  /* ====================================================================== */

"if"                   return tIF;
"then"                 return tTHEN;
"else"                 return tELSE;

"while"                return tWHILE;
"do"                   return tDO;
"finally"              return tFINALLY;
"leave"                return tLEAVE;
"restart"              return tRESTART;

"write"                return tPRINT;
"writeln"              return tPRINTLN; 
"read"                 return tREAD; 


"int"                  return tTYPE_INT;
"string"               return tTYPE_STRING;

"with"                 return tWITH;
"apply"                return tAPPLY;
"in"                   return tIN;
".."                   return tSCALE;

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

\'                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>~            yy_push_state(X_TILDE);
<X_STRING>"\'"        {
                        yylval.s = new std::string(strlit.str());
                        strlit.str("");
                        yy_pop_state();
                        return tSTRING;
                      }
<X_STRING>.            strlit << *yytext;

<X_TILDE>n             strlit << '\n'; yy_pop_state();
<X_TILDE>r             strlit << '\r'; yy_pop_state();
<X_TILDE>t             strlit << '\t'; yy_pop_state();
<X_TILDE>\'            strlit << "'" ; yy_pop_state();          
<X_TILDE>\~            strlit << '~'; yy_pop_state();          

<X_TILDE>[[:xdigit:]]{1,2} {
                            strlit << (char)(unsigned char)strtoul(yytext, NULL, 16);
                            yy_pop_state();
                           }
<X_TILDE>.             strlit << '~' + *yytext; yy_pop_state();

0[0-7]{1,2}            yylval.i = strtol(yytext, nullptr, 8); return tINTEGER;

[0-9]+                 yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;

[-()<>=+*/%;{}.]       return *yytext;

[ \t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
