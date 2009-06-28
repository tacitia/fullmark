/**
 * \mainpage COMP151H Programming Assignment 1, Spring 2008
 *
 * \author XIAO Sa
 * \author 07592272
 * \author cs_xsx@stu.ust.hk
 * \author T1A
 *
 * \date 2008.3.18
 *
 * Bonus: Proper destruction
 * Bonus: Detailed error messages
 * Bonus: Expressions like ((if 1 + ceiling) 3.1)
 * Instructor: <a href="http://www.cs.ust.hk/~dekai/">Dekai Wu</a>
 * Due: 2008.03.17 at 23:00 by CASS
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

using namespace std;

/**
 * \class Cell
 * \brief Class Cell
 */
class Cell {
public:
  /**
   * \brief Constructor to make int cell.
   */
  Cell(const int i);

  /**
   * \brief Constructor to make double cell.
   */
  Cell(const double d);

  /**
   * \brief Constructor to make symbol cell.
   */
  Cell(const char* const s);

  /**
   * \brief Constructor to make cons cell.
   */
  Cell(Cell* const my_car, Cell* const my_cdr);
  

  /**
   * \brief Check if this is an int cell.
   * \return True iff this is an int cell.
   */
  bool is_int() const;

  /**
   * \brief Check if this is a double cell.
   * \return True iff this is a double cell.
   */
  bool is_double() const;

  /**
   * \brief Check if this is a symbol cell.
   * \return True iff this is a symbol cell.
   */
  bool is_symbol() const;

  /**
   * \brief Check if this is a cons cell.
   * \return True iff this is a cons cell.
   */
  bool is_cons() const;

  /**
   * \brief Accessor (error if this is not an int cell).
   * \return The value in this int cell.
   */
  int get_int() const;

  /**
   * \brief Accessor (error if this is not a double cell).
   * \return The value in this double cell.
   */
  double get_double() const;

  /**
   * \brief Accessor (error if this is not a symbol cell).
   * \return The symbol name in this symbol cell.
   */
  string get_symbol() const;

  /**
   * \brief Accessor (error if this is not a cons cell).
   * \return First child cell.
   */
  Cell* get_car() const;

  /**
   * \brief Accessor only for cons cell(error if this is not a cons cell).
   * \return Rest child cell.
   */
  Cell* get_cdr() const;
  
  /**
   * \brief Print the subtree rooted at this cell, in s-expression notation.
   * \param os The output stream to print to.
   */
  void print(ostream& os = cout) const;

private:

  /**
   * \enum TypeTag
   * \brief enum TypeTag of four different types of cell
   */
  enum TypeTag {type_int, type_double,
		type_symbol, type_conspair};

  TypeTag tag_m;      //!< to tell us which of the data members is the valid one

  union
  {
    int int_m;        //!< to store int numbers if it's int cell
    double double_m;  //!< to store double numbers if it's double cell
    char* symbol_m;   //!< to point to the string if it's a symbol cell
    Cell* car_m;      //!< to point to another cell if it's a cons cell
  };

  Cell* cdr_m;        //!< to point to the next cell if it's a cons cell otherwise point to nil

};

// Reminder: cons.hpp expects nil to be defined somewhere (for this
// implementation, Cell.cpp is the logical place to define it).
// Here we promise this again, just to be safe.
extern Cell* const nil;

#endif // CELL_HPP
