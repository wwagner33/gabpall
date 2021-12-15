
/*
 ****************************************************************
 *                                                              *
 *      Module: controller_cell.c                               *
 *      Description: the code for the cell which starts things  *
 *                   off                                        *
 *      Version: 1.0 of 25/Aug/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

# include <stdio.h>
# include "/usr/include/mpi/mpi.h" /*"/export/home/LIPN-SF1/users/Associes/gap/MPI/h/mpi.h"*/
# include "../include/const.h"
# include "../include/types.h"
# include "../include/rtbag.h"
# include "../include/fnc.h"
# include "../include/msg.h"
# include "../include/vars.h"

void cell_1 ()
{
  int response, processes, i;
  MPI_Status status;

  MPI_Comm_size(MPI_COMM_WORLD, &processes);

  for (;;) {
    MPI_Send(NULL, 0, MPI_INT, 2, BR_DOIT, MPI_COMM_WORLD);
    MPI_Recv(&response, 1, MPI_INT, 2, BR_RESPONSE, MPI_COMM_WORLD, &status);
    if (response == FALSE)
      break;
  }

  MPI_Send(NULL, 0, MPI_INT, 0, BAG_PRINT, MPI_COMM_WORLD);

  for (i = 2;i < processes; ++i)
    MPI_Send(NULL, 0, MPI_INT, i, DIE, MPI_COMM_WORLD);

} /* end cell_1 */
