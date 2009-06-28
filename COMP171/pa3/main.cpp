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
#include <iostream>
#include <string>
#include <stdio.h>
#include "FPS.h"
#include "graph.h"

using namespace std;

void printMsg(int type, string arg1, string arg2) {
  switch(type) {
  case 0:
    cout << "City " << arg1 << " inserted" << endl;
    break;
  case 1:
    cout << "City exists" << endl;
    break;
  case 2:
    cout << "City " << arg1 << " removed" << endl;
    break;
  case 3:
    cout << "No such city" << endl;
    break;
  case 4:
    cout << "Flight " << arg1 << " -> " << arg2 << " inserted" << endl;
    break;
  case 5:
    cout << "Error in inserting flight" << endl;
    break;
  case 6:
    cout << "Flight " << arg1 << " -> " << arg2 << " removed" << endl;
    break;
  case 7:
    cout << "Error in removing flight" << endl;
    break;
  case 8:
    cout << "Invalid departure and destination city" << endl;
    break;
  case 9:
    cout << "Invalid departure city" << endl;
    break;
  case 10:
    cout << "Invalid destination city" << endl;
    break;
  case 11:
    cout << "Impossible to find flights" << endl;
    break;
  case 12:
    cout << "Number of Flights: " << arg1 << endl;
    break;
  case 13:
    cout << "No city in system" << endl;
    break;
  case 14:
    cout << "Number of Cities: " << arg1 << endl;
    break;
  case 15:
    cout << "Invalid command" << endl;
    break;
  }
}

void showAck() {
  cout << ">";
}


int main()
{
  string command = "";
  int type = -1;
  string city1 = "";
  string city2 = "";
  char a[3];
  FPS fps;
  showAck();

  // shell
  while (cin >> command)
    {      
      // insert a city to the FPS system
      if (command == "insertCity")
	{
	  cin >> city1;
	  type = fps.insertCity(city1) ? 0 : 1;
	}
      
      // delete a city matching with the name supplied
      else if (command == "removeCity")
	{
	  cin >> city1;
	  type = fps.removeCity(city1) ? 2 : 3;
	}
      
      // insert a flight between the input cities
      else if (command == "insertFlight")
	{
	  cin >> city1;
	  cin >> city2;
	  type = fps.insertFlight(city1, city2) ? 4 : 5;
	}
      
      // delete a filght between the input cities
      else if (command == "removeFlight")
	{
	  cin >> city1;
	  cin >> city2;
	  type = fps.removeFlight(city1, city2) ? 6 : 7;
	}
      
      // find and print the flight with minimal number of connecting flight 
      // between two different inputted cities.
      else if (command == "findFlight")
	{
	  cin >> city1;
	  cin >> city2;
	  int _case = fps.printFlight(city1, city2);
	  
	  switch(_case)
	    {
	    case 0: type = 11; break;
	    case -1: type = 8; break;
	    case -2: type = 9; break;
	    case -3: type = 10; break;
	    default: type = 12; break;
	    }
	  
	  if (_case < 10)
	    {
	      a[0] = '1' + _case % 10 - 1;
	      a[1] = '\0';
	    }
	  else
	    {
	      a[0] = '1' + _case / 10 -1;
	      a[1] = '1' + _case % 10 -1;
	      a[2] = '\0';
	    }
	  city1 = a;
	}
      
      // display all the cities in the system in alphabetical order
      else if (command == "printAllCities")
	{
	  int n = fps.printAllCities();
	  
	  // check whether the city exists
	  if (n == 0)
	    {
	      type = 13;
	    }
	  else
	    {
	      if (n < 10)
		{
		  a[0] = '1' + n % 10 - 1;
		  a[1] = '\0';
		}
	      else
		{
		  a[0] = '1' + n / 10 - 1;
		  a[1] = '1' + n % 10 - 1;
		  a[2] = '\0';
		}
	      type = 14;
	      city1 = a;
	    }
	}
      
      // quit the shell program
      else if (command == "exit")
	{
	  break;
	}
      
      // invalid command
      else
	{
	  type = 15;
	}
      printMsg(type, city1, city2);
      showAck();
    }
}

