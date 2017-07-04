#include "population.hpp"
#include <tr1/functional>

using namespace std;
using namespace std::tr1::placeholders;

population::population(){
	this->population_size=GA_POPSIZE/3;

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
		this->population_size=GA_POPSIZE/3;
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

void population::addChromossomeBeginning(chromossome individual){
	ChromoPopulation.insert(ChromoPopulation.begin(), individual);
	return;
}

chromossome *population::tournamentSelection(){
	//initiate population for tournament
	population *tournamentPop = new population(true);
	int i;
	chromossome*addC;
	for ( i = 0; i < TOURNAMENTSIZE; i++) {
        int randomId = (int) (std::rand() % (GA_POPSIZE/3));
        addC=cloneChromossome(this->getElement(randomId));
        tournamentPop->addChromossome(*addC);
        delete addC;
    }
    // Get the fittest
    //tournamentPop->calcPopFitness();
    tournamentPop->popSort();
    
    chromossome *newC=new chromossome(tournamentPop->getElement(0).getValues(), false);
	
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
	for_each(ChromoPopulation.begin()+1, ChromoPopulation.end(), []( chromossome & n){n.mutate();});
	return;
}

vector<chromossome> population::getList(){
	return this->ChromoPopulation;
}

chromossome *population::cloneChromossome(chromossome c){
	return new chromossome(c.getValues(), false); 
}

void population::removeChromossome(){
	ChromoPopulation.erase(ChromoPopulation.begin());
	return; 
}

void population::removeChromossome(int position){
	ChromoPopulation.erase(ChromoPopulation.begin()+position);
	return; 
}

void population::evolvePop(){
	//by default it is considered eleitism
	population *newPop=new population(true);
	//initialize new population with fitest member of previous pop
	chromossome fittest(this->getElement(0).getValues(), false);
	newPop->addChromossome(fittest);
	/*cout<< "The best is: ";
	for(int k=0; k<	NUMBERVARIABLES; k++)
		cout<<to_string((newPop->getElement(0).getValues())[k])+", ";
	cout<<"\n";*/
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
	while(ChromoPopulation.size()!=(GA_POPSIZE/3)) {
		chromossome*n=cloneChromossome(newPop->getElement(0));
        this->addChromossome(*n);
        delete n;
        newPop->deleteFirst();
    }
    delete newPop;
	return;
}

void population::deleteFirst(){
	this->deleteElement(0);
	return;
}

void population::deleteElement(int position){
	this->getElement(position).deleteVector();
	this->removeChromossome(position);
	return;
}


void population::cleanup(){
	while(ChromoPopulation.size()!=0) {
		this->deleteFirst();
	}
	this->ChromoPopulation.clear();
	return;
}

int * population::codifChamp(){
	return this->codifElement(0);
}

int * population::codifElement(int position){
	int * vector=new int[NUMBERVARIABLES+1];
	int *aux=this->getElement(position).getValues();
	for(int i=0; i<NUMBERVARIABLES; i++){
		vector[i]=aux[i];
	}
	vector[NUMBERVARIABLES]=this->getElement(position).getFitness();
	return vector;
}

void population::receiveOutsideChromossome(bool beginning){
	int *values_champ=new int[NUMBERVARIABLES+1];
	MPI_Recv(values_champ, NUMBERVARIABLES+1, MPI_INT, MPI_ANY_SOURCE,  MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	chromossome*newChamp=new chromossome(values_champ, true);
	delete [] values_champ;
	//cout<<newChamp->getString();
	//add the champion to the beggining of the list
	if(beginning)
		this->addChromossomeBeginning(*newChamp);
	else this->addChromossome(*newChamp);
	//free the memory
	delete newChamp;
	return;
}

void population::receiveAllChromossomes(int m){
	int receives=0;
	while(receives!=(((GA_POPSIZE/3)*2)-m)) {
		this->receiveOutsideChromossome(false);
		receives++;
	}
	return;
}

void population::sendChromossome(bool champ, int position, int dest_rank){
	int*sendVec;
	if(champ)sendVec=this->codifChamp();
		else sendVec=this->codifElement(position);
	MPI_Send(sendVec, NUMBERVARIABLES+1, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
	delete [] sendVec;
	//delete sent element from pop
	if(champ) this->deleteFirst();
		else this->deleteElement(position);
	
	return;
}

void population::sendAll(){
	while(ChromoPopulation.size()!=0) {
		this->sendChromossome(true, 0, 0);
	}
	return;
}

void population::distribPop(){
	int*sendElement;
	int position;
	int popNumber=GA_POPSIZE;
	for(int y=0; y<(GA_POPSIZE/3); y++){//send pop elements to first process
		do{
			position=std::rand() % popNumber;
		}while(position==0);
		this->sendChromossome(false, position, 1);
		popNumber--;
	}
	for(int z=0; z<(GA_POPSIZE/3); z++){//send pop elements to second process
		do{
			position=std::rand() % popNumber;
		}while(position==0);
		this->sendChromossome(false, position, 2);
		popNumber--;
	}
	return;
}

void population::receivePopPart(){
	for(int q=0; q<(GA_POPSIZE/3); q++){//send pop elements to second process
		this->receiveOutsideChromossome(false);
	}
	return;
}
