#include "chromossome.hpp"
#include <cmath>  


using namespace std;

chromossome::chromossome(){
	this->fitness=0;
	this->values=new int[NUMBERVARIABLES];

	
    int j;
    int I=MAXINVESTMENT;
	for (j = 0; j < (NUMBERVARIABLES-1); j++){
        this->values[j] = (std::rand() % I);
        I=I-this->values[j];
    }
    this->values[j]=I;
    
}

/*void chromossome::printChromossome(){
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
*/
unsigned int chromossome::getFitness(){
	return this->fitness;
}

void chromossome::setFitness(int fitness){
	this->fitness=fitness;
	return;
}

int* chromossome::getValues(){
		return values;
}	




string chromossome::getString() const{
	string s="Fitness is "+to_string(fitness);
	for (int j = 0; j < NUMBERVARIABLES; j++)
        s=s+", x["+to_string(j+1)+"]="+to_string(this->values[j])+" ";
    s=s+"\n";
	return s;
}

void chromossome::setGene(int i, int c){
	this->values[i]=c;
	return;
}

int chromossome::getGene(int i){
	return this->values[i];
}

void chromossome::mutate(){
	 int I=MAXINVESTMENT;
	int i;
	for (i = 0; i < (NUMBERVARIABLES-1); i++) {
		this->values[i] = (std::rand() % I);
        I=I-this->values[i];
    }
    this->values[i]=I;
	return;
}

