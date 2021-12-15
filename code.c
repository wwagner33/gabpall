
/*
 ****************************************************************
 *                                                              *
 *      Module: newcode.c                                       *
 *      Description: Code Generation                            *
 *      Version: 1.0 of 24/Aug/95                               *
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
# include "include/vars.h"

/*
 ****************************************************************
 * Look up the definition of a name in a list of definitions.   *
 * Return NULL if the name is not defined.                      *
 ****************************************************************
 */

BODY *
LookUp(name, deflist)
char *name;
DEFS *deflist;
{
  DEFS *dl;

  for (dl = deflist; dl != (DEFS *)NULL; dl = dl->d_next) {
    if (strcmp(name, dl->d_definition->d_name)==0) {
      return (dl->d_definition->d_body);
    }
  }
  return ((BODY *)NULL);
} /* end LookUp */

/*
 ****************************************************************
 * Expand all the identifiers in a body.                        *
 * Also generate a cell number for each node in the tree.       *
 ****************************************************************
 */

EXPANDED_BODY *
Expand(body, defs, start)
BODY *body;
DEFS *defs;
int *start;
{
  EXPANDED_BODY *ebody;
  COMBINATOR *com;
  BODY *named_body;

  switch (body->b_tag)
    {
    case T_BASIC_BODY: {
      ebody = (EXPANDED_BODY *)Alloc(sizeof(EXPANDED_BODY));
      ebody->eb_tag = T_BASIC_BODY;
      ebody->eb_val.eb_bbody = body->b_val.b_bbody;
      ebody->eb_number = *start;
      *start = *start + 1;
      return (ebody);
    }
    case T_COMBINATOR: {
      ebody = (EXPANDED_BODY *)Alloc(sizeof(EXPANDED_BODY));
      ebody->eb_tag = T_COMBINATOR;
      ebody->eb_val.eb_comb = (EXPANDED_COMBINATOR *)Alloc(sizeof(EXPANDED_COMBINATOR));
      ebody->eb_number = *start;
      *start = *start + 1;
      ebody->eb_val.eb_comb->ec_combinator = body->b_val.b_comb->c_combinator;
      ebody->eb_val.eb_comb->ec_left = Expand(body->b_val.b_comb->c_left, defs, start);
      ebody->eb_val.eb_comb->ec_right = Expand(body->b_val.b_comb->c_right, defs, start);
      return (ebody);
    }
    case T_IDENTIFIER: {
      named_body = LookUp(body->b_val.b_name, defs);
      if (named_body == (BODY *)NULL) {
	fprintf(stderr, "gm: Error - undefined name %s.\n\n", body->b_val.b_name);
	exit(1);
      }
      return (Expand(named_body, defs, start));
    }
    }
} /* end Expand */

/*
 ****************************************************************
 * Write Header with Included Files                             *
 ****************************************************************
 */

void
WriteHeader ()
{
	fprintf (Output, "# include <stdio.h>\n");
	fprintf (Output, "# include <sys/types.h>\n");
	fprintf (Output, "# include <sys/time.h>\n");
	fprintf (Output, "# include \"/usr/include/mpi/mpi.h\"\n"); /*/u/SO/paillard//MPI/MPI/h/mpi.h*/
	fprintf (Output, "# include \"../include/const.h\"\n");
	fprintf (Output, "# include \"../include/types.h\"\n");
	fprintf (Output, "# include \"../include/rtbag.h\"\n");
	fprintf (Output, "# include \"../include/fnc.h\"\n");
	fprintf (Output, "# include \"../include/msg.h\"\n");
	fprintf (Output, "# include \"../include/vars.h\"\n\n\n");

}	/* end WriteHeader */

/*
 ****************************************************************
 * Generate Code for the Initial Bag                            *
 ****************************************************************
 */

void
CodeInitialBag (pb)
BAG *pb;
{
	register BAG *paux = pb;

	fprintf (Output, "void\nCreateInitialBag ()\n{\n");
	while (paux != (BAG *) NULL)
	{
	    if (paux->b_tag == T_TUPLE)
		GenTuple (paux);
	    else 
		GenBasicExpression (paux);
            paux = paux->b_next;
        }
        fprintf (Output, "\n}   /* end CreateInitialBag */\n\n");

}       /* end CodeInitialBag */

/*
 ****************************************************************
 * Generate Code for An Expression in the Initial Bag           *
 ****************************************************************
 */

void
GenBasicExpression (paux)
BAG *paux;
{
	if (paux->b_val.b_expression->e_tag == T_NUMERAL)
	    fprintf (Output, "\tRunTimeBag = AddRTBagNode(TY_INT, %d, RunTimeBag);\n",
	                               paux->b_val.b_expression->e_val.e_numval);
	else    /* let's treat boolean values as integers */
	    fprintf (Output, "\tRunTimeBag = AddRTBagNode(TY_INT, %d, RunTimeBag);\n",
                              paux->b_val.b_expression->e_val.e_boolval);
	return;	

}	/* end GenBasicExpression */


