#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char *argv[])
{
  int num, rank, rc, count, a, b;
  MPI_Status stat;

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &num);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  if (num != 2){
	if (rank == 0){
	  printf("ERROR: Only works with 2 processes!");
	}
    
	MPI_Finalize();
	return 0;
  }

  printf("Process %d starting...\n",rank);

  if (rank == 0) 
  {
    a  = 10;
    rc = MPI_Send(&a, 1, MPI_INT, rank+1, rank, MPI_COMM_WORLD);
    rc = MPI_Recv(&b, 1, MPI_INT, rank+1, rank, MPI_COMM_WORLD, &stat);
  }
  else if (rank == 1) 
  {
    a  = 10;
    rc = MPI_Recv(&b, 1, MPI_INT, rank-1, rank-1, MPI_COMM_WORLD, &stat);
    rc = MPI_Send(&a, 1, MPI_INT, rank-1, rank-1, MPI_COMM_WORLD);
  }

  rc = MPI_Get_count(&stat, MPI_INT, &count);
  printf("Process %d: Received %d elements with %d value from process %d\n",
         rank, count, b, stat.MPI_SOURCE);

  MPI_Finalize();
  return 0;
}

