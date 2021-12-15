/*
 ****************************************************************
 *                                                              *
 *      Module: rtbag.c                                         *
 *      Description: Run Time Bag Manipulating Routines         *
 *      Version: 1.0 of 06/Jun/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

# include <stdio.h>
# include "../include/types.h"
# include "../include/rtbag.h"
# include "../include/fnc.h"
# include "../include/vars.h"


/*
 ****************************************************************
 * Delete an Element From a Run-time Bag                        *
 ****************************************************************
 */

void
DeleteRTBagNode (pb)
RT_BAG *pb;
{
	/*
	 * If there's only one element in the bag
	 */
	if (pb->rtb_next == (RT_BAG *) NULL && pb->rtb_prev == (RT_BAG *) NULL)
	{
	    Free (RunTimeBag);
	    RunTimeBag = (RT_BAG *) NULL;
	    return;
	}
	if (pb->rtb_prev != (RT_BAG *) NULL) 
	    pb->rtb_prev->rtb_next = pb->rtb_next;
	else
	    RunTimeBag = pb->rtb_next;      /* we're deleting the first element */
	if (pb->rtb_next != (RT_BAG *) NULL)
	    pb->rtb_next->rtb_prev = pb->rtb_prev;
	Free (pb);
	return;

}	/* end DeleteRTBagNode */

/*
 ****************************************************************
 * Creat a Run-time Bag Node                                    *
 ****************************************************************
 */

RT_BAG *
CreateRTBagNode ()
{
	register RT_BAG *pb;

	pb = (RT_BAG *) Alloc (sizeof (RT_BAG));
	pb->rtb_element = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));
	pb->rtb_next = pb->rtb_prev = (RT_BAG *) NULL;
	return (pb);

}	/* end CreateRTBagNode */

/*
 ****************************************************************
 * Add an Integer or Boolean to Run Time Bag                    *
 ****************************************************************
 */

RT_BAG *
AddRTBagNode (tag, val)
int tag;
int val;
{
	register RT_BAG *pb;

	if (RunTimeBag == (RT_BAG *) NULL)
	{
	    RunTimeBag = pb = CreateRTBagNode ();
            pb->rtb_tag = tag;
            pb->rtb_element->rte_val = val;
	} else {
	    pb = CreateRTBagNode ();
	    pb->rtb_tag = tag;
	    pb->rtb_element->rte_val = val;
	    pb->rtb_next = RunTimeBag;
	    pb->rtb_prev = (RT_BAG *) NULL;
	    RunTimeBag->rtb_prev = pb;
	    RunTimeBag = pb;
	}
	return (pb);         /* return pointer to created node */

}	/* end AddRTBagNode */

/*
 ****************************************************************
 * Add a tuple node in the run-time bag                         *
 ****************************************************************
*/

RT_BAG *
AddRTTupleBagNode (pt)
RT_TUPLE *pt;
{
	register RT_BAG *pb;

        if (RunTimeBag == (RT_BAG *) NULL)
        {
            RunTimeBag = pb = CreateRTBagNode ();
            pb->rtb_tag = TY_TUPLE;
	    pb->rtb_element->rte_tuple = pt;
        } else {
	    pb = CreateRTBagNode ();
	    pb->rtb_tag = TY_TUPLE;
	    pb->rtb_element->rte_tuple = pt;
	    pb->rtb_next = RunTimeBag;
	    pb->rtb_prev = (RT_BAG *) NULL;
	    RunTimeBag->rtb_prev = pb;
	    RunTimeBag = pb;
        }
        return (pb);         /* return pointer to created node */

}	/* end AddRTTupleBagNode */


/*
 ****************************************************************
 * Search a Bag for any Integer Value                           *
 * Note: If found, the node is locked 				*
 ****************************************************************
*/
	
RT_BAG *
SearchRTBagInteger ()
{
	register RT_BAG *pb = RunTimeBag;
	
	if (RunTimeBag == (RT_BAG *) NULL)
	    return ((RT_BAG *) NULL);
	do {
	    if (!NODE_LOCKED(pb->rtb_tag) && (NODE_TYPE(pb->rtb_tag) == TY_INT))
	    {
		    LOCK_NODE(pb->rtb_tag);
		    return (pb);
	    }
	    pb = pb->rtb_next;
	} while (pb != (RT_BAG *) NULL);
	return ((RT_BAG *) NULL);

}	/* end SearchRTBagValue */