/*
 ****************************************************************
 * Get the size of a tuple                                      *
 ****************************************************************
 */

int
GetTupleSize (pt)
TUPLE *pt;
{
	register int size = 0;
	register TUPLE *paux;

	for (paux = pt; paux != (TUPLE *) NULL; paux = paux->t_next)
	    size++;
	return (size);

}	/* end GetTupleSize */

/*
 ****************************************************************
 * Generate Code for Tuple in the Initial Bag                   *
 ****************************************************************
 */

void
GenTuple (pb)
BAG *pb;
{
	register TUPLE *pt;	
	register int size;
	register int tag;

	fprintf (Output, "\t{\n\t    register RT_TUPLE *pt;\n");
	fprintf (Output, "\t    register RT_LIST *pl;\n\n");
	size = GetTupleSize (pb->b_val.b_tuple);
	fprintf (Output, "\t    pt = GenTupleSkeleton (%d);\n", size);
	fprintf (Output, "\t    pl = pt->rtt_list;\n");
	for (pt = pb->b_val.b_tuple; pt != (TUPLE *) NULL; pt = pt->t_next)
	{
	    if ((tag = pt->t_expression->e_tag) == T_NUMERAL)
	    {
		fprintf (Output, "\t    pl->rtl_tag = TY_INT;\n");
		fprintf (Output, "\t    pl->rtl_element = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));\n");
		fprintf (Output, "\t    pl->rtl_element->rte_val = %d;\n", pt->t_expression->e_val.e_numval);
	    } else if (tag == T_BOOLEAN) {  /* we treat booleans as integers */
                fprintf (Output, "\t    pl->rtl_tag = TY_INT;\n");
                fprintf (Output, "\t    pl->rtl_element = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));\n");
                fprintf (Output, "\t    pl->rtl_element->rte_val = %d;\n", pt->t_expression->e_val.e_boolval);
	    } 
	    fprintf (Output, "\t    pl = pl->rtl_next;\n");
	}
	fprintf (Output, "\t    RunTimeBag = AddRTTupleBagNode (pt, RunTimeBag);\n\t}\n");
	return;	    

}	/* end GenTuple */

/*
 ****************************************************************
 * Generate Code for Expressions  via a Namelist                *
 ****************************************************************
 */

void
GenCodeExpression (pe, name,nl)
EXPRESSION *pe;
char *name;                   /* the name of the generated function */
NAMELIST *nl;
{
	register EXPRESSION *paux = pe;

	/*
	 * Generate the Header of the Function.
	 */
	fprintf (Output, "%s (", name);
	while (nl != (NAMELIST *) NULL)
	{
	    fprintf (Output, "%s", nl->n_name);
	    if ((nl = nl->n_next) != (NAMELIST *) NULL)
		fprintf (Output, ", ");
	}
	fprintf (Output, ")\n{\n");
	fprintf (Output, "register int Result = 0;\n\n");
	fprintf (Output, "Result = ");
        CodePrintExpression (pe);
	fprintf (Output, ";\nreturn (Result);\n");
	fprintf (Output, "\n}\t/* end %s */\n\n", name);
	
}	/* end GenCodeExpression */


/*
 ****************************************************************
 * Generate Code for Expressions  via a Pattern                 *
 ****************************************************************
 */

void
GenCodeExpressionWithPattern (pe, name, pp)
EXPRESSION *pe;
char *name;                   /* the name of the generated function */
PATTERN *pp;
{
        register PATTERN *paux;

        /*
         * Generate the Header of the Function.
         */
        fprintf (Output, "%s (", name);
	for (paux = pp; paux != (PATTERN *) NULL; paux = paux->p_next)
	{
	    if (paux->p_expression->e_tag == T_IDENTIFIER)	
                fprintf (Output, "%s", paux->p_expression->e_val.e_name);
	    else if (paux->p_expression->e_tag == T_TUPLE)
	    {
		register PATTERN *pt = paux->p_expression->e_val.e_pattern;

		while (pt != (PATTERN *) NULL)
		{
		    fprintf (Output, "%s", pt->p_expression->e_val.e_name);
		    if (pt->p_next != (PATTERN *) NULL)
			fprintf (Output, ", ");
		    pt = pt->p_next;
		}
	    }
	    if (paux->p_next != (PATTERN *) NULL)
		fprintf (Output, ", ");
        }
        fprintf (Output, ")\n{\n");
        fprintf (Output, "register int Result = 0;\n\n");
        fprintf (Output, "Result = ");
        CodePrintExpression (pe);
        fprintf (Output, ";\nreturn (Result);\n");
        fprintf (Output, "\n}\t/* end %s */\n\n", name);

}       /* end GenCodeExpressionWithPattern */



