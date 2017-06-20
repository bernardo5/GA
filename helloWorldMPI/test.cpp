#include <mpi.h>
#include <iostream>
#include <string>


using namespace std;

int main(){
	MPI_Init(NULL, NULL);
	
	/* Get the number of computers */
	int world_size;
	
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	/*Get the rank of the process*/
	
	int world_rank;
	
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
	
	cout<<"Hello world from processor "+string(processor_name)+", rank "+to_string(world_rank)+" out of "+to_string(world_size)+" number of processes.\n";
	
	MPI_Finalize();
	
	return 0;
}
