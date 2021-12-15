
/*
 ****************************************************************
 *                                                              *
 *      Module: copy.c                                          *
 *      Description: Routines to Copy a Run Time Bag            *
 *      Version: 1.0 of 06/Jun/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

# include <stdio.h>
# include <memory.h>
# include "../include/const.h"
# include "../include/types.h"
# include "../include/rtbag.h"
# include "../include/vars.h"


static char *Beg = (char *) NULL;          /* begining of next free area in the copy area */
static int total = 0;

SetUpArea (area, size)
char *area;
int size;
{
	Beg = area;
	total = size;

}	/* end SetUpArea */

char *
GetArea (size)
int size;
{
	register char *pc;

	total -= size;
	if (total < 0)
	{
	    fprintf (stderr, "gm: Out of Memory in GetArea\n");
	    exit (1);
	}
	pc = Beg;
	Beg += size;
	return (pc);

}	/* end GetArea */

void
ReleaseArea (area)
char *area;
{
	Free (area);
	Beg = (char *) NULL;
	total = 0;

}	/* end ReleaseArea */

/*
 ****************************************************************
 * Creat a Run-time Bag Node in the Copy Area                   *
 ****************************************************************
 */

RT_BAG *
CreateRTBagNodeCopyArea ()
{
        register RT_BAG *pb;

        pb = (RT_BAG *) GetArea (sizeof (RT_BAG));
        pb->rtb_element = (RT_ELEMENT *) GetArea (sizeof (RT_ELEMENT));
        pb->rtb_next = pb->rtb_prev = (RT_BAG *) NULL;
        return (pb);

}       /* end CreateRTBagNodeCopyArea */

/*
 ****************************************************************
 * Add an Integer or Boolean to Run Time Bag in the Copy Area   *
 ****************************************************************
 */

RT_BAG *
AddRTBagNodeCopyArea (tag, val, back)
int tag;
int val;
RT_BAG *back;
{
  RT_BAG *pb;

  pb = CreateRTBagNodeCopyArea ();
  pb->rtb_tag = tag;
  pb->rtb_element->rte_val = val;
  pb->rtb_next = (RT_BAG *)NULL;
  pb->rtb_prev = back;
  if (back != (RT_BAG *)NULL)
    back->rtb_next = pb; 
  return (pb);
} /* end AddRTBagNodeCopyArea */

/* Old version.
RT_BAG *
AddRTBagNodeCopyArea (tag, val)
int tag;
int val;
{
        register RT_BAG *pb;

        if (RunTimeCopyBag == (RT_BAG *) NULL)
        {
            RunTimeCopyBag = pb = CreateRTBagNodeCopyArea ();
            pb->rtb_tag = tag;
            pb->rtb_element->rte_val = val;
        } else {
            pb = CreateRTBagNodeCopyArea ();
            pb->rtb_tag = tag;
            pb->rtb_element->rte_val = val;
            pb->rtb_next = RunTimeCopyBag;
            pb->rtb_prev = (RT_BAG *) NULL;
            RunTimeCopyBag->rtb_prev = pb;
            RunTimeCopyBag = pb;
        }
        return (pb);

}        end AddRTBagNodeCopyArea */

/*
 ****************************************************************
 * Add a tuple node in the run-time copy bag                    *
 ****************************************************************
*/

RT_BAG *
AddRTTupleBagNodeCopyArea (pt, back)
RT_TUPLE *pt;
RT_BAG *back;
{
  RT_TUPLE *CopyTuple ();
  RT_BAG *CreateRTBagNodeCopyArea ();
  RT_BAG *pb;

  pb = CreateRTBagNodeCopyArea ();
  pb->rtb_tag = TY_TUPLE;
  pb->rtb_element->rte_tuple = CopyTuple (pt);
  pb->rtb_next = (RT_BAG *)NULL;
  pb->rtb_prev = back;
  if (back != (RT_BAG *)NULL)
    back->rtb_next = pb;
  return (pb);
} /* end AddRTTupleBagNodeCopyArea */

/* Old version.
RT_BAG *
AddRTTupleBagNodeCopyArea (pt)
RT_TUPLE *pt;
{
	RT_TUPLE *CopyTuple ();
	RT_BAG *CreateRTBagNodeCopyArea ();
        register RT_BAG *pb;

        if (RunTimeCopyBag == (RT_BAG *) NULL)
        {
            RunTimeCopyBag = pb = CreateRTBagNodeCopyArea ();
            pb->rtb_tag = TY_TUPLE;
            pb->rtb_element->rte_tuple = CopyTuple (pt);
        } else {
            pb = CreateRTBagNodeCopyArea ();
            pb->rtb_tag = TY_TUPLE;
            pb->rtb_element->rte_tuple = CopyTuple (pt);
            pb->rtb_next = RunTimeCopyBag;
            pb->rtb_prev = (RT_BAG *) NULL;
            RunTimeCopyBag->rtb_prev = pb;
            RunTimeCopyBag = pb;
        }
        return (pb);

}       end AddRTTupleBagNodeCopyArea */