/*
 ****************************************************************
 * Generate Code for Parallel and Sequential Composition        *
 ****************************************************************
*/

void
GenCodeCombinator (pc, cell, parent)
EXPANDED_COMBINATOR *pc;
int cell, parent;
{
	void GenCodeParComposition (),
	     GenCodeSeqComposition ();

	if (pc->ec_combinator == T_PAR)
	    GenCodeParComposition (pc, cell, parent);
	else
	    GenCodeSeqComposition (pc, cell, parent);
	
}	/* end GenCodeCombinator */

/*
 ****************************************************************
 * Generate Code for Sequential Composition                     *
 ****************************************************************
 */

void
GenCodeSeqComposition (pc, cell, parent)
EXPANDED_COMBINATOR *pc;
int cell, parent;
{
	int child1, child2;

	void GenCodeCell();

	child1 = pc->ec_left->eb_number;
	child2 = pc->ec_right->eb_number;

	fprintf (Output, "void cell_%d ()\n{\n", cell);
	fprintf (Output, "int response;\n");
	fprintf (Output, "int tag;\n");
	fprintf (Output, "MPI_Status status;\n");
	fprintf (Output, "int false_buffer = FALSE;\n\n");
	fprintf (Output, "for (;;) {\n");
	fprintf (Output, "\tMPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);\n");
	fprintf (Output, "\ttag = status.MPI_TAG;\n");
	fprintf (Output, "\tif (tag == DIE) return;\n");
	fprintf (Output, "\tfor (;;) {\n");
	fprintf (Output, "\t    MPI_Send(NULL, 0, MPI_INT, %d, BR_DOIT, MPI_COMM_WORLD);\n", child1);
	fprintf (Output, "\t    MPI_Recv(&response, 1, MPI_INT, %d, BR_RESPONSE, MPI_COMM_WORLD, &status);\n", child1);
	fprintf (Output, "\t    if (response == TRUE) continue;\n");
	fprintf (Output, "\t    break;\n}\n");
        fprintf (Output, "\tfor (;;) {\n");
	fprintf (Output, "\t    MPI_Send(NULL, 0, MPI_INT, %d, BR_DOIT, MPI_COMM_WORLD);\n", child2);
	fprintf (Output, "\t    MPI_Recv(&response, 1, MPI_INT, %d, BR_RESPONSE, MPI_COMM_WORLD, &status);\n", child2);
	fprintf (Output, "\t    if (response == TRUE) continue;\n");
        fprintf (Output, "\t    break;\n}\n");
	fprintf (Output, "\tMPI_Send(&false_buffer, 1, MPI_INT, %d, BR_RESPONSE, MPI_COMM_WORLD);\n", parent);
	fprintf (Output, "}\n");
	fprintf (Output, "\n}\t/* end cell_%d */\n\n", cell);
	GenCodeCell (pc->ec_right, cell);
	GenCodeCell (pc->ec_left, cell);

}	/* end GenCodeSeqComposition */

/*
 ****************************************************************
 * Generate Code for Parallel Composition                       *
 ****************************************************************
 */

