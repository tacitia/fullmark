/**
 * \mainpage COMP151H Programming Assignment 4, Spring 2008
 *
 * \author XIAO Sa
 * \author 07592272
 * \author cs_xsx@stu.ust.hk
 * \author TA
 *
 * \date 2008.5.6
 *
 * Bonus: Detailed error messages
 * 
 * Acknowlegement: 
 * 
 * Instructor: <a href="http://www.cs.ust.hk/~dekai/">Dekai Wu</a>
 * Due: 2008.04.13 at 23:00 by CASS
 */

/**
 * \file Cell.hpp
 *
 * Encapsulates the abstract interface for a concrete class-based
 * implementation of cells for a cons list data structure.
 */

#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

/**
 * \class Cell
 * \brief Class Cell
 */
class Cell {
public:
  /**
   * \brief Check whether this is an IntCell.
   * \return False if this is not an IntCell.
   */
  virtual bool is_int() const;
  
  /**
   * \brief Check whether this is a DoubleCell.
   * \return False if this is not a DoubleCell.
   */
  virtual bool is_double() const;
  
  /**
   * \brief Check whether this is a SymbolCell.
   * \return False if this is not a SymbolCell.
   */
  virtual bool is_symbol() const;
  
  /**
   * \brief Check whether this is a ConsCell.
   * \return False if this is not a ConsCell.
   */
  virtual bool is_cons() const;

  /**
   * \brief Check whether this is a ProcedureCell.
   * \return False if this is not a ProcedureCell.
   */
  virtual bool is_procedure() const;

  /**
   * \brief Accessor (error: if this is not an IntCell).
   * \return The value in this IntCell.
   */
  virtual int get_int() const;
  
  /**
   * \brief Accessor (error: if this is not a DoubleCell).
   * \return The value in this DoubleCell.
   */
  virtual double get_double() const;
  
  /**
   * \brief Accessor (error: if this is not a SymbolCell).
   * \return The symbol name in this SymbolCell.
   */
  virtual string get_symbol() const;
  
  /**
   * \brief Accessor (error: if this is not a ConsCell).
   * \return First child cell.
   */
  virtual Cell* get_car() const;
  
  /**
   * \brief Accessor (error: if this is not a ConsCell).
   * \return Rest child cell.
   */
  virtual Cell* get_cdr() const;

  /**
   * \brief Accessor (error: if this is not a ProcedureCell).
   * \return Pointer to the cons list of formal parameters for the function.
   */
  virtual Cell* get_formals() const;

  /**
   * \breif Accessor (error: if this is not a ProcedureCell).
   * \return Pointer to the cons list containing the expression defining the
   * body for the function pointed to by c.
   */
  virtual Cell* get_body() const;

  /**
   * \brief Accessor(error: if this is a ConsCell).
   * \return The pointer which points to the value.
   */
  virtual Cell* get_value() const;
  
  /**
   * \brief Accessor (error: if this is not an IntCell or DoubleCell).
   * \return The value in this IntCell or DoubleCell.
   */
  virtual double arithmetic_o(bool& has_double) const;
  
  /**
   * \brief Accessor (error: if this is not a DoubleCell).
   * \return The value of ceiling result for double data member.
   */
  virtual Cell* ceiling_o() const;
  
  /**
   * \brief Accessor (error: if this is not a DoubleCell).
   * \return The value of floor result for double data member.
   */
  virtual Cell* floor_o() const;
  
  /**
   * \brief Accessor(error: if this is not an IntCell or DoubleCell).
   * \return The value in this IntCell or DoubleCell.
   */
  virtual double less_o() const;

  /**
   * \brief Define a symbol(error: if this is not a SymbolCell).
   * \param value The value to assign to the symbol.
   */
  virtual void define_o(Cell*& value) const;

  /**
   * \brief Apply the argument to function (error: if this is not a ProcedureCell).
   * \param args The arguement list.
   * \return pointer store the function expressions.
   */
  virtual Cell* apply(Cell* const args) const;

  /**
   * \brief Check whether the data is zero
   * \return True if the data is zero.
   */
  virtual bool is_zero() const = 0;
  
