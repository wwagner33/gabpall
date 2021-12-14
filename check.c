
/*
 ****************************************************************
 *                                                              *
 *      Module: check.c                                         *
 *      Description: Simple checking of Gamma definitions       *
 *      Version: 1.0 of 09/Jun/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

/* The functions defined in this module do the following checks 
on a collection of Gamma definitions.

1) every program name which is used, must be defined first.

2) there must be no repeated definitions.

3) for each basic reaction, all the identifiers in the reaction condition 
   and action must appear in the pattern.

*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "include/const.h"
# include "include/types.h"
# include "include/rtbag.h"
# include "include/fnc.h"

/* Test whether a name is defined. The pointer limit is treated as the end 
   of the list of definitions; this is so that we can check that names are
   defined before being used. */

int is_defined(the_defs, limit, name, print)
DEFS *the_defs;
DEFS *limit;
char *name;
int print;
{
DEFS *head = the_defs;

while (head != limit)
  {
    if (strcmp(name, head->d_definition->d_name)==0)
      return(TRUE);
    head = head->d_next;
  }
if (print)
  printf("\nThe identifier %s is used before being defined.\n", name);
return(FALSE);

} /* END is_defined */

/* Test whether all the names used in a particular body are defined. */

int check_names_in_body(the_defs, limit, the_body)
DEFS *the_defs;
DEFS *limit;
BODY *the_body;
{
int result = TRUE;
int left,right;

switch (the_body->b_tag)
  {
  case T_BASIC_BODY: 
    break;
  case T_IDENTIFIER: 
    result = result && is_defined(the_defs, limit, the_body->b_val.b_name, TRUE);
    break;
  case T_COMBINATOR: 
    left = check_names_in_body(the_defs, limit, the_body->b_val.b_comb->c_left); 
    right = check_names_in_body(the_defs, limit, the_body->b_val.b_comb->c_right);
    result = result && left && right;
    break;
  }
return(result);

} /* END check_names_in_body */

/* Check that name use is OK in all bodies. */

int check_names_in_all_bodies(the_defs)
DEFS *the_defs;
{
DEFS *head = the_defs;
int result = TRUE;

while (head != (DEFS *)NULL)
  {
    result = result && 
      check_names_in_body(the_defs, head, head->d_definition->d_body);
    head = head->d_next;
  }
return(result);

} /* END check_names_in_all_bodies */

/* Check all the uses of names in a program. */

int check_names_in_program(prog)
PROGRAM *prog;
{
  return(check_names_in_body(prog->p_defs, (DEFS *)NULL, prog->p_body)
	 && check_names_in_all_bodies(prog->p_defs));

} /* END check_names_in_program */

/* Check that there are no multiple definitions. */

int check_multiple_defs(the_defs)
DEFS *the_defs;
{
DEFS *head = the_defs;
int result = TRUE; 
char *name;   

while (head != (DEFS *)NULL)
  {
    name = head -> d_definition -> d_name;
    if (is_defined(head -> d_next, (DEFS *)NULL, name, FALSE))
      {
	printf("\nThe identifier %s is redefined.\n", name);
	result = FALSE;
      }
    head = head -> d_next;
  }
return(result);
} /* END check_multiple_defs */

/* CONS a name onto a list. */

NAMELIST *ConsName(name, nl)
char *name;
NAMELIST *nl;
{
  NAMELIST *newp;
  newp = (NAMELIST *)Alloc(sizeof(NAMELIST));
  newp -> n_name = name;
  newp -> n_next = nl;
  return(newp);

} /* END ConsName */ 

/* Test for membership in a list of names. */

int IsElement(name, nl)
char *name;
NAMELIST *nl;
{
  NAMELIST *head = nl;
  while (head != (NAMELIST *)NULL)
    {
      if (strcmp(name, head->n_name)==0)
	return(TRUE);
      head = head->n_next;
    }
  return(FALSE);

} /* END IsElement */

/* Add a name to a list if it is not already there. */

NAMELIST *AddElement(name, nl)
char *name;
NAMELIST *nl;
{
  if (IsElement(name, nl))
    return(nl);
  return(ConsName(name, nl));

} /* END AddElement */

/* Print a list of names. */

void PrintNameList(nl)
NAMELIST *nl;
{
  NAMELIST *head = nl;

  while (head != (NAMELIST *)NULL) {
    printf("%s ", head->n_name);
    head = head->n_next;
  }

} /* END PrintNameList */

/* Compute a list of the names which are in l but not in m. */