void
GenCodeParComposition (pc, cell, parent)
EXPANDED_COMBINATOR *pc;
int cell, parent;
{
  int child1, child2;

  void GenCodeCell();

	child1 = pc->ec_left->eb_number;
	child2 = pc->ec_right->eb_number;

        fprintf (Output, "void cell_%d ()\n{\n", cell);
	fprintf (Output, "\tint response, cell_replied, cell_working;\n");
	fprintf (Output, "int false_buffer = FALSE;\n");
	fprintf (Output, "int tag;\n");
	fprintf (Output, "\tMPI_Status status;\n\n");
        fprintf (Output, "for (;;) {\n");
	fprintf (Output, "\tMPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);\n");
	fprintf (Output, "\ttag = status.MPI_TAG;\n");
	fprintf (Output, "\tif (tag == DIE) return;\n");
	fprintf (Output, "\tMPI_Send(NULL, 0, MPI_INT, %d, BR_DOIT, MPI_COMM_WORLD);\n", child1);
	fprintf (Output, "\tMPI_Send(NULL, 0, MPI_INT, %d, BR_DOIT, MPI_COMM_WORLD);\n", child2);
	fprintf (Output, "\tfor (;;) {\n");
	fprintf (Output, "\t\tMPI_Recv(&response, 1, MPI_INT, MPI_ANY_SOURCE, BR_RESPONSE, MPI_COMM_WORLD, &status);\n");
	fprintf (Output, "\t\tcell_replied = status.MPI_SOURCE;\n");
	fprintf (Output, "\t\tif (response == TRUE) {\n");
	fprintf (Output, "\t\t\tMPI_Send(NULL, 0, MPI_INT, cell_replied, BR_DOIT, MPI_COMM_WORLD);\n");
	fprintf (Output, "\t\t\tcontinue;\n} else {\n");
	fprintf (Output, "\t\t\tif (cell_replied == %d)\n", child1);
	fprintf (Output, "\t\t\t\tcell_working = %d;\n", child2);
	fprintf (Output, "\t\t\telse\n");
	fprintf (Output, "\t\t\t\tcell_working = %d;\n", child1);
	fprintf (Output, "\t\t\tMPI_Recv(&response, 1, MPI_INT, cell_working, BR_RESPONSE, MPI_COMM_WORLD, &status);\n");
	fprintf (Output, "\t\t\tif (response == FALSE) {\n");
	fprintf (Output, "\t\t\t\tMPI_Send(&false_buffer, 1, MPI_INT, %d, BR_RESPONSE, MPI_COMM_WORLD);\n", parent);
	fprintf (Output, "\t\t\t\tbreak;\n");
	fprintf (Output, "\t\t\t} else {\n");
	fprintf (Output, "\t\t\t\tMPI_Send(NULL, 0, MPI_INT, %d, BR_DOIT, MPI_COMM_WORLD);\n", child1);
	fprintf (Output, "\t\t\t\tMPI_Send(NULL, 0, MPI_INT, %d, BR_DOIT, MPI_COMM_WORLD);\n", child2);
	fprintf (Output, "\t\t\t\tcontinue;\n\t\t\t}\n\t\t}\n\t}\n}");
        fprintf (Output, "\n}\t/* end cell_%d */\n\n", cell);
        GenCodeCell (pc->ec_right, cell);
        GenCodeCell (pc->ec_left, cell);

}	/* end GenCodeParComposition */

/*
 ****************************************************************
 * Generate Code for Reaction Condition                         *
 ****************************************************************
*/

void
GenCodeReaction (pa, pr, cell, parent)
PATTERN *pa;
EXPRESSION *pr;
int cell, parent;
{
	NAMELIST *NamesInPattern ();
	char buffer[BUFSIZ];
	register NAMELIST *nl, *nh = (NAMELIST *) NULL;
	register PATTERN *paux;
	int i, j;

	nh = nl = NamesInPattern (pa, (NAMELIST *) NULL);
	fprintf (Output, "void cell_%d ()\n{\nRT_VALUE *GenValueList ();\n", cell);
	/*
	 * For each pattern we have an associated variable.
	 */
	for (paux = pa, i = 0; paux != (PATTERN *) NULL; paux = paux->p_next, i++)
	{
		if (pa->p_expression->e_tag == T_IDENTIFIER)
		{
		    fprintf (Output, "register RT_BAG *ptr_%d;\n", i);
		    fprintf (Output, "register int x_%d;\n", i);	    
		} else if (pa->p_expression->e_tag == T_TUPLE) {
		    fprintf (Output, "register RT_BAG *ptr_%d;\n", i); /* the tuple itself */
		    GenTupleVariables (pa->p_expression->e_val.e_pattern, &i); /* the elemts. of the tuple */
		} else
		    printf ("gm: Internal bloody error!\n"), exit (1);
	}
	fprintf (Output, "\nint result; \nregister RT_VALUE *link, *linkhead;\n");
	fprintf (Output, "MPI_Status status;\n");
	fprintf (Output, "int false_buffer = FALSE;\n");
	fprintf (Output, "int tag;\n\n");
	fprintf (Output, "void ReceiveBag();\n");
	fprintf (Output, "int RequestBag();\n");

	fprintf (Output, "for (;;) {\n");
	fprintf (Output, "result = FALSE;\n");
	fprintf (Output, "MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);\n");
	fprintf (Output, "tag = status.MPI_TAG;\n");
	fprintf (Output, "if (tag == DIE) return;\n");
	/* 
	 * Generate code to get the bag.
	 */
	fprintf (Output, "for (;!RequestBag(0););\n");
	fprintf (Output, "ReceiveBag(0);\n");
/*	fprintf (Output, "printf(\"cell_%d: received bag \");\n", cell);
	fprintf (Output, "PrintRTBag();\n"); */
	/*
	 * Generate the nested for loops 
	 */
	for (paux = pa, i = 0; paux != (PATTERN *) NULL; paux = paux->p_next, i++)
	{
	    if (pa->p_expression->e_tag == T_IDENTIFIER)
	        GenCodeBagIntSearch (i);
	    else if (pa->p_expression->e_tag == T_TUPLE) 
		GenCodeBagTupleSearch (&i, pa->p_expression->e_val.e_pattern); 
		
	}
	/*
	 * Generate the Call to the reaction function proper.
	 */
	GenCallReaction (cell, pa);
	
        fprintf (Output, "    if (result == TRUE) {\n");
	/*
	 * Generate Call to Corresponding Action.
	 * But first we make a list of values with their names.
	 * This list is stored in linkhead.
	 */
	GenCallAction (pa);
	sprintf(buffer, "cell_%d_action", cell);
	fprintf (Output, "%s (linkhead);\n", buffer);
	fprintf (Output, "}\n");

	/*
	 * Close all the nested for loops.
	 */
	CloseForLoops (pa,0);
	fprintf (Output, "if (result == FALSE)\n");
	fprintf (Output, "UnlockBagNodes ();\n");
	fprintf (Output, "MPI_Send(NULL, 0, MPI_INT, 0, BAG_SEND, MPI_COMM_WORLD);\n");
/*	fprintf (Output, "printf(\"cell_%d: returning bag \");\n", cell);
	fprintf (Output, "PrintRTBag();\n"); */
	fprintf (Output, "SendBag(0);\n");
	fprintf (Output, "MPI_Send(&result, 1, MPI_INT, %d, BR_RESPONSE, MPI_COMM_WORLD);\n", parent); 
	fprintf (Output, "}\n");
	fprintf (Output, "\n}\t/* end cell_%d */\n\n", cell);
	sprintf (buffer, "cell_%d_reaction_fnc", cell);
	GenCodeExpressionWithPattern (pr, buffer, pa);		

}	/* end GenCodeReaction */