  /**
   * \brief Copy the tree deeply.
   * \return The Cell pointer which points to the whole tree.
   */
  virtual Cell* copy() const = 0;
  
  /**
   * \brief Print the subtree rooted at this cell, in s-expression notation.
   * \param os The output stream to print to.
   */
  void print(ostream& os = cout) const;
};

/**
 * \class IntCell
 * \brief Class IntCell contains the implements and int datas for integers.
 */
class IntCell : public Cell
{
public:
  /** 
   * \brief To create an IntCell object.
   * \param i The value assigned to the data member.
   */
  IntCell(const int i);
  
  /**
   * \brief Check whether this is an IntCell.
   * \return True if this is an IntCell.
   */
  virtual bool is_int() const;

  /**
   * \brief Accessor (error: if this is not an IntCell).
   * \return The value in this IntCell.
   */
  virtual int get_int() const;
  
  /**
   * \brief Accessor(error: if this is a ConsCell).
   * \return The pointer which points to the value.
   */
  virtual Cell* get_value() const;

  /**
   * \brief Accessor (error: if this is not an IntCell or DoubleCell).
   * \return The value in the IntCell.
   */
  virtual double arithmetic_o(bool& has_double) const;

  /**
   * \brief Accessor (error: if this is not an IntCell or DoubleCell).
   * \return The value in the IntCell.
   */
  virtual double less_o() const;
  
  /**
   * \brief Check whether the data is zero.
   * \return True if the data in the IntCell equals zero.
   */
  virtual bool is_zero() const;
  
  /**
   * \brief Copy the tree deeply
   * \return The Cell pointer which points to the whole tree.
   */
  virtual Cell* copy() const;
private:
  int int_m;    //!< to store the int data
};

/**
 * \class DoubleCell
 * \brief Class DoubleCell contains the implements and double datas for doubles.
 */
class DoubleCell : public Cell
{
public:
  /**
   * \brief To create an DoubleCell object.
   * \param d The value assigned to the data members.
   */
  DoubleCell(const double d);
  
  /**
   * \brief Check whether this is an DoubleCell.
   * \return True if this is an DoubleCell.
   */
  virtual bool is_double() const;

  /**
   * \brief Accessor (error: if this is not an DoubleCell).
   * \return The value in the DoubleCell.
   */
  virtual double get_double() const;

  /**
   * \brief Accessor(error: if this is a ConsCell).
   * \return The pointer which points to the value.
   */
  virtual Cell* get_value() const;
  
  /**
   * \brief Accessor (error: if this is not an IntCell or DoubleCell).
   * \return The value in the DoubleCell.
   */
  virtual double arithmetic_o(bool& has_double) const;
  
  /**
   * \brief Accessor (error: if this is not a DoubleCell).
   * \return The value of ceiling result for double data member.
   */
  virtual Cell* ceiling_o() const;
  
  /**
   * \brief Accessor (error: if this is not a DoubleCell).
   * \return The value of floor result for double data member.
   */
  virtual Cell* floor_o() const;
  
  /**
   * \brief Accessor (error: if this is not an IntCell or DoubleCell).
   * \return The value in the DoubleCell.
   */
  virtual double less_o() const;
  
  /**
   * \brief Check whether the data is zero.
   * \return True if the data is zero.
   */
  virtual bool is_zero() const;
  
  /**
   * \brief Copy the tree deeply.
   * \return The Cell pointer which points to the whole tree.
   */
  virtual Cell* copy() const;
private:
  double double_m;    //!< to store the double data 
};

/**
 * \class SymbolCell
 * \brief Class SymbolCell contains the implements and symbol data for symbol names.
 */
class SymbolCell : public Cell
{
public:
  /**
   * \brief To create an SymbolCell object.
   * \param s The pointer pointed to symbol names.
   */
  SymbolCell(const char* const s);
  
  /**
   * \brief Define a symbol.
   * \param value The value to assign to the symbol
   */
  virtual void define_o(Cell*& value) const;

  /**
   * \brief Check whether this is a SymbolCell.
   * \return True if this is a SymbolCell.
   */
  virtual bool is_symbol() const;

