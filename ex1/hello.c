/* Hello World! */

#include <string.h>
#include <stdio.h>
#include <mpi.h>

int main (int argc, char ** argv)
{
	int numtasks;       
	int rank;
	int version, subversion;
	char hello[35];

	MPI_Init ( &argc, &argv );                   
	MPI_Comm_size ( MPI_COMM_WORLD, &numtasks );  
	MPI_Comm_rank ( MPI_COMM_WORLD, &rank );
	
	if (rank == 0){
		MPI_Get_version(&version, &subversion);
    	printf("Process %d : MPI Version: %d.%d\n", rank, version, subversion);     

		char processor_name[MPI_MAX_PROCESSOR_NAME];
		int name_len;
		MPI_Get_processor_name(processor_name, &name_len);

		sprintf(hello, "Hello world! from %s", processor_name);
	}
	
	MPI_Bcast(&hello, 35, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	if(rank != 0){		
		printf("Process %d : %s\n", rank, hello);
	}

	MPI_Finalize();                              

	return 0;
}

