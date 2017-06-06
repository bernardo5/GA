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
		double * returnRates;
		static bool Compare (chromossome i, chromossome j) {return ( i.getFitness() < j.getFitness() ); }
	public:
		population();
		population(bool newIteration, double*ReturnRates);
		//string getPop();
		void printRates();
		int getSize();
		
		//bool compare(chromossome&a, chromossome&b)const;
		
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