/*
 ****************************************************************
 * Generate Code for a Search of an integer in the Run Time Bag *
 ****************************************************************
*/

void
GenCodeBagIntSearch (i)
int i;
{
	fprintf (Output, "for (ptr_%d = RunTimeBag; (ptr_%d != (RT_BAG *) NULL) && (result == FALSE); ptr_%d = ptr_%d->rtb_next) {\n", 
										i,i,i,i);
	fprintf (Output, "   if (!NODE_LOCKED(ptr_%d->rtb_tag) && (NODE_TYPE(ptr_%d->rtb_tag) == TY_INT)) {\n", 
                                               i,i);
	fprintf (Output, "      LOCK_NODE(ptr_%d->rtb_tag);\n      x_%d = ptr_%d->rtb_element->rte_val;\n  }\n",
							i,i,i);	
	fprintf (Output, "   else continue;\n");
	return;

}	/* end GenCodeBagSearch */	

	
/*
 ****************************************************************
 * Generate Code for Actions                                    *
 ****************************************************************
*/

void
GenCodeActions (pa, cell)
ACTION *pa;
int cell;
{
	char buffer_a[BUFSIZ], buffer_e[BUFSIZ];
	register NAMELIST *nl, *nh;
	register int i;
	register ACTION *paux;

	fprintf (Output, "cell_%d_action (link) \nRT_VALUE *link;\n{", cell);
	for (i = 0, paux = pa; paux != (ACTION *) NULL; i++, paux = paux->a_next)
	{
	    if (paux->a_expression->e_tag != T_EMPTY)
	        fprintf (Output, "cell_%d_action_%d (link); \n", cell, i);
	}
	fprintf (Output, "RemoveLockedBagNodes ();\n");
	fprintf (Output, "FreeListValues (link);\n");
	fprintf (Output, "\n}	/* end cell_%d_action */\n\n", cell);	
	for (i = 0, paux = pa; paux != (ACTION *) NULL; i++, paux = paux->a_next)
	{
            nh = nl = NamesInExpression (paux->a_expression, (NAMELIST *) NULL);
	    sprintf (buffer_a, "cell_%d_action_%d", cell, i);
	    sprintf (buffer_e, "cell_%d_action_%d_e", cell, i);
	    if (paux->a_expression->e_tag == T_TUPLE)
		GenCodeTupleActions (paux->a_expression->e_val.e_pattern, buffer_a);
	    else {
		GenCodeExpression (paux->a_expression, buffer_e, nl);
	        GenCodeActionItself (buffer_a, buffer_e, nl); 
	    }
	}

}	/* end GenCodeActions */


/*
 ****************************************************************
 * Generate Code for an Action                                  *
 ****************************************************************
*/

