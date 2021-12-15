
/*
 ****************************************************************
 *                                                              *
 *      Module: bagcomm.c                                       *
 *      Description: Sending and receiving bags                 *
 *      Version: 1.0 of 23/Aug/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

# include <stdio.h>
# include "/usr/include/mpi/mpi.h"   /*"/export/home/LIPN-SF1/users/Associes/gap/MPI/h/mpi.h"*/
# include "../include/const.h"
# include "../include/types.h"
# include "../include/rtbag.h"
# include "../include/fnc.h"
# include "../include/msg.h"
# include "../include/vars.h"

/*
 ****************************************************************
 * Request access to the bag                                    *
 ****************************************************************
 */

int
RequestBag(bagcell)
int bagcell;
{
  int response;
  MPI_Status status;

  MPI_Send(NULL, 0, MPI_INT, bagcell, BAG_REQUEST, MPI_COMM_WORLD);
  MPI_Recv(&response, 1, MPI_INT, bagcell, BAG_RESPONSE, MPI_COMM_WORLD, &status);  
  return (response);

} /* RequestBag */

RT_TUPLE *ReceiveBagTuple();

RT_ELEMENT *
ReceiveBagElement(tag, cell)
int *tag;
int cell;
{
  int data, mpitag;
  MPI_Status status;
  RT_ELEMENT *element;

  MPI_Recv(&data, 1, MPI_INT, cell, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  mpitag = status.MPI_TAG;
  element = (RT_ELEMENT *)Alloc(sizeof(RT_ELEMENT));
  switch (mpitag) {
  case BAG_INT:
    element->rte_val = data;
    *tag = TY_INT;
    break;
  case BAG_BOOL:
    element->rte_val = data;
    *tag = TY_BOOL;
    break;
  case BAG_TUPLE_SIZE:
    element->rte_tuple = ReceiveBagTuple(data, cell);
    *tag = TY_TUPLE;
    break;
  }
  return(element);
} /* end ReceiveBagElement */


RT_TUPLE *
ReceiveBagTuple(size, cell)
int size, cell;
{
  RT_TUPLE *tuple;
  RT_LIST *list, *last, *next;
  int i;

  list = (RT_LIST *)Alloc(sizeof(RT_LIST));
  last = list;
  for (i = 0; i < size; ++i) {
    next = (RT_LIST *)Alloc(sizeof(RT_LIST));
    next->rtl_element = ReceiveBagElement(&(next->rtl_tag), cell);
    next->rtl_next = (RT_LIST *)NULL;
    last->rtl_next = next;
    last = next;
  }
  tuple = (RT_TUPLE *)Alloc(sizeof(RT_TUPLE));
  tuple->rtt_size = size;
  tuple-> rtt_list = list->rtl_next;
  Free(list);
  return(tuple);
} /* end ReceiveBagTuple */

/*
 ****************************************************************
 * Receive a bag                                                *
 ****************************************************************
 */

void
ReceiveBag(cell)
int cell;
{
  MPI_Status status;
  int data, tag;
  RT_TUPLE *tuple;

  DeleteBag (); 

  RunTimeBag = (RT_BAG *)NULL;

  for (;;) {
    MPI_Recv(&data, 1, MPI_INT, cell, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    tag = status.MPI_TAG;
    if (tag == BAG_END) 
      break;
    switch (tag) {
    case BAG_INT:
      RunTimeBag = AddRTBagNode (TY_INT, data);
      break;
    case BAG_BOOL:
      RunTimeBag = AddRTBagNode (TY_BOOL, data);
      break;
    case BAG_TUPLE_SIZE:
      tuple = ReceiveBagTuple(data, cell);
      RunTimeBag = AddRTTupleBagNode(tuple);
      break;
    }
  }
} /* ReceiveBag */

void
SendBagInteger(val, cell)
int val, cell;
{
  MPI_Send(&val, 1, MPI_INT, cell, BAG_INT, MPI_COMM_WORLD);
} /* end SendBagInteger */

void
SendBagBoolean(val, cell)
int val, cell;
{
  MPI_Send(&val, 1, MPI_INT, cell, BAG_BOOL, MPI_COMM_WORLD);
} /* end SendBagBoolean */

void SendBagTuple();

void
SendBagElement(element, tag, cell)
RT_ELEMENT *element;
int tag, cell;
{
  switch (tag) {
  case TY_INT:
    SendBagInteger(element->rte_val, cell);
    break;
  case TY_BOOL:
    SendBagBoolean(element->rte_val, cell);
    break;
  case TY_TUPLE:
    SendBagTuple(element->rte_tuple, cell);
    break;
  }
} /* end SendBagElement */


void
SendBagTuple(tuple, cell)
RT_TUPLE *tuple;
int cell;
{
  int size = tuple->rtt_size;
  RT_LIST *list = tuple->rtt_list;
  int i;

  MPI_Send(&size, 1, MPI_INT, cell, BAG_TUPLE_SIZE, MPI_COMM_WORLD);
  for (i = 0; i < size; ++i) {
    SendBagElement(list->rtl_element, list->rtl_tag, cell);
    list = list->rtl_next;
  } 
} /* end SendBagTuple */



/*
 ****************************************************************
 * Send a bag                                                   *
 ****************************************************************
 */

void
SendBag(cell)
int cell;
{
  RT_BAG *pb = RunTimeBag;
  int dummy = 0;

  while (pb != (RT_BAG *)NULL) {
    SendBagElement(pb->rtb_element, pb->rtb_tag, cell);
    pb = pb->rtb_next;
  }
  MPI_Send(&dummy, 1, MPI_INT, cell, BAG_END, MPI_COMM_WORLD);
} /* end SendBag */ 
