#include "population.hpp"

/*Libraries includes*/
#include <iostream>
#include <string>

//using expressions
using namespace std;

int main(){
	srand(time(0));
	population *pop=new population();
	//pop->printPopulation();
	//cout<<"First string fitness is: "+to_string((pop->getElement(0)).calcFitness())+"\n";
	pop->calcPopFitness();
	pop->popSort();
	int i=1;
	while((pop->getElement(0).getFitness())!=0){
		pop->evolvePop();
		pop->calcPopFitness();
		pop->popSort();
		cout<<"\nBest string fit in ("+to_string(i)+") iteration: "+string(pop->getElement(0).getStringc())+"\n";
		i++;
	}

	cout<<"\nGA algorithms work!\n";
	
	return 0;
}