RT_TUPLE *
CopyTuple (prtt)
RT_TUPLE *prtt;
{
	RT_TUPLE *GenTupleSkeletonCopyArea ();
        register RT_LIST *pt;
        register RT_TUPLE *ptuple;
        register RT_LIST *pl;

        ptuple = GenTupleSkeletonCopyArea (prtt->rtt_size);
	ptuple->rtt_size = prtt->rtt_size;
        pl = ptuple->rtt_list;
        for (pt = prtt->rtt_list; pt != (RT_LIST *) NULL; pt = pt->rtl_next)
        {
                pl->rtl_tag = TY_INT;
                pl->rtl_element = (RT_ELEMENT *) GetArea (sizeof (RT_ELEMENT));
                pl->rtl_element->rte_val = pt->rtl_element->rte_val;
                pl = pl->rtl_next;
        }
        return (ptuple);

}       /* end CopyTuple */




/*
 ****************************************************************
 * Generate a Tuple Skeleton in the Copy Area                   *
 ****************************************************************
 */

RT_TUPLE *
GenTupleSkeletonCopyArea (size)
int size;
{
        register RT_TUPLE *pt;
        register RT_LIST  *pl;
        register int i = size - 1;

        pt = (RT_TUPLE *) GetArea (sizeof (RT_TUPLE));
        pt->rtt_size = size;
        pt->rtt_list = pl = (RT_LIST *) GetArea (sizeof (RT_LIST));
        while (i-- > 0)
            pl = pl->rtl_next = (RT_LIST *) GetArea (sizeof (RT_LIST));
        return (pt);

}       /* end GenTupleSkeletonCopyArea */


/*
 ****************************************************************
 * Determine the size of a run time bag in bytes                *
 ****************************************************************
 */

int
RunTimeBagSize (bag)
RT_BAG *bag;
{
	register RT_BAG *pb;
	register int size = 0;
	register int tag;

	for (pb = bag; pb != (RT_BAG *) NULL; pb = pb->rtb_next)
	{
	    size += sizeof (RT_BAG) + sizeof (RT_ELEMENT);
	    if (pb->rtb_tag == TY_TUPLE)
		size += RunTimeTupleSize (pb->rtb_element->rte_tuple);
	}
	return (size);

}	/* end RunTimeBagSize */

/*
 ****************************************************************
 * Determine the size of a run time tuple in bytes              *
 ****************************************************************
 */

int
RunTimeTupleSize (ptuple)
RT_TUPLE *ptuple;
{
	register RT_LIST *pt = ptuple->rtt_list;	
	register int size = 0;
	register int tag;

	size = sizeof (RT_TUPLE);
	while (pt != (RT_LIST *) NULL)
	{
	    size += sizeof (RT_LIST);
            size += sizeof (RT_ELEMENT);
	    pt = pt->rtl_next;
	}	
	return (size);

}	/* end RunTimeTupleSize */

char *
CopyRunTimeBag (bag, address)
RT_BAG *bag;
int address;
{
	void RearrangePointersInBag ();
	void CopyBag ();
	char *Alloc ();
	int size;
	char *area;

	size = RunTimeBagSize (bag);
	area = Alloc (size);
	SetUpArea (area, size);
        CopyBag (bag);
        RearrangePointersInBag (address-(int)area, (RT_BAG *)area);
	return (area);

}	/* end CopyRunTimeBag */

void
CopyBag (bag)
RT_BAG *bag;
{
  RT_BAG *CopyBagExpression ();
  RT_BAG *back = (RT_BAG *)NULL;
  RT_BAG *pb = bag;

  while (pb != (RT_BAG *)NULL) {
    back = CopyBagExpression(pb->rtb_tag, pb->rtb_element, back);
    pb = pb->rtb_next;
  }
} /* end CopyBag */


RT_BAG *
CopyBagExpression (tag, pe)
int tag;
RT_ELEMENT *pe;
{
  RT_BAG *result;

        switch (tag) {
                case TY_INT:
                case TY_BOOL:
		    result = AddRTBagNodeCopyArea (tag, pe->rte_val);
                    break;
                case TY_TUPLE:
                    result = AddRTTupleBagNodeCopyArea (pe->rte_tuple);
                    break;
                default:
                    fprintf (stderr, "gm: error in CopyBagExpression!!\n");
                    exit (1);
        }
        return (result);

}       /* end CopyBagExpression */



