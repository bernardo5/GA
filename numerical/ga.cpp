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
	
	
	//initialize rand parameter
	srand(time(0));
	//start counting clock
	auto start_time = std::chrono::high_resolution_clock::now();
	//start GA
	population *pop=new population();
	//cout<<"Cretion\n";
	pop->calcPopFitness();
	//cout<<"fitness calc\n";
	/*cout<<"---------------------------\nPrinting...\n";
	pop->printPopulation();
	cout<<"\n-------------------------------------------\n";*/
	pop->popSort();
<<<<<<< HEAD
	int fitness_check=pop->getElement(0).getFitness();
	string s=pop->getElement(0).getString();
=======
	//cout<<"sort\n";
	/*cout<<"---------------------------\nPrinting...\n";
	pop->printPopulation();
	cout<<"\n-------------------------------------------\n";*/
	cout<<"fitness of best: "+to_string(pop->getElement(0).getFitness())+"---"+string(pop->getElement(0).getString())+"\n";
	//string s=pop->getElement(0).getString();
	
	fitness_check=pop->getElement(0).getFitness();
>>>>>>> frees
	while(pop->getElement(0).getFitness()!=0){
		pop->evolvePop();
		
		
		
		/*cout<<"evolve\n";
		cout<<"---------------------------\nPrinting after evolve...\n";
		pop->printPopulation();
		cout<<"\n-------------------------------------------\n";*/
		
		
		pop->calcPopFitness();
		
		
		/*cout<<"fitness calc\n";
		cout<<"---------------------------\nPrinting after fitness...\n";
		pop->printPopulation();
		cout<<"\n-------------------------------------------\n";*/
		
		
		pop->popSort();
<<<<<<< HEAD
=======
		
		
		
		/*cout<<"---------------------------\nPrinting after sort...\n";
		pop->printPopulation();
		cout<<"\n-------------------------------------------\n";
		exit(0);*/
		//cout<<"sort\n";
		
		
>>>>>>> frees
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
	delete pop;
	return 0;
}
