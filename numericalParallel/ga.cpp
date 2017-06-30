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
	
	if(world_rank==0){
		//start counting clock
		auto start_time = std::chrono::high_resolution_clock::now();
	}
	
	population *pop=new population();
	pop->calcPopFitness();
	
/* This is just for testing-------------------------------------*/
/************************************************************************/
	int bcast_flag;
	if(world_rank==0){
		cout<<"Master population:\n";
		pop->printPopulation();
		cout<<"-------------------------------------------\n";
		bcast_flag=1;
		
		MPI_Send(&bcast_flag, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}else{
		if(world_rank==1){
			MPI_Recv(&bcast_flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
			cout<<"Slave "+to_string(world_rank)+" population:\n";
			pop->printPopulation();
			cout<<"-------------------------------------------\n";
			sleep(1);
			MPI_Send(&bcast_flag, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		}else{//world_rank==2
			MPI_Recv(&bcast_flag, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
            // MPI_Barrier(MPI_COMM_WORLD);
			cout<<"Slave "+to_string(world_rank)+" population:\n";
			pop->printPopulation();
			cout<<"-------------------------------------------\n";
		}
	}	
/************************************************************************/	
	//calculate the champions
	pop->popSort();
	int*champ=getChampion(world_rank, pop);
	cout<<"Rank "+to_string(champ[0])+" champion has fitness "+to_string(champ[1])+"\n";
	if(world_rank==0){
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
        //-----------------------------------------------------------------------------------------------
		cout<<"Selected champions are:\n";
        cout<<"From rank "+to_string(ch.at(0).arr[0])+" with fitness "+to_string(ch.at(0).arr[1])+"\n";
        cout<<"From rank "+to_string(ch.at(1).arr[0])+" with fitness "+to_string(ch.at(1).arr[1])+"\n";
        cout<<"From rank "+to_string(ch.at(2).arr[0])+" with fitness "+to_string(ch.at(2).arr[1])+"\n";
        //-----------------------------------------------------------------------------------------------
        sort(ch.begin(), ch.end()); //compute the champion of all champions
        
        //-----------------------------------------------------------------------------------------------
        cout<<"The world champion is:\n";
        cout<<"From rank "+to_string(ch.at(0).arr[0])+" with fitness "+to_string(ch.at(0).arr[1])+"\n";
        cout<<"Second world champion is:\n";
        cout<<"From rank "+to_string(ch.at(1).arr[0])+" with fitness "+to_string(ch.at(1).arr[1])+"\n";
        cout<<"Third world champion is:\n";
        cout<<"From rank "+to_string(ch.at(2).arr[0])+" with fitness "+to_string(ch.at(2).arr[1])+"\n";
        //-----------------------------------------------------------------------------------------------
        
        //
	}else{
		//send champions fitnesses
		MPI_Send(champ, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}	
	
	
	
	
	delete champ;
	
	/*
	fitness_check=pop->getElement(0).getFitness();
	while(pop->getElement(0).getFitness()!=0){
		pop->evolvePop();
		pop->calcPopFitness();
		pop->popSort();
		
		if(fitness_check<(pop->getElement(0).getFitness())){
			cout<<"Error in elitism\n";
			cout<<"---------------------------\nPrinting after sort...\n";
			pop->printPopulation();
			cout<<"\n-------------------------------------------\n";
			exit(1);
		}else{
			if(fitness_check>(pop->getElement(0).getFitness()))
				fitness_check=(pop->getElement(0).getFitness());
		}
		if(print==1)cout<<"\nBest string fit in ("+to_string(i)+") iteration: "+string(pop->getElement(0).getString())+"\n";
		i++;
	}
	if(print==1)cout<<"\nGA algorithms work!\n";
	//end of GA algorithm and stop counting time
	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time;
	if(print==1)std::cout << "It took " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << " milliseconds to run.\n";
    writeFile(pop->getElement(0).getValues(), to_string(std::chrono::duration_cast<std::chrono::milliseconds>(time).count()));
	pop->cleanup();
	delete pop;*/
	MPI_Finalize();
	return 0;
}