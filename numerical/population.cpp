#include "population.hpp"
#include <tr1/functional>

using namespace std;
using namespace std::tr1::placeholders;

population::population(){
	this->population_size=GA_POPSIZE;

	chromossome *individual;
	ChromoPopulation.clear();
	for(int i=0; i<this->population_size; i++){
		individual = new chromossome();
		ChromoPopulation.push_back(*individual);
		delete individual;
	}
	
	this->returnRates=new double[NUMBERVARIABLES];
	for (int j = 0; j < NUMBERVARIABLES; j++)
        (this->returnRates)[j] = (((double) std::rand()/(RAND_MAX)));
}

population::population(bool newIteration){
	if(newIteration){
		this->population_size=GA_POPSIZE;
		ChromoPopulation.clear();
		//no individual push to vector
	}else{
		cout<<"Unspecified context\n";
		exit(0);
	}
}

void population::printRates(){
	string s="Rates are ";
	for (int j = 0; j < NUMBERVARIABLES; j++)
        s=s+", r["+to_string(j+1)+"]="+to_string(this->returnRates[j])+" ";
    s=s+"\n";
	cout<<s;
}	

int population::getSize(){
	return population_size;
}

void population::printPopulation(){
	for (std::vector<chromossome>::const_iterator i = (this->ChromoPopulation).begin();
													 i != (this->ChromoPopulation).end(); ++i)
    std::cout << i->getString()<<"\n";
}
/*
chromossome population::getElement(int position){
	return ChromoPopulation.at(position);
}
*/
void population::calcPopFitness(){
	for_each(ChromoPopulation.begin(), ChromoPopulation.end(), [=]( chromossome & n)
		{
			double inner=0;
			int*v=n.getValues();
			for(int j=0; j<NUMBERVARIABLES; j++)
				inner+=(v[j]*(this->returnRates[j]));
			n.setFitness(inner);});
	return;
}


void population::popSort(){
	sort(ChromoPopulation.begin(), ChromoPopulation.end());
}
/*
void population::addChromossome(chromossome individual){
	ChromoPopulation.push_back(individual);
	return;
}

chromossome population::tournamentSelection(){
	//initiate population for tournament
	population tournamentPop = new population(true);
	
	for (int i = 0; i < TOURNAMENTSIZE; i++) {
        int randomId = (int) (std::rand() % this->getSize());
        tournamentPop.addChromossome(this->getElement(randomId));
    }
    // Get the fittest
    tournamentPop.calcPopFitness();
    tournamentPop.popSort();
    return tournamentPop.getElement(0);
}

chromossome population::crossover(chromossome a, chromossome b){
	int size=string(GA_TARGET).length();
	chromossome *newSol = new chromossome(size);
        // Loop through genes
        for (int i = 0; i < size; i++) {
            // Crossover
            int randomnumber=(std::rand()%2);
            if ( randomnumber<= UNIFORMRATE) {
                newSol->setGene(i, a.getGene(i));
            } else {
                newSol->setGene(i, b.getGene(i));
            }
        }
    chromossome *returnVect=newSol;
    delete newSol;
    return *returnVect;
}

void population::mutate(){
	for_each(ChromoPopulation.begin(), ChromoPopulation.end(), []( chromossome & n)
		{n.mutate();});
	return;
}

vector<chromossome> population::getList(){
	return this->ChromoPopulation;
}

void population::evolvePop(){
	//by default it is considered eleitism
	population newPop=new population(true);
	//initialize new population with fitest member of previous pop
	newPop.addChromossome(this->getElement(0));
	//evolve population through crossover
	for (int i = 1; i < this->getSize(); i++) {
        chromossome indiv1 = this->tournamentSelection();
        chromossome indiv2 = this->tournamentSelection();
        chromossome newIndiv = this->crossover(indiv1, indiv2);
        newPop.addChromossome(newIndiv);
    }
    //mutate population
    newPop.mutate();
    this->ChromoPopulation=newPop.getList();
	return;
}
*/
