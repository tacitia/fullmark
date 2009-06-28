/**
 * \file eval.cpp
 *
 * Implementation of a eval that evaluate the expression stored in the tree 
 * data structure, by doing a depth-first traversal. The evaluator should 
 * return the value resulting from evaluating the entire expression. The eval 
 * deals with three basic types: ints, doubles and symbols. The operator symbols 
 * are +, -, *, /, <, not, define, if, ceiling, floor, quote, car, cdr, cons, 
 * nullp, print and eval. The + and * operator accepts any number of operands to 
 * be added or multiplied. The operators - and / are called with 1 or more than 
 * one operands to do the minus or divide operation. The ceiling returns an int 
 * which is the smallest integer that is greater or equal to the operand. The 
 * floor returns an int which is the biggest integer that is less or equal 
 * to the oprand. The if operator first checks if the first operand is non-zero. 
 * If so, then it should return the result of evaluating the second operand. 
 * Otherwise, it should return the result of evaluating the third operand( if 
 * only two operands were given, then the return the result value is undefined ). 
 * The quote takes exactly one operand , which can be any s-expression, and simply 
 * returns the exact same expression. The car returns the car pointer in the operand 
 * using deep copy. The cdr returns the cdr pointer in the operand using deep copy. 
 * The cons make a conspair cell using its two operands. The nullp check if c points 
 * to an empty list. The < operator accepts zero or more operands, each of which can
 * be either int or double, returning 0 if any two consecutive operands are not 
 * monotonically increasing, and 1 otherwise. The not operator accepts exactly one
 * operand, returning 1 if the operand is zero(either int or double), and 0 otherwise.
 * The print operator accepts one operandm which is evaluates, printing the resulting 
 * value onto the cout output stream. The return value from a print expression is 
 * always nil. The eval operator accepts one operand, and returns the result of
 * evaluating the operand.
 */

#include "eval.hpp"

/**
 * \brief Check whether the pointer is nil or not(error: if nil).
 * \param A pointer which points to the operand to be checked.
 */
void test_nil(Cell* const c)
{
  if(nullp(c))
    {
      throw runtime_error("The operand can't be nil for some operators!");
    }
}

/**
 * \brief Check whether the data in the cell is zero or not(error: if zero).
 * \param c A pointer which points to the operand to be checked.
 */
void test_zero(Cell* const c)
{
  if(c->is_zero())
    {
      throw runtime_error("Zero can't be the operand for the operator '/'!");
    }
}

/**
 * \brief Get the total number of the operands(error: if zero).
 * \param c A pointer which points to the first operand.
 * \return The number of operands.
 */
int test_operand_num(Cell* const c)
{
  if (nullp(c))
    {
      throw runtime_error("There is no operand for operators");
    }
  else
    {
      Cell* temp_ptr = c;
      int num = 0;
      
      while (!nullp(temp_ptr))
	{
	  num++;
	  temp_ptr = cdr(temp_ptr);
	}
      return num;
    }
}

/**
 * \brief Destroy the tree pointed by c.
 * \param c A pointer which points to the tree to be destroyed.
 */
void destroy(Cell* c)
{
  if (nullp(c))
    {
      return;
    }
  else if (intp(c) || doublep(c) || symbolp(c))
    {
      delete c;
      return;
    }
  else
    {
      destroy(car(c));
      destroy(cdr(c));
      delete c;
      return;
    }
}

/**
 * \param c A pointer which points to a tree stored s-expressions
 * \return The value resulting from evaluating the expression.
 */
