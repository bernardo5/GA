#include "chromossome.hpp"
#include <cmath>  


using namespace std;

chromossome::chromossome(){
	this->fitness=0;
	this->values=new int[NUMBERVARIABLES];

	
    int j;
	for (j = 0; j < NUMBERVARIABLES; j++){
        this->values[j] = (std::rand() % 1024);
    }
}

chromossome::chromossome(int *vector, bool Fitness){
	if(!Fitness){
		this->fitness=100;
		
	}else this->fitness=vector[NUMBERVARIABLES];
    this->values=new int[NUMBERVARIABLES];
	int j;
	for (j = 0; j < NUMBERVARIABLES; j++){
		this->values[j] = vector[j];
	}
}

void chromossome::deleteVector(){
	delete[] values;
}

/*void chromossome::operator =(const chromossome&another){
	int*temp=new int[NUMBERVARIABLES];
	for (int j = 0; j < NUMBERVARIABLES; j++){
        temp[j] = another.getGene(j);
    }
    delete[] values;
    values=temp;
    fitness=another.fitness;
}*/

unsigned int chromossome::getFitness(){
	return this->fitness;
}

void chromossome::calcFitness(){
	int result=0;
	for(int i=0; i<NUMBERVARIABLES; i++){
		result+=values[i]*(i+1);
	}
	result-=1024;
	fitness=result;
	return;
}

int* chromossome::getValues(){
		return values;
}	




string chromossome::getString()const{
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
	double randomnumber;
	int gene;
	for (int i = 0; i < NUMBERVARIABLES; i++) {
		randomnumber=((double) rand() / (RAND_MAX));
        if (randomnumber <= MUTATIONRATE) {
            gene=(std::rand() % 1024);	
            this->values[i] = gene;
        }
    }
	return;
}
	

