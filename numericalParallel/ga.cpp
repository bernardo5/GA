#include "population.hpp"

/*Libraries includes*/
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <fstream>
#include <unistd.h>
//using expressions
using namespace std;

void writeFile(int*finalSequence, string final_time){
	std::ofstream outputFile;
	outputFile.open("output.txt", std::ios_base::app);
	//checkVerifications
	if(outputFile.is_open()){
		for(int i=0; i<NUMBERVARIABLES; i++){
			outputFile<<to_string(finalSequence[i])+"\t";	
		}	
		outputFile<<final_time+"\n";
		outputFile.close();
	}else{		
		cout<<"Error opening file\n";
	}
	
	return;
}

int * getChampion(int world_rank, population*pop){
	int *champion= new int[2];
		champion[0]=world_rank;
		champion[1]=pop->getElement(0).getFitness();
	
	return champion;
}

typedef struct champs{		
		int arr[2];
		bool operator<( const champs& rhs ) const
        { return arr[1] < rhs.arr[1]; }
}champs_list;

void copy_integer_array_to_struct(champs_list *l, int * arr2){		
	l->arr[0]=arr2[0];
	l->arr[1]=arr2[1];
	return;
}

int main(int argc, char *argv[]){
	int i=1;
	int print=0;
	int fitness_check;
	
	if ( argc < 2 ) print=1; 
	
	//initialize paralelism world
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	
	//initialize rand parameter
	srand(world_rank+1);
	
	if(world_rank==0){
		//start counting clock
		auto start_time = std::chrono::high_resolution_clock::now();
	}
	
	population *pop=new population();
	pop->calcPopFitness();
	
/* This is just for testing-------------------------------------*/
/************************************************************************/
	int bcast_flag;
	if(world_rank==0){
		cout<<"Master population:\n";
		pop->printPopulation();
		cout<<"-------------------------------------------\n";
		bcast_flag=1;
		
		MPI_Send(&bcast_flag, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}else{
		if(world_rank==1){
			MPI_Recv(&bcast_flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
			cout<<"Slave "+to_string(world_rank)+" population:\n";
			pop->printPopulation();
			cout<<"-------------------------------------------\n";
			sleep(1);
			MPI_Send(&bcast_flag, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		}else{//world_rank==2
			MPI_Recv(&bcast_flag, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
            // MPI_Barrier(MPI_COMM_WORLD);
			cout<<"Slave "+to_string(world_rank)+" population:\n";
			pop->printPopulation();
			cout<<"-------------------------------------------\n";
		}
	}	
/************************************************************************/	
	//calculate the champions
	pop->popSort();
	int*champ=getChampion(world_rank, pop);
	cout<<"Rank "+to_string(champ[0])+" champion has fitness "+to_string(champ[1])+"\n";
	if(world_rank==0){
		//receive champions fitnesses
		champs master;
		champs p1;
		champs p2;
		MPI_Recv(p1.arr, 2, MPI_INT, 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
		MPI_Recv(p2.arr, 2, MPI_INT, 2, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
        MPI_Barrier(MPI_COMM_WORLD);
		copy_integer_array_to_struct(&master, champ);
		
        std::vector<champs> ch;
        ch.push_back(master);
        ch.push_back(p1);
        ch.push_back(p2);
        //-----------------------------------------------------------------------------------------------
		cout<<"Selected champions are:\n";
        cout<<"From rank "+to_string(ch.at(0).arr[0])+" with fitness "+to_string(ch.at(0).arr[1])+"\n";
        cout<<"From rank "+to_string(ch.at(1).arr[0])+" with fitness "+to_string(ch.at(1).arr[1])+"\n";
        cout<<"From rank "+to_string(ch.at(2).arr[0])+" with fitness "+to_string(ch.at(2).arr[1])+"\n";
        //-----------------------------------------------------------------------------------------------
        sort(ch.begin(), ch.end()); //compute the champion of all champions
        
        //-----------------------------------------------------------------------------------------------
        cout<<"The world champion is:\n";
        cout<<"From rank "+to_string(ch.at(0).arr[0])+" with fitness "+to_string(ch.at(0).arr[1])+"\n";
        cout<<"Second world champion is:\n";
        cout<<"From rank "+to_string(ch.at(1).arr[0])+" with fitness "+to_string(ch.at(1).arr[1])+"\n";
        cout<<"Third world champion is:\n";
        cout<<"From rank "+to_string(ch.at(2).arr[0])+" with fitness "+to_string(ch.at(2).arr[1])+"\n";
        //-----------------------------------------------------------------------------------------------
        
        //Broadcast the champion rank number to the machines, so they know if they send the champion over
        int ch_number=ch.at(0).arr[0];//CHANGE IT LATER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        MPI_Bcast( &ch_number, 1, MPI_INT, 0, MPI_COMM_WORLD);
		int m=0;
		if(ch_number!=world_rank){
			//receive the champion
			m=1;
			MPI_Status status;
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			int l;
			MPI_Get_count(&status, MPI_CHAR, &l);
			//int l = status.Get_count(MPI_CHAR);
			char *buf = new char[l];
			MPI_Recv(buf, l, MPI_CHAR, MPI_ANY_SOURCE,  MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			string champ_aux(buf, l);
			delete [] buf;
			cout<<champ_aux;
			int *values_champ=new int[NUMBERVARIABLES+1];
			MPI_Recv(values_champ, NUMBERVARIABLES+1, MPI_INT, MPI_ANY_SOURCE,  MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			chromossome*newChamp=new chromossome(values_champ, true);
			delete [] values_champ;
			//cout<<newChamp->getString();
			//add the champion to the beggining of the list
			pop->addChromossomeBeginning(*newChamp);
			//free the memory
			delete newChamp;
		}else cout<<"Im the master and im the champion holder!\n";
		MPI_Barrier(MPI_COMM_WORLD); //we are sure now that the master has the champion for sure 
		//receive all pop elements of slaves
		int receives=0;
		int *values_champEle;
		chromossome*newChampEle;
		while(receives!=(((GA_POPSIZE/3)*2)-m)) {
			values_champEle=new int[NUMBERVARIABLES+1];
			MPI_Recv(values_champEle, NUMBERVARIABLES+1, MPI_INT, MPI_ANY_SOURCE,  MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			newChampEle=new chromossome(values_champEle, true);
			delete [] values_champEle;
			//add the champion to the beggining of the list
			pop->addChromossome(*newChampEle);
			//free the memory
			delete newChampEle;
			receives++;
		}
		//has now all elements
		MPI_Barrier(MPI_COMM_WORLD);
		pop->printPopulation(); //Master now has all the population
	}else{
		//send champions fitnesses
		MPI_Send(champ, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		int champ_rank;
		MPI_Bcast(&champ_rank, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if(champ_rank==world_rank){//sends the champ over
			string champ_string="Thanks for the tip! Im the champion older! ["+to_string(world_rank)+"]\n";
			MPI_Send(champ_string.c_str(), champ_string.length(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			int*sendVec=pop->codifChamp();
			MPI_Send(sendVec, NUMBERVARIABLES+1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			delete [] sendVec;
			//delete champion from pop
			pop->deleteFirst();
		}
		MPI_Barrier(MPI_COMM_WORLD);//knows now that master has the champion
		//send all population elements
		int*sendVecEle;
		while(pop->getList().size()!=0) {
			sendVecEle=pop->codifChamp();
			MPI_Send(sendVecEle, NUMBERVARIABLES+1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			delete [] sendVecEle;
			//delete sent element from pop
			pop->deleteFirst();
		}
		
		MPI_Barrier(MPI_COMM_WORLD);
		//pop->printPopulation(); //Slaves have now zero pop
	}	
	
	
	
	
	delete champ;
	
	/*
	fitness_check=pop->getElement(0).getFitness();
	while(pop->getElement(0).getFitness()!=0){
		pop->evolvePop();
		pop->calcPopFitness();
		pop->popSort();
		
		if(fitness_check<(pop->getElement(0).getFitness())){
			cout<<"Error in elitism\n";
			cout<<"---------------------------\nPrinting after sort...\n";
			pop->printPopulation();
			cout<<"\n-------------------------------------------\n";
			exit(1);
		}else{
			if(fitness_check>(pop->getElement(0).getFitness()))
				fitness_check=(pop->getElement(0).getFitness());
		}
		if(print==1)cout<<"\nBest string fit in ("+to_string(i)+") iteration: "+string(pop->getElement(0).getString())+"\n";
		i++;
	}
	if(print==1)cout<<"\nGA algorithms work!\n";
	//end of GA algorithm and stop counting time
	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time;
	if(print==1)std::cout << "It took " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << " milliseconds to run.\n";
    writeFile(pop->getElement(0).getValues(), to_string(std::chrono::duration_cast<std::chrono::milliseconds>(time).count()));
	pop->cleanup();
	delete pop;*/
	MPI_Finalize();
	return 0;
}
