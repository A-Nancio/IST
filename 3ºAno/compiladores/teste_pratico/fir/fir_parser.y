%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include "ast/all.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;	/* integer value */
  std::string          *s;	/* symbol name or string literal */
  cdk::basic_node      *node;	/* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;

  fir::block_node       *block;
};


%token <i> tINTEGER
%token <s> tIDENTIFIER tSTRING
%token tIF tTYPE_INT tTYPE_STRING tARROW
%token tREAD tPRINT tPRINTLN
%token tPUBLIC tPRIVATE tREQUIRE
%token tWHILE tDO tLEAVE tRESTART
%token tAPPLY tWITH tSCALE tIN

%nonassoc tIFX
%nonassoc tTHEN
%nonassoc tELSE
%nonassoc tEMPTY
%nonassoc tFINALLY 

%right '='
%left tOR
%left tAND 
%left tEQ tNE
%left tGE tLE '>' '<' 
%left '+' '-'
%left '*' '/' '%'
%right tUNARY

%type <node> instruction program decl fundef vardec ptr
%type <sequence> instructions decls
%type <expression> expr opt_initializer
%type <type> data_type
%type <lvalue> lval
%type <block> block
%type<s> string

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

program	: decls    { compiler->ast($$ = $1); }
	     ;


decls : decl	     { $$ = new cdk::sequence_node(LINE, $1); }
      | decls decl  { $$ = new cdk::sequence_node(LINE, $2, $1); }
	 ;

decl  : vardec ';'  { $$ = $1; }
      | fundef      { $$ = $1; }
      ;

vardec : data_type '?'tIDENTIFIER                        { $$ = new fir::variable_declaration_node(LINE, tPUBLIC, $1, *$3, nullptr); }
       | data_type    tIDENTIFIER opt_initializer        { $$ = new fir::variable_declaration_node(LINE, tPRIVATE, $1, *$2, $3); }
       | data_type '*'tIDENTIFIER opt_initializer        { $$ = new fir::variable_declaration_node(LINE, tPUBLIC, $1, *$3, $4); }
       ;

block    : '{' decls instructions '}' { $$ = new fir::block_node(LINE, $2, $3); }
         | '{' instructions '}' { $$ = new fir::block_node(LINE, NULL, $2); }
         | '{' decls '}' { $$ = new fir::block_node(LINE, $2, NULL); }
         | '{''}' { $$ = new fir::block_node(LINE, NULL, NULL); }
         | /*empty*/ {$$ = nullptr;}
         ;

fundef : data_type '*' tIDENTIFIER '(' ')' tARROW tINTEGER block block block { $$ = new fir::function_definition_node(LINE, tPUBLIC, $1, *$3 ,nullptr, $8, $9, $10); }
       ;


instructions : instruction	     { $$ = new cdk::sequence_node(LINE, $1); }
	   | instructions instruction { $$ = new cdk::sequence_node(LINE, $2, $1); }
	   ;

instruction : expr ';'                                      { $$ = new fir::evaluation_node(LINE, $1); }
 	| tPRINT expr ';'                                      { $$ = new fir::print_node(LINE, $2, false); }
     | tPRINTLN expr ';'                                    { $$ = new fir::print_node(LINE, $2, true); }
     | tREAD lval ';'                                       { $$ = new fir::read_node(LINE, $2); }
     | tIF expr tTHEN instruction %prec tIFX                { $$ = new fir::if_node(LINE, $2, $4); }
     | tIF expr tTHEN instruction tELSE instruction         { $$ = new fir::if_else_node(LINE, $2, $4, $6); }
     | tWHILE expr tDO instruction tFINALLY instruction     { $$ = new fir::while_node(LINE, $2, $4, $6); }
     | tWHILE expr tDO instruction %prec tEMPTY             { $$ = new fir::while_node(LINE, $2, $4); }
     | tLEAVE ';'                                           { $$ = new fir::leave_node(LINE);  }
     | tLEAVE tINTEGER ';'                                  { $$ = new fir::leave_node(LINE, $2);  }
     | tRESTART ';'                                         { $$ = new fir::restart_node(LINE); }
     | tRESTART tINTEGER ';'                                { $$ = new fir::restart_node(LINE, $2); }
     | block                                                { $$ = $1; }
     | tWITH lval tAPPLY tIDENTIFIER tIN expr tSCALE expr ';'{ $$ = new fir::apply_node(LINE, $2, $6, $8, $4);}
     ;

ptr : '<' data_type '>'                 { $$ = cdk::reference_type::create(4, $2); }

data_type    : tTYPE_STRING                     { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
             | tTYPE_INT                        { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
             /* ADICIONAR TIPOS RESTANTES */

opt_initializer  : /* empty */            { $$ = nullptr; /* must be nullptr, not NIL */ }
                 | '=' expr               { $$ = $2; }
                 ;

expr : tINTEGER                { $$ = new cdk::integer_node(LINE, $1); }
	| string                  { $$ = new cdk::string_node(LINE, $1); }
     /* UNARY EXPRESSION */
     | '-' expr %prec tUNARY      { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY      { $$ = $2; }
     /* ARITHMETIC EXPRESSIONS */
     | expr '+' expr	         { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr	         { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr	         { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr	         { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr	         { $$ = new cdk::mod_node(LINE, $1, $3); }
     /* LOGICAL EXPRESSIONS */
     | expr '<' expr	         { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr	         { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr	         { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr              { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr	         { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr	         { $$ = new cdk::eq_node(LINE, $1, $3); }
     | expr tAND expr             { $$ = new cdk::and_node(LINE, $1, $3); }
     | expr tOR expr              { $$ = new cdk::or_node(LINE, $1, $3); }
     /* OTHER EXPRESSION */
     | '(' expr ')'               { $$ = $2; }
     | lval                       { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
     | lval '=' expr              { $$ = new cdk::assignment_node(LINE, $1, $3); }
     ;

string          : tSTRING                       { $$ = $1; }
                | string tSTRING                { $$ = $1; $$->append(*$2); delete $2; }
                ;

lval : tIDENTIFIER             { $$ = new cdk::variable_node(LINE, $1); }
     ;

%%
