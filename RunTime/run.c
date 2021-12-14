# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/time.h>
# include "/usr/local/include/mpi.h"
# include "../include/const.h"
# include "../include/types.h"
# include "../include/rtbag.h"
# include "../include/fnc.h"
# include "../include/msg.h"
# include "../include/vars.h"


void
CreateInitialBag ()
{
	RunTimeBag = AddRTBagNode(TY_INT, 10, RunTimeBag);

}   /* end CreateInitialBag */

extern void cell_0();

extern void cell_1();

void cell_2 ()
{
int response;
int tag;
MPI_Status status;
int false_buffer = FALSE;

for (;;) {
	MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	tag = status.MPI_TAG;
	if (tag == DIE) return;
	for (;;) {
	    MPI_Send(NULL, 0, MPI_INT, 3, BR_DOIT, MPI_COMM_WORLD);
	    MPI_Recv(&response, 1, MPI_INT, 3, BR_RESPONSE, MPI_COMM_WORLD, &status);
	    if (response == TRUE) continue;
	    break;
}
	for (;;) {
	    MPI_Send(NULL, 0, MPI_INT, 6, BR_DOIT, MPI_COMM_WORLD);
	    MPI_Recv(&response, 1, MPI_INT, 6, BR_RESPONSE, MPI_COMM_WORLD, &status);
	    if (response == TRUE) continue;
	    break;
}
	MPI_Send(&false_buffer, 1, MPI_INT, 1, BR_RESPONSE, MPI_COMM_WORLD);
}

}	/* end cell_2 */

void cell_6 ()
{
RT_VALUE *GenValueList ();
register RT_BAG *ptr_0;
register RT_ELEMENT *ptr_1;
register int x_1;
register RT_ELEMENT *ptr_2;
register int x_2;

int result; 
register RT_VALUE *link, *linkhead;
MPI_Status status;
int false_buffer = FALSE;
int tag;

void ReceiveBag();
int RequestBag();
for (;;) {
result = FALSE;
MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
tag = status.MPI_TAG;
if (tag == DIE) return;
for (;!RequestBag(0););
ReceiveBag(0);
for(ptr_0=RunTimeBag;(ptr_0!=(RT_BAG *)NULL) && (result == FALSE);ptr_0=ptr_0->rtb_next) {
if(!NODE_LOCKED(ptr_0->rtb_tag)&&(NODE_TYPE(ptr_0->rtb_tag)==TY_TUPLE)){
if (ptr_0->rtb_element->rte_tuple->rtt_size == 2) {
      LOCK_NODE(ptr_0->rtb_tag);
{
register RT_LIST *pl;
pl = ptr_0->rtb_element->rte_tuple->rtt_list;
x_1 = pl->rtl_element->rte_val;
ptr_1 = pl->rtl_element;
pl = pl->rtl_next;
x_2 = pl->rtl_element->rte_val;
ptr_2 = pl->rtl_element;
pl = pl->rtl_next;
}
}
   } else continue;
    result = cell_6_reaction_fnc (x_1, x_2);
    if (result == TRUE) {
linkhead = link = GenValueList (2);
link->rtv_val = ptr_1;
link->rtv_name =  Alloc (2);
strcpy (link->rtv_name, "n");
link = link->rtv_next;
link->rtv_val = ptr_2;
link->rtv_name =  Alloc (5);
strcpy (link->rtv_name, "__x1");
link = link->rtv_next;
cell_6_action (linkhead);
}
if (result == FALSE)
UNLOCK_NODE(ptr_0->rtb_tag);
}
if (result == FALSE)
UnlockBagNodes ();
MPI_Send(NULL, 0, MPI_INT, 0, BAG_SEND, MPI_COMM_WORLD);
SendBag(0);
MPI_Send(&result, 1, MPI_INT, 2, BR_RESPONSE, MPI_COMM_WORLD);
}

}	/* end cell_6 */

cell_6_reaction_fnc (n, __x1)
{
register int Result = 0;

Result = (1 && (__x1  ==  0 ));
return (Result);

}	/* end cell_6_reaction_fnc */

cell_6_action (link) 
RT_VALUE *link;
{cell_6_action_0 (link); 
RemoveLockedBagNodes ();
FreeListValues (link);

}	/* end cell_6_action */

