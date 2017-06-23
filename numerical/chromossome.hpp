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
		//constructor
		chromossome();
		chromossome(int *vector);
		//destructor
		//~chromossome();
		void deleteVector();
		//copy constructor
		//chromossome(const chromossome&c_in):values(new int(*c_in.values)){fitness=0;}
		//assign operator
		//void operator=(const chromossome&another);
		
		bool operator<(const chromossome& other) const {
			return fitness < other.fitness;
		}
		unsigned int getFitness();
		int* getValues();
		void calcFitness();
		void setGene(int i, int gene);
		int getGene(int i);
		//int constgetGene(int i) const;
		void mutate();
		string getString() const;
};

