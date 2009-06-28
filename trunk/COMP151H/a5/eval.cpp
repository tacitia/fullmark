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
 * \param c A pointer which points to the operand to be checked.
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
	  ++num;
	  temp_ptr = cdr(temp_ptr);
	}
      return num;
    }
}

/**
 * \brief Check whether it's an operator.
 * \param c A pointer which points to the Cell to be checked.
 * \return True if it's an operator.
 */
bool is_operator(Cell* const c)
{
  // check whethr it's an operator
  if (symbolp(c))
    {
      string operators = get_symbol(c);
      if (operators == "+" || operators == "-" || operators == "listp"
	  || operators == "*" || operators == "/" 
	  || operators == "apply" || operators == "let"
	  || operators == "ceiling" || operators == "floor"
	  || operators == "if" || operators == "define"
	  || operators == "quote" || operators == "not"
	  || operators == "<" || operators == "print"
	  || operators == "car" || operators == "cdr"
	  || operators == "cons" || operators == "nullp"
	  || operators == "lambda" || operators == "eval")
	{
	  return true;
	}
      else
	{
	  return false;
	}
    }
  else
    {
      return false;
    }
}


/**
 * \param c A pointer which points to a list to be evaluated.
 * \return The pointer which pointes to a list which has been evaluated.
 */
