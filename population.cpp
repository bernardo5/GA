#include "population.hpp"


using namespace std;

population::population(){
	this->population_size=GA_POPSIZE;
	this->targetString=GA_TARGET;

	chromossome *individual;
	ChromoPopulation.clear();
	std::srand(std::time(0));
	for(int i=0; i<this->population_size; i++){
		individual = new chromossome(string(GA_TARGET).length());
		ChromoPopulation.push_back(*individual);
	}
}

population::population(bool newIteration){
	if(newIteration){
		this->population_size=GA_POPSIZE;
		this->targetString=GA_TARGET;

		//chromossome *individual;
		ChromoPopulation.clear();
		//no individual push to vector
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
    std::cout << (*i).getString()<<"\n";
}

chromossome population::getElement(int position){
	return ChromoPopulation.at(position);
}

void population::calcPopFitness(){
	for_each(ChromoPopulation.begin(), ChromoPopulation.end(), []( chromossome & n)
		{n.setFitness(n.calcFitness());});
	return;
}

void population::popSort(){
	sort(ChromoPopulation.begin(), ChromoPopulation.end(), [](chromossome &a, chromossome &b){
        return a.getFitness() < b.getFitness();});
}

void population::addChromossome(chromossome individual){
	ChromoPopulation.push_back(individual);
	return;
}

chromossome population::tournamentSelection(){
	//initiate population for tournament
	population tournamentPop = new population(true);
	srand(time(0));
	for (int i = 0; i < TOURNAMENTSIZE; i++) {
        int randomId = (int) (std::rand() % this->getSize());
        cout<<"\n";
        cout<<"Generated random integer is: "+to_string(randomId);
		cout<<"\n";
        
        tournamentPop.addChromossome(this->getElement(randomId));
    }
    cout<<"\n _____________________________________\n Population tournament:\n\n";
    tournamentPop.printPopulation();
    cout<<"\n _____________________________________\n";
    // Get the fittest
    tournamentPop.calcPopFitness();
    tournamentPop.popSort();
    cout<<"\nFitest is: "+string(tournamentPop.getElement(0).getString())+"\n";
    cout<<"\n _____________________________________\n";
    return tournamentPop.getElement(0);
}

chromossome population::crossover(chromossome a, chromossome b){
	/*cout<<"\n";
	cout<<"these are the chromossome: ";
	a.printChromossome();
	cout<<"\n";
	b.printChromossome();*/
	int size=string(GA_TARGET).length();
	chromossome *newSol = new chromossome(size);
        // Loop through genes
	srand(time(0));
        for (int i = 0; i < size; i++) {
            // Crossover
            int randomnumber=(std::rand()%2);
            //cout<<"\n random number generated is: "+to_string(randomnumber)+"\n";
            if ( randomnumber<= UNIFORMRATE) {
                newSol->setGene(i, a.getGene(i));
            } else {
                newSol->setGene(i, b.getGene(i));
            }
        }
        /*cout<<"\n";
        newSol->printChromossome();
        cout<<"\n";*/
        return *newSol;
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
    cout<<"\n --------------------------------------------------------------- \n";

    newPop.printPopulation();

      /*  // Mutate population
        for (int i = elitismOffset; i < newPopulation.size(); i++) {
            mutate(newPopulation.getIndividual(i));
        }

        return newPopulation;*/
	return;
}