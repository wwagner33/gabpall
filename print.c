
/*
 ****************************************************************
 *                                                              *
 *      Module: print.c                                         *
 *      Description: Printing routines                          *
 *      Version: 1.0 of 06/Jun/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
# include "include/const.h"
# include "include/types.h"
# include "include/rtbag.h"
# include "include/fnc.h"

char *OperatorTable[] = 
           { "What?", "What?", "What?", " == ", " != ", " < ", " > ",
             " <= ", " >= ", " - ", " + ", " * ", " / ", " % ", " and ", " or ",
	     "What?", "What?", "What?", "What?", "What?", "What?", "empty"};
/*
 ****************************************************************
 * Print a Numeral Node                                         *
 ****************************************************************
 */

void
PrintNumNode (pe)
EXPRESSION *pe;
{
	printf (" %d ",  pe->e_val.e_numval );
	return;

}	/* end PrintNumNode */

/*
 ****************************************************************
 * Print a Boolean Node                                         *
 ****************************************************************
 */

void
PrintBoolNode (pe)
EXPRESSION *pe;
{
	if (pe->e_val.e_boolval == TRUE_VAL)
	    printf ("true ");
	else
	    printf ("false ");
	return;

}	/* end PrintBoolNode */

/*
 ****************************************************************
 * Print an Identifier Node                                     *
 ****************************************************************
 */

void
PrintIdentNode (pe)
EXPRESSION *pe;
{
	printf ("%s ", pe->e_val.e_name);
	return;

}	/* end PrintIdentNode */

/*
 ****************************************************************
 * Print an Expression                                          *
 ****************************************************************
 */

void
PrintExpression (pe)
EXPRESSION *pe;
{
	switch (pe->e_tag) {
               case T_NUMERAL:
        	    PrintNumNode (pe);
        	    break;
        	case T_BOOLEAN:
        	    PrintBoolNode (pe);
        	    break;
        	case T_IDENTIFIER:
        	    PrintIdentNode (pe);
        	    break;
		case T_TUPLE:
		    putchar ('(');
		    PrintPattern (pe->e_val.e_pattern);
		    putchar (')');
		    break;
		case T_EMPTY:
		    printf (" empty ");
		    break;
        	default:
        	    putchar ('(');
        	    PrintExpression (pe->e_left);
        	    printf ("%s", OperatorTable[pe->e_tag]);
        	    PrintExpression (pe->e_right);
                    putchar (')');
        	    break;
        }
        return; 

}	/* end PrintExpression */
/*
 ****************************************************************
 * Print a List of Definitions                                  *
 ****************************************************************
 */

void
PrintListDefinitions (pds)
DEFS *pds;
{
	register DEFS *pt;

	for (pt = pds; pt != (DEFS *) NULL; pt = pt->d_next)
	{
	    printf ("\n\n\t%s = ", pt->d_definition->d_name);
	    PrintBody (pt->d_definition->d_body);
	}
	return;

}	/* end PrintListDefinitions */


/*
 ****************************************************************
 * Print a Body of a Definition                                 *
 ****************************************************************
 */

void
PrintBody (pb)
BODY *pb;
{
	if (pb->b_tag == T_IDENTIFIER)
	    printf ("%s ", pb->b_val.b_name);
	else if (pb->b_tag == T_BASIC_BODY)
	    PrintBasicBody (pb->b_val.b_bbody);
	else if (pb->b_tag == T_COMBINATOR)
	    PrintCombinatorBody (pb->b_val.b_comb);
	else
	    fprintf (stderr, "error in PrintBody!\n"), exit (1);

}	/* end PrintBody */

/*
 ****************************************************************
 * Print a Basic Body of a Definition                           *
 ****************************************************************
 */

void
PrintBasicBody (pb)
B_BODY *pb;
{
	printf ("\n \t \t replace ");
	PrintPattern (pb->b_pattern);
	printf ("\n \t \t by ");
	PrintAction (pb->b_action);
	printf ("\n \t \t if ");
	PrintExpression (pb->b_reaction);
	return;

}	/* end PrintBody */

/*
 ****************************************************************
 * Print a Combinator Expression                                *
 ****************************************************************
 */

void
PrintCombinatorBody (pc)
COMBINATOR *pc;
{
	printf ("( ");
	PrintBody (pc->c_left);
	if (pc->c_combinator == T_PAR)
	    putchar ('|');
	else if (pc->c_combinator == T_SEQ)
	    putchar (';');
	else
	    printf ("Error in PrintCombinatorBody!\n"), exit (1);
	PrintBody (pc->c_right);
        printf (") ");
	return;

}	/* end PrintCombinatorBody */

/*
 ****************************************************************
 * Print a Pattern                                              *
 ****************************************************************
 */

void 
PrintPattern (pp)
PATTERN *pp;
{
	register PATTERN *p;

	for (p = pp; p != (PATTERN *) NULL; p = p->p_next)
	{
	    PrintExpression (p->p_expression);
	    if (p->p_next != (PATTERN *) NULL)
		printf (", ");
	}
	return;

}	/* end PrintPattern */

/*
 ****************************************************************
 * Print an Action                                              *
 ****************************************************************
 */

void
PrintAction (pa)
ACTION *pa;
{
	register ACTION *p;

	for (p = pa; p != (ACTION *) NULL; p = p->a_next)
	{
            PrintExpression (p->a_expression);
            if (p->a_next != (ACTION *) NULL)
                printf (", ");
        }
        return;

}       /* end PrintAction */

/*
 ****************************************************************
 * Print a Bag                                                  *
 ****************************************************************
 */
 
void
PrintBag (pb)
BAG *pb;
{
	register BAG *p;

	printf ("{ ");
	for (p = pb; p != (BAG *) NULL; p = p->b_next)
	{
	    if (p->b_tag == T_EXPRESSION)
		PrintExpression (p->b_val.b_expression);
	    else if (p->b_tag == T_TUPLE)
		PrintTuple (p->b_val.b_tuple);
	    else
		printf ("Error in PrintBag!\n"), exit (1);
	    if (p->b_next != (BAG *) NULL)
                printf (", ");
        }
	printf ("} ");
        return;

}       /* end PrintBag */


/*
 ****************************************************************
 * Print a Tuple in a Bag                                       *
 ****************************************************************
 */

void
PrintTuple (pt)
TUPLE *pt;
{
	register TUPLE *p;

	printf ("( ");
	for (p = pt; p != (TUPLE *) NULL; p = p->t_next)
	{
	    PrintExpression (p->t_expression);
	    if (p->t_next != (TUPLE *) NULL) 
                printf (", ");
        }
	printf (") ");
        return;

}	/* end PrintTuple */


/*
 ****************************************************************
 * Print a Program                                              *
 ****************************************************************
 */

void 
PrintProgram (pp)
PROGRAM *pp;
{
	PrintBody (pp->p_body);
	PrintBag (pp->p_bag);
	printf (" where ");
	PrintListDefinitions (pp->p_defs);
	putchar ('\n');
	return;

}	/* end PrintProgram */


