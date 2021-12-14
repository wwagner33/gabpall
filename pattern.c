/*
 ****************************************************************
 *                                                              *
 *      Module: pattern.c                                       *
 *      Description: Treatment of Patterns                      *
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
# include "include/rtbag.h"
# include "include/fnc.h"

int
CheckPatternsInDefinitions (pd)
DEFS *pd;
{
	NAMELIST *CheckRepeatedPatterns ();
	register DEFS *paux;
	int code = FALSE;

	for (paux = pd; paux != (DEFS *) NULL; paux = paux->d_next)
	{
	    if (paux->d_definition->d_body->b_tag == T_BASIC_BODY)
	        (void) CheckRepeatedPatterns (paux->d_definition->d_body->b_val.b_bbody->b_pattern,
				paux->d_definition->d_name, &code, (NAMELIST *) NULL);
	}	
	return (code);

}	/* end CheckPatternsInDefinitions */

/*
 ****************************************************************
 * Check for repeated variables in a pattern                    *
 ****************************************************************
 */

NAMELIST *
CheckRepeatedPatterns (pp, name, code, nl)
PATTERN *pp;
char *name;
int *code;
NAMELIST *nl;
{
	NAMELIST *AddElement ();
	register PATTERN *paux;
	register EXPRESSION *pe;
	register int tag;
	
	for (paux = pp; paux != (PATTERN *) NULL; paux = paux->p_next)
	{
	    tag = paux->p_expression->e_tag;
	    pe = paux->p_expression;
	    if (tag == T_IDENTIFIER)
		if (IsElement (pe->e_val.e_name, nl))
		{
		    printf ("gm: pattern %s repeated in definition of %s\n", 
				pe->e_val.e_name, name);
		    *code = TRUE;
		} else
		    nl = AddElement (pe->e_val.e_name, nl);
	    else if (tag == T_TUPLE)
		nl = CheckRepeatedPatterns (pe->e_val.e_pattern, name, code, nl);
	}
	return (nl);

}	/* end CheckRepeatedPatterns */


/*
 ****************************************************************
 * Generate a unique variable name                              *
 ****************************************************************
 */

char *
UniqueVarName ()
{
	static int val = 0;
	static char buffer[BUFSIZ];

	sprintf (buffer, "__x%d", val++);
	return (buffer);

}	/* end UniqueVarName */

/*
 ****************************************************************
 * Translate Patterns in a Body                                 *
 ****************************************************************
 */

B_BODY *
TranslatePatternInBody (pb)
B_BODY *pb;
{
	EXPRESSION *TranslatePattern ();
	register PATTERN *pp = pb->b_pattern;
	register EXPRESSION *pe = pb->b_reaction;
	register int tag;

	while (pp != (PATTERN *) NULL)
	{
	    pe = TranslatePattern (pp, pe);
	    pp = pp->p_next;
	}	
	pb->b_reaction = pe;

}	/* end TranslatePatternBody */

/*
 ****************************************************************
 * Translate Patterns in a Body                                 *
 ****************************************************************
 */

EXPRESSION *
TranslatePattern (pp, pe)
PATTERN *pp;
EXPRESSION *pe;
{
	EXPRESSION *IntegerPattern (), *TuplePattern ();
	register int tag;

	if ((tag = pp->p_expression->e_tag) == T_NUMERAL)
            pe = IntegerPattern (pp, pp->p_expression->e_val.e_numval, pe);
        else if (tag == T_BOOLEAN)
            pe = IntegerPattern (pp, pp->p_expression->e_val.e_boolval, pe);
        else if (tag == T_IDENTIFIER)
            ;            /* skip identifiers */
        else
            pe = TuplePattern (pp->p_expression->e_val.e_pattern, pe);
	return (pe);

}	/* end TranslatePattern */


/*
 ****************************************************************
 * Translate Patterns in a Body                                 *
 ****************************************************************
 */

EXPRESSION *
TuplePattern (pt, pe)
PATTERN *pt;
EXPRESSION *pe;
{
	register PATTERN *paux;

	for (paux = pt; paux != (PATTERN *) NULL; paux = paux->p_next)
	{
	    pe = TranslatePattern (paux, pe);
	}
	return (pe);

}	/* end TuplePattern */

/*
 ****************************************************************
 * Include another clause for pattern in reaction               *
 ****************************************************************
 */

EXPRESSION *
IntegerPattern (pp, val, pe)
PATTERN *pp;
int val;
EXPRESSION *pe;
{
	register EXPRESSION *paux;
	char *var;

	var = UniqueVarName ();
	pp->p_expression->e_tag = T_IDENTIFIER;
	pp->p_expression->e_val.e_name = Alloc (strlen (var) + 1);
	strcpy (pp->p_expression->e_val.e_name, var);	
	paux = MakeBinNode (T_EQ, MakeIdentNode (var), MakeNumNode (val));
	paux = MakeBinNode (T_AND, pe, paux);
	return (paux);

}	/* end IntegerPattern */

/*
 ****************************************************************
 * Translate All Patterns In a Program                          *
 ****************************************************************
 */

void
TranslateProgramPatterns (prog)
PROGRAM *prog;
{
	register DEFS *pd;

	for (pd = prog->p_defs; pd != (DEFS *) NULL; pd = pd->d_next)
	{
	    if (pd->d_definition->d_body->b_tag == T_BASIC_BODY)
		pd->d_definition->d_body->b_val.b_bbody = 
                   TranslatePatternInBody (pd->d_definition->d_body->b_val.b_bbody); 
	}	

}	/* end TranslateProgramPatterns */

