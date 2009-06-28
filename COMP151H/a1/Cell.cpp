/**
 * \file Cell.cpp
 *
 * Implementation of Cell class that can be used to build a tree data 
 * structures and allow user to access the tree.
 */

#include "Cell.hpp"
// Reminder: cons.hpp expects nil to be defined somewhere.  For this
// implementation, this is the logical place to define it.
Cell* const nil = 0;

Cell::Cell(const int i) 
{ 
  tag_m = type_int;
  int_m = i;
  cdr_m = nil;
}

Cell::Cell(const double d)
{
  tag_m = type_double;
  double_m = d;
  cdr_m = nil;
}

Cell::Cell(const char* const s)
{
  tag_m = type_symbol;
  symbol_m = new char [strlen(s)+1];
  strcpy(symbol_m, s);
  cdr_m = nil;
}

Cell::Cell(Cell* const my_car, Cell* const my_cdr)
{
  tag_m = type_conspair;
  car_m = my_car;
  cdr_m = my_cdr;
}

void Cell::print(ostream& os) const
{
  if (this == nil)
    {
      os<<"()";
    }
  else if (is_int() == true)
    {
      os<<int_m;
    }
  else if (is_double() == true)
    {
      os.setf(ios::showpoint);
      os<<double_m;
    }
  else if (is_symbol() == true)
    {
      os<<symbol_m;
    }
  else if (is_cons() == true)
    {
      os<<"(";
      get_car()->print();
      
      Cell* temp = get_cdr();
      
      while (temp != nil)
	{
	  os<<" ";
	  temp->get_car()->print();
	  temp = temp->get_cdr();
	}
      
      os<<")";
	
    }
  
}

bool Cell::is_int() const
{
  return (tag_m == type_int ? true : false);
}

bool Cell::is_double() const
{
  return (tag_m == type_double ? true : false);
}

bool Cell::is_symbol() const
{
  return (tag_m == type_symbol ? true : false);
}

bool Cell::is_cons() const
{
  return (tag_m == type_conspair ? true : false);
}

int Cell::get_int() const
{
  if (tag_m != type_int)
    {
      cerr<<"ERROR:You can't get int because this is not a int cell!"<<endl;
      exit(1);
    }
  else
    {
      return int_m;
    }
}

double Cell::get_double() const
{
  if (tag_m != type_double)
    {
      cerr<<"ERROR:You can't get double because this is not a double cell!"<<endl;
      exit(1);
    }
  else
    {
      return double_m;
    }
}

string Cell:: get_symbol() const
{
  if (tag_m != type_symbol)
    {
      cerr<<"ERROR:You can't get symbol because this is not a symbol cell!"<<endl;
      exit(1);
    }
  else
    {
      return symbol_m;
    }
}

Cell* Cell::get_car() const
{
  if (tag_m != type_conspair)
    {
      cerr<<"ERROR:You can't get car because this is not a cons cell!"<<endl;
      exit(1);
    }
  else
    {
      return car_m;
    }
}

Cell* Cell::get_cdr() const
{
  if (tag_m != type_conspair)
    {
      cerr<<"ERROR:You can't get cdr because this is not a cons cell!"<<endl;
      exit(1);
    }
  else
    {
      return cdr_m;
    }
}

