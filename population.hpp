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
		population(bool newIteration);
		int getSize();
		void printPopulation();
		chromossome getElement(int position);
		void calcPopFitness();
		void popSort();
		void addChromossome(chromossome individual);
		chromossome tournamentSelection();
		chromossome crossover(chromossome a, chromossome b);

		void evolvePop();
};