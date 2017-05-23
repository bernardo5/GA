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