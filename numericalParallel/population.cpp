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
	/*for_each(ChromoPopulation.begin(), ChromoPopulation.end(), [=]( chromossome & n)
		{n.calcFitness();});*/
	
	//initialize paralelism world
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	//distribute jobs through processors
	if(world_rank==0){
		//send objects to the other processors
		
		/*place the genes in int vector*/
		chromossome*p0=cloneChromossome(this->getElement(0));
		chromossome*p1=cloneChromossome(this->getElement(1));
		chromossome*p2=cloneChromossome(this->getElement(2));
		int fitness_p1, fitness_p2;
		
		
		
		MPI_Send(&p1->getValues()[0], NUMBERVARIABLES, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Send(&p2->getValues()[0], NUMBERVARIABLES, MPI_INT, 2, 0, MPI_COMM_WORLD);
		//do part of the computations
		
		//reassemble the computed info
		/*MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
		MPI_Recv(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);*/
        
        //free the allocated memory
        delete p0;
        delete p1;
        delete p2;
	}else{
		if(world_rank==1){
			//declare auxiliar variables
			int *p1_=new int[NUMBERVARIABLES];
			//receive objects to process from master
			MPI_Recv(&p1_, NUMBERVARIABLES, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
             
            //perform fitness calculations
			chromossome *aux1=new chromossome(p1_);
			aux1->calcFitness();
			cout<<"chromossome fitness is: "+to_string(aux1->getFitness())+"\n";	
			//send info back
			//MPI_Send(&number1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			
			//free memory
			delete aux1;
			delete p1_;
		}else{//world_rank=2
			//declare auxiliar variables
			int *p2_=new int[NUMBERVARIABLES];
			//receive objects to process from master
            MPI_Recv(&p2_, NUMBERVARIABLES, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
             
			//perform fitness calculations
			chromossome *aux2=new chromossome(p2_);
			aux2->calcFitness();
			cout<<"chromossome fitness is: "+to_string(aux2->getFitness())+"\n";
			//send info back
			//MPI_Send(&number1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			
			//free memory
			delete aux2;
			delete p2_;
		}
			
	}
	MPI_Finalize();
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
	for_each(ChromoPopulation.begin()+1, ChromoPopulation.end(), []( chromossome & n){n.mutate();});
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
