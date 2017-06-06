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

population::population(bool newIteration, double*ReturnRates){
	if(newIteration){
		this->population_size=GA_POPSIZE;
		ChromoPopulation.clear();
		this->returnRates=ReturnRates;
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

chromossome population::getElement(int position){
	return ChromoPopulation.at(position);
}

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

void population::addChromossome(chromossome individual){
	ChromoPopulation.push_back(individual);
	return;
}

chromossome population::tournamentSelection(){
	//initiate population for tournament
	population *tournamentPop = new population(true, returnRates);
	
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
        // Loop through genes
       // cout<<"Sera?\n";
    while(sum>MAXINVESTMENT){
		//cout<<"matou\n";
        for (int i = 0; i < size; i++) {
            // Crossover
            int randomnumber=(std::rand()%2);
            if ( randomnumber<= UNIFORMRATE){
				newSol->setGene(i, a.getGene(i));	
			}else{
				newSol->setGene(i, b.getGene(i));
            }
        }
        sum=0;
        for (int j = 0; j < size; j++)sum+=newSol->getGene(j);
        //cout<<"Soma e : "+to_string(sum)+"\n";
    }
    return *newSol;
}

void population::mutate(){
	for_each(ChromoPopulation.begin(), ChromoPopulation.end(), []( chromossome & n)
		{
			double randomnumber=((double) rand() / (RAND_MAX));
			if (randomnumber <= MUTATIONRATE) {
				n.mutate();
			}
		});
	return;
}

vector<chromossome> population::getList(){
	return this->ChromoPopulation;
}

void population::evolvePop(){
	//cout<<"\n\n--------------------------\n Now evolving...\n\n";
	//by default it is considered eleitism
	population *newPop=new population(true, returnRates);
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

