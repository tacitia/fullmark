/**
 * \file eval.cpp
 *
 * Implementation of a eval that evaluate the expression stored in the tree 
 * data structure, by doing a depth-first traversal. The evaluator should 
 * return the value resulting from dvaluating the entire expression.
 */

#include "eval.hpp"
#include <cmath>
#include <cstring>

/**
 * \brief Evaluate the smallest integer that is greater than or equal to the
 * operand
 * \param operand The data to be ceiled
 * \return The smallest integer that is greater than or equal to the operand
 */
int ceiling (double operand)
{
  return (int)ceil(operand);
}

/**
 * \brief Check whether the string is one of the legal operators
 * \param str A string to be tested
 * \return true if the str is one of the legal operators
 */ 
bool is_legal_operator(string const str)
{
  if(str == "+" || str == "ceiling" || str == "if")
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * \brief Check whether the data stored in the cell pointed by c is zero
 * (error if c is nil or the cell is cons cell)
 * \param c The pointer which points to cell, the data in which to be checked
 * \return True if the data equals zero
 */
bool is_zero(Cell* const c)
{
  if (intp(c))
    {
      int operand = get_int(c);
      return (operand == 0 ? true : false);
    }
  else if (doublep(c))
    {
      double operand = get_double(c);
      return (operand == 0 ? true : false);
    }
  else if (symbolp(c))
    {
      string operand = get_symbol(c);
      return (operand == "" ? true : false);
    }
  else
    {
      cerr<<"ERROR: You can't use is_zero ";
      cerr<<"because it's a cons Cell pointed by c or c is a null pointer"<<endl;
      exit(1);
    }
}
  
/**
 * \brief Evaluate the expression stored in the tree data, by doing a depth-first
 * traveral. If there is any error in the types or number of operands in the input
 * expression, the function would print the string ERROR and give the user advice
 * to input the correct input expressions.
 * \param c A pointer which points to a tree stored s-expressions
 * \return A pointer which points to the value from evaluating the entire expression
 */
Cell* eval(Cell* const c)
{
  // check whether the pointer c is nil.
  if (nullp(c))
    {
      cerr<<"ERROR: There is an empty tree!"<<endl;
      cerr<<"ADVICE: You should input some operators and operands!"<<endl;
      exit(1);
    }

  // check whether the cell pointed by c is cons cell
  else if (listp(c))
    {
      Cell* eval_car1 = eval(car(c));   // store the pointer which points to the operator
      Cell* eval_car2 = nil;            // store the pointer which points to the operand
      Cell* eval_car3 = nil;            // store the pointer which points to the operand
      Cell* eval_car4 = nil;            // store the pointer which points to the operand
      
      // check whether the first element is operator(error if it's not)
      if (!symbolp(eval_car1))
	{
	  cerr<<"EEROR: There is no operator before operands!"<<endl;
	  cerr<<"ADVICE: You should input exactly one operator before operands!"<<endl;
	  exit(1);
	}

      // do the "+" evaluation
      if (get_symbol(eval_car1) == "+")
	{
	  Cell* temp_ptr = cdr(c);    // a temporary pointer to store the cdr of c
	  int int_sum = 0;            // store the sum of the whole int numbers 
	  double double_sum = 0;      // store the sum of the whole double numbers 
	  bool has_double = false;    // check whether double numbers exist in the expression
	  
	  // add the following operands in the experissions
	  while (temp_ptr != nil)
	    {
	      // get the pointer which points to the operand
	      eval_car2 = eval(car(temp_ptr));
	      
	      // accumulate the sum of int numbers
	      if (intp(eval_car2))
		{
		  int_sum += get_int(eval_car2);
		  delete eval_car2;
		}

	      // accumulate the sum of double numbers
	      else if (doublep(eval_car2))
		{
		  double_sum += get_double(eval_car2);
		  has_double = true;
		  delete eval_car2;
		}

	      //check the invalid operands for '+'(error:if invalid)
	      else 
		{
		  if (is_legal_operator(get_symbol(eval_car2)))
		    {
		      cerr<<"ERROR: The number of operators is more than one in one pair of braces!"<<endl;
		      cerr<<"ADVICE: You should input exactly one operator for each pair of braces!"<<endl;
		    }
		  else
		    {
		      cerr<<"ERROR: Illegal input characters!"<<endl;
		      cerr<<"ADVICE: You should input numbers as the operands for the operator '+'!"<<endl;
		    }
		  exit(1);
		}
	      temp_ptr = cdr(temp_ptr);
	    }
	  
	  // make the type of the result double if there exists double operand 
	  if (has_double == true)
	    {
	      delete eval_car1;
	      return make_double(double_sum + int_sum);
	    }

	  // make the type of the result int if there doesn't exist int operand
	  else
	    {
	      delete eval_car1;
	      return make_int(int_sum);
	    }
	}

      // do the 'ceiling' evaluation
      else if (get_symbol(eval_car1) == "ceiling")
	{
	  Cell* temp_ptr = cdr(c);    // a temporary pointer to store the cdr of c
	  
	  // check whether the number of operands is 0(error: if no operands)
	  if (temp_ptr == nil)
	    {
	      cerr<<"ERROR: There is no operand for the operator 'ceiling'!"<<endl;
	      cerr<<"ADVICE: You should input exactly one operand for the operator 'ceiling'!"<<endl;
	      exit(1);
	    }

	  // check whether the number of operands is more than 2(error: if more than 2)
	  if (!nullp(cdr(temp_ptr)))
	    {
	      cerr<<"ERROR: The number of operands for 'ceiling' is more than one!"<<endl;
	      cerr<<"ADVICE: You should input exactly one operand for the operator 'ceiling'!"<<endl;
	      exit(1);
	    }

	  eval_car2 = eval(car(temp_ptr));    // a pointer which points to the operand

	  // check whether the operands for ceiling is legal or not(error:if illegal)
	  if (symbolp(eval_car2))
	    {
	      if (is_legal_operator(get_symbol(eval_car2)))
		{
		  cerr<<"ERROR: The numbenr of operators is more than one in one pair of braces!"<<endl;
		  cerr<<"ADVICE: You should input exactly one operator for each pair of braces!"<<endl;
		}
	      else
		{
		  cerr<<"EEROR: Illegal input characters!"<<endl;
		  cerr<<"ADVICE: You should input numbers as operands for the operator 'ceiling'!"<<endl;
		}
	      exit(1);
	    }
	  else if (!doublep(eval_car2))
	    {
      	      cerr<<"ERROR: The operand for the operator 'ceiling' must be a double!"<<endl;
	      cerr<<"ADVICE: You should input a double as the operand for the operator 'ceiling'!"<<endl;
	      exit(1);
	    }

	  // do the ceiling if the expression is ilegal
	  else 
	    {
	      int result_ceiling = ceiling(get_double(eval_car2));    //store the value resulting from ceiling
	      delete eval_car1;
	      delete eval_car2;
	      return make_int(result_ceiling);
	    }
	}

      // do the 'if' evaluation
      else if (get_symbol(eval_car1) == "if")
	{
	  int result_if_int;            // store the int result
	  double result_if_double;      // store the double result 
	  string result_if_symbol;      // store the symbol result 
	  const char* result_if_ptr;    // a pointer which points to the symbol result
	  Cell* temp_ptr1 = cdr(c);     // a temporary pointer to store the cdr of c
	  
	  // check whether 'if' has a operand(error:if no)
	  if (nullp(temp_ptr1))
	    {
	      cerr<<"ERROR: There is no operand for the operator 'if'!"<<endl;	
	      cerr<<"ADVICE: You should input two or three operand for the operator 'if'!"<<endl;
	      exit(1);
	    }

	  // check whether 'if' has less than two operands(error:if no)
	  else if (nullp(cdr(temp_ptr1)))
	    {
	      cerr<<"ERROR: There is only one operand for the operator 'if'!"<<endl;
	      cerr<<"ADVICE: You should input two or three operand for the operator 'if'!"<<endl;
	      exit(1);
	    }

	  // do the if evaluation for the two or three operands
	  else if (nullp(cdr(cdr(temp_ptr1))) || nullp(cdr(cdr(cdr(temp_ptr1)))))
	    {
	      Cell* temp_ptr2 = cdr(temp_ptr1);    // a temporary pointer to store the cdr of cdr of c
	      Cell* temp_ptr3 = cdr(temp_ptr2);    // a temporary pointer to store the cdr of cdr of cdr of c
	      eval_car2 = eval(car(temp_ptr1));    // a pointer which points to the fisrt operand
	      
	      // give the second operand if the first operand is non-zero
	      if (!is_zero(eval_car2))
		{
		  eval_car3 = eval(car(temp_ptr2));    // a pointer which points to the second operand

		  if (intp(eval_car3))
		    {
		      result_if_int = get_int(eval_car3);
		      delete eval_car1;
		      delete eval_car2;
		      delete eval_car3;
		      return make_int(result_if_int);
		    }
		  else if (doublep(eval_car3))
		    {
		      result_if_double = get_double(eval_car3);
		      delete eval_car1;
		      delete eval_car2;
		      delete eval_car3;
		      return make_double(result_if_double);
		    }
		  else 
		    {
		      result_if_symbol = get_symbol(eval_car3);
		      result_if_ptr = result_if_symbol.c_str(); 
		      delete eval_car1;
		      delete eval_car2;
		      delete eval_car3;
		      return make_symbol(result_if_ptr);
		    }
		}

	      // give the third operand if the first operand is zero
	      else
		{
		  // check whether the expression has the third operand, if not give the value of result 0
		  if(nullp(temp_ptr3))
		  {
		    delete eval_car1;
		    delete eval_car2;
		    delete eval_car3;
		    return make_int(0);
		  }

		  // give the third operand if the third operand exists
		  else
		    {
		      eval_car4 = eval(car(temp_ptr3));    // a pointer which points to the third operand
		      
		      if (intp(eval_car4))
			{
			  result_if_int = get_int(eval_car4);
			  delete eval_car1;
			  delete eval_car2;
			  delete eval_car4;
			  return make_int(result_if_int);
			}
		      else if (doublep(eval_car4))
			{
			  result_if_double = get_double(eval_car4);
			  delete eval_car1;
			  delete eval_car2;
			  delete eval_car4;
			  return make_double(result_if_double);
			}
		      else 
			{
			  result_if_symbol = get_symbol(eval_car4);
			  result_if_ptr = result_if_symbol.c_str();
			  delete eval_car1;
			  delete eval_car2;
			  delete eval_car4;
			  return make_symbol(result_if_ptr);
			}		      
		      
		    }
		}
	    }

	  // check whether the 'if' has more than three operators(error:if yes)
	  else
	    {
	      cerr<<"ERROR: The number of operands is more than three for the operator 'if'!"<<endl;
	      cerr<<"ADVICE: You should input two or three operand for the operator 'if'!"<<endl;
	      exit(1);
	    }
	}

      // check whether the expression has the operator before the operands(error:if no)
      else
	{
	  cerr<<"ERROR: There is no operator before operands!"<<endl;
	  cerr<<"ADVICE: You should input exactly one operator before operands!"<<endl;
	  exit(1);
	}
    }

  // if the tree has only one int cell, return the pointer which points to the same int cell
  else if (intp(c))
    {
      return make_int(get_int(c));
    }

  // if the tree has only double cell, return the pointer which points to the same double cell
  else if (doublep(c))
    {
      return make_double(get_double(c));
    }

  // if the tree has only symbol cell, return the pointer which points to the same symbol cell
  else
    {
      string temp = get_symbol(c);
      const char* ptr = temp.c_str();
      return make_symbol(ptr);
    }
}





