#include "Cell.hpp"

Cell* const nil = 0;    // a null Cell pointer

// return false if it's not an IntCell
bool Cell::is_int() const
{
  return false;
}

// return false if it's not a DoubleCell
bool Cell::is_double() const
{
  return false;
}

// return false if it's not a SymbolCell
bool Cell::is_symbol() const
{
  return false;
}

// return false if it's not a ConsCell
bool Cell::is_cons() const
{
  return false;
}

// check whether it's a IntCell(error: if not)
int Cell::get_int() const
{
  cerr<<"ERROR: It is not a int cell!"<<endl;
  exit(1);
}

// check whether it's a DoubleCell(error: if not)
double Cell::get_double() const
{
  cerr<<"ERROR: It is not a double cell!"<<endl;
  exit(1);
}

// check whether it's a SymbolCell(error: if not) 
string Cell::get_symbol() const
{
  cerr<<"ERROR: It is not a symbol cell!"<<endl;
  exit(1);
}

// check whether it's a SymbolCell(error: if not)
Cell* Cell::get_car() const
{
  cerr<<"ERROR: It is not a cons cell!"<<endl;
  exit(1);
}

// check whether it's a SymbolCell(error: if not)
Cell* Cell::get_cdr() const
{
  cerr<<"ERROR: It is not a cons cell!"<<endl;
  exit(1);
}

// check whether the operands are valid for the arithmetic operators(error: if not)
double Cell::arithmetic_o(bool& has_double) const
{
  cerr<<"ERROR:Illegal operand for arithmetic operators!"<<endl;
  exit(1);
}

// check whether the operands are valid for the ceiling operator(error: if not)
Cell* Cell::ceiling_o() const
{
  cerr<<"ERROR: The operand for the operator 'ceiling' must be double!"<<endl;
  exit(1);
}

// check whether the operands are valid for the floor operator(error: if not)
Cell* Cell::floor_o() const
{
  cerr<<"ERROR: No double for 'floor'!"<<endl;
  exit(1);
}

// print the whole tree
void Cell::print(ostream& os) const
{
  // check whether it's an empty tree
  if (this == nil)
    {
      os<<"()";
    }

  // print int if it's a IntCell
  else if (is_int())
    {
      os<<get_int();
    }

  // print double if it's a DoubleCell
  else if (is_double())
    {
      os.setf(ios::showpoint);
      os<<get_double();
    }

  // print symbol if it's a SymbolCell
  else if (is_symbol())
    {
      os<<get_symbol();
    }

  // do recursive print if it's a ConsCell
  else if (is_cons())
    {
      os<<"(";
      get_car()->print();
      Cell* temp = get_cdr();
      
      while (temp != nil && temp->is_cons())
	{
	  os<<" ";
	  temp->get_car()->print();
	  temp = temp->get_cdr();
	}

      if (temp != nil && !temp->is_cons())
	{
	  os<<" . ";
	  temp->print();
	}

      os<<")";
    }
  else
    {
      cerr<<"ERROR: It is not a valid cell"<<endl;
      exit(1);
    }
}

// create an IntCell object
IntCell::IntCell(const int i)
{
  int_m = i;
}

// return true if it's an IntCell
bool IntCell::is_int() const
{
  return true;
}

// return int_m if it's a IntCell
int IntCell::get_int() const
{
  return int_m;
}

// return int_m if it's an IntCell
double IntCell::arithmetic_o(bool& has_double) const
{
  return int_m;
}

// return true if int_m equals zero
bool IntCell::is_zero() const
{
  return (int_m == 0 ? true : false);
}

// copy the same tree deeply
Cell* IntCell::copy() const
{
  return new IntCell(int_m);
}

// create a DoubleCell object
DoubleCell::DoubleCell(const double d)
{
  double_m = d;
}

// return true if it's a DoubleCell
bool DoubleCell::is_double() const
{
  return true;
}

// return double_m if it's a DoubleCell
double DoubleCell::get_double() const
{
  return double_m;
}

// return double_m and set has_double true if it's a DoubleCell
double DoubleCell::arithmetic_o(bool& has_double) const
{
  has_double = true;
  return double_m;
}

// return the ceiling result if it's a DoubleCell
Cell* DoubleCell::ceiling_o() const
{
  return new IntCell((int)ceil(double_m));
}

// return the floor result if it's a DoubleCell
Cell* DoubleCell::floor_o() const
{
  return new IntCell((int)floor(double_m));
}

// return true if double_m equals zero
bool DoubleCell::is_zero() const
{
  return (double_m == 0 ? true : false);
}

// copy the same tree deeply
Cell* DoubleCell::copy() const
{
  return new DoubleCell(double_m);
}

// create a SymbolCell object
SymbolCell::SymbolCell(const char* const s)
{
  symbol_m = new char [strlen(s) + 1];
  strcpy(symbol_m, s);
}

// return true if it's a SymbolCell
bool SymbolCell::is_symbol() const
{
  return true;
}

// return the symbol if it's a SymbolCell
string SymbolCell::get_symbol() const
{
  return symbol_m;
}

// return false because symbol is nonzero
bool SymbolCell::is_zero() const
{
  return false;
}

// copy the same tree deeply
Cell* SymbolCell::copy() const
{
  return new SymbolCell(symbol_m);
}

// create a ConsCell object
ConsCell::ConsCell(Cell* const my_car, Cell* const my_cdr)
{
      car_m = my_car;
      cdr_m = my_cdr;
}

// return true if it's a ConsCell
bool ConsCell::is_cons() const
{
  return true;
}

// return car_m if it's a ConsCell
Cell* ConsCell::get_car() const
{
  return car_m;
}

// return cdr_m if it's a ConsCell
Cell* ConsCell::get_cdr() const
{
  return cdr_m;
}

// return false because it's nonzero
bool ConsCell::is_zero() const
{
  return false;
}

// copy the same tree deeply
Cell* ConsCell::copy() const
{
  Cell* temp_car;
  Cell* temp_cdr;
  temp_car = (car_m == nil ? nil : car_m->copy());
  temp_cdr = (cdr_m == nil ? nil : cdr_m->copy());
  return new ConsCell(temp_car, temp_cdr);
}
