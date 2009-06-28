#include "Cell.hpp"

Cell* const nil = 0;                                  //!< a null Cell pointer
vector <map <string, Cell*> > v;                      //!< a container to hold global and local tables
vector <map <string, Cell*> >::iterator it_vector;    //!< an iterator used to iterate through vector 
map <string, Cell*> global_table;                     //!< a global tables to hold the defined symbol
map <string, Cell*>::iterator it_map;                 //!< an iterator used to iterate through map

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

// return false if it's not a ProcedureCell
bool Cell::is_procedure() const
{
  return false;
}

// check whether it's a IntCell(error: if not)
int Cell::get_int() const
{
  throw runtime_error("It is not a int cell!");
}

// check whether it's a DoubleCell(error: if not)
double Cell::get_double() const
{
  throw runtime_error("It is not a double cell!");
}

// check whether it's a SymbolCell(error: if not) 
string Cell::get_symbol() const
{
  throw runtime_error("It is not a symbol cell!");
}

// check whether it's a ConsCell(error: if not)
Cell* Cell::get_car() const
{
  throw runtime_error("It is not a cons cell!");
}

// check whether it's a ConsCell(error: if not)
Cell* Cell::get_cdr() const
{
  throw runtime_error("It is not a cons cell!");
}

// check whether it's a ProcedureCell(error: if not)
Cell* Cell::get_formals() const
{
  throw runtime_error("It is not a procedure cell");
}

// check whether it's a ProcedureCell(error: if not)
Cell* Cell::get_body() const
{
  throw runtime_error("It is not a procedure cell");
}

// check whether the operands are valid
Cell* Cell::get_value() const
{
  throw runtime_error("The operands are invalid!");
}

// check whether the operands are valid for the arithmetic operators(error: if not)
double Cell::arithmetic_o(bool& has_double) const
{
  throw runtime_error("Illegal operand for arithmetic operators!");
}

// check whether the operands are valid for the ceiling operator(error: if not)
Cell* Cell::ceiling_o() const
{
  throw runtime_error("The operand for the operator 'ceiling' must be double!");
}

// check whether the operands are valid for the floor operator(error: if not)
Cell* Cell::floor_o() const
{
  throw runtime_error("No double for 'floor'!");
}

// check whether the operands are valid for the boolean oeprator(error: if not)
double Cell::less_o() const
{
  throw runtime_error("Illegal operand for boolean operators!");
}

// check whether the operands are valid for the define oeprator(error: if not)
void Cell::define_o(Cell*& eval_car3) const
{
  throw runtime_error("The first operand for 'define' must be symbol!");
}

// check whether it's a ProcedureCell
Cell* Cell::apply(Cell* const args) const
{
  throw runtime_error("Can't apply a value that is not a funtion");
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

  // print '#<function>' if it's a ProcedureCell
  else if (is_procedure())
    {
      os<<"#<function>";
    }

  else
    {
      throw logic_error("It is not a valid cell");
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

// return the pointer which points to the IntCell holdling the same value
Cell* IntCell::get_value() const
{
  return new IntCell(int_m);
}

// return int_m if it's an IntCell
double IntCell::arithmetic_o(bool& has_double) const
{
  return int_m;
}

// return int_m if it's an IntCell
double IntCell::less_o() const
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


// return the pointer which points to the DoubleCell holding the same value
Cell* DoubleCell::get_value() const
{
  return new DoubleCell(double_m);
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

// return double_m if it's a DoubleCell
double DoubleCell::less_o() const
{
 return double_m;
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

// difine a symbol(error: if redefine) 
void SymbolCell::define_o(Cell*& value) const
{
  string key = get_symbol();

  // construct a global table when the vector is empty
  if (v.empty())
    {
      v.push_back(global_table);
      (v.front()).insert(make_pair<string, Cell*>(key, value));
    }
  else
    {
      it_vector = v.end();
      --it_vector;
      it_map = (*it_vector).find(key);

      // check whether to be redefined`
      if (it_map == (*it_vector).end())
	{
	  (*it_vector).insert(make_pair<string, Cell*>(key, value));
	}
      else
	{
	  throw runtime_error("The symbols cannot be redefined!");
	}
    }
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

// return the pointer which points to the value te symbol bounds to
Cell* SymbolCell::get_value() const
{
  string key = get_symbol();
  it_vector = v.end();
  --it_vector;

  // search the defined symbol from low level to high level
  for (int i = 1; i <= v.size(); ++i)
    {
      it_map = (*it_vector).find(key);

      // get the local variable
      if (it_map != (*it_vector).end())
	{
	  // check whether it's nil
	  if (it_map->second == nil)
	    {
	      return nil;
	    }
	  else
	    {
	      return (it_map->second)->copy();
	    }
	}
      --it_vector;
    }
  throw runtime_error("Attempt to reference an undefined symbol " + key);
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
  
  if (my_cdr == nil || my_cdr->is_cons())
    {
      cdr_m = my_cdr;
    }
  else
    {
      throw runtime_error("Dot pair is invalid");
    }
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

// create an ProcedureCell object
ProcedureCell::ProcedureCell(Cell* const my_formals, Cell* const my_body)
{  
  if (my_body == nil || !my_body->is_cons())
    {
      throw runtime_error("The function body for ProcedureCell is illegal");
    }
  else
    {
      Cell* temp_ptr = my_formals;
      Cell* temp_formal = nil;

      // check whether the formals are symbol
      while (temp_ptr != nil)
	{
	  temp_formal = temp_ptr->get_car();
	  if (temp_formal == nil || !temp_formal->is_symbol())
	    {
	      throw runtime_error("The formals for funtion should be symbol");
	    }
	  temp_ptr = temp_ptr->get_cdr();
	}
      formals_m = my_formals;
      body_m = my_body;
    }
}

// return formals_m if it's a ProcedureCell
Cell* ProcedureCell::get_formals() const
{
  return formals_m;
}

// return body_m if it's a ProcedureCell
Cell* ProcedureCell::get_body() const
{
  return body_m;
}

// reutrn a same ProcedureCell
Cell* ProcedureCell::get_value() const
{
  return copy();
}

// apply a function
Cell* ProcedureCell::apply(Cell* const args) const
{
  return new ConsCell((Cell* const)this, args);
}

// return true if it's a ProcedureCell
bool ProcedureCell::is_procedure() const
{
  return true;
}

// return false because it is nonzero
bool ProcedureCell::is_zero() const
{
  return false;
}

// copy the same tree deeply
Cell* ProcedureCell::copy() const
{
  Cell* temp_formals;
  Cell* temp_body;
  temp_formals = (formals_m == nil) ? nil : formals_m->copy();
  temp_body = body_m->copy();
  return new ProcedureCell(temp_formals, temp_body);
}
