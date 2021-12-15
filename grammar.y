%{
# include <stdio.h>
# include "include/const.h"
# include "include/types.h"
# include "include/rtbag.h"
# include "include/fnc.h"
# include "include/vars.h"
%}

%union {
	int         numval;
	char        *idval;
	EXPRESSION  *pe;
	ACTION      *pa;
	PATTERN     *pp;
	BODY        *pb;
	DEFINITION  *pd;
	DEFS        *pds;
	BAG         *pbag;
	TUPLE       *pt;
	PROGRAM     *pprog;
}

%token TK_IF TK_REPLACE TK_BY 
%token <idval> TK_IDENTIFIER 
%token <numval> TK_CONSTANT 
%token TK_LE TK_GE TK_EQ TK_NE TK_COMMA TK_DEF TK_OBRA
%token TK_CBRA TK_MINUS TK_PLUS TK_TIMES TK_DIV TK_MOD
%token TK_LT TK_GT TK_TRUE TK_FALSE TK_AND TK_OR TK_SEQ TK_PAR
%token <pa> TK_EMPTY
%token TK_WHERE TK_OBRACE TK_CBRACE TK_OSBRA TK_CSBRA

%left TK_PLUS TK_MINUS
%left TK_TIMES TK_DIV
%left TK_MOD
%left TK_SEQ
%left TK_PAR

%type <pe> primary_arithmetic_expression
%type <pe> arithmetic_expression
%type <pe> relational_expression
%type <pe> equality_expression
%type <pe> logical_and_expression
%type <pe> logical_or_expression
%type <pe> expression
%type <pa> action
%type <pp> list_of_patterns
%type <pe> pattern
%type <pb> body
%type <pe> reaction
%type <pd> definition
%type <pds> list_of_definitions
%type <pbag> multiset
%type <pbag> list_of_elements
%type <pbag> element
%type <pe> basic_element
%type <pt> tuple tuple_expression
%type <pprog> program

%start program
%%

program 
	:  body multiset TK_WHERE list_of_definitions
		{ $$ = Program = MakeProgramNode ($1, $2, $4); }
        ;

list_of_definitions
	: definition 
		{ $$ = MakeSingleDefNode ($1); }
	| definition list_of_definitions
		{ $$ = LinkDefNodes ($1, $2); }
	;

definition
	: TK_IDENTIFIER TK_DEF body
		{ $$ = MakeDefNode ($1, $3); }
	;

body
	: TK_REPLACE list_of_patterns TK_BY action TK_IF reaction
		{ $$ = MakeBasicBodyNode ($2, $4, $6); }
        | TK_OBRA body TK_CBRA
		{ $$ = $2; }
	| TK_IDENTIFIER
		{ $$ = MakeIdentBodyNode ($1); }
	| body TK_PAR body
		{ $$ = MakeCombBodyNode (T_PAR, $1, $3); }
	| body TK_SEQ body
		{ $$ = MakeCombBodyNode (T_SEQ, $1, $3); }
	;

list_of_patterns 
	: pattern
		{ $$ = MakePatNode ($1); }
	| pattern TK_COMMA list_of_patterns
		{ $$ = LinkPatNodes ($1, $3); }
	;

pattern 
	: TK_IDENTIFIER
		{ $$ = MakeIdentNode ($1); }
	| TK_CONSTANT
		{ $$ = MakeNumNode ($1); }
	| TK_TRUE
		{ $$ = MakeBoolNode (TRUE_VAL); }
	| TK_FALSE
		{ $$ = MakeBoolNode (FALSE_VAL); }
	| TK_OSBRA list_of_patterns TK_CSBRA
		{ $$ = MakeTupleNode ($2); }
	;

action
	: TK_EMPTY
		{ $$ = MakeEmptyAction (); }
	| expression
		{ $$ = MakeActNode ($1); }
	| expression TK_COMMA action
		{ $$ = LinkActNodes ($1, $3); }
	;

reaction
	: expression
	;

