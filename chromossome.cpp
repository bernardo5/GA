#include "chromossome.hpp"
#include <cmath>  


using namespace std;

chromossome::chromossome(int stringSize){
	this->fitness=0;
	this->stringc=new char[stringSize];

	

	for (int j = 0; j < stringSize; j++)
        this->stringc[j] = (std::rand() % 91) + 32;

}

void chromossome::printChromossome(){
	cout<<(string(this->stringc)+"\n");
	return;
}

char * chromossome::getString() const{
	string x=string((this->stringc))+"with fitness: "+to_string(fitness);
	char*y=new char [x.length()+1];
	strcpy(y, x.c_str());
	return y;
}

unsigned int chromossome::getFitness(){
	return this->fitness;
}

void chromossome::setFitness(int fitness){
	this->fitness=fitness;
	return;
}


int chromossome::calcFitness(){
	int fitness=0;
	string targetString=string(GA_TARGET);
	int stringSize=targetString.length();

	for (int j = 0; j < stringSize; j++) {
        fitness += abs((int)(this->stringc[j] - targetString[j]));
    }

	return fitness;
}