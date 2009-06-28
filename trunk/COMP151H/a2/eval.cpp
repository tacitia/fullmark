/**
 * \file eval.cpp
 *
 * Implementation of a eval that evaluate the expression stored in the tree 
 * data structure, by doing a depth-first traversal. The evaluator should 
 * return the value resulting from evaluating the entire expression. The eval 
 * deals with three basic types: ints, doubles and symbols. The operator symbols 
 * are +, -, *, /, if, ceiling, floor, quote, car, cdr, cons and nullp. The + 
 * and * operator accepts any number of operands to be added or multiplied. The 
 * operators - and / are called with 1 or more than one operands to do the minus 
 * or divide operation. The ceiling returns an int which is the smallest integer 
 * that is greater or equal to the operand. The floor returns an int which is 
 * the biggest integer that is less or equal to the oprand. The if operator 
 * first checks if the first operand is non-zero. If so, then it should return 
 * the result of evaluating the second operand. Otherwise, it should return the 
 * result of evaluating the third operand( if only two operands were given, then 
 * the return the result value is undefined ). The quote takes exactly one operand 
 * , which can be any s-expression, and simply returns the exact same expression.
 * The car returns the car pointer in the operand using deep copy. The cdr returns 
 * the cdr pointer in the operand using deep copy. The cons make a conspair cell 
 * using its two operands. The nullp check if c points to an empty list.
 */

#include "eval.hpp"
#include <cstring>

/**
 * \brief Check whether the pointer is nil or not(error: if nil).
 * \param A pointer which points to the operand to be checked.
 */