GenCodeActionItself (name_a, name_e, nh)
char *name_a;
char *name_e;
NAMELIST *nh;
{
	register NAMELIST *nl = nh;

	fprintf (Output, "%s (link) \n RT_VALUE *link; \n{\n", name_a);
	/*
	 * Generate all the values.
	 */
        while (nl != (NAMELIST *) NULL)
        {
            fprintf (Output, "\tregister int %s = GetValue (\"%s\", link);\n", nl->n_name, nl->n_name);
            nl = nl->n_next;
        }
	fprintf (Output, "register int result = 0;\n\n");
	/*
	 * Now invoke the function that evaluates the action itself.
	 */
	fprintf (Output, "result = %s (", name_e);
	nl = nh;
        while (nl != (NAMELIST *) NULL)
        {
            fprintf (Output, "%s", nl->n_name);
            if ((nl = nl->n_next) != (NAMELIST *) NULL)
                fprintf (Output, ", ");
        }
        fprintf (Output, ");");
	fprintf (Output, "RunTimeBag = AddRTBagNode (TY_INT, result);\n");
	fprintf (Output, "\n}	/* end %s */\n\n", name_a);
	
}	/* end GenCodeActionItself */

GenCodeTupleActions (pt, name)
PATTERN *pt;
char *name;
{
        char buffer_a[BUFSIZ], buffer_e[BUFSIZ];
        register NAMELIST *nl, *nh;
        register int i;
        register PATTERN *paux;

        fprintf (Output, "%s (ln) \nRT_VALUE *ln;\n{", name);
	fprintf (Output, "\tregister int result;\n");
	fprintf (Output, "\tregister RT_VALUE *linkhead, *link;\n\n");
        for (i = 0, paux = pt; paux != (PATTERN *) NULL; i++, paux = paux->p_next)
        {
            fprintf (Output, "result = %s_tuple_action_%d (ln); \n", name, i);
	    if (i == 0)
	    {
       		fprintf (Output, "linkhead = link = (RT_VALUE *) Alloc (sizeof (RT_VALUE));\n");
	 	fprintf (Output, "link->rtv_val = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));\n");
                fprintf (Output, "link->rtv_val->rte_val = result;\n");
		fprintf (Output, "link->rtv_name = (char *) NULL;");
		fprintf (Output, "link->rtv_next = (RT_VALUE *) NULL;\n");
	    } else {
		fprintf (Output, "link = link->rtv_next = (RT_VALUE *) Alloc (sizeof (RT_VALUE));\n");
	 	fprintf (Output, "link->rtv_val = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));\n");
                fprintf (Output, "link->rtv_val->rte_val = result;\n");
		fprintf (Output, "link->rtv_name = (char *) NULL;");
		fprintf (Output, "link->rtv_next = (RT_VALUE *) NULL;\n");
	    }	
        }
	fprintf (Output, "GenRTTuple (linkhead);\n");
        fprintf (Output, "RemoveLockedBagNodes ();\n");
        fprintf (Output, "FreeListValues (linkhead);\n");
        fprintf (Output, "\n}   /* end %s_tuple_action */\n\n", name);
        for (i = 0, paux = pt; paux != (PATTERN *) NULL; i++, paux = paux->p_next)
        {
            nh = nl = NamesInExpression (paux->p_expression, (NAMELIST *) NULL);
            sprintf (buffer_a, "%s_tuple_action_%d", name, i);
            sprintf (buffer_e, "%s_tuple_action_%d_e", name, i);
            GenCodeExpression (paux->p_expression, buffer_e, nl);
            GenCodeTupleActionItself (buffer_a, buffer_e, nl);
        }

}       /* end GenCodeTupleActions */

GenCodeTupleActionItself (name_a, name_e, nh)
char *name_a;
char *name_e;
NAMELIST *nh;
{
        register NAMELIST *nl = nh;

        fprintf (Output, "%s (link) \n RT_VALUE *link; \n{\n", name_a);
        /*
         * Generate all the values.
         */
        while (nl != (NAMELIST *) NULL)
        {
            fprintf (Output, "\tregister int %s = GetValue (\"%s\", link);\n", nl->n_name, nl->n_name);
            nl = nl->n_next;
        }
        fprintf (Output, "register int result = 0;\n\n");
        /*
         * Now invoke the function that evaluates the action itself.
         */
        fprintf (Output, "result = %s (", name_e);
        nl = nh;
        while (nl != (NAMELIST *) NULL)
        {
            fprintf (Output, "%s", nl->n_name);
            if ((nl = nl->n_next) != (NAMELIST *) NULL)
                fprintf (Output, ", ");
        }
        fprintf (Output, ");");
        fprintf (Output, "return (result);\n");
        fprintf (Output, "\n}   /* end %s */\n\n", name_a);

}       /* end GenCodeActionItself */




GenTupleVariables (pt,i)
PATTERN *pt;
int *i;
{
	register PATTERN *paux;

	for (paux = pt; paux != (PATTERN *) NULL; paux = paux->p_next)
	{
	    (*i)++;
	    fprintf (Output, "register RT_ELEMENT *ptr_%d;\n", *i);
	    fprintf (Output, "register int x_%d;\n", *i); 
	}

}	/* end GenTupleVariables */


