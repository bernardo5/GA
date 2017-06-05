#include <cstring>
#include <stdlib.h>
#include <vector>
#include "chromossome.hpp"
#include <algorithm>
using namespace std;

class population{
	private:
		int population_size;
		vector<chromossome> ChromoPopulation;
		double * returnRates;
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
		void mutate();
		vector<chromossome> getList();
		void evolvePop();
};
