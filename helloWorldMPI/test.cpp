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
	
	//0 will be master flag
	//1 will be rank 1 slave flag
	//2 will be rank 2 slave flag
	int number, number1;
	if(world_rank==0){
		cout<<"Master talking here...\n";
		cout<<"Hello world from processor "+string(processor_name)+", rank "+to_string(world_rank)+" out of "+to_string(world_size)+" number of processes.\n";
		cout<<"\n\n";
		number = 0;
		MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}else{
		if(world_rank==1){
			MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
			printf("Slave 1 received number %d from master\n",
				   number);
			
			cout<<"Slave talking here...\n";
			cout<<"Hello world from processor "+string(processor_name)+", rank "+to_string(world_rank)+" out of "+to_string(world_size)+" number of processes.\n";
			cout<<"\n\n";
			number1=1;
			MPI_Send(&number1, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
			
		}else{
			//MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
            // MPI_STATUS_IGNORE);
            
            MPI_Recv(&number1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
             
			printf("Slave 2 received number %d from slave 1\n",
				   number1);
			
			
			cout<<"Slave talking here...\n";
			cout<<"Hello world from processor "+string(processor_name)+", rank "+to_string(world_rank)+" out of "+to_string(world_size)+" number of processes.\n";
			cout<<"\n\n";
		}
	}
	
	
	MPI_Finalize();
	
	return 0;
}