GenCodeBagTupleSearch (i, pt)
int *i;
PATTERN *pt;
{
	register int size = 0;
	register PATTERN *paux;

	for (paux = pt; paux != (PATTERN *) NULL; paux = paux->p_next)
	    size++;
        fprintf (Output, "for(ptr_%d=RunTimeBag;(ptr_%d!=(RT_BAG *)NULL) && (result == FALSE);ptr_%d=ptr_%d->rtb_next) {\n", 
                                      *i, *i, *i, *i);
        fprintf (Output, "if(!NODE_LOCKED(ptr_%d->rtb_tag)&&(NODE_TYPE(ptr_%d->rtb_tag)==TY_TUPLE)){\n",
                                               *i,*i);
	fprintf (Output, "if (ptr_%d->rtb_element->rte_tuple->rtt_size == %d) {\n", *i, size);   

/* Modificacao feita por Gabriel Paillard em 01/07/99, apenas passei o */
/* argumento para a funcao fprintf. */

        fprintf (Output, "      LOCK_NODE(ptr_%d->rtb_tag);\n",*i);
	fprintf (Output, "{\nregister RT_LIST *pl;\n");
	fprintf (Output, "pl = ptr_%d->rtb_element->rte_tuple->rtt_list;\n", *i);
	for (paux = pt; paux != (PATTERN *) NULL; paux = paux->p_next)	
	{
	    (*i)++;
	    fprintf (Output, "x_%d = pl->rtl_element->rte_val;\n",*i );
	    fprintf (Output, "ptr_%d = pl->rtl_element;\n", *i);
	    fprintf (Output, "pl = pl->rtl_next;\n");
	}
	fprintf (Output, "}\n}\n");
        fprintf (Output, "   } else continue;\n");
        return;

}       /* end GenCodeBagSearch */

GenCallAction (pp)
PATTERN *pp;
{
	char buffer[BUFSIZ];
	register PATTERN *paux;
	register int size = 0;
	register int j;

	for (paux = pp; paux != (PATTERN *) NULL; paux = paux->p_next)
	{
	    if (paux->p_expression->e_tag == T_IDENTIFIER)
	        size++;
	    else {
                register PATTERN *pt = paux->p_expression->e_val.e_pattern;

                while (pt != (PATTERN *) NULL)
                {
		    size++;
		    pt = pt->p_next;
		}
	    }
	}
	fprintf (Output, "linkhead = link = GenValueList (%d);\n", size);
        for (paux = pp, j = 0; paux != (PATTERN *) NULL; paux = paux->p_next, j++)
        {
            if (paux->p_expression->e_tag == T_IDENTIFIER)
	    {
                sprintf (buffer, "ptr_%d", j);
                fprintf (Output, "link->rtv_val = %s->rtb_element;\n", buffer);
		fprintf (Output, "link->rtv_name =  Alloc (%d);\n", 
			strlen (paux->p_expression->e_val.e_name)+1);
		fprintf (Output, "strcpy (link->rtv_name, \"%s\");\n",
				paux->p_expression->e_val.e_name);
		fprintf (Output, "link = link->rtv_next;\n");
            } else if (paux->p_expression->e_tag == T_TUPLE) {
                register PATTERN *pt = paux->p_expression->e_val.e_pattern;

		j++; /* skip the pointer to the tuple node itself */
                while (pt != (PATTERN *) NULL)
                {
		    sprintf (buffer, "ptr_%d", j);
		    fprintf (Output, "link->rtv_val = %s;\n", buffer);
		     fprintf (Output, "link->rtv_name =  Alloc (%d);\n", 
                        strlen (pt->p_expression->e_val.e_name) + 1);
		     fprintf (Output, "strcpy (link->rtv_name, \"%s\");\n",
				pt->p_expression->e_val.e_name);
		    fprintf (Output, "link = link->rtv_next;\n");
                    if ((pt = pt->p_next) != (PATTERN *) NULL)
		        j++;
                }
            }
        }

}	/* end GenCallAction */



GenCallReaction (cell, pp)
int cell;
PATTERN *pp;
{
	register PATTERN *paux;
	register int j;

	fprintf (Output, "    result = cell_%d_reaction_fnc (", cell);
	for (paux = pp, j = 0; paux != (PATTERN *) NULL; paux = paux->p_next)
	{
	    if (paux->p_expression->e_tag == T_IDENTIFIER)
	        fprintf (Output, "x_%d", j++);
	    else {
                register PATTERN *pt = paux->p_expression->e_val.e_pattern;

		j++;      /* skip the index for pointer to tuple itself */
                while (pt != (PATTERN *) NULL)
                {
		    fprintf (Output, "x_%d", j++);
		    if ((pt = pt->p_next) != (PATTERN *) NULL)
			fprintf (Output, ", ");
		}
	    }
	    if (paux->p_next != (PATTERN *) NULL)
		fprintf (Output, ", ");
	}
	fprintf (Output, ");\n");

}	/* end GenCallReaction */

