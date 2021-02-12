/* Broadcast */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

#define SIZE 5
#define MAXSTR 500

/* Broadcast using MPI_send in a ring */
void Bcast_alt (void* data, int count, MPI_Datatype datatype, MPI_Comm comm, double *time)
{
  	int rank, num;
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &num);


	if (rank == 0){
		MPI_Send(data, count, datatype, rank+1, 0, comm);
	}else if (rank == num-1){
		MPI_Recv(data, count, datatype, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}else{
		MPI_Recv(data, count, datatype, rank-1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
   		MPI_Send(data, count, datatype, rank+1, 0, MPI_COMM_WORLD);
	}
}


/* Broadcast using MPI_Bcast*/
void Bcast (void* data, int count, MPI_Datatype datatype, MPI_Comm comm, double *time)
{
	int rank, num;
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &num);

  	MPI_Bcast(data, SIZE, datatype, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
}


/* Main program */
int main (int argc, char *argv[])
{
  int    i, rank, num;
  int    data[SIZE];
  char   msg[MAXSTR];
  double tBcast, tSend;

  /* MPI Initialization */
  MPI_Init (NULL, NULL);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &num);

  /* Check num processes and Initialize structure */
  if (rank == 0)
  {
      if ((num < 2) || (num>16))
      {
         printf("Minim number of processes should be greater than 1 and less than 16\n");
         MPI_Abort(MPI_COMM_WORLD, -1);
      }
      srand(time(NULL));
      for (i=0; i<SIZE; i++)
          data[i] = rand()%100;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  double tBcast1 = MPI_Wtime();

  /* broadcasting n with MPI_BCast */
  Bcast (data, SIZE, MPI_INT, MPI_COMM_WORLD, &tBcast);

  //MPI_Bcast(data, SIZE, MPI_INT, 0, MPI_COMM_WORLD);
  //MPI_Barrier(MPI_COMM_WORLD);

  double tBcast2 = MPI_Wtime();
  tBcast = tBcast2 - tBcast1;

  

  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  sprintf (msg,"Process %d: Bcast.     data= ", rank);
  for (i=0; i<SIZE; i++)
      sprintf(msg,"%s %d ",msg, data[i]);
  printf("%s\n", msg);
  fflush (stdout);

  /* clean data */
  if (rank != 0)
  {
     for (i=0; i<SIZE; i++)
        data[i] = 0;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  double tSend1 = MPI_Wtime();

  /* broadcasting using sends in a ring */
  Bcast_alt (data, SIZE, MPI_INT, MPI_COMM_WORLD, &tSend);

  MPI_Barrier(MPI_COMM_WORLD);
  double tSend2 = MPI_Wtime();
  tSend = tSend2 - tSend1;

  sprintf (msg,"Process %d: Bcast_alt. data= ", rank);
  for (i=0; i<SIZE; i++)
      sprintf(msg,"%s %d ",msg, data[i]);
  printf("%s\n",msg);
  fflush (stdout);

  MPI_Barrier(MPI_COMM_WORLD);

  /* show results */
  if (rank == 0)
  {
     printf ("\nBcast time = %lf\n", tBcast );
     printf ("Bcast_alt time  = %lf\n", tSend );
     fflush(stdout);
  }

  /* End MPI */
  MPI_Finalize ();
  return 0;
}
