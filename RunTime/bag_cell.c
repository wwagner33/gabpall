
/*
 ****************************************************************
 *                                                              *
 *      Module: bag_cell.c                                      *
 *      Description: the code for the bag cell                  *
 *      Version: 1.0 of 25/Aug/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

# include <stdio.h>
# include "/usr/local/include/mpi.h" /*"/export/home/LIPN-SF1/users/Associes/gap/MPI/h/mpi.h"*/
# include "../include/const.h"
# include "../include/types.h"
# include "../include/rtbag.h"
# include "../include/fnc.h"
# include "../include/msg.h"
# include "../include/vars.h"
# define DEBUG

void cell_0 ()
{
  int bag_free;
  MPI_Status status;
  int true_buffer = TRUE, false_buffer = FALSE;
  int cell, tag;
  int dummy = 0;

  void ReceiveBag();
  void SendBag();

# ifdef DEBUG
  printf ("bag_cell: started.\n");
# endif

  CreateInitialBag();

# ifdef DEBUG
PrintRTBag ();
printf ("\n");
#endif

  bag_free = TRUE;

  for (;;) {
    MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    cell = status.MPI_SOURCE;
    tag = status.MPI_TAG;
# ifdef DEBUG
    printf ("bag_cell: message received from cell %d\n", cell);
# endif
    switch (tag) {

    case BAG_SEND:
      ReceiveBag(cell);
# ifdef DEBUG
      printf ("bag_cell: bag received from cell %d\n", cell);
      PrintRTBag ();
# endif
      bag_free = TRUE;
      break;
    
    case BAG_REQUEST:
# ifdef DEBUG
      printf ("bag_cell: bag requested by cell %d\n", cell);
# endif
      if (bag_free) {
	bag_free = FALSE;
	MPI_Send(&true_buffer, 1, MPI_INT, cell, BAG_RESPONSE, MPI_COMM_WORLD);
	SendBag(cell);
# ifdef DEBUG
	printf ("bag_cell: bag sent to cell %d\n", cell);
# endif
      }
      else 
	MPI_Send(&false_buffer, 1, MPI_INT, cell, BAG_RESPONSE, MPI_COMM_WORLD);
      break;

    case BAG_PRINT:
      if (bag_free) {
	PrintRTBag();
	return;
      }
      break;
    }
  }
} /* end cell_0 */