/*
 ****************************************************************      
 * Remove All Locked Bag Nodes                                  *      
 ****************************************************************      
*/
	
void
RemoveLockedBagNodes ()	
{

	register RT_BAG *pb = RunTimeBag;

        if (RunTimeBag == (RT_BAG *) NULL)
            return;
	do {
	    if (NODE_LOCKED(pb->rtb_tag))
	    {
		    DeleteRTBagNode (pb);
		    pb = RunTimeBag;
	    } else
		pb = pb->rtb_next;
        } while (pb != (RT_BAG *) NULL);    
	return;

}	/* end RemoveLockedBagNodes */

/*
 ****************************************************************
 * Delete a bag                                                 *
 ****************************************************************
*/

void
DeleteBag ()
{
  RT_BAG *pb = RunTimeBag;

  while (pb != (RT_BAG *)NULL) {
    DeleteRTBagNode (pb);
    pb = RunTimeBag;
  }
  RunTimeBag = (RT_BAG *)NULL;
} /* end DeleteBag */

/*
 ****************************************************************
 * Unlock All Locked Bag Nodes                                  *
 ****************************************************************
*/

void
UnlockBagNodes ()
{
	register RT_BAG *pb = RunTimeBag;

	if (RunTimeBag == (RT_BAG *) NULL)
	    return;
	do {
	    if (NODE_LOCKED(pb->rtb_tag))
		UNLOCK_NODE(pb->rtb_tag);
		pb = pb->rtb_next;
        } while (pb != (RT_BAG *) NULL);
        return;

}       /* end UnlockBagNodes */

/*
 ****************************************************************
 * Search for a given Tuple in the Run Time Bag                 *
 * Note: If found, the node is locked                           *
 ****************************************************************
*/


RT_BAG *
SearchTuple (size)
int size;
{
	register RT_BAG *pb = RunTimeBag;

	if (pb == (RT_BAG *) NULL)
	    return ((RT_BAG *) NULL);
	do {
	    if (!NODE_LOCKED(pb->rtb_tag) && (NODE_TYPE(pb->rtb_tag) == TY_TUPLE))
  	    	if (pb->rtb_element->rte_tuple->rtt_size == size)
		{
		    LOCK_NODE(pb->rtb_tag);
	            return (pb);
		}
	    pb = pb->rtb_next;
	} while (pb != (RT_BAG *) NULL);
	return ((RT_BAG *) NULL);

}	/* end SearchTuple */


/*
 ****************************************************************
 * See If Two Tuples Match                                      *
 ****************************************************************
*/

int
TuplesMatch (tuple, rt_tuple)
RT_TUPLE *tuple;                   /* that's the "pattern" tuple */
RT_TUPLE *rt_tuple;                /* that's the tuple in the r.t. bag */
{
	register RT_LIST *pt1, *pt2;
	register short tag1, tag2;

	if (tuple->rtt_size != rt_tuple->rtt_size)
	    return (0);
	pt1 = tuple->rtt_list;
	pt2 = rt_tuple->rtt_list;
	while (pt1 != (RT_LIST *) NULL)
	{
	    tag1 = NODE_TYPE(pt1->rtl_tag);
	    tag2 = NODE_TYPE(pt2->rtl_tag);
	    if (tag1 != TY_IDENT && tag1 != tag2)
		return (0);
	    if (tag1 == TY_INT || tag1 == TY_BOOL) {
	        if (pt1->rtl_element->rte_val != pt2->rtl_element->rte_val)
	            return (0);
	    } else if (tag1 == TY_TUPLE) {
		    if (!TuplesMatch (pt1->rtl_element->rte_tuple,
		  		      pt2->rtl_element->rte_tuple))
		        return (0);
	    } else 
		; /* it has to be the case that tag1 == TY_IDENT, so they match */
	    pt1 = pt1->rtl_next;
	    pt2 = pt2->rtl_next;
	}
	return (1);

}	/* end TuplesMatch */



