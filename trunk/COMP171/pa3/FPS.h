/*
  Name: XIAO Sa
  Student ID: 07592272
  Lecture Section: L1
  Tutorial Section: T1C
  I did not cheat in this assignment
  Acknowledge: The textbook "Data Structures and Algorithm Analysis in C++"
  and the lecture notes. The work included in this program is all my own work.
  I did not cheat in this assignment.
*/

#ifndef _FPS_H_
#define _FPS_H_

#define CITY_MAXSIZE 50

#include <string>
#include "graph.h"
using namespace std;

/* This city struct is to store city information.
	Every vertex should have an unique index
*/
struct City {
	int index;	// a unique index for each city
	string name;	// city name
};

/* This FPS class is to implement a Flight Planning System (FPS for short).
	This class maintains a mapping between the city names and the numerical indexes used in Graph.
	Remarks: Assume max 50 cities, city index must be in [0..49]
*/
/* Do not put any code for print out yourself.
	Only call the provided printPath and printCity functions with proper arguments for display.
*/
class FPS {
public:
	// Default constructor
	FPS();

	// Default destructor
	~FPS();

	/* Add a city to the system, and assign a numerical ID to it
		Return true if success, false if failed
	*/
	bool insertCity(string cityname);

	/* Delete a city from the system
		Return true if success, false if failed
	*/
	bool removeCity(string cityname);

	/* Add a flight between two cities
		Return true if success, false if failed
	*/
	bool insertFlight(string depart_city, string dest_city);

	/* Delete a flight between two cities
		Return true if success, false if failed
	*/
	bool removeFlight(string depart_city, string dest_city);

	/* Find and print the flight with minimal number of connecting flight between two different inputted cities
		(e.g. A -> B means 1 flight; A -> B -> C means 2 flights)
		Print the flight plan out with " -> " between each city, finally end with endl
		Return the number of flights if a successful flight is found
		Return 0 when no flight is possible for the inputted cities
		Return -1 when both departure city are destination city are invalid
		Return -2 when departure city is invalid
		Return -3 when destination city is invalid
	*/
	int printFlight(string depart_city, string dest_city);

	/* Display all the cities in the system
		one city name per row in their alphabetical order
		Return the number of city in the system
	*/
	int printAllCities();



/*
=======================================
Add your own member functions here only.
Do not change anything else
=======================================
*/


/*
End of additional member functions
=======================================
*/


private:
	/* Helper function that maps the input city to a numerical index by accessing the array
		Return assigned ID of the input city, -1 if the city does not exist
	*/
	int nameTold(string cityname);

	/* Helper function that maps the a numerical index to its city by accessing the array
		Return the city name of the input ID
		Return "" if ID not found
	*/
	string giveName(int index);

	/* Helper function that prints out the path found city by city
	*/
	void printPath(Path path) {
		cout << giveName(path.node[0]);
		for (int i=1; i<path.numNodes; i++) {
			cout << " -> " << giveName(path.node[i]);
		}
		cout << endl;
	}

	/* Helper function that prints out the city
	*/
	void printCity(string city) {
		cout << city << endl;
	}

private:
	/* array of pointer to all vertices
		map[i] should store the pointer to City of index i
		map[i] should point to NULL if there are no City of index i
	*/
	City* city[CITY_MAXSIZE];
	Graph graph;	// graph of cities
};

#endif
