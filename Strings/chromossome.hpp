//include assumed values
#include "macro.hpp"

//include libraries
#include <iostream>
#include <cstring>
#include <ctime>
#include <algorithm>
using namespace std;

class chromossome{
	private:
		char * stringc;
		unsigned int fitness;

	public:
		chromossome(int stringSize);
		void printChromossome();
		//static std::ostream & operator<<( std::ostream & o, const chromossome & s );
		char * getString() const;
		char * getStringc();
		unsigned int getFitness();
		void setFitness(int fitness);
		int calcFitness();
		void setGene(int i, char c);
		char getGene(int i);
		void mutate();
};

