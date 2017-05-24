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
	string x=string((this->stringc))+" with fitness: "+to_string(fitness);
	char*y=new char [x.length()+1];
	strcpy(y, x.c_str());
	return y;
}

char * chromossome::getStringc(){
	return this->stringc;
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

void chromossome::setGene(int i, char c){
	this->stringc[i]=c;
	return;
}

char chromossome::getGene(int i){
	return (this->getString())[i];
}

void chromossome::mutate(){
	double randomnumber;
	char gene;
	for (int i = 0; i < strlen(this->getStringc()); i++) {
		randomnumber=((double) rand() / (RAND_MAX));
		//cout<<"random number for mutation: "+to_string(randomnumber)+"\n";
        if (randomnumber <= MUTATIONRATE) {
        	cout<<"\nString was "+string(this->getString())+" in "+to_string(i)+"position\n";
        	cout<<"\nmutated "+to_string(i)+" position\n";
            // Create random gene
           /* gene = (std::rand() % 91) + 32;
            cout<<"\ngenerated gene: "+to_string(gene)+"\n";
            this->setGene(i, gene);*/
            gene=(char)(std::rand() % 91) + 32;
            this->stringc[i] = gene;
            cout<<"\nString now "+string(this->getString())+" and new char is "+to_string((char)gene)+"\n";
        }
    }
	return;
}