void test_nil(Cell* const c)
{
  if(nullp(c))
    {
      cerr<<"ERROR: The operand can't be nil for some operators!"<<endl;
      exit(1);
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
      cerr<<"ERROR: Zero can't be the operand for the operator '/'!"<<endl;
      exit(1);
    }

  return;
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
      cerr<<"ERROR: There is no operand for operators"<<endl;
      exit(1);
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
      cerr<<"ERROR: This is an empty tree!"<<endl;
      exit(1);
    }
  
  // check whether the cell pointed by c is ConsCell
  else if (listp(c))
    {
      Cell* eval_car1 = eval(car(c));   // store the pointer which points to the operator
      Cell* eval_car2 = nil;            // store the pointer which points to the operand
      Cell* eval_car3 = nil;            // store the pointer which points to the operand
      Cell* eval_car4 = nil;            // store the pointer which points to the operand
      
      // check whether the operator exist(error: if nil)
      test_nil(eval_car1);
      
      // do the "+" evaluation
      if (get_symbol(eval_car1) == "+")
	{
	  Cell* temp_ptr = cdr(c);    // store the cdr of c
	  double result = 0;          // store the sum result
	  bool has_double = false;    // record the existence of double
	  
	  // add each operands
	  while (temp_ptr != nil)
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      // check whether the operand is nil(error: if nil)
	      test_nil(eval_car2);
	      
	      result += eval_car2->arithmetic_o(has_double);
	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	    }
	  
	  destroy(eval_car1);
	  return has_double == true ? make_double(result) : make_int((int)result);
	}
      
      // do the "*" evaluation
      else if (get_symbol(eval_car1) == "*")
	{
	  Cell* temp_ptr = cdr(c);    // store the cdr of c
	  double result = 1;          // store the product
	  bool has_double = false;    // record the existence of double
	  
	  // multiply each operand
	  while (temp_ptr != nil)
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      // check whether the operand is nil(error: if nil)
	      test_nil(eval_car2);
	      
	      result *= eval_car2->arithmetic_o(has_double);
	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	    }
	  
	  destroy(eval_car1);
	  return has_double == true ? make_double(result) : make_int((int)result);
	}
      
      // do the "-" evaluation
      else if (get_symbol(eval_car1) == "-")
	{
	  Cell* temp_ptr = cdr(c);    // store the cdr of c
	  double result = 0;          // store the result
	  bool has_double = false;    // record the existence of double
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // return the negative of the argument if operand_num = 1
	  if (operand_num == 1)
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      // check whether the operand is nil(error: if nil)
	      test_nil(eval_car2);
	      
	      result = 0 - eval_car2->arithmetic_o(has_double);
	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	    }
	  else 
	    {
	      eval_car2 = eval(car(temp_ptr));
	      test_nil(eval_car2);
	      result = eval_car2->arithmetic_o(has_double);
	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	      
	      // minus each operand
	      while (temp_ptr != nil)
		{
		  eval_car2 = eval(car(temp_ptr));
		  test_nil(eval_car2);
		  result -= eval_car2->arithmetic_o(has_double);
		  destroy(eval_car2);
		  temp_ptr = cdr(temp_ptr);
		}
	    }

	  destroy(eval_car1);
	  return has_double == true ? make_double(result) : make_int((int)result);
	}
      
      // do the "/" evaluation
      else if (get_symbol(eval_car1) == "/")
	{
	  Cell* temp_ptr = cdr(c);    // store the cdr of c
	  int result_int = 1;         // store the int result
	  double result_double = 1;   // store the double result
	  bool has_double = false;    // record the existence of double
          double temp = 0;            // store the temperary operand	  

	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // return the inverse of the argument if operand_num = 1
	  if (operand_num == 1)
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
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

	      destroy(eval_car2);
	    }
	  else 
	    {
	      eval_car2 = eval(car(temp_ptr));
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

	      destroy(eval_car2);
	      temp_ptr = cdr(temp_ptr);
	      
	      // divide each operand
	      while (temp_ptr != nil)
		{
		  eval_car2 = eval(car(temp_ptr));
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

		  destroy(eval_car2);
		  temp_ptr = cdr(temp_ptr);
		}
	    }

	  destroy(eval_car1);
	  return has_double == true ? make_double(result_double) : make_int(result_int);
	}
      
      // do the "ceiling" evaluation
      else if (get_symbol(eval_car1) == "ceiling")
	{
	  Cell* result = nil;        // store the result
	  Cell* temp_ptr = cdr(c);   // store the cdr of c
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals one(error: if not)
	  if (operand_num != 1)
	    {
	      cerr<<"ERROR: The number of operands for the operator 'ceiling' ";
	      cerr<<"is more than 1!"<<endl;
	      exit(1);
	    }
	  
	  eval_car2 = eval(car(temp_ptr));

	  // check whether the operand is nil(error: if nil)
	  test_nil(eval_car2);
	  
	  result = eval_car2->ceiling_o();
	  destroy(eval_car1);
	  destroy(eval_car2);
	  return result;
	}
      
      // do the "floor" evaluation
      else if (get_symbol(eval_car1) == "floor")
	{
	  Cell* result = nil;        // store the result
	  Cell* temp_ptr = cdr(c);   // store the cdr of c
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals one(error: if not)
	  if (operand_num != 1)
	    {
	      cerr<<"ERROR: The number of operands for the operator 'floor' ";
	      cerr<<"is more than 1!"<<endl;
	      exit(1);
	    }
	  
	  eval_car2 = eval(car(temp_ptr));
	  
	  // check whether the operand is nil(error: if nil)
	  test_nil(eval_car2);
	  
	  result = eval_car2->floor_o();
	  destroy(eval_car1);
	  destroy(eval_car2);
	  return result;
	}
      
      // do the "if" evaluation
      else if (get_symbol(eval_car1) == "if")
	{
	  Cell* result = nil;        // store the result
	  Cell* temp_ptr1 = cdr(c);  // store the cdr of c
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr1);
	  
	  // check whether the number of operands equals two or three(error: if not)
	  if (operand_num != 2 && operand_num != 3)
	    {
	      cerr<<"ERROR: The number of operands for the operator 'if' ";
	      cerr<<"is more than 3 or less than 2!"<<endl;
	      exit(1);
	    }
	  else
	    {
	      Cell* temp_ptr2 = cdr(temp_ptr1);    // store the cdr of cdr of c
	      Cell* temp_ptr3 = cdr(temp_ptr2);    // store cdr of cdr of cdr of c
	      eval_car2 = eval(car(temp_ptr1));    // store fisrt operand
	      
	      // give the second operand if the first operand is non-zero
	      if (eval_car2 == nil || !eval_car2->is_zero())
		{
		  eval_car3 = eval(car(temp_ptr2));
		  destroy(eval_car1);
		  destroy(eval_car2);
		  return eval_car3;
		}
	      
	      // give the third operand if the first operand is zero
	      else
		{
		  // give the value of result 0 if the third operand doesn't exit
		  if (operand_num == 2)
		    {
		      destroy(eval_car1);
		      destroy(eval_car2);
		      return make_int(0);
		    }
		  
		  // give the third operand if the third operand exists
		  else
		    {
		      eval_car4 = eval(car(temp_ptr3));
		      destroy(eval_car1);
		      destroy(eval_car2);
		      return eval_car4;
		      
		    }
		}
	    }
	}
      
      // do the "qutoe" evaluation
      else if (get_symbol(eval_car1) == "quote")
	{
	  Cell* temp_ptr = cdr(c);    // store the cdr of c
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals one(error: if not)
	  if (operand_num != 1)
	    {
	      cerr<<"ERROR: The number of operands for 'quote' is more than one!"<<endl;
	      exit(1);
	    }
	  
	  destroy(eval_car1);
	  return (car(temp_ptr) == nil) ? nil : car(temp_ptr)->copy();
	}
      
      else if (get_symbol(eval_car1) == "cons")
	{
	  Cell* temp_ptr1 = cdr(c);
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr1);
	  
	  // check whether the number of operands equals 2(error: if not)
	  if (operand_num != 2)
	    {
	      cerr<<"ERROR: The numbers of operands for the operator 'cons' ";
	      cerr<<"is less than 2 or more than 2!"<<endl;
	      exit(1);
	    }
	  else
	    {
	      Cell* temp_ptr2 = cdr(temp_ptr1);
	      eval_car2 = eval(car(temp_ptr1));
	      eval_car3 = eval(car(temp_ptr2));
	      destroy(eval_car1);
	      return cons(eval_car2, eval_car3);
	    }	  
	}
      
      // do the "car" evaluation
      else if (get_symbol(eval_car1) == "car")
	{
	  Cell* temp_ptr = cdr(c);    // store the cdr of c
	  Cell* result = nil;         // store the result

	  // store the number of the operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals 1(error: if not)
	  if (operand_num != 1)
	    {
	      cerr<<"ERROR: The number of operands for the operator 'car' ";
	      cerr<<"is more than 1!"<<endl;
	      exit(1);
	    }
	  else
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      // check whether the operand is nil(error: if nil)
	      test_nil(eval_car2);
	      
	      result = car(eval_car2);
	      destroy(eval_car1);
	      destroy(cdr(eval_car2));
	      delete eval_car2;
	      return result;
	    }
	}
      
      // do the "cdr" evaluation
      else if (get_symbol(eval_car1) == "cdr")
	{
	  Cell* temp_ptr = cdr(c);    // store the cdr of c
	  Cell* result = nil;         // store the result
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals 1(error: if not)
	  if (operand_num != 1)
	    {
	      cerr<<"ERROR: The number of operands for the operator 'cdr' ";
	      cerr<<"is more than 1!"<<endl;
	      exit(1);
	    }
	  else
	    {
	      eval_car2 = eval(car(temp_ptr));
	      
	      // check whehter the operand is nil(error: if nil)
	      test_nil(eval_car2);
	      
	      result = cdr(eval_car2);
	      destroy(eval_car1);
	      destroy(car(eval_car2));
	      delete eval_car2;
	      return result;
	    }
	}
      
      // do the "nullp" evaluation
      else if (get_symbol(eval_car1) == "nullp")
	{
	  Cell* temp_ptr = cdr(c);    // store the cdr of c
	  bool result = false;        // store the result
	  
	  // store the number of operands(error: if zero)
	  int operand_num = test_operand_num(temp_ptr);
	  
	  // check whether the number of operands equals one(error: if not)
	  if (operand_num != 1)
	    {
	      cerr<<"ERROR: The number of operands for the operator 'nullp' ";
	      cerr<<"is more than 1!"<<endl;
	      exit(1);
	    }
	  else
	    {
	      eval_car2 = eval(car(temp_ptr));
	      result = nullp(eval_car2);
	      destroy(eval_car1);
	      destroy(eval_car2);
	      return make_int(result);
	    }
	}

      // check whether the operator is defined
      else
	{
	  cerr<<"ERROR: The operator is undefined!"<<endl;
	  exit(1);
	}
    }

  // return the IntCell, DoubleCell and SymbolCell by deep copy
  else
    {
      return c->copy();
    }
}