cell_6_action_0_e (n)
{
register int Result = 0;

Result = n ;
return (Result);

}	/* end cell_6_action_0_e */

cell_6_action_0 (link) 
 RT_VALUE *link; 
{
	register int n = GetValue ("n", link);
register int result = 0;

result = cell_6_action_0_e (n);RunTimeBag = AddRTBagNode (TY_INT, result);

}	/* end cell_6_action_0 */

void cell_3 ()
{
int response;
int tag;
MPI_Status status;
int false_buffer = FALSE;

for (;;) {
	MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	tag = status.MPI_TAG;
	if (tag == DIE) return;
	for (;;) {
	    MPI_Send(NULL, 0, MPI_INT, 4, BR_DOIT, MPI_COMM_WORLD);
	    MPI_Recv(&response, 1, MPI_INT, 4, BR_RESPONSE, MPI_COMM_WORLD, &status);
	    if (response == TRUE) continue;
	    break;
}
	for (;;) {
	    MPI_Send(NULL, 0, MPI_INT, 5, BR_DOIT, MPI_COMM_WORLD);
	    MPI_Recv(&response, 1, MPI_INT, 5, BR_RESPONSE, MPI_COMM_WORLD, &status);
	    if (response == TRUE) continue;
	    break;
}
	MPI_Send(&false_buffer, 1, MPI_INT, 2, BR_RESPONSE, MPI_COMM_WORLD);
}

}	/* end cell_3 */

void cell_5 ()
{
RT_VALUE *GenValueList ();
register RT_BAG *ptr_0;
register RT_ELEMENT *ptr_1;
register int x_1;
register RT_ELEMENT *ptr_2;
register int x_2;

int result; 
register RT_VALUE *link, *linkhead;
MPI_Status status;
int false_buffer = FALSE;
int tag;

void ReceiveBag();
int RequestBag();
for (;;) {
result = FALSE;
MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
tag = status.MPI_TAG;
if (tag == DIE) return;
for (;!RequestBag(0););
ReceiveBag(0);
for(ptr_0=RunTimeBag;(ptr_0!=(RT_BAG *)NULL) && (result == FALSE);ptr_0=ptr_0->rtb_next) {
if(!NODE_LOCKED(ptr_0->rtb_tag)&&(NODE_TYPE(ptr_0->rtb_tag)==TY_TUPLE)){
if (ptr_0->rtb_element->rte_tuple->rtt_size == 2) {
      LOCK_NODE(ptr_0->rtb_tag);
{
register RT_LIST *pl;
pl = ptr_0->rtb_element->rte_tuple->rtt_list;
x_1 = pl->rtl_element->rte_val;
ptr_1 = pl->rtl_element;
pl = pl->rtl_next;
x_2 = pl->rtl_element->rte_val;
ptr_2 = pl->rtl_element;
pl = pl->rtl_next;
}
}
   } else continue;
    result = cell_5_reaction_fnc (x_1, x_2);
    if (result == TRUE) {
linkhead = link = GenValueList (2);
link->rtv_val = ptr_1;
link->rtv_name =  Alloc (2);
strcpy (link->rtv_name, "n");
link = link->rtv_next;
link->rtv_val = ptr_2;
link->rtv_name =  Alloc (5);
strcpy (link->rtv_name, "__x0");
link = link->rtv_next;
cell_5_action (linkhead);
}
if (result == FALSE)
UNLOCK_NODE(ptr_0->rtb_tag);
}
if (result == FALSE)
UnlockBagNodes ();
MPI_Send(NULL, 0, MPI_INT, 0, BAG_SEND, MPI_COMM_WORLD);
SendBag(0);
MPI_Send(&result, 1, MPI_INT, 3, BR_RESPONSE, MPI_COMM_WORLD);
}

}	/* end cell_5 */

cell_5_reaction_fnc (n, __x0)
{
register int Result = 0;

Result = ((n  >  1 ) && (__x0  ==  1 ));
return (Result);

}	/* end cell_5_reaction_fnc */

