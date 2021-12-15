
/*
 ****************************************************************
 *                                                              *
 *      Module: misc.c                                          *
 *      Description: Miscelaneous Routines                      *
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


/*
 ****************************************************************
 * Allocate a region of memory                                  *
 ****************************************************************
 */

char *
Alloc (size)
unsigned size;
{
	register char *pt;

	pt = malloc (size);
	if (pt == (char *) NULL)
	{
	    fprintf (stderr, "gm: out of memory!\n");
	    exit (1);
	}
	return (pt);

}	/* end Alloc */

/*
 ****************************************************************
 * Free a Region of Memory                                      *
 ****************************************************************
 */

void
Free (pt)
char *pt;
{
	free (pt);

}	/* end Free */



