
typedef union  {
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
} YYSTYPE;
extern YYSTYPE yylval;
# define TK_IF 257
# define TK_REPLACE 258
# define TK_BY 259
# define TK_IDENTIFIER 260
# define TK_CONSTANT 261
# define TK_LE 262
# define TK_GE 263
# define TK_EQ 264
# define TK_NE 265
# define TK_COMMA 266
# define TK_DEF 267
# define TK_OBRA 268
# define TK_CBRA 269
# define TK_MINUS 270
# define TK_PLUS 271
# define TK_TIMES 272
# define TK_DIV 273
# define TK_MOD 274
# define TK_LT 275
# define TK_GT 276
# define TK_TRUE 277
# define TK_FALSE 278
# define TK_AND 279
# define TK_OR 280
# define TK_SEQ 281
# define TK_PAR 282
# define TK_EMPTY 283
# define TK_WHERE 284
# define TK_OBRACE 285
# define TK_CBRACE 286
