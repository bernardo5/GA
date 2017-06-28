main: 
	sudo /mirror/openmpi2/bin/mpiCC ga.cpp population.cpp chromossome.cpp -o main -O3 -std=c++0x
	
test:
	gcc test.c -o test -O3 

clean:
	sudo rm -f *.o *.~ main *.gch *.~ test