cell_5_action (link) 
RT_VALUE *link;
{cell_5_action_0 (link); 
cell_5_action_1 (link); 
RemoveLockedBagNodes ();
FreeListValues (link);

}	/* end cell_5_action */

cell_5_action_0 (ln) 
RT_VALUE *ln;
{	register int result;
	register RT_VALUE *linkhead, *link;

result = cell_5_action_0_tuple_action_0 (ln); 
linkhead = link = (RT_VALUE *) Alloc (sizeof (RT_VALUE));
link->rtv_val = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));
link->rtv_val->rte_val = result;
link->rtv_name = (char *) NULL;link->rtv_next = (RT_VALUE *) NULL;
result = cell_5_action_0_tuple_action_1 (ln); 
link = link->rtv_next = (RT_VALUE *) Alloc (sizeof (RT_VALUE));
link->rtv_val = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));
link->rtv_val->rte_val = result;
link->rtv_name = (char *) NULL;link->rtv_next = (RT_VALUE *) NULL;
GenRTTuple (linkhead);
RemoveLockedBagNodes ();
FreeListValues (linkhead);

}   /* end cell_5_action_0_tuple_action */

cell_5_action_0_tuple_action_0_e (n)
{
register int Result = 0;

Result = n ;
return (Result);

}	/* end cell_5_action_0_tuple_action_0_e */

cell_5_action_0_tuple_action_0 (link) 
 RT_VALUE *link; 
{
	register int n = GetValue ("n", link);
register int result = 0;

result = cell_5_action_0_tuple_action_0_e (n);return (result);

}   /* end cell_5_action_0_tuple_action_0 */

cell_5_action_0_tuple_action_1_e ()
{
register int Result = 0;

Result = 0;
return (Result);

}	/* end cell_5_action_0_tuple_action_1_e */

cell_5_action_0_tuple_action_1 (link) 
 RT_VALUE *link; 
{
register int result = 0;

result = cell_5_action_0_tuple_action_1_e ();return (result);

}   /* end cell_5_action_0_tuple_action_1 */

cell_5_action_1 (ln) 
RT_VALUE *ln;
{	register int result;
	register RT_VALUE *linkhead, *link;

result = cell_5_action_1_tuple_action_0 (ln); 
linkhead = link = (RT_VALUE *) Alloc (sizeof (RT_VALUE));
link->rtv_val = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));
link->rtv_val->rte_val = result;
link->rtv_name = (char *) NULL;link->rtv_next = (RT_VALUE *) NULL;
result = cell_5_action_1_tuple_action_1 (ln); 
link = link->rtv_next = (RT_VALUE *) Alloc (sizeof (RT_VALUE));
link->rtv_val = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));
link->rtv_val->rte_val = result;
link->rtv_name = (char *) NULL;link->rtv_next = (RT_VALUE *) NULL;
GenRTTuple (linkhead);
RemoveLockedBagNodes ();
FreeListValues (linkhead);

}   /* end cell_5_action_1_tuple_action */

cell_5_action_1_tuple_action_0_e (n)
{
register int Result = 0;

Result = (n  -  1 );
return (Result);

}	/* end cell_5_action_1_tuple_action_0_e */

cell_5_action_1_tuple_action_0 (link) 
 RT_VALUE *link; 
{
	register int n = GetValue ("n", link);
register int result = 0;

result = cell_5_action_1_tuple_action_0_e (n);return (result);

}   /* end cell_5_action_1_tuple_action_0 */

cell_5_action_1_tuple_action_1_e ()
{
register int Result = 0;

Result = 1;
return (Result);

}	/* end cell_5_action_1_tuple_action_1_e */

cell_5_action_1_tuple_action_1 (link) 
 RT_VALUE *link; 
{
register int result = 0;

result = cell_5_action_1_tuple_action_1_e ();return (result);

}   /* end cell_5_action_1_tuple_action_1 */

