/*
 ****************************************************************
 *                                                              *
 *      Module: parser.c                                        *
 *      Description: The parser                                 *
 *      Version: 1.0 of 06/Jun/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "include/const.h"
# include "include/types.h"

extern char *Alloc ();

/*
 ****************************************************************
 * Alocate an Node for a numeral in the expression tree         *
 ****************************************************************
 */


EXPRESSION *
MakeEmptyExpression ()
{
        register EXPRESSION *pe;

        pe = (EXPRESSION *) Alloc (sizeof (EXPRESSION));
        pe->e_tag = T_EMPTY;
        pe->e_left = pe->e_right = (EXPRESSION *) NULL;
        return (pe);

}       /* end MakeEmptyExpression */

/*
 ****************************************************************
 * Alocate an Node for a numeral in the expression tree         *
 ****************************************************************
 */


EXPRESSION *
MakeNumNode (val)
NUMERAL val;
{
	register EXPRESSION *pe;

	pe = (EXPRESSION *) Alloc (sizeof (EXPRESSION));
	pe->e_tag = T_NUMERAL;
	pe->e_val.e_numval = val;
	pe->e_left = pe->e_right = (EXPRESSION *) NULL;
	return (pe);

}	/* end MakeIntNode */

/*
 ****************************************************************
 * Alocate an Node for a boolean in the expression tree         *
 ****************************************************************
 */


EXPRESSION *
MakeBoolNode (val)
BOOLEAN val;
{
        register EXPRESSION *pe;

        pe = (EXPRESSION *) Alloc (sizeof (EXPRESSION));
        pe->e_tag = T_BOOLEAN;
        pe->e_val.e_boolval = val;
        pe->e_left = pe->e_right = (EXPRESSION *) NULL;
        return (pe);

}       /* end MakeBoolNode */

/*
 ****************************************************************
 * Alocate an Node for an indentifier in the expression tree    *
 ****************************************************************
 */


EXPRESSION *
MakeIdentNode (val)
char *val;
{
        register EXPRESSION *pe;

        pe = (EXPRESSION *) Alloc (sizeof (EXPRESSION));
        pe->e_tag = T_IDENTIFIER;
        pe->e_val.e_name = Alloc (strlen (val) + 1);
	strcpy (pe->e_val.e_name, val);
        pe->e_left = pe->e_right = (EXPRESSION *) NULL;
        return (pe);

}       /* end MakeIdNode */

/*
 ****************************************************************
 * Alocate an Node for an binary operator in the expression tree*
 ****************************************************************
 */

EXPRESSION *
MakeBinNode (operator, left, right)
int operator;
EXPRESSION *left;
EXPRESSION *right;
{
	register EXPRESSION *pe;

	pe = (EXPRESSION *) Alloc (sizeof (EXPRESSION));
	pe->e_tag = operator;
	pe->e_left = left;
	pe->e_right = right;
	return (pe);

}	/* end MakeBinNode */
/*
 ****************************************************************
 * Alocate an Node for an empty action                          *
 ****************************************************************
 */

ACTION *
MakeEmptyAction ()
{
        register ACTION *pa;

        pa = (ACTION *) Alloc (sizeof (ACTION));
        pa->a_next = (ACTION *) NULL;
        pa->a_expression = MakeEmptyExpression ();;
        return (pa);

}       /* end MakeActNode */


/*
 ****************************************************************
 * Alocate an Node for a single action                          *
 ****************************************************************
 */

ACTION *
MakeActNode (pe)
EXPRESSION *pe;
{
	register ACTION *pa;

	pa = (ACTION *) Alloc (sizeof (ACTION));
	pa->a_next = (ACTION *) NULL;
	pa->a_expression = pe;
	return (pa);

}	/* end MakeActNode */

/*
 ****************************************************************
 * Link two actions                                             *
 ****************************************************************
 */

ACTION *
LinkActNodes (pe, pa)
EXPRESSION *pe;
ACTION *pa;
{
	register ACTION *paux;

	paux = (ACTION *) Alloc (sizeof (ACTION));
	paux->a_expression = pe;
	paux->a_next = pa;
	return (paux);

}	/* end LinkActNodes */
/*
 ****************************************************************
 * Alocate an Node for a single pattern                         *
 ****************************************************************
 */

PATTERN *
MakePatNode (pe)
EXPRESSION *pe;
{
        register  PATTERN *pp;

        pp = (PATTERN *) Alloc (sizeof (PATTERN));
        pp->p_next = (PATTERN *) NULL;
        pp->p_expression = pe;
        return (pp);

}       /* end MakePatNode */

/*
 ****************************************************************
 * Link two patterns                                            *
 ****************************************************************
 */

PATTERN *
LinkPatNodes (pe, pp)
EXPRESSION *pe;
PATTERN *pp;
{
        register PATTERN *paux;

        paux = (PATTERN *) Alloc (sizeof (PATTERN));
        paux->p_expression = pe;
        paux->p_next = pp;
        return (paux);

}       /* end LinkPatNodes */

/*
 ****************************************************************
 * Make a Node for a Tuple                                      *
 ****************************************************************
 */

EXPRESSION *
MakeTupleNode (pp)
PATTERN *pp;
{
        register EXPRESSION *pe;

        pe = (EXPRESSION *) Alloc (sizeof (EXPRESSION));
        pe->e_tag = T_TUPLE;
        pe->e_val.e_pattern = pp;
	pe->e_left = pe->e_right = (EXPRESSION *) NULL;
        return (pe);

}       /* end MakeTupleNode */

/*
 ****************************************************************
 * Make a Basic Body Node                                       *
 ****************************************************************
 */