  /**
   * \brief Accessor (error: if this is not a SymbolCell).
   * \return The value in this SymbolCell.
   */
  virtual string get_symbol() const;
  
  /**
   * \brief Accessor(error: if this is a ConsCell).
   * \return The pointer which points to the value.
   */
  virtual Cell* get_value() const;
  
  /**
   * \brief Check whether the data is zero.
   * \return False if the data is nonzero.
   */
  virtual bool is_zero () const;
  
  /**
   * \brief Copy the tree deeply
   * \return The Cell pointer which points to the whole tree.
   */
  virtual Cell* copy() const;
private:
  char* symbol_m;    //!< to point to a symbol name
};

/**
 * \class ConsCell
 * \brief Class ConsCell contains the implements and cell pointers for sub-expressions.
 */
class ConsCell : public Cell
{
public:
  /**
   * \brief To create an ConsCell object.
   * \param my_car The initial car pointer to be stored in the new cell.
   * \parma my_cdr The initial cdr pointer to be stored in the new cell.
   */
  ConsCell(Cell* const my_car, Cell* const my_cdr);
  
  /**
   * \brief Check whether this is a ConsCell.
   * \return True if this is a ConsCell.
   */
  virtual bool is_cons() const;
  
  /**
   * \brief Accessor (error: if this is not a ConsCell).
   * \return First child cell.
   */
  virtual Cell* get_car() const;
  
  /**
   * \brief Accessor (error: if this is not an ConsCell).
   * \return Rest child cell.
   */
  virtual Cell* get_cdr() const;
  
  /**
   * \brief Check whether the data is zero.
   * \return False if the data is nonzero.
   */
  virtual bool is_zero() const;
  
  /**
   * \brief Copy the tree deeply.
   * \return The Cell pointer which points to the whole tree.
   */
  virtual Cell* copy() const;
private:
  Cell* car_m;    //!< to point to the first child
  Cell* cdr_m;    //!< to point to the rest child
};

/**
 * \class ProcedureCell
 * \brief Class ProcedureCell holds the newly constructed function value.
 */
class ProcedureCell : public Cell
{
public:
  /**
   * \brief To create an ProcedureCell object.
   * \param my_formals A list of procedure's frmal parameter names.
   * \param my_body The body (an expression) of the procedure.
   */
  ProcedureCell(Cell* const  my_formals, Cell* const my_body);

  /**
   * \brief Accessor (error: if this is not a ProcedureCell).
   * \return Pointer to the cons list of formal parameters for the function.
   */
  virtual Cell* get_formals() const;

  /**
   * \breif Accessor (error: if this is not a ProcedureCell).
   * \return Pointer to the cons list containing the expression defining the
   * body for the function pointed to by c.
   */
  virtual Cell* get_body() const;

  /**
   * \brief Accessor (error: if this is a ConsCell).
   * \return The value in this SymbolCell.
   */
  virtual Cell* get_value() const;

  /**
   * \brief Apply the argument to function (error: if this is not a ProcedureCell).
   * \param args The arguement list.
   * \return pointer store the function expressions.
   */
  virtual Cell* apply(Cell* const args) const;

  /**
   * \brief Check whether this is a ProcedureCell.
   * \return True if this is a ProcedureCell.
   */
  virtual bool is_procedure() const;

  /**
   * \brief Check whether the data is zero.
   * \return False if the data is nonzero.
   */
  virtual bool is_zero() const;

  /**
   * \brief Copy the tree deeply.
   * \return The Cell pointer which points to the whole tree.
   */
  virtual Cell* copy() const;
private:
  Cell* formals_m;    //!< to point to the list of arguments
  Cell* body_m;       //!< to point to the list of expressions in the body
};



// Reminder: cons.hpp expects nil to be defined somewhere (for this
// implementation, Cell.cpp is the logical place to define it).
// Here we promise this again, just to be safe.
extern Cell* const nil;
extern vector < map <string, Cell*> > v;
extern vector < map <string, Cell*> >::iterator it_vector;
#endif // CELL_HPP
