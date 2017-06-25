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
}

population::population(bool newIteration){
	if(newIteration){
		this->population_size=GA_POPSIZE;
		ChromoPopulation.clear();
	}else{
		cout<<"Unspecified context\n";
		exit(0);
	}
}

int population::getSize(){
	return population_size;
}

void population::printPopulation(){
	for (std::vector<chromossome>::const_iterator i = (this->ChromoPopulation).begin();
													 i != (this->ChromoPopulation).end(); ++i)
    std::cout << i->getString()<<"\n";
}	

chromossome population::getElement(int position){
	return ChromoPopulation.at(position);
}

void population::calcPopFitness(){
	for_each(ChromoPopulation.begin(), ChromoPopulation.end(), [=]( chromossome & n)
		{n.calcFitness();});
	return;
}


void population::popSort(){
	sort(ChromoPopulation.begin(), ChromoPopulation.end());
}

void population::addChromossome(chromossome individual){
	ChromoPopulation.push_back(individual);
	return;
}

chromossome *population::tournamentSelection(){
	//initiate population for tournament
	population *tournamentPop = new population(true);
	int i;
	chromossome*addC;
	for ( i = 0; i < TOURNAMENTSIZE; i++) {
        int randomId = (int) (std::rand() % GA_POPSIZE);
        addC=cloneChromossome(this->getElement(randomId));
        tournamentPop->addChromossome(*addC);
        delete addC;
    }
    // Get the fittest
    //tournamentPop->calcPopFitness();
    tournamentPop->popSort();
    
    chromossome *newC=new chromossome(tournamentPop->getElement(0).getValues());
	
	tournamentPop->cleanup();
	
    delete tournamentPop;
    
    return newC;
}

chromossome* population::crossover(chromossome a, chromossome b){
	int size=NUMBERVARIABLES;
	chromossome *newSol = new chromossome();
    for (int i = 0; i < size; i++) {
       // Crossover
       int randomnumber=(std::rand()%2);
       if ( randomnumber<= UNIFORMRATE){
			newSol->setGene(i, a.getGene(i));	
	   }else{
			newSol->setGene(i, b.getGene(i));
       }
     }
    return newSol;
}

void population::mutate(){
	for_each(ChromoPopulation.begin(), ChromoPopulation.end(), []( chromossome & n)
		{n.mutate();});
	return;
}

vector<chromossome> population::getList(){
	return this->ChromoPopulation;
}

chromossome *population::cloneChromossome(chromossome c){
	return new chromossome(c.getValues()); 
}

void population::removeChromossome(){
	ChromoPopulation.erase(ChromoPopulation.begin());
	return; 
}

void population::evolvePop(){
	//by default it is considered eleitism
	population *newPop=new population(true);
	//initialize new population with fitest member of previous pop
	chromossome fittest(this->getElement(0).getValues());
	newPop->addChromossome(fittest);
	//evolve population through crossover
	int i;
	for (i = 1; i < this->getSize(); i++) {
        chromossome *indiv1 = this->tournamentSelection();
        chromossome *indiv2 = this->tournamentSelection();
        chromossome *newIndiv = this->crossover(*indiv1, *indiv2);
        indiv1->deleteVector();
        delete indiv1;
        indiv2->deleteVector();
        delete indiv2;
        newPop->addChromossome(*newIndiv);
        //newIndiv->deleteVector();
        delete newIndiv;
    }
    //mutate population
    newPop->mutate();
    
    //clear previous list and assign to new one
    this->cleanup();
	while(ChromoPopulation.size()!=GA_POPSIZE) {
		chromossome*n=cloneChromossome(newPop->getElement(0));
        this->addChromossome(*n);
        delete n;
        newPop->getElement(0).deleteVector();
        newPop->removeChromossome();
    }
    
    delete newPop;
	return;
}

void population::cleanup(){
	while(ChromoPopulation.size()!=0) {
		this->getElement(0).deleteVector();
		this->removeChromossome();
	}
	this->ChromoPopulation.clear();
	return;
}
