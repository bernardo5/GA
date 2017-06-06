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
		unsigned int getFitness();
		int* getValues();
		void calcFitness();
		void setGene(int i, int gene);
		int getGene(int i);
		void mutate();
		string getString() const;
};

