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

chromossome population::tournamentSelection(){
	//initiate population for tournament
	population *tournamentPop = new population(true);
	
	for (int i = 0; i < TOURNAMENTSIZE; i++) {
        int randomId = (int) (std::rand() % GA_POPSIZE);
        tournamentPop->addChromossome(this->getElement(randomId));
    }
    // Get the fittest
    tournamentPop->calcPopFitness();
    tournamentPop->popSort();
    chromossome c=tournamentPop->getElement(0);
    delete tournamentPop;
    //cout<<string(c.getString())+"\n";
    return c;
}

chromossome population::crossover(chromossome a, chromossome b){
	int size=NUMBERVARIABLES;
	int sum=1025;
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
    return *newSol;
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
	//cout<<"\n\n--------------------------\n Now evolving...\n\n";
	//by default it is considered eleitism
	population *newPop=new population(true);
	//initialize new population with fitest member of previous pop
	newPop->addChromossome(this->getElement(0));
	//evolve population through crossover
	for (int i = 1; i < this->getSize(); i++) {
        chromossome indiv1 = this->tournamentSelection();
       // cout<<"\n\n---------------------\nIndiv1\n";
        //cout<<string(indiv1.getString())+"\n---------------\n";
        chromossome indiv2 = this->tournamentSelection();
       // cout<<"\n\n---------------------\nIndiv2\n";
       // cout<<string(indiv2.getString())+"\n---------------\n";
        chromossome newIndiv = this->crossover(indiv1, indiv2);
       // cout<<"\n\n-------------------\nNew individual coming\n";
        //cout<<string(newIndiv.getString())+"\n--------------------\n\n";
        newPop->addChromossome(newIndiv);
    }
    //mutate population
    newPop->mutate();
    vector<chromossome> C=newPop->getList();
    delete newPop;
    this->ChromoPopulation=C;
	return;
}