/*
 ****************************************************************
 * Print An Element of the Run Time Bag                         *
 ****************************************************************
 */

void
PrintCopyBagExpression (tag, pe)
int tag;
RT_ELEMENT *pe;
{
	void PrintBagTuple ();

	switch (tag) {
               case TY_INT:
        	    printf ("%d", pe->rte_val);
        	    break;
        	case TY_BOOL:
                    printf ("bool(%d)", pe->rte_val);
        	    break;
        	case TY_TUPLE:
        	    PrintBagTuple (pe->rte_tuple);
        	    break;
		default:
		    printf ("gm: error in PrintBagExpression!!\n");
		    exit (1);
        }
        return; 

}	/* end PrintBagExpression */

/*
 ****************************************************************
 * Print a Tuple                                                *
 ****************************************************************
 */

void
PrintCopyBagTuple (pt)
RT_TUPLE *pt;
{
	register RT_LIST *paux = pt->rtt_list;

	printf ("[ ");
	while (paux != (RT_LIST *) NULL)
	{
	    PrintBagExpression (paux->rtl_tag, paux->rtl_element);
	    paux = paux->rtl_next;
	    if (paux != (RT_LIST *) NULL)
		printf (", ");
	}

	printf (" ]");

}	/* end PrintRTBag */

/*
 ****************************************************************
 * Print a Bag                                                  *
 ****************************************************************
 */

void
PrintCopyBag (bag)
RT_BAG *bag;
{
	register RT_BAG *pb = bag; 

	printf ("{ ");
	while (pb != (RT_BAG *) NULL)
	{
	    PrintCopyBagExpression (pb->rtb_tag, pb->rtb_element);
	    pb = pb->rtb_next;
	    if (pb != (RT_BAG *) NULL)
		printf (", ");
	}
	printf (" }\n");

}	/* end PrintCopyBag */



/*
 ****************************************************************
 * Rearrange the pointers in a bag element                      *
 ****************************************************************
 */

void
RearrangePointersInBagElement (adjustment, bag)
int adjustment;
RT_BAG *bag;
{
        void RearrangePointersInTuple ();
	RT_TUPLE *tuple;

	if (bag->rtb_tag != TY_TUPLE)
	    bag->rtb_element = (RT_ELEMENT *) (adjustment + ((int) bag->rtb_element));
	else {
	    tuple = (RT_TUPLE *) (adjustment + ((int) bag->rtb_element->rte_tuple));
	    bag->rtb_element = (RT_ELEMENT *) (adjustment + ((int) bag->rtb_element));
            bag->rtb_element->rte_tuple = tuple;
	    RearrangePointersInTuple (adjustment, tuple);
        }

}	/* end RearrangePointersInBagElement */

 
/*
 ****************************************************************
 * Rearrange Pointers in a Tuple                                *
 ****************************************************************
 */

void
RearrangePointersInTuple (adjustment, tuple)
int adjustment;
RT_TUPLE *tuple;
{
	register RT_LIST *pl = tuple->rtt_list;
	register RT_LIST *paux;

	while (pl != (RT_LIST *) NULL)
	{
	    paux = pl->rtl_next;
       	    pl->rtl_element = (RT_ELEMENT *) (adjustment + ((int) pl->rtl_element));
            if (pl->rtl_next != (RT_LIST *) NULL)
	        pl->rtl_next = (RT_LIST *) (adjustment + ((int) pl->rtl_next));
	    pl = paux;
	}


}	/* end RearrangePointersInTuple */

/*
 ****************************************************************
 * Rearrange pointers in a Bag                                  *
 ****************************************************************
 */

void
RearrangePointersInBag (adjustment, bag)
int adjustment;
RT_BAG *bag;
{
	RT_BAG *pb = bag;
	RT_BAG *paux;

	while (pb != (RT_BAG *) NULL)
	{
	    RearrangePointersInBagElement (adjustment, pb);
	    paux = pb->rtb_next;
	    if (paux != (RT_BAG *) NULL)
	        pb->rtb_next = (RT_BAG *) (adjustment + ((int) pb->rtb_next));
	    if (pb->rtb_prev != (RT_BAG *) NULL)
	        pb->rtb_prev = (RT_BAG *) (adjustment + ((int) pb->rtb_prev));
	    pb = paux;
	  }

/*
	    if (paux != (RT_BAG *) NULL)
	        pb =  (RT_BAG *) (target + ((int) paux - source));
	    else
		pb = paux;
*/

}	/* end RearrangePointersInBag */
