//include assumed values
#include "macro.hpp"

//include libraries
#include <iostream>
#include <string.h>
#include <cstring>
#include <ctime>
#include <algorithm>
using namespace std;

class chromossome{
	private:
		int * values;
		unsigned int fitness;

	public:
		chromossome();
		void printChromossome();
		unsigned int getFitness();
		int* getValues();
		void setFitness(int fitness);
		//int calcFitness();
		void setGene(int i, int gene);
		char getGene(int i);
		void mutate();
		string getString() const;
};

