pa3: main.cpp FPS.o
	g++ -o pa3 main.cpp FPS.o graph.o queue.o

queue.o: queue.h queue.cpp
	g++ -c queue.cpp

graph.o: graph.h graph.cpp queue.o
	g++ -c graph.cpp

FPS.o: FPS.h FPS.cpp graph.o    
	g++ -c FPS.cpp  

clean:
	rm *.o
