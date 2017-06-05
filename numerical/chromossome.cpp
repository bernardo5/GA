#include "chromossome.hpp"
#include <cmath>  


using namespace std;

chromossome::chromossome(){
	this->fitness=0;
	this->values=new int[9];

	
    int j;
	for (j = 0; j < NUMBERVARIABLES; j++)
        this->values[j] = (std::rand() % 1024);
    if(NUMBERVARIABLES<8){
		for (int i = j; i < 9; i++)
        this->values[j] = 0;
	}
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


int chromossome::calcFitness(){
	int fitness=0;
	//fitness function is:
	//f (x) = ((a + 2b + 3c + 4d + (e*f+g-h+i)) - 30)

	fitness=values[0]+(2*values[1])+(3*values[2])+(4*values[3])+(values[4]*values[5]+values[6]-values[7]+values[8]);

	return fitness;
}

string chromossome::getString() const{
	string s="Fitness is "+to_string(fitness);
	for (int j = 0; j < NUMBERVARIABLES; j++)
        s=s+", x["+to_string(j+1)+"]="+to_string(this->values[j])+" ";
    s=s+"\n";
	return s;
}
/*
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
        if (randomnumber <= MUTATIONRATE) {
            gene=(char)(std::rand() % 91) + 32;
            this->stringc[i] = gene;
        }
    }
	return;
}
*/
