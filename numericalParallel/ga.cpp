#include "population.hpp"

/*Libraries includes*/
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <fstream>
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
		srand(world_rank);
	
	if(world_rank==0){
		//start counting clock
		auto start_time = std::chrono::high_resolution_clock::now();
	}
	
	population *pop=new population();
	pop->calcPopFitness();
	pop->printPopulation();
	
	if(world_rank==0){
		//receive data
		
		
		
		/*cout<<"--------------------------------------\n";
		pop->printPopulation();
		pop->popSort();*/
	}else{
		//send data
		
		
		
		
	}	
		
	
	//pop->calcPopFitness();
	/*cout<<"--------------------------------------\n";
	pop->printPopulation();*/
	/*pop->popSort();
	
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