void cell_4 ()
{
RT_VALUE *GenValueList ();
register RT_BAG *ptr_0;
register int x_0;

int result; 
register RT_VALUE *link, *linkhead;
MPI_Status status;
int false_buffer = FALSE;
int tag;

void ReceiveBag();
int RequestBag();
for (;;) {
result = FALSE;
MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
tag = status.MPI_TAG;
if (tag == DIE) return;
for (;!RequestBag(0););
ReceiveBag(0);
for (ptr_0 = RunTimeBag; (ptr_0 != (RT_BAG *) NULL) && (result == FALSE); ptr_0 = ptr_0->rtb_next) {
   if (!NODE_LOCKED(ptr_0->rtb_tag) && (NODE_TYPE(ptr_0->rtb_tag) == TY_INT)) {
      LOCK_NODE(ptr_0->rtb_tag);
      x_0 = ptr_0->rtb_element->rte_val;
  }
   else continue;
    result = cell_4_reaction_fnc (x_0);
    if (result == TRUE) {
linkhead = link = GenValueList (1);
link->rtv_val = ptr_0->rtb_element;
link->rtv_name =  Alloc (2);
strcpy (link->rtv_name, "n");
link = link->rtv_next;
cell_4_action (linkhead);
}
if (result == FALSE)
UNLOCK_NODE(ptr_0->rtb_tag);
}
if (result == FALSE)
UnlockBagNodes ();
MPI_Send(NULL, 0, MPI_INT, 0, BAG_SEND, MPI_COMM_WORLD);
SendBag(0);
MPI_Send(&result, 1, MPI_INT, 3, BR_RESPONSE, MPI_COMM_WORLD);
}

}	/* end cell_4 */

cell_4_reaction_fnc (n)
{
register int Result = 0;

Result = 1;
return (Result);

}	/* end cell_4_reaction_fnc */

cell_4_action (link) 
RT_VALUE *link;
{cell_4_action_0 (link); 
RemoveLockedBagNodes ();
FreeListValues (link);

}	/* end cell_4_action */

cell_4_action_0 (ln) 
RT_VALUE *ln;
{	register int result;
	register RT_VALUE *linkhead, *link;

result = cell_4_action_0_tuple_action_0 (ln); 
linkhead = link = (RT_VALUE *) Alloc (sizeof (RT_VALUE));
link->rtv_val = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));
link->rtv_val->rte_val = result;
link->rtv_name = (char *) NULL;link->rtv_next = (RT_VALUE *) NULL;
result = cell_4_action_0_tuple_action_1 (ln); 
link = link->rtv_next = (RT_VALUE *) Alloc (sizeof (RT_VALUE));
link->rtv_val = (RT_ELEMENT *) Alloc (sizeof (RT_ELEMENT));
link->rtv_val->rte_val = result;
link->rtv_name = (char *) NULL;link->rtv_next = (RT_VALUE *) NULL;
GenRTTuple (linkhead);
RemoveLockedBagNodes ();
FreeListValues (linkhead);

}   /* end cell_4_action_0_tuple_action */

cell_4_action_0_tuple_action_0_e (n)
{
register int Result = 0;

Result = n ;
return (Result);

}	/* end cell_4_action_0_tuple_action_0_e */

cell_4_action_0_tuple_action_0 (link) 
 RT_VALUE *link; 
{
	register int n = GetValue ("n", link);
register int result = 0;

result = cell_4_action_0_tuple_action_0_e (n);return (result);

}   /* end cell_4_action_0_tuple_action_0 */

cell_4_action_0_tuple_action_1_e ()
{
register int Result = 0;

Result = 1;
return (Result);

}	/* end cell_4_action_0_tuple_action_1_e */

cell_4_action_0_tuple_action_1 (link) 
 RT_VALUE *link; 
{
register int result = 0;

result = cell_4_action_0_tuple_action_1_e ();return (result);

}   /* end cell_4_action_0_tuple_action_1 */

void main(argc, argv)
int argc;
char *argv[];
{
void (*c[7])();
int mynumber, ierr;

c[0] = cell_0;
c[1] = cell_1;
c[2] = cell_2;
c[3] = cell_3;
c[4] = cell_4;
c[5] = cell_5;
c[6] = cell_6;

ierr = MPI_Init(&argc, &argv);
if (ierr != MPI_SUCCESS) {
  fprintf(stderr, "MPI initialisation error",0x0A);
  exit(1);
}
MPI_Comm_rank(MPI_COMM_WORLD, &mynumber);
(c[mynumber])();
MPI_Finalize();
} /* end main */