Cell* make_list(Cell* const c)
{
  if (c == nil)
    {
      return c;
    }
  else
    {
      return cons(eval(car(c)), make_list(cdr(c)));
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
      Cell* eval_car1 = eval(car(c));
      Cell* eval_car2 = nil;
      Cell* eval_car3 = nil;
      Cell* eval_car4 = nil;
      string operators = "";
      
      // apply the function if the first operand is ProcedureCell
      if (procedurep(eval_car1))
	{
	  Cell* my_formals = get_formals(eval_car1);
	  Cell* my_body = get_body(eval_car1);
	  Cell* temp_ptr = cdr(c);
	  Cell* return_value = nil;
	  
	  // constuct a local_table
	  map <string, Cell*> local_table;
	  
	  // the formal is a symbol
	  if (symbolp(my_formals))
	    {
	      int args_num = (temp_ptr == nil) ? 0 : test_operand_num(temp_ptr);
	      int body_num = test_operand_num(my_body);
	      Cell* list_head = nil;
	      
	      // evaluate each arguments if the number of arguments is nonzero
	      if (args_num != 0)
		{
		  list_head = make_list(temp_ptr);
		}
	      
	      // check whether the symbol is basic operators
	      if (is_operator(my_formals))
		{
		  throw runtime_error("The basic operators can't be redefined");
		}

	      // push a local table into vector
	      v.push_back(local_table);
	      try
		{
		  my_formals->define_o(list_head);

		  // evaluate the body of function
		  for (int i = 1; i <= body_num; ++i)
		    {
		      return_value = eval(car(my_body));
		      my_body = cdr(my_body);
		    }
		}
	      catch(...)
		{
		  v.pop_back();
		  throw;
		}
	      v.pop_back();
	      return return_value;
	    }
	  
	  // the formal is a symbol list
	  else
	    {
	      int formals_num = (my_formals == nil) ? 0 : test_operand_num(my_formals);
	      int args_num = (temp_ptr == nil) ? 0 : test_operand_num(temp_ptr);
	      int body_num = test_operand_num(my_body);
	      
	      // check whether the number of arguments equals to the number of formals
	      if (formals_num != args_num)
		{
		  throw runtime_error("The number of arguments are different from the number of formals of funtion");
		}
	      else
		{
		  // construct a vector to store the result of evaluation of arguments
		  vector <Cell*> formals_v;
		  
		  // evalute the arguments
		  for (int i = 0; i < formals_num; ++i)
		    {
		      formals_v.push_back(eval(car(temp_ptr)));
		      temp_ptr=cdr(temp_ptr);
		    }
		  
		  // push the local table to the global vector
		  v.push_back(local_table);
		  try
		    {
		      Cell* temp_formal = my_formals;
		      
		      // initialize each formals
		      for (int i = 0; i < formals_num; ++i)
			{
			  my_formals = car(temp_formal);
			  
			  // check whether the formals are basic operators
			  if (is_operator(my_formals))
			    {
			      throw runtime_error("The basic operators can't be redefined!");
			    }
			  my_formals->define_o(formals_v[i]);
			  temp_formal = cdr(temp_formal);
			}
		      
		      // eval each body
		      for(int i = 0; i < body_num; ++i)
			{
			  return_value = eval(car(my_body));
			  my_body = cdr(my_body);
			}
		    }
		  catch(...)
		  {
		    v.pop_back();
		    throw;
		  }
		  
		  // destroy the local_table
		  v.pop_back();
		  return return_value;
		}
	    }
	}
      
      // do the basic evaluation
      else
	{
	  try
	    {
	      // test whether the operator is nil (error: if nil)
	      test_nil(eval_car1);
	      operators = get_symbol(eval_car1);
	    }
	  catch(...)
	    {
	      throw runtime_error("There is no function and oeprators for expressions");
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
		  
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  result += eval_car2->arithmetic_o(has_double);
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
		  
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  result *= eval_car2->arithmetic_o(has_double);
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
		  
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  result = 0 - eval_car2->arithmetic_o(has_double);
		}
	      else 
		{
		  eval_car2 = eval(car(temp_ptr));
		  test_nil(eval_car2);
		  result = eval_car2->arithmetic_o(has_double);
		  temp_ptr = cdr(temp_ptr);
		  
		  // minus each operand
		  while (!nullp(temp_ptr))
		    {
		      eval_car2 = eval(car(temp_ptr));
		      test_nil(eval_car2);
		      result -= eval_car2->arithmetic_o(has_double);
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
		  temp_ptr = cdr(temp_ptr);
		  
		  // divide each operand
		  while (!nullp(temp_ptr))
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
		  
		  // check whether the operand is nil(error: if nil)
		  test_nil(eval_car2);
		  temp1 = eval_car2->less_o();
		  temp_ptr = cdr(temp_ptr);
		  
		  // compare each operands
		  while (!nullp(temp_ptr))
		    {
		      eval_car2 = eval(car(temp_ptr));
		      test_nil(eval_car2);
		      temp2 = eval_car2->less_o();
		      
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
	      
	      // check whether the operand is nil(error: if nil)
	      test_nil(eval_car2);
	      result = eval_car2->ceiling_o();
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
	      
	      // check whether the operand is nil(error: if nil)
	      test_nil(eval_car2);
	      result = eval_car2->floor_o();
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
		      eval_car3 = eval(car(temp_ptr2));
		      return eval_car3;
		    }
		  
		  // give the third operand if the first operand is zero
		  else
		    {
		      // give the value of result 0 if the third operand doesn't exit
		      if (operand_num == 2)
			{
			  return make_int(0);
			}
		      
		      // give the third operand if the third operand exists
		      else
			{
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
		  
		  // check whether the operand is nill (error: if so)
		  test_nil(eval_car2);
		  
		  if (is_operator(eval_car2))
		    {
		      throw runtime_error("The basic oeprators can't be defined");
		    }
		  
		  // define the symbol(error: if redefine)
		  eval_car2->define_o(eval_car3);
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
		  eval_car3 = eval(car(temp_ptr2));
		  return cons(eval_car2, eval_car3);
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
		      throw runtime_error("The operand for cdr should be ConsCell");
		    }
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
		      throw runtime_error("The operand for cdr should be ConsCell");
		    }
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
		  return make_int(result);
		}
	    }

	  // do the "listp" evaluation
	  else if (operators == "listp")
	    {
	      Cell* temp_ptr = cdr(c);
	      bool result = false;
	      
	      // store the number of operands(error: if zero)
	      int operand_num = test_operand_num(temp_ptr);
	      
	      // check whether the number of operands equals one(error: if not)
	      if (operand_num != 1)
		{
		  throw runtime_error("The number of operands for the operator 'listp' is more than 1!");
		}
	      else
		{
		  eval_car2 = eval(car(temp_ptr));
		  result = listp(eval_car2);
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
		  cout << endl;
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
		  
		  // evaluate the expression
		  result = eval(eval_car2);
		  return result;
		}
	    }
	  
	  // do the "lambda" evaluation
	  else if (operators == "lambda")
	    {
	      Cell* temp_ptr = cdr(c);
	      Cell* my_formals = nil;
	      Cell* my_body = nil;
	      
	      // store the number of operands (error: if zero)
	      int operand_num = test_operand_num(temp_ptr);
	      
	      // check whether the number of operands are larger than 1
	      if (operand_num <= 1)
		{
		  throw runtime_error("The number of operands for the operator 'lambda' is less than 2!");
		}
	      
	      // construct a procedure cell
	      else
		{
		  eval_car2 = car(temp_ptr);
		  my_formals = (eval_car2 == nil) ? nil : eval_car2->copy();
		  temp_ptr = cdr(temp_ptr);
		  my_body = temp_ptr->copy();
		  return lambda(my_formals, my_body);
		}
	    }
	  
	  // do the "apply" evaluation
	  else if (operators == "apply")
	    {
	      Cell* temp_ptr = cdr(c);
	      Cell* eval_func = nil;
	      
	      // store the number of operands (error: if zero)
	      int operand_num = test_operand_num(temp_ptr);
	      
	      // check whether the number of operands equals to 2
	      if (operand_num != 2)
		{
		  throw runtime_error("The number of operands for the operator 'apply' must be 2");
		}
	      else
		{
		  eval_car2 = eval(car(temp_ptr));
		  temp_ptr = cdr(temp_ptr);
		  eval_car3 = eval(car(temp_ptr));
		  
		  // check whether the procedure is basic operators
		  if (is_operator(eval_car2))
		    {
		      try
			{
			  eval_func = cons(eval_car2, eval_car3);
			}
		      catch(...)
			{
			  throw runtime_error("The second operand for apply should be operands list");
			}
		    }
		  else
		    {
		      // check whether the ProcedureCell exist (error: if not)
		      test_nil(eval_car2);
		      eval_func = eval_car2->apply(eval_car3);
		    }
		  return eval(eval_func); 
		}
	    }
	  
	  // do the "let" evaluation
	  else if (operators == "let")
	    {
	      Cell* temp_ptr = cdr(c);
	      Cell* return_value = nil;
	      map<string, Cell*> local_table;
	      
	      // store the number of operands (error: if zero)
	      int operand_num = test_operand_num(temp_ptr);
	      
	      if (operand_num <= 1)
		{
		  throw runtime_error("The number of operands fro the operator 'let' must larger than 2");
		}
	      else
		{
		  Cell* temp_formals = car(temp_ptr);
		  Cell* temp_body = nil;
		  
		  // check whether the second operand for 'let' is legal or not
		  if (temp_formals != nil && !listp(temp_formals))
		    {
		      throw runtime_error("The second operand for 'let' is illegal");
		    }
		  int formals_num = (temp_formals == nil) ? 0 : test_operand_num(temp_formals);
		  v.push_back(local_table);
		  try
		    {
		      // initialize each formals
		      for (int i = 1; i <= formals_num; ++i)
			{
			  Cell* temp_def = car(temp_formals);
			  
			  if (nullp(temp_def) || !listp(temp_def))
			    {
			      throw runtime_error("The initialization is illegal for 'let'");
			    }
			  int num_def = test_operand_num(temp_def);
			  
			  // check the number of operands for initialization
			  if (num_def != 2)
			    {
			      throw runtime_error("The initialization is illegal for 'let'");
			    }
			  Cell* temp_s = car(temp_def);
			  Cell* temp_v = eval(car(cdr(temp_def)));
			  
			  // check whether it's a basic operators
			  if (is_operator(temp_s))
			    {
			      throw runtime_error("The basic operators can't be redefined");
			    }
			  
			  // check whether the symbol exists
			  test_nil(temp_s);
			  temp_s->define_o(temp_v);
			  temp_formals = cdr(temp_formals);
			}
		      temp_ptr = cdr(temp_ptr);
		      
		      // evaluate the body for 'let'
		      for (int i = 1; i <= operand_num - 1; ++i)
			{
			  temp_body = car(temp_ptr);
			  return_value = eval(temp_body);
			  temp_ptr = cdr(temp_ptr);
			}
		    }
		  catch(...)
		    {
		      v.pop_back();
		      throw;
		    }
		  v.pop_back();
		  return return_value;
		}
	    }
	  // check whether the operator is defined
	  else
	    {
	      throw runtime_error("Cannot apply a value that is not a function");
	    }
	}
    }
  
  // return the value of IntCell, DoubleCell and SymbolCell
  else
    {
      if (is_operator(c))
	{
	  return c->copy();
	}
      else
	{
	  return c->get_value();
	}
    }
}

