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
	cout<<"First string fitness is: "+to_string((pop->getElement(0)).calcFitness())+"\n";
	pop->calcPopFitness();
	cout<<"\n ---------------------------------------------------------------- \n";
	//pop->printPopulation();
	cout<<"\n ---------------------------------------------------------------- \n";
	pop->popSort();
	//pop->printPopulation();
	cout<<"\n ---------------------------------------------------------------- \n";
	pop->evolvePop();
	return 0;
}