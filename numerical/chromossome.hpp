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
		

	public:
		unsigned int fitness;
		chromossome();
		bool operator<(const chromossome& other) const {
			return fitness < other.fitness;
		}
		void printChromossome();
		unsigned int getFitness();
		int* getValues();
		void setFitness(int fitness);
		void setGene(int i, int gene);
		char getGene(int i);
		void mutate();
		string getString() const;
};

