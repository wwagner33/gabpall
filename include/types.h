
/*
 ****************************************************************
 *                                                              *
 *      Module: types.h                                         *
 *      Description: The types used in the compiler             *
 *      Version: 1.0 of 06/Jun/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

/* values of the various tag fields below   */


# define	T_NUMERAL	0
# define	T_BOOLEAN	1
# define	T_IDENTIFIER	2
# define        T_EQ            3
# define        T_NE            4
# define        T_LT            5
# define        T_GT            6
# define        T_LE            7
# define        T_GE            8
# define        T_MINUS         9
# define        T_PLUS         10 
# define        T_TIMES        11
# define        T_DIV          12
# define        T_MOD          13
# define	T_AND	       14
# define	T_OR           15
# define        T_TUPLE        16
# define        T_PAR          17
# define        T_SEQ          18
# define        T_COMBINATOR   19
# define     	T_BASIC_BODY   20
# define	T_EXPRESSION   21
# define        T_EMPTY        22

# define        FALSE_VAL       0
# define        TRUE_VAL	1

typedef int NUMERAL;             /* the type of numerals */
typedef short BOOLEAN;	           /* the type of booleans */


/* A list of action/reaction defs     */

struct defs {
	struct definition      *d_definition;
	struct defs            *d_next;
};

typedef struct defs DEFS;


/* A definition of an action/reaction  */

struct definition {
	char            *d_name;
	struct body     *d_body;
	int             d_lineno; /* line where it is defined */
};

typedef struct definition DEFINITION;

/* A definition of a basic action body */

struct body {
	int b_tag;
	union {
	    struct basic_body *b_bbody;
	    char *b_name;
	    struct combinator *b_comb;
	} b_val;
};

typedef struct body BODY;

/* A body in which identifiers have been expanded. */

struct expanded_body {
  int eb_tag;
  int eb_number; 
  union {
    struct basic_body *eb_bbody;
    struct expanded_combinator *eb_comb;
  } eb_val;
};

typedef struct expanded_body EXPANDED_BODY; 

/* A body consisting of a basic action */

struct basic_body {
	struct pattern  *b_pattern;
	struct action   *b_action;
	struct expression *b_reaction;
};

typedef struct basic_body B_BODY;

/* a body consisting of an expression involving combinators */

struct combinator {
	int c_combinator;
	struct body *c_left;
	struct body *c_right;
};

typedef struct combinator COMBINATOR;

/* an expanded body consisting of an expression involving combinators */

struct expanded_combinator {
	int ec_combinator;
	struct expanded_body *ec_left;
	struct expanded_body *ec_right;
};

typedef struct expanded_combinator EXPANDED_COMBINATOR;

/* definition of list of patterns */

struct pattern {
	struct expression *p_expression;
	struct pattern *p_next;
};

typedef struct pattern PATTERN;


/* A definition of a action  */

struct action {
	struct expression *a_expression;
	struct action     *a_next;
};

typedef struct action ACTION;



/* The definition of expressions */

struct expression {
	int e_tag;
	union {
	    char *e_name;
	    NUMERAL e_numval;
	    int   e_boolval;
	    struct pattern *e_pattern;     /* for tuples */
	} e_val;
	struct expression *e_left;
	struct expression *e_right;
};

typedef struct expression EXPRESSION;

/* The bag structure */


struct bag {
	int b_tag;
	union {
	    struct tuple *b_tuple;
	    struct expression *b_expression;
	} b_val;
	struct bag *b_next;
};

typedef struct bag BAG;


/* the structure for tuples */

struct tuple {
	struct expression *t_expression;
	struct tuple *t_next;
};

typedef struct tuple TUPLE;

/* the structure for a program */

struct program {
	struct body *p_body;
	struct bag *p_bag;
	struct defs *p_defs;
};

typedef struct program PROGRAM;

/* A list of names. */

struct namelist {
  char *n_name;
  struct namelist *n_next;
};

typedef struct namelist NAMELIST;



