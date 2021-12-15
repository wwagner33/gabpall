/*
 ****************************************************************
 *                                                              *
 *      Module: printcode.c                                     *
 *      Description: CodePrinting routines for code generation  *
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
# include "include/vars.h"

char *OperatorCodeTable[] =
           { "What?", "What?", "What?", " == ", " != ", " < ", " > ",
             " <= ", " >= ", " - ", " + ", " * ", " / ", " % ", " && ", " || ",
             "What?", "What?", "What?", "What?", "What?", "What?", "empty"};

/*
 ****************************************************************
 * Print a Numeral Node                                         *
 ****************************************************************
 */

void
CodePrintNumNode (pe)
EXPRESSION *pe;
{
	fprintf (Output, " %d ",  pe->e_val.e_numval );
	return;

}	/* end CodePrintNumNode */

/*
 ****************************************************************
 * Print a Boolean Node                                         *
 ****************************************************************
 */

void
CodePrintBoolNode (pe)
EXPRESSION *pe;
{
	if (pe->e_val.e_boolval == TRUE_VAL)
	    fprintf (Output, "1");
	else
	    fprintf (Output, "0");
	return;

}	/* end CodePrintBoolNode */

/*
 ****************************************************************
 * Print an Identifier Node                                     *
 ****************************************************************
 */

void
CodePrintIdentNode (pe)
EXPRESSION *pe;
{
	fprintf (Output, "%s ", pe->e_val.e_name);
	return;

}	/* end CodePrintIdentNode */

/*
 ****************************************************************
 * Print an Expression                                          *
 ****************************************************************
 */

void
CodePrintExpression (pe)
EXPRESSION *pe;
{
	switch (pe->e_tag) {
               case T_NUMERAL:
        	    CodePrintNumNode (pe);
        	    break;
        	case T_BOOLEAN:
        	    CodePrintBoolNode (pe);
        	    break;
        	case T_IDENTIFIER:
        	    CodePrintIdentNode (pe);
        	    break;
		case T_TUPLE:
		    putchar ('(');
		    CodePrintPattern (pe->e_val.e_pattern);
		    putchar (')');
		    break;
		case T_EMPTY:
		    fprintf (Output, " 0 "); /* any value would do because in the treatment of 
						an empty action we ignore it's value */
		    break;
        	default:
        	    putchar ('(');
        	    CodePrintExpression (pe->e_left);
        	    fprintf (Output, "%s", OperatorCodeTable[pe->e_tag]);
        	    CodePrintExpression (pe->e_right);
                    putchar (')');
        	    break;
        }
        return; 

}	/* end CodePrintExpression */
/*
 ****************************************************************
 * Print a List of Definitions                                  *
 ****************************************************************
 */

void
CodePrintListDefinitions (pds)
DEFS *pds;
{
	register DEFS *pt;

	for (pt = pds; pt != (DEFS *) NULL; pt = pt->d_next)
	{
	    fprintf (Output, "\n\n\t%s = ", pt->d_definition->d_name);
	    CodePrintBody (pt->d_definition->d_body);
	}
	return;

}	/* end CodePrintListDefinitions */


/*
 ****************************************************************
 * Print a Body of a Definition                                 *
 ****************************************************************
 */

void
CodePrintBody (pb)
BODY *pb;
{
	if (pb->b_tag == T_IDENTIFIER)
	    fprintf (Output, "%s ", pb->b_val.b_name);
	else if (pb->b_tag == T_BASIC_BODY)
	    CodePrintBasicBody (pb->b_val.b_bbody);
	else if (pb->b_tag == T_COMBINATOR)
	    CodePrintCombinatorBody (pb->b_val.b_comb);
	else
	    fprintf (stderr, "error in CodePrintBody!\n"), exit (1);

}	/* end CodePrintBody */

/*
 ****************************************************************
 * Print a Basic Body of a Definition                           *
 ****************************************************************
 */

void
CodePrintBasicBody (pb)
B_BODY *pb;
{
	fprintf (Output, "\n \t \t replace ");
	CodePrintPattern (pb->b_pattern);
	fprintf (Output, "\n \t \t by ");
	CodePrintAction (pb->b_action);
	fprintf (Output, "\n \t \t if ");
	CodePrintExpression (pb->b_reaction);
	return;

}	/* end CodePrintBody */

/*
 ****************************************************************
 * Print a Combinator Expression                                *
 ****************************************************************
 */

void
CodePrintCombinatorBody (pc)
COMBINATOR *pc;
{
	fprintf (Output, "( ");
	CodePrintBody (pc->c_left);
	if (pc->c_combinator == T_PAR)
	    putchar ('|');
	else if (pc->c_combinator == T_SEQ)
	    putchar (';');
	else
	    fprintf (Output, "Error in CodePrintCombinatorBody!\n"), exit (1);
	CodePrintBody (pc->c_right);
        fprintf (Output, ") ");
	return;

}	/* end CodePrintCombinatorBody */

/*
 ****************************************************************
 * Print a Pattern                                              *
 ****************************************************************
 */

void 
CodePrintPattern (pp)
PATTERN *pp;
{
	register PATTERN *p;

	for (p = pp; p != (PATTERN *) NULL; p = p->p_next)
	{
	    CodePrintExpression (p->p_expression);
	    if (p->p_next != (PATTERN *) NULL)
		fprintf (Output, ", ");
	}
	return;

}	/* end CodePrintPattern */

/*
 ****************************************************************
 * Print an Action                                              *
 ****************************************************************
 */

void
CodePrintAction (pa)
ACTION *pa;
{
	register ACTION *p;

	for (p = pa; p != (ACTION *) NULL; p = p->a_next)
	{
            CodePrintExpression (p->a_expression);
            if (p->a_next != (ACTION *) NULL)
                fprintf (Output, ", ");
        }
        return;

}       /* end CodePrintAction */

/*
 ****************************************************************
 * Print a Bag                                                  *
 ****************************************************************
 */
 
void
CodePrintBag (pb)
BAG *pb;
{
	register BAG *p;

	fprintf (Output, "{ ");
	for (p = pb; p != (BAG *) NULL; p = p->b_next)
	{
	    if (p->b_tag == T_EXPRESSION)
		CodePrintExpression (p->b_val.b_expression);
	    else if (p->b_tag == T_TUPLE)
		CodePrintTuple (p->b_val.b_tuple);
	    else
		fprintf (Output, "Error in CodePrintBag!\n"), exit (1);
	    if (p->b_next != (BAG *) NULL)
                fprintf (Output, ", ");
        }
	fprintf (Output, "} ");
        return;

}       /* end CodePrintBag */


/*
 ****************************************************************
 * Print a Tuple in a Bag                                       *
 ****************************************************************
 */

void
CodePrintTuple (pt)
TUPLE *pt;
{
	register TUPLE *p;

	fprintf (Output, "[ ");
	for (p = pt; p != (TUPLE *) NULL; p = p->t_next)
	{
	    CodePrintExpression (p->t_expression);
	    if (p->t_next != (TUPLE *) NULL) 
                fprintf (Output, ", ");
        }
	fprintf (Output, "] ");
        return;

}	/* end CodePrintTuple */


/*
 ****************************************************************
 * Print a Program                                              *
 ****************************************************************
 */

void 
CodePrintProgram (pp)
PROGRAM *pp;
{
	CodePrintBody (pp->p_body);
	CodePrintBag (pp->p_bag);
	fprintf (Output, " where ");
	CodePrintListDefinitions (pp->p_defs);
	putchar ('\n');
	return;

}	/* end CodePrintProgram */


