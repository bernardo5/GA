main: 
	g++ ga.cpp population.cpp chromossome.cpp -o main -O3 -std=c++11

clean:
	rm -f *.o *.~ main *.gch