CloseForLoops (pp,i)
PATTERN *pp;
int i;
{
	int j;

	if (pp == (PATTERN *) NULL)
	    return;
        if (pp->p_expression->e_tag == T_IDENTIFIER)
	{
	    CloseForLoops (pp->p_next, ++i);
	    fprintf (Output, "if (result == FALSE)\n");
	    fprintf (Output, "UNLOCK_NODE(ptr_%d->rtb_tag);\n}\n", i-1);
        } else {
                register PATTERN *pt = pp->p_expression->e_val.e_pattern;

		j = i++;
                while (pt != (PATTERN *) NULL)
                {
		    if ((pt = pt->p_next) != (PATTERN *) NULL)
		        i++;
                }
		CloseForLoops (pp->p_next, ++i);
		fprintf (Output, "if (result == FALSE)\n");
		fprintf (Output, "UNLOCK_NODE(ptr_%d->rtb_tag);\n}\n", j);
        }

}	/* end CloseForLoops */


/*
 ****************************************************************
 * Generate Code for each cell, from the expanded program.      *
 ****************************************************************
 */

void
GenCodeCell (exp_body, parent)
EXPANDED_BODY *exp_body;
int parent;
{
  switch (exp_body->eb_tag) {

  case T_COMBINATOR: {
    GenCodeCombinator(exp_body->eb_val.eb_comb, exp_body->eb_number, parent);
    break;
  }

  case T_BASIC_BODY: {
    GenCodeReaction(exp_body->eb_val.eb_bbody->b_pattern, exp_body->eb_val.eb_bbody->b_reaction, exp_body->eb_number, parent);
    GenCodeActions (exp_body->eb_val.eb_bbody->b_action, exp_body->eb_number);
    break;
  }
  }
} /* end GenCodeCell */

/*
 ****************************************************************
 * Generate Code for the program                                *
 ****************************************************************
 */

int 
GenMainCode (Program)
PROGRAM *Program;
{
  EXPANDED_BODY *exp_program;
  int total_cells = 2;
  int i;

  exp_program = Expand(Program->p_body, Program->p_defs, &total_cells);

/* cell 0 is always the bag cell - the code for it is defined elsewhere */

  fprintf (Output, "extern void cell_0();\n\n");

/* cell 1 is always the top-level controller cell - the code for it is 
   defined elsewhere. */

  fprintf (Output, "extern void cell_1();\n\n");

/* Now generate code for the rest of the cells, which correspond to 
   basic reactions and combinators. The top-level node in the program
   will always be cell 2. */

  GenCodeCell(exp_program, 1);

/* Finally, generate the main() function. This will run on every cell, 
   and must work out which cell to be. */

  fprintf (Output, "void main(argc, argv)\nint argc;\nchar *argv[];\n{\n");
  fprintf (Output, "void (*c[%d])();\n", total_cells);
  fprintf (Output, "int mynumber, ierr;\n\n");

  for (i = 0; i < total_cells; ++i) 
    fprintf (Output, "c[%d] = cell_%d;\n", i, i);

  fprintf (Output, "\n");
  fprintf (Output, "ierr = MPI_Init(&argc, &argv);\n");
  fprintf (Output, "if (ierr != MPI_SUCCESS) {\n");
  fprintf (Output, "  fprintf(stderr, \"MPI initialisation error%c\",0x0A);\n");
  fprintf (Output, "  exit(1);\n");
  fprintf (Output, "}\n");

  fprintf (Output, "MPI_Comm_rank(MPI_COMM_WORLD, &mynumber);\n");

  fprintf (Output, "(c[mynumber])();\n");
/*
  fprintf (Output, "printf(\"Cell %cd is about to execute barrier.%cn\", mynumber)\n;", '%', '\\');

  fprintf (Output, "MPI_Barrier(MPI_COMM_WORLD);\n");
*/
  fprintf (Output, "MPI_Finalize();\n");
  
  fprintf (Output, "} /* end main */\n");

  return (total_cells);

} /* end GenMainCode */

/*
 ****************************************************************
 * Generate the script which will run the program               *
 ****************************************************************
 */

void GenScript (cells)
int cells;
{
FILE *script;

script = fopen("exe","w");

fprintf(script, "mpirun /u/SO/paillard/Gamma/MPI/RunTime/run -c %d -v",cells);

/*"mpirun -np %d -machinefile machines.sun4 run\n", cells);*/

fclose(script);

} /* end GenScript */
