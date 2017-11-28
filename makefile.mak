
build: 
	g++ maxcardinality.cpp graph.cpp subroutines.cpp perfectmatching.cpp -o maxcardinality.o
		
clean: 
	-rm -f *.o core