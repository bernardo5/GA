#include <cstring>
#include <stdlib.h>
#include <vector>
#include "chromossome.hpp"
#include <algorithm>
#include <list>
#include <string.h>
#include <tr1/functional>
using namespace std;
using namespace std::tr1::placeholders;

class population{
	private:
		int population_size;
		vector<chromossome> ChromoPopulation;
		static bool Compare (chromossome i, chromossome j) {return ( i.getFitness() < j.getFitness() ); }
	public:
		population();
		population(bool newIteration);
		int getSize();
		void printPopulation();
		void removeChromossome();
		void removeChromossome(int position);
		chromossome getElement(int position);
		void calcPopFitness();
		void popSort();
		void addChromossome(chromossome individual);
		void addChromossomeBeginning(chromossome individual);
		chromossome *tournamentSelection();
		chromossome* crossover(chromossome a, chromossome b);
		void mutate();
		chromossome * cloneChromossome(chromossome c);
		vector<chromossome> getList();
		void evolvePop();
		void cleanup();
		void deleteFirst();
		int * codifChamp();
		int * codifElement(int position);
		void deleteElement(int position);
};
