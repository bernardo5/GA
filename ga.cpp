#include "population.hpp"

/*Libraries includes*/
#include <iostream>
#include <string>

//using expressions
using namespace std;

int main(){
	population *pop=new population();
	pop->printPopulation();
	cout<<"First string fitness is: "+to_string((pop->getElement(1)).calcFitness())+"\n";
	pop->calcPopFitness();
	cout<<"\n ---------------------------------------------------------------- \n";
	pop->printPopulation();
	cout<<"\n ---------------------------------------------------------------- \n";
	pop->popSort();
	pop->printPopulation();
	return 0;
}