/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_IF = 258,
    TK_REPLACE = 259,
    TK_BY = 260,
    TK_IDENTIFIER = 261,
    TK_CONSTANT = 262,
    TK_LE = 263,
    TK_GE = 264,
    TK_EQ = 265,
    TK_NE = 266,
    TK_COMMA = 267,
    TK_DEF = 268,
    TK_OBRA = 269,
    TK_CBRA = 270,
    TK_MINUS = 271,
    TK_PLUS = 272,
    TK_TIMES = 273,
    TK_DIV = 274,
    TK_MOD = 275,
    TK_LT = 276,
    TK_GT = 277,
    TK_TRUE = 278,
    TK_FALSE = 279,
    TK_AND = 280,
    TK_OR = 281,
    TK_SEQ = 282,
    TK_PAR = 283,
    TK_EMPTY = 284,
    TK_WHERE = 285,
    TK_OBRACE = 286,
    TK_CBRACE = 287,
    TK_OSBRA = 288,
    TK_CSBRA = 289
  };
#endif
/* Tokens.  */
#define TK_IF 258
#define TK_REPLACE 259
#define TK_BY 260
#define TK_IDENTIFIER 261
#define TK_CONSTANT 262
#define TK_LE 263
#define TK_GE 264
#define TK_EQ 265
#define TK_NE 266
#define TK_COMMA 267
#define TK_DEF 268
#define TK_OBRA 269
#define TK_CBRA 270
#define TK_MINUS 271
#define TK_PLUS 272
#define TK_TIMES 273
#define TK_DIV 274
#define TK_MOD 275
#define TK_LT 276
#define TK_GT 277
#define TK_TRUE 278
#define TK_FALSE 279
#define TK_AND 280
#define TK_OR 281
#define TK_SEQ 282
#define TK_PAR 283
#define TK_EMPTY 284
#define TK_WHERE 285
#define TK_OBRACE 286
#define TK_CBRACE 287
#define TK_OSBRA 288
#define TK_CSBRA 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 10 "grammar.y" /* yacc.c:1909  */

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

#line 136 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
