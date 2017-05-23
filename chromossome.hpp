//include assumed values
#include "macro.hpp"

//include libraries
#include <iostream>
#include <string>
#include <ctime>

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
		unsigned int getFitness();
		void setFitness(int fitness);
		int calcFitness();
		void setGene(int i, char c);
		char getGene(int i);
};