/*
 ****************************************************************
 * Generate a Tuple Skeleton                                    *
 ****************************************************************
 */

RT_TUPLE *
GenTupleSkeleton (size)
int size;
{
        register RT_TUPLE *pt;
        register RT_LIST  *pl;
        register int i = size - 1;

        pt = (RT_TUPLE *) Alloc (sizeof (RT_TUPLE));
        pt->rtt_size = size;
        pt->rtt_list = pl = (RT_LIST *) Alloc (sizeof (RT_LIST));
        while (i-- > 0)
            pl = pl->rtl_next = (RT_LIST *) Alloc (sizeof (RT_LIST));
        return (pt);

}       /* end GenTupleSkeleton */

/*
 ****************************************************************
 * Get a Value from a List RT_VALUE                             *
 ****************************************************************
 */

GetValue (name, link)
char *name;
RT_VALUE *link;
{
	register RT_VALUE *pl;

	for (pl = link; pl != (RT_VALUE *) NULL; pl = pl->rtv_next)
	    if (pl->rtv_name != (char *) NULL)
	        if (strcmp (pl->rtv_name, name) == 0)
		    return (pl->rtv_val->rte_val);  
printf ("gm: error in GetValue!!\n");
exit (1);
	
}	/* end GetValue */

/*
 ****************************************************************
 * Free the space of a list of values                           *
 ****************************************************************
 */

FreeListValues (link)
RT_VALUE *link;
{
        register RT_VALUE *pl, *paux;

        for (pl = link; pl != (RT_VALUE *) NULL; pl = paux)
	{
	    paux = pl->rtv_next;
	    if (pl->rtv_name != (char *) NULL)
	        Free (pl->rtv_name);
	    Free (pl);
	}

}       /* end GetValue */



/*
 ****************************************************************
 * Print An Element of the Run Time Bag                         *
 ****************************************************************
 */

void
PrintBagExpression (tag, pe)
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
PrintBagTuple (pt)
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
PrintRTBag ()
{
	register RT_BAG *pb = RunTimeBag;

	printf ("{ ");
	while (pb != (RT_BAG *) NULL)
	{
	    PrintBagExpression (pb->rtb_tag, pb->rtb_element);
	    pb = pb->rtb_next;
	    if (pb != (RT_BAG *) NULL)
		printf (", ");
	}
	printf (" }\n");

}	/* end PrintRunTimeBag */

/*
 ****************************************************************
 * Generate a Run Time Tuple given a list of values             *
 ****************************************************************
 */

void
GenRTTuple (pv)
RT_VALUE *pv;
{
	register RT_VALUE *pt;
	register RT_TUPLE *ptuple;
        register int size;
        register int tag;
        register RT_LIST *pl;

        size = GetListValueSize (pv);
        ptuple = GenTupleSkeleton (size);
        pl = ptuple->rtt_list;
        for (pt = pv; pt != (RT_VALUE *) NULL; pt = pt->rtv_next)
        {
                pl->rtl_tag = TY_INT;
                pl->rtl_element = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));
                pl->rtl_element->rte_val = pt->rtv_val->rte_val;
                pl = pl->rtl_next;
        }
        RunTimeBag = AddRTTupleBagNode (ptuple);
        return;

}       /* end GenRTTuple */

GetListValueSize (pl)
RT_VALUE *pl;
{
	register int i = 0;

	while (pl != (RT_VALUE *) NULL)
	{
		i++;
		pl = pl->rtv_next;
	}
	return (i);

}	/* end GetListValueSize */

RT_VALUE *
GenValueList (size)
int size;
{
	register RT_VALUE *linkhead = (RT_VALUE *) NULL, *link;
	register int i;

	for (i = 0; i < size; i++)
	{
	    if (linkhead == (RT_VALUE *) NULL) {
		linkhead = link = (RT_VALUE *) Alloc (sizeof (RT_VALUE));
		link->rtv_next = (RT_VALUE *) NULL;
	    } else {
		link = link->rtv_next = (RT_VALUE *) Alloc (sizeof (RT_VALUE));
		link->rtv_next = (RT_VALUE *) NULL;
	    }
	}
	return (linkhead);

}	/* end GenValueList */