Cell* eval(Cell* const c)
{
  // check whether the pointer is nil(error: if nil)
  if (nullp(c))
    {
      throw runtime_error("The empty list () can't be evaluated!");
    }
  
  // check whether the cell pointed by c is ConsCell
  else if (listp(c))
    {
      Cell* eval_car1 = car(c);
      Cell* eval_car2 = nil;
      Cell* eval_car3 = nil;
      Cell* eval_car4 = nil;
      string operators = "";
      
      // check whether the operator exist(error: if nil)
      if (nullp(eval_car1))
	{
	  throw runtime_error("There is no operator!");
	}
      
      // do the evaluation if it's a ConsCell
      if (listp(eval_car1))
	{
	  eval_car1 = eval(eval_car1);
	  
	  try
	    {
	      if (nullp(eval_car1))
		{
		  throw runtime_error("There is no operator!");
		}
	      operators = get_symbol(eval_car1);
	    }
	  catch(...)
	    {
	      destroy(eval_car1);
	      throw;
	    }
	  destroy(eval_car1);
	}
      
      // get the operators if it's a SymbolCell
      else
	{
	  operators = get_symbol(eval_car1);
	}
      
      // do the "+" evaluation
      if (operators == "+")
	{
	  Cell* temp_ptr = cdr(c);
	  double result = 0;
	  bool has_double = false;
	  
	  // add each operands
	  while (!nullp(temp_ptr))
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  result += eval_car2->arithmetic_o(has_double);
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	    }
	  return has_double == true ? make_double(result) : make_int((int)result);
	}
      
      // do the "*" evaluation
      else if (operators == "*")
	{
	  Cell* temp_ptr = cdr(c);
	  double result = 1;
	  bool has_double = false;
	  
	  // multiply each operand
	  while (!nullp(temp_ptr))
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  result *= eval_car2->arithmetic_o(has_double);
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	    }
	  return has_double == true ? make_double(result) : make_int((int)result);
	}
      
      // do the "-" evaluation
      else if (operators == "-")
	{
	  Cell* temp_ptr = cdr(c);
	  double result = 0;
	  bool has_double = false;
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // return the negative of the argument if operand_num = 1
	  if (operand_num == 1)
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  result = 0 - eval_car2->arithmetic_o(has_double);
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(eval_car2);
	    }
	  else 
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{
		  test_nil(eval_car2);
		  result = eval_car2->arithmetic_o(has_double);
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	      
	      // minus each operand
	      while (!nullp(temp_ptr))
		{
		  eval_car2 = eval(car(temp_ptr));
		  
		  try
		    {
		      test_nil(eval_car2);
		      result -= eval_car2->arithmetic_o(has_double);
		    }
		  catch(...)
		    {
		      destroy(eval_car2);
		      throw;
		    }
		  destroy(eval_car2);
		  temp_ptr = cdr(temp_ptr);
		}
	    }
	  return has_double == true ? make_double(result) : make_int((int)result);
	}
      
      // do the "/" evaluation
      else if (operators == "/")
	{
	  Cell* temp_ptr = cdr(c);
	  int result_int = 1;
	  double result_double = 1;
	  bool has_double = false;
          double temp = 0;
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // return the inverse of the argument if operand_num = 1
	  if (operand_num == 1)
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  
		  // check whether the operand equal zero(error: if zero)
		  test_zero(eval_car2);
		  temp = eval_car2->arithmetic_o(has_double);
		  
		  if (has_double)
		    {
		      result_double = 1 / temp;
		    }
		  else
		    {
		      result_int = 1 / (int)temp;
		    }
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(eval_car2);
	    }
	  else 
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{
		  test_nil(eval_car2);
		  temp = eval_car2->arithmetic_o(has_double);
		  
		  if (has_double)
		    {
		      result_double = temp;
		    }
		  else
		    {
		      result_int = (int)temp;
		    }
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	      
	      // divide each operand
	      while (!nullp(temp_ptr))
		{
		  eval_car2 = eval(car(temp_ptr));
		  
		  try
		    {
		      test_nil(eval_car2);
		      test_zero(eval_car2);
		      temp = eval_car2->arithmetic_o(has_double);
		      
		      if (has_double && result_int == 1)
			{
			  result_double /= temp;
			}
		      else if (has_double && result_int != 1)
			{
			  result_double = result_int / temp;
			  result_int = 1;
			}
		      else
			{
			  result_int /= (int)temp;
			}
		    }
		  catch(...)
		    {
		      destroy(eval_car2);
		      throw;
		    }
		  destroy(eval_car2);
		  temp_ptr = cdr(temp_ptr);
		}
	    }
	  return has_double == true ? make_double(result_double) : make_int(result_int);
	}
      
      // do the "<" evaluation
      else if (operators == "<")
	{
	  Cell* temp_ptr = cdr(c);
	  bool result = true;
	  double temp1 = 0;
	  double temp2 = 0;
	  
	  if (!nullp(temp_ptr))
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  temp1 = eval_car2->less_o();
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	      
	      // compare each operands
	      while (!nullp(temp_ptr))
		{
		  eval_car2 = eval(car(temp_ptr));
		  
		  try
		    {
		      test_nil(eval_car2);
		      temp2 = eval_car2->less_o();
		    }
		  catch(...)
		    {
		      destroy(eval_car2);
		      throw;
		    }
		  destroy(eval_car2);
		  
		  // if temp1 is larger than temp2, then the result is false
		  if (temp1 >= temp2)
		    {
		      result = false;
                      temp_ptr = cdr(temp_ptr);
                      temp1 = temp2;
		    }
		  else
		    {
		      temp_ptr = cdr(temp_ptr);
		      temp1 = temp2;
		    }
		}
	    }
	  return make_int((int)result);
	}
      
      // do the "not" evaluation
      else if (operators == "not")
	{
	  bool result = true;
	  Cell* temp_ptr = cdr(c);
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  if (operand_num != 1)
	    {
	      throw runtime_error("The number of operands for the operator 'not' is more than 1!");
	    }
	  eval_car2 = eval(car(temp_ptr));
	  
	  // check whether the operand is nonzero
	  if (nullp(eval_car2) || !eval_car2->is_zero())
	    {
	      result = false;
	    }
	  destroy(eval_car2);
	  return make_int((int)result);
	}
      
      // do the "ceiling" evaluation
      else if (operators == "ceiling")
	{
	  Cell* result = nil;
	  Cell* temp_ptr = cdr(c);

	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals one(error: if not)
	  if (operand_num != 1)
	    {
	      throw runtime_error("The number of operands for the operator 'ceiling' is more than 1!");
	    }
	  eval_car2 = eval(car(temp_ptr));
	  
	  try
	    {
	      // check whether the operand is nil(error: if nil)
	      test_nil(eval_car2);
	      result = eval_car2->ceiling_o();
	    }
	  catch(...)
	    {
	      destroy(eval_car2);
	      throw;
	    }
	  destroy(eval_car2);
	  return result;
	}
      
      // do the "floor" evaluation
      else if (operators == "floor")
	{
	  Cell* result = nil;
	  Cell* temp_ptr = cdr(c);
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals one(error: if not)
	  if (operand_num != 1)
	    {
	      throw runtime_error("The number of operands for the operator 'floor' is more than 1!");
	    }
	  eval_car2 = eval(car(temp_ptr));
	  
	  try
	    {
	      // check whether the operand is nil(error: if nil)
	      test_nil(eval_car2);
	      result = eval_car2->floor_o();
	    }
	  catch(...)
	    {
	      destroy(eval_car2);
	      throw;
	    }
	  destroy(eval_car2);
	  return result;
	}
      
      // do the "if" evaluation
      else if (operators == "if")
	{
	  Cell* result = nil;
	  Cell* temp_ptr1 = cdr(c);
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr1);
	  
	  // check whether the number of operands equals two or three(error: if not)
	  if (operand_num != 2 && operand_num != 3)
	    {
	      throw runtime_error("The number of operands for the operator 'if' is more than 3 or less than 2!");
	    }
	  else
	    {
	      Cell* temp_ptr2 = cdr(temp_ptr1);
	      Cell* temp_ptr3 = cdr(temp_ptr2);
	      eval_car2 = eval(car(temp_ptr1));
	      
	      // give the second operand if the first operand is non-zero
	      if (nullp(eval_car2) || !eval_car2->is_zero())
		{
		  destroy(eval_car2);
		  eval_car3 = eval(car(temp_ptr2));
		  return eval_car3;
		}
	      
	      // give the third operand if the first operand is zero
	      else
		{
		  // give the value of result 0 if the third operand doesn't exit
		  if (operand_num == 2)
		    {
		      destroy(eval_car2);
		      return make_int(0);
		    }
		  
		  // give the third operand if the third operand exists
		  else
		    {
		      destroy(eval_car2);
		      eval_car4 = eval(car(temp_ptr3));
		      return eval_car4;
		    }
		}
	    }
	}
      
      // do the "define" evaluation
      else if (operators == "define")
	{
	  Cell* temp_ptr1 = cdr(c);
	  
	  // store the number of operands equals one(error: if not)
	  int operand_num = test_operand_num(temp_ptr1);
	  
	  // check whether the number of oeprands equals two(error: if not)
	  if (operand_num != 2)
	    {
	      throw runtime_error("The number of operands for 'define' is less than or larger than 2!");
	    }
	  else
	    {
	      Cell* temp_ptr2 = cdr(temp_ptr1);
	      eval_car2 = car(temp_ptr1);
	      eval_car3 = eval(car(temp_ptr2));

	      try
		{
		  test_nil(eval_car2);
		  
		  // define the symbol(error: if redefine)
		  eval_car2->define_o(eval_car3);
		}
	      catch(...)
		{
		  destroy(eval_car3);
		  throw;
		}
	      return nil;
	    }
	}
      
      // do the "quote" evaluation
      else if (operators == "quote")
	{
	  Cell* temp_ptr = cdr(c);
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals one(error: if not)
	  if (operand_num != 1)
	    {
	      throw runtime_error("The number of operands for 'quote' is more than one!");
	    }
	  return (car(temp_ptr) == nil) ? nil : car(temp_ptr)->copy();
	}
      
      else if (operators == "cons")
	{
	  Cell* temp_ptr1 = cdr(c);
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr1);
	  
	  // check whether the number of operands equals 2(error: if not)
	  if (operand_num != 2)
	    {
	      throw runtime_error("The numbers of operands for the operator 'cons' is less than 2 or more than 2!");
	    }
	  else
	    {
	      Cell* temp_ptr2 = cdr(temp_ptr1);
	      eval_car2 = eval(car(temp_ptr1));
	      
	      try
		{
		  eval_car3 = eval(car(temp_ptr2));
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      
	      try
		{
		  return cons(eval_car2, eval_car3);
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  destroy(eval_car3);
		  throw;
		}
	    }	  
	}
      
      // do the "car" evaluation
      else if (operators == "car")
	{
	  Cell* temp_ptr = cdr(c);
	  Cell* result = nil;
	  
	  // store the number of the operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals 1(error: if not)
	  if (operand_num != 1)
	    {
	      throw runtime_error("The number of operands for the operator 'car' is more than 1!");
	    }
	  else
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  result = car(eval_car2);
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(cdr(eval_car2));
	      delete eval_car2;
	      return result;
	    }
	}
      
      // do the "cdr" evaluation
      else if (operators == "cdr")
	{
	  Cell* temp_ptr = cdr(c);
	  Cell* result = nil;
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals 1(error: if not)
	  if (operand_num != 1)
	    {
	      throw runtime_error("The number of operands for the operator 'cdr' is more than 1!");
	    }
	  else
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{	      
		  // check whehter the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  result = cdr(eval_car2);
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(car(eval_car2));
	      delete eval_car2;
	      return result;
	    }
	}
      
      // do the "nullp" evaluation
      else if (operators == "nullp")
	{
	  Cell* temp_ptr = cdr(c);
	  bool result = false;
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals one(error: if not)
	  if (operand_num != 1)
	    {
	      throw runtime_error("The number of operands for the operator 'nullp' is more than 1!");
	    }
	  else
	    {
	      eval_car2 = eval(car(temp_ptr));
	      result = nullp(eval_car2);
	      destroy(eval_car2);
	      return make_int(result);
	    }
	}
      
      // do the "print" evaluation
      else if (operators == "print")
	{
	  Cell* temp_ptr = cdr(c);
	  
	  // store the number of operands (error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals 1
	  if (operand_num != 1)
	    {
	      throw runtime_error("The number of operands for the operator 'print' is more than 1!");
	    }
	  else
	    {
	      eval_car2 = eval(car(temp_ptr));
	      eval_car2->print();
	      cout<<endl;
	      destroy(eval_car2);
	      return nil;
	    }
	}
      
      // do the "eval" evaluation
      else if (operators == "eval")
	{
	  Cell* temp_ptr = cdr(c);
	  Cell* result = nil;
	  
	  // store the number of operands (error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals 1
	  if (operand_num != 1)
	    {
	      throw runtime_error("The number of operands for the operator 'eval' is more than 1!");
	    }
	  else
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      try
		{
		  // evaluate the expression
		  result = eval(eval_car2);
		}
	      catch(...)
		{
		  destroy(eval_car2);
		  throw;
		}
	      destroy(eval_car2);
	      return result;
	    }
	}
      
      // check whether the operator is defined
      else
	{
	  throw runtime_error("The operator is undefined!");
	}
    }
  
  // return the value of IntCell, DoubleCell and SymbolCell
  else
    {
      return c->get_value();
    }
}

