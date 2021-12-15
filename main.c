
/*
 ****************************************************************
 *                                                              *
 *      Module: main.c                                          *
 *      Description: The Compiler Entry Point                   *
 *      Version: 1.0 of 06/Jun/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */


# include <stdio.h>
# include <stdlib.h>
# include "include/const.h"
# include "include/types.h"
# include "include/rtbag.h"
# include "include/fnc.h"
# include "include/vars.h"
# include "include/y.tab.h"


int main(void)
{
	int yyparse();
	register int errflag = 0;
	int cells;

	if (yyparse() != 0)
		exit (1);

	printf ("\nNow the Syntax Tree\n\n");
	PrintProgram (Program);


	if (check_names_in_program(Program) == FALSE)
	    errflag = 1;

	if (check_multiple_defs(Program -> p_defs) == FALSE)
	    errflag = 1;

	if (CheckNamesInProgram(Program) == FALSE)
	    errflag = 1;
	if (errflag)
	    exit (1);
	if (CheckPatternsInDefinitions (Program->p_defs))
	    exit (1);
	TranslateProgramPatterns (Program);
	/*PrintProgram (Program);  */

	WriteHeader ();
	CodeInitialBag (Program->p_bag);
	cells = GenMainCode (Program);
	GenScript(cells);
	return 0;
}

