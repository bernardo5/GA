#include <string>
#include <stdlib.h>
#include <vector>
#include "chromossome.hpp"

using namespace std;

class population{
	private:
		int population_size;
		vector<chromossome> ChromoPopulation;
		string targetString;
	public:
		population();
		void printPopulation();
		chromossome getElement(int position);
		void calcPopFitness();
		void popSort();
};