primary_arithmetic_expression
	: TK_IDENTIFIER
		{ $$ = MakeIdentNode ($1); }	
	| TK_CONSTANT
		{ $$ = MakeNumNode ($1); }
	| TK_TRUE
		{ $$ = MakeBoolNode (TRUE_VAL); }
	| TK_FALSE
		{ $$ = MakeBoolNode (FALSE_VAL); }
	| TK_OBRA expression TK_CBRA
		{ $$ = $2; }
	| TK_OSBRA tuple_expression TK_CSBRA
		{ $$ = MakeTupleNode ($2); }
	;

arithmetic_expression
	: primary_arithmetic_expression
	| arithmetic_expression TK_MINUS arithmetic_expression
		{ $$ = MakeBinNode (T_MINUS, $1, $3); }
	| arithmetic_expression TK_PLUS arithmetic_expression
		{ $$ = MakeBinNode (T_PLUS, $1, $3); }
	| arithmetic_expression TK_TIMES arithmetic_expression
		{ $$ = MakeBinNode (T_TIMES, $1, $3); }
	| arithmetic_expression TK_DIV arithmetic_expression
		{ $$ = MakeBinNode (T_DIV, $1, $3); }
	| arithmetic_expression TK_MOD arithmetic_expression
		{ $$ = MakeBinNode (T_MOD, $1, $3); }
	;

relational_expression
	: arithmetic_expression
	| relational_expression TK_LE arithmetic_expression
		{ $$ = MakeBinNode (T_LE, $1, $3); }
	| relational_expression TK_GE arithmetic_expression
		{ $$ = MakeBinNode (T_GE, $1, $3); }
	| relational_expression TK_LT arithmetic_expression
		{ $$ = MakeBinNode (T_LT, $1, $3); }
	| relational_expression TK_GT arithmetic_expression
		{ $$ = MakeBinNode (T_GT, $1, $3); }
	;

equality_expression
	: relational_expression
	| equality_expression TK_EQ relational_expression
		{ $$ = MakeBinNode (T_EQ, $1, $3); }
	| equality_expression TK_NE relational_expression
		{ $$ = MakeBinNode (T_NE, $1, $3); }
	;

logical_and_expression
	: equality_expression
	| logical_and_expression TK_AND equality_expression
		{ $$ = MakeBinNode (T_AND, $1, $3); }
	;

logical_or_expression
        : logical_and_expression
        | logical_or_expression TK_OR logical_and_expression
		{ $$ = MakeBinNode (T_OR, $1, $3); }
        ;

expression
	: logical_or_expression
	;

multiset
	: TK_OBRACE list_of_elements TK_CBRACE
		{ $$ = $2; }
	;

list_of_elements
	: element
	| element TK_COMMA list_of_elements
		{ $$ = LinkBagNodes ($1, $3); }
	;

element
	: basic_element
		{ $$ = MakeSingleBagNode ($1); }
	| TK_OSBRA tuple TK_CSBRA
		{ $$ = MakeTupleBagNode ($2); }
	;

basic_element
	: TK_CONSTANT
		{ $$ = MakeNumNode ($1); }
	| TK_FALSE
		{ $$ = MakeBoolNode (FALSE_VAL); }
	| TK_TRUE
		{ $$ = MakeBoolNode (TRUE_VAL); }
	;

tuple
	: basic_element
		{ $$ = MakeSingleTupleBagNode ($1); }
	| basic_element TK_COMMA tuple
		{ $$ = LinkTupleNodes ($1, $3); }
	;

tuple_expression
        : arithmetic_expression
                { $$ = MakeSingleTupleBagNode ($1); }
        | arithmetic_expression TK_COMMA tuple_expression
                { $$ = LinkTupleNodes ($1, $3); }
        ;

%%

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

yyerror(s)
char *s;
{
	extern int yylineno;

        fflush(stderr);
        fprintf(stderr, "%s near line %d\n", s, yylineno);
	exit (1);
}

