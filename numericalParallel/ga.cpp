#include "population.hpp"

/*Libraries includes*/
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <fstream>
#include <unistd.h>
//using expressions
using namespace std;

bool cycleBreak(int world_rank, population*pop){
	int cycle=0;
	if(world_rank==0)
		if(pop->getElement(0).getFitness()==0) cycle=1;
	MPI_Bcast(&cycle, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if(cycle==1) return true;
	return false;
}

void printAllPops(int world_rank, population*pop){		
	if(world_rank==0){		
		cout<<"Master pop\n";
		pop->printPopulation();
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}else if(world_rank==1){		
		MPI_Barrier(MPI_COMM_WORLD);
		cout<<"Slave1 pop\n";
		pop->printPopulation();
		MPI_Barrier(MPI_COMM_WORLD);
	}else{		
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		cout<<"Slave2 pop\n";
		pop->printPopulation();
	}
	return;
}


void writeFile(int*finalSequence, string final_time){
	std::ofstream outputFile;
	outputFile.open("output.txt", std::ios_base::app);
	//checkVerifications
	if(outputFile.is_open()){
		for(int i=0; i<NUMBERVARIABLES; i++){
			outputFile<<to_string(finalSequence[i])+"\t";	
		}	
		outputFile<<final_time+"\n";
		outputFile.close();
	}else{		
		cout<<"Error opening file\n";
	}
	
	return;
}

int * getChampion(int world_rank, population*pop){
	int *champion= new int[2];
		champion[0]=world_rank;
		champion[1]=pop->getElement(0).getFitness();
	
	return champion;
}

typedef struct champs{		
		int arr[2];
		bool operator<( const champs& rhs ) const
        { return arr[1] < rhs.arr[1]; }
}champs_list;

void copy_integer_array_to_struct(champs_list *l, int * arr2){		
	l->arr[0]=arr2[0];
	l->arr[1]=arr2[1];
	return;
}

int handleFittest(int * champ){		
	//receive champions fitnesses
	champs master;
	champs p1;
	champs p2;
	MPI_Recv(p1.arr, 2, MPI_INT, 1, 0, MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);
	MPI_Recv(p2.arr, 2, MPI_INT, 2, 0, MPI_COMM_WORLD,
          MPI_STATUS_IGNORE);
    MPI_Barrier(MPI_COMM_WORLD);
        
	copy_integer_array_to_struct(&master, champ);
		
    std::vector<champs> ch;
    ch.push_back(master);
    ch.push_back(p1);
    ch.push_back(p2);
        
    sort(ch.begin(), ch.end()); //compute the champion of all champions
    int returnInt=ch.at(0).arr[0];
    ch.clear();
    return returnInt;    
}

void champSync(int world_rank, population*pop){		
	int*champ=getChampion(world_rank, pop);
	cout<<"Rank "+to_string(champ[0])+" champion has fitness "+to_string(champ[1])+"\n";
	if(world_rank==0){
        //Broadcast the champion rank number to the machines, so they know if they send the champion over
        int ch_number=handleFittest(champ);
        MPI_Bcast( &ch_number, 1, MPI_INT, 0, MPI_COMM_WORLD);
		int m=0;
		if(ch_number!=world_rank){
			//receive the champion
			m=1;
			pop->receiveOutsideChromossome(true);
		}else{
			m=0;
		} 
		MPI_Barrier(MPI_COMM_WORLD); //we are sure now that the master has the champion for sure 
		//receive all pop elements of slaves
		pop->receiveAllChromossomes(m);
		//has now all elements
		MPI_Barrier(MPI_COMM_WORLD);
		//pop->printPopulation(); //Master now has all the population
		cout<<"-------------------------------------------------------\n";
	}else{
		//send champions fitnesses
		MPI_Send(champ, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		int champ_rank;
		MPI_Bcast(&champ_rank, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if(champ_rank==world_rank){//sends the champ over
			pop->sendChromossome(true, 0, 0);
		}
		MPI_Barrier(MPI_COMM_WORLD);//knows now that master has the champion
		//send all population elements
		pop->sendAll();
		MPI_Barrier(MPI_COMM_WORLD);
		//pop->printPopulation(); //Slaves have now zero pop
	}	
	delete champ;
	return;
}

void distributePop(int world_rank, population*pop){
	if(world_rank==0){//send pop elements
		pop->distribPop();
		MPI_Barrier(MPI_COMM_WORLD); //population is now distributed again
		/*cout<<"\n------------------------\n";
			pop->printPopulation();*/
	}else{//receive pop elements
		pop->receivePopPart();
		MPI_Barrier(MPI_COMM_WORLD); //population is now distributed again
		/*if(world_rank==1){//send pop elements to second process
			cout<<"\n------------------------\n";
			pop->printPopulation();
		}*/
	}
	return;
}


int main(int argc, char *argv[]){
	int i=1;
	int print=0;
	int fitness_check;
	
	if ( argc < 2 ) print=1; 
	
	//initialize paralelism world
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	
	//initialize rand parameter
	srand(world_rank+1);
	
	//start counting clock
	auto start_time = std::chrono::high_resolution_clock::now();
	
	population *pop=new population();
	pop->calcPopFitness();
	pop->popSort();
	champSync(world_rank, pop);
	//distribute the pop
	distributePop(world_rank, pop);
	//if(world_rank==0)pop->printPopulation();
	int iteration=1;
	while(true/* stop condition*/){
		MPI_Barrier(MPI_COMM_WORLD);
		champSync(world_rank, pop);
		if(cycleBreak(world_rank, pop)) break;
		//all elements are in the master now
		if(world_rank==0) pop->evolvePop();
		MPI_Barrier(MPI_COMM_WORLD);
		distributePop(world_rank, pop);
		//pop distributed again
		//mutate population
		pop->mutate();
		pop->calcPopFitness();
		pop->popSort();
		
		iteration++;
	}
	if(world_rank==0)
			if(print==1)
				cout<<"\nBest string fit in ("+to_string(iteration)+") iteration: "+string(pop->getElement(0).getString())+"\n";
	if(world_rank==0){
		 if(print==1)cout<<"\nGA algorithms work!\n";
		//end of GA algorithm and stop counting time
		auto end_time = std::chrono::high_resolution_clock::now();
		auto time = end_time - start_time;
		if(print==1)std::cout << "It took " <<
		std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << " milliseconds to run.\n";
		//writeFile(pop->getElement(0).getValues(), to_string(std::chrono::duration_cast<std::chrono::milliseconds>(time).count()));
	}
	pop->cleanup();
	delete pop;
	MPI_Finalize();
	return 0;
}
