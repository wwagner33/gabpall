D			[0-9]
L			[a-zA-Z_]

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/const.h"
#include "include/types.h"
#include "include/y.tab.h"

//#include "include/fnc.h"
%}

%%
"/*"			{ comment(); }

"if"			{ return(TK_IF); }
"replace"		{ return(TK_REPLACE); }
"by"			{ return(TK_BY); }
"true"                  { return(TK_TRUE); }
"false"                 { return(TK_FALSE); }
"and"                   { return(TK_AND); }
"or"                    { return(TK_OR); }
"empty"                 { return(TK_EMPTY); }
"where"                 { return(TK_WHERE); }

{L}({L}|{D})*		{ LexIdentifier (); return(TK_IDENTIFIER); }

{D}+    		{  GetConstant (); return(TK_CONSTANT); }
'(\\.|[^\\'])+'		{  GetCharConstant (); return(TK_CONSTANT); }


"<="			{  return(TK_LE); }
">="			{  return(TK_GE); }
"=="			{  return(TK_EQ); }
"!="			{  return(TK_NE); }
","			{  return(TK_COMMA); }
"="			{  return(TK_DEF); }
"("			{  return(TK_OBRA); }
")"			{  return(TK_CBRA); }
"{"			{  return(TK_OBRACE); }
"}"			{  return(TK_CBRACE); }
"-"			{  return(TK_MINUS); }
"+"			{  return(TK_PLUS); }
"*"			{  return(TK_TIMES); }
"/"			{  return(TK_DIV); }
"%"			{  return(TK_MOD); }
"<"			{  return(TK_LT); }
">"			{  return(TK_GT); }
";"                     {  return(TK_SEQ); }
"|"                     {  return(TK_PAR); }
"["                    {  return(TK_OSBRA); }
"]"                    {  return(TK_CSBRA); }
[ \t\v\n\f]		{  }
.			{ bad_character (); }

%%

extern YYSTYPE yylval;

yywrap()
{
	return(1);
}


comment()
{
	char c, c1;

loop:
	while ((c = input()) != '*' && c != 0)
		;

	if ((c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}

	if (c != 0)
	    ;	
}

bad_character ()
{
	fprintf (stderr, "bad character `%c' at line %d.\n", yytext[0], 
              							yylineno);
	exit (1);

}	/* end bad_character */

LexIdentifier ()
{
	extern char *Alloc ();

	yylval.idval = Alloc (strlen (yytext)+1);
	strcpy (yylval.idval, yytext);
}

GetConstant ()
{
	yylval.numval = atoi (yytext);

}

GetCharConstant ( )
{
	yylval.numval = yytext[0];

}
