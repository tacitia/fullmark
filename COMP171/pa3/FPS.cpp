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
#include "FPS.h"

/**
 * \brief Default constructor to create an empty FPS.
 */
FPS::FPS()
{
  // set each pointer to be NULL
  for (int i = 0; i < CITY_MAXSIZE; i++)
    {
      city[i] = NULL;
    }
}

/**
 * \brief Default destructor to release dynamic memory.
 */
FPS::~FPS()
{
  // release dynamic memory and set the pointer to be NULL for safty
  for (int i = 0; i < CITY_MAXSIZE; i++)
    {
      delete city[i];
      city[i] = NULL;
    }
}

/**
 * \brief Add a city to the system, and assign a numerical ID to it.
 * \param cityname The name of city.
 * \return True if success, otherwise false.
 */
bool FPS::insertCity(string cityname)
{
  int index = nameTold(cityname);
  
  // check whether the city has already existed
  if (index != -1)
    {
      return false;
    }
  
  // insert the city if the FPS is not full
  for (int i = 0; i < CITY_MAXSIZE; i++)
    {
      if (city[i] == NULL)
	{
	  city[i] = new City;
	  city[i]->index = i;
	  city[i]->name = cityname;
	  graph.addVertex(i);
	  return true;
	}
    }
  return false;
}

/**
 * \brief Delete a city from the system.
 * \param city The name of city to be removed.
 * \return True if success, otherwise false.
 */
bool FPS::removeCity(string cityname)
{
  int index = nameTold(cityname);
  
  // check whether the city exists
  if (index == -1)
    {
      return false;
    }
  else
    {
      graph.removeVertex(index);
      delete city[index];
      city[index] = NULL;
      return true;
    }
}

/**
 * \brief Add a flight between two cities.
 * \param depart_city The name of departure city. 
 * \param dest_city The name of destination city.
 * \return True if success, otherwise flase.
 */
bool FPS::insertFlight(string depart_city, string dest_city)
{
  // get the index of depart_city and dest_city
  int depart_index = nameTold(depart_city);
  int dest_index = nameTold(dest_city);
  
  return graph.addEdge(depart_index, dest_index);
}

/**
 * \brief Delete the flight between two cities.
 * \param depart_city The name of departure city. 
 * \param dest_city The name of destination city.
 * \return True if success, otherwise false.
 */
bool FPS::removeFlight(string depart_city, string dest_city)
{
  // get the index of depart_city and dest_city
  int depart_index = nameTold(depart_city);
  int dest_index = nameTold(dest_city);
  
  return graph.removeEdge(depart_index, dest_index);
}

/**
 * \brief Find and print the flight with minimal number of connecting flight between two different inputted cities
 * (e.g. A -> B means 1 flight; A -> B -> C means 2 flights).
 * Print the flight plan out with " -> " between each city, finally end with endl.
 * \param depart_city The name if departure city. 
 * \param dest_city The name of destination city.
 * \return The number of flights if a successful flight is found
 * Return 0 when no flight is possible for the inputted cities
 * Return -1 when both departure city are destination city are invalid
 * Return -2 when departure city is invalid
 * Return -3 when destination city is invalid
 */
int FPS::printFlight(string depart_city, string dest_city)
{
  // get the index of depart_city and dest_city
  int depart_index = nameTold(depart_city);
  int dest_index = nameTold(dest_city);
  
  // check whether the cities are invalid
  if (depart_index == -1 && dest_index == -1)
    {
      return -1;
    }
  else if (depart_index == -1 && dest_index != -1)
    {
      return -2;
    }
  else if (depart_index != -1 && dest_index == -1)
    {
      return -3;
    }
  else
    {
      Path path = graph.performBFS(depart_index, dest_index);
      
      // check whether the flight exists
      if (path.numNodes == 0 || path.numNodes == 1)
      	{
      	  return 0;
      	}
      printPath(path);
      return (path.numNodes -  1);
    }
}

/**
 * \brief Display all the cities in the system. One city name per row in their alphabetical order
 * \return The number of city in the system
 */
int FPS::printAllCities()
{
  int city_num = 0;                // the number of cities stored in the system
  string cities[CITY_MAXSIZE];     // an array to store the name of whole cities
  
  for (int i = 0; i < CITY_MAXSIZE; i++)
    {
      if (giveName(i) != "")
	{
	  cities[city_num++] = giveName(i);
	}
    }
  int j, p;
  
  // using insertion store to make cities to be stored in alphabetical order
  for (p = 1; p < city_num; p++)
    {
      string temp = cities[p];
      
      for (j = p; j > 0 && temp < cities[j - 1]; j--)
	{
	  cities[j] = cities[j - 1];
	}
      cities[j] = temp;
    }
  
  // print the name of city in order
  for (int i = 0; i < city_num; i++)
    {
      printCity(cities[i]);
    }
  return city_num;
}

/**
 * \brief Helper function that maps the input city to a numerical index by accessing the array.
 * \param cityname The name of city to be found.
 * \return Assigned ID of the input city, -1 if the city does not exist.
 */ 
int FPS::nameTold(string cityname)
{
  for (int i = 0; i < CITY_MAXSIZE; i++)
    {
      if (city[i] == NULL)
	{
	  continue;
	}
      else if (city[i]->name == cityname)
	{
	  return i;
	}
    }
  
  // return -1 if the city does not exist
  return -1;
}

/**
 * \brief Helper function that maps the a numerical index to its city by accessing the array.
 * \param index The index of city to be search for.
 * \return The city name of the input ID. Return "" if ID not found.
 */
string FPS::giveName(int index)
{
  // check whether the index is valid or the city exists
  if (index  < 0 || index > 49 || city[index] == NULL)
    {
      return "";
    }
  else
    {
      return city[index]->name;
    }
}

