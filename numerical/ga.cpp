#include "population.hpp"

/*Libraries includes*/
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
//using expressions
using namespace std;

int main(){
	int i=1;
	int old_fitness=0;
	float threshold=0.001;
	//initialize rand parameter
	srand(time(0));
	//start counting clock
	auto start_time = std::chrono::high_resolution_clock::now();
	//start GA
	population *pop=new population();
	//pop->printRates();
	pop->calcPopFitness();
	
	//pop->printPopulation();
	
	
	pop->popSort();
	
	//pop->printPopulation();
	string s=pop->getElement(0).getString();
	while(pop->getElement(0).getFitness()!=0){
		pop->evolvePop();
		pop->calcPopFitness();
		pop->popSort();
		cout<<"\nBest string fit in ("+to_string(i)+") iteration: "+string(pop->getElement(0).getString())+"\n";
		i++;
	}
	cout<<"\nGA algorithms work!\n";
	//end of GA algorithm and stop counting time
	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time;
	std::cout << "It took " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << " milliseconds to run.\n";
	return 0;
}