NAMELIST* Difference(l, m)
NAMELIST *l;
NAMELIST *m;
{
  NAMELIST *result = (NAMELIST *)NULL;
  NAMELIST *head = l;

  while (head != (NAMELIST *)NULL) {
    if (!IsElement(head->n_name, m)) 
      result = ConsName(head->n_name, result);
    head = head->n_next;
  }

  return(result);

} /* END Difference */

/* Make a list of the names used in an expression. Append it to nl. */

NAMELIST *NamesInPattern();

NAMELIST *NamesInExpression(pe, nl)
EXPRESSION *pe;
NAMELIST *nl;
{
NAMELIST *result = nl;

  switch (pe->e_tag) {
    case T_EMPTY:
      break;
    case T_BOOLEAN:
      break;
    case T_NUMERAL:
      break;
    case T_IDENTIFIER: 
      result = AddElement(pe->e_val.e_name, result);
      break;
    case T_TUPLE:
      result = NamesInPattern(pe->e_val.e_pattern, result);
      break;
    default:
      result = NamesInExpression(pe->e_left, result);
      result = NamesInExpression(pe->e_right, result);
      break;
    }
  return(result);

} /* END NamesInExpression */

/* Make a list of the names used in a pattern, and append it to nl. */

NAMELIST *NamesInPattern(pp, nl)
PATTERN *pp;
NAMELIST *nl;
{
  NAMELIST *result = nl;
  PATTERN *head = pp;

  while (head != (PATTERN *)NULL) {
    result = NamesInExpression(head->p_expression, result);
    head = head->p_next;
  }

  return(result);

} /* END NamesInPattern */

/* Make a list of the names used in an action, and append to nl. */

NAMELIST *NamesInAction(pa, nl)
ACTION *pa;
NAMELIST *nl;
{
  NAMELIST *result = nl;
  ACTION *head = pa;

  while (head != (ACTION *)NULL) {
    result = NamesInExpression(head->a_expression, result);
    head = head->a_next;
  }

  return(result);

} /* END NamesInAction */

/* Check name use in a basic reaction. */

int CheckNamesInBasicReaction(bp)
B_BODY* bp;
{
  int result = TRUE;
  NAMELIST *names_in_pattern, *names_in_action, *names_in_reaction,
    *bad_names_in_action, *bad_names_in_reaction;

  names_in_pattern = NamesInPattern(bp->b_pattern, (NAMELIST *)NULL);
  names_in_action = NamesInAction(bp->b_action, (NAMELIST *)NULL);
  names_in_reaction = NamesInExpression(bp->b_reaction, (NAMELIST *)NULL);

/*
PrintNameList(names_in_pattern);
printf("\n");
PrintNameList(names_in_action);
printf("\n");
PrintNameList(names_in_reaction);
printf("\n");
*/

  bad_names_in_action = Difference(names_in_action, names_in_pattern);
  bad_names_in_reaction = Difference(names_in_reaction, names_in_pattern);

  if (bad_names_in_action != (NAMELIST *)NULL) {
    printf("Bad names in action: ");
    PrintNameList(bad_names_in_action);
    printf("\n");
    result = FALSE;
  }

  if (bad_names_in_reaction != (NAMELIST *)NULL) {
    printf("Bad names in reaction: ");
    PrintNameList(bad_names_in_reaction);
    printf("\n");
    result = FALSE;
  }

  return(result);

} /* END CheckNamesInBasicReaction */

/* Check name use in a body. */

int CheckNamesInBody(bp)
BODY *bp;
{
  switch (bp->b_tag) {
    case T_IDENTIFIER:
      return(TRUE);
    case T_COMBINATOR:
      return(TRUE);
    case T_BASIC_BODY:
      return(CheckNamesInBasicReaction(bp->b_val.b_bbody));
    }

} /* END CheckNamesInBody */


/* Check name use in all the bodies in a program. */

int CheckNamesInProgram(pp)
PROGRAM *pp;
{
  int result = TRUE;
  DEFS *head;

  result = CheckNamesInBody(pp->p_body);
  head = pp->p_defs;
  while (head != (DEFS *)NULL) {
    result = result && CheckNamesInBody(head->d_definition->d_body);
    head = head->d_next;
  }

  return(result);

} /* END CheckNamesInProgram */

/* Returns the size of a Namelist */

int SizeOfNameList (nl)
NAMELIST *nl;
{
  int size = 0;

  while (nl != (NAMELIST *)NULL) {
    size++;
    nl = nl->n_next;
  }

  return(size);

} /* END SizeOfNameList */