BODY *
MakeBasicBodyNode (pp, pa, pr)
PATTERN *pp;
ACTION *pa;
EXPRESSION *pr;
{
	register BODY *pb;

	pb = (BODY *) Alloc (sizeof (BODY));
	pb->b_tag = T_BASIC_BODY;
	pb->b_val.b_bbody = (B_BODY *) Alloc (sizeof (B_BODY));
	pb->b_val.b_bbody->b_pattern = pp;
	pb->b_val.b_bbody->b_action = pa;
	pb->b_val.b_bbody->b_reaction = pr;
	return (pb);

}	/* end MakeBasicBodyNode */


/*
 ****************************************************************
 * Make a Indent Body Node                                      *
 ****************************************************************
 */

BODY *
MakeIdentBodyNode (val)
char *val;
{
        register BODY *pb;

        pb = (BODY *) Alloc (sizeof (BODY));
        pb->b_tag = T_IDENTIFIER;
	pb->b_val.b_name = Alloc (strlen (val) + 1);
        strcpy (pb->b_val.b_name, val);
	return (pb);

}	/* end MakeIdentBodyNode */

/*
 ****************************************************************
 * Make a Combinator Body Node                                  *
 ****************************************************************
 */

BODY *
MakeCombBodyNode (combinator, pb1, pb2)
int combinator;
BODY *pb1, *pb2;
{
	register BODY *pb;

	pb = (BODY *) Alloc (sizeof (BODY));
	pb->b_tag = T_COMBINATOR;
	pb->b_val.b_comb = (COMBINATOR *) Alloc (sizeof (COMBINATOR));
	pb->b_val.b_comb->c_combinator = combinator;
	pb->b_val.b_comb->c_left = pb1;
	pb->b_val.b_comb->c_right = pb2;
	return (pb);

}	/* end MakeCombBodyNode */

/*
 ****************************************************************
 * Make a Definition Node                                       *
 ****************************************************************
 */

DEFINITION *
MakeDefNode (val, pb)
char *val;
BODY *pb;
{
	extern int yylineno;
	register DEFINITION *pd;

	pd = (DEFINITION *) Alloc (sizeof (DEFINITION));
	pd->d_name = Alloc (strlen (val) + 1);
	strcpy (pd->d_name, val);
	pd->d_body = pb;
	pd->d_lineno = yylineno;
	return (pd);

}	/* end MakeDefNode */



/*
 ****************************************************************
 * Alocate a Node for a single definition                       *
 ****************************************************************
 */

DEFS *
MakeSingleDefNode (pd)
DEFINITION *pd;
{
        register DEFS *paux;

        paux = (DEFS *) Alloc (sizeof (DEFS));
        paux->d_next = (DEFS *) NULL;
        paux->d_definition = pd;
        return (paux);

}       /* end MakeSingleDefNode */

/*
 ****************************************************************
 * Link two definitions                                         *
 ****************************************************************
 */

DEFS *
LinkDefNodes (pd, pds)
DEFINITION *pd;
DEFS *pds;
{
        register DEFS *paux;

        paux = (DEFS *) Alloc (sizeof (DEFS));
        paux->d_definition = pd;
        paux->d_next = pds;
        return (paux);

}       /* end LinkDefNodes */

/*
 ****************************************************************
 * Alocate a Node for an element of the multiset                *
 ****************************************************************
 */

BAG *
MakeSingleBagNode (pe)
EXPRESSION *pe;
{
        register BAG *paux; 

        paux = (BAG *) Alloc (sizeof (BAG));
	paux->b_tag = T_EXPRESSION;
        paux->b_next = (BAG *) NULL;
        paux->b_val.b_expression = pe;
        return (paux);

}       /* end MakeSingleBagNode */

/*
 ****************************************************************
 * Link two an element to a bag                                 *
 ****************************************************************
 */

BAG *
MakeTupleBagNode (pt)
TUPLE *pt;
{
	register BAG *paux;

	paux = (BAG *) Alloc (sizeof (BAG));
	paux->b_tag = T_TUPLE;
	paux->b_val.b_tuple = pt;
	paux->b_next = (BAG *) NULL;
	return (paux);

}	/* end MakeTupleBagNode */


/*
 ****************************************************************
 * Link two an element to a bag                                 *
 ****************************************************************
 */

BAG *
LinkBagNodes (pe, pb)
BAG *pe;
BAG *pb;
{
        pe->b_next = pb;
        return (pe);

}       /* end LinkBagNodes */

/*
 ****************************************************************
 * Alocate a Node for a tuple element of a multiset             *
 ****************************************************************
 */

TUPLE *
MakeSingleTupleBagNode (pe)
EXPRESSION *pe;
{
        register TUPLE *paux;

        paux = (TUPLE *) Alloc (sizeof (TUPLE));
        paux->t_next = (TUPLE *) NULL;
        paux->t_expression = pe;
        return (paux);

}       /* end MakeSingleTupleBagNode */

/*
 ****************************************************************
 * Link two elements of a tuple                                 *
 ****************************************************************
 */

TUPLE *
LinkTupleNodes (pe, pt)
EXPRESSION *pe;
TUPLE *pt;
{
	register TUPLE *paux;

	paux = (TUPLE *) Alloc (sizeof (TUPLE));
	paux->t_expression = pe;
        paux->t_next = pt;
        return (paux);

}       /* end LinkBagNodes */



/*
 ****************************************************************
 * Make a Program Node                                          *
 ****************************************************************
 */

PROGRAM *
MakeProgramNode (pbody, pbag, pdefs)
BODY *pbody;
BAG *pbag;
DEFS *pdefs;
{
	register PROGRAM *paux;

	paux = (PROGRAM *) Alloc (sizeof (PROGRAM));
	paux->p_body = pbody;
	paux->p_bag = pbag;
	paux->p_defs = pdefs;
	return (paux);

}	/* end MakeProgramNode */


