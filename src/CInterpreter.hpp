/*
FILE:		CInterpreter.hpp

REMARKS:	This file contains the class "CInterpreter" with which an abstract syntax tree can be evaluated.
*/
#pragma once

#include <iostream>
#include <string>

#include "CAbstractSyntaxTree.hpp"
#include "CFileScanner.hpp"
#include "CInterpreter.hpp"
#include "CLinkedList.hpp"
#include "CParser.hpp"
#include "CRV.hpp"
#include "CToken.hpp"
#include "CTokenizer.hpp"
#include "CVariable.hpp"
#include "CFunction.hpp"
#include "Variables/ErrorMessages.hpp"
#include "Variables/KeywordTypes.hpp"
#include "Variables/TokenTypes.hpp"

using namespace std;



/**
* The class "CInterpreter" can be used to evaluate an abstract syntax tree.
* 
* @author	ChosenChris
* @version	14j.07.2021
*/
class CInterpreter {
private:
	/**
	* Stores a list of every variable which is used by the interpreter.
	*/
	CLinkedList<CVariable> lVariables;

	/**
	* Stores a list of every function which is implemented by the user.
	*/
	CLinkedList<CFunction> lFunctions;



private:
	/**
	* This function returns a variable, which has the passed argument as name.
	* 
	* @param psName	Name of the variable.
	* @return		Variable.
	*/
	CRV<CVariable> getVariable(string psName) {
		for (unsigned int i = 0; i < lVariables.size(); i++) {
			if (lVariables[i].getName() == psName) {
				//found variable:
				return CRV<CVariable>(lVariables[i], Error::SUCCESS);
			}
		}
		//Variable does not exist.
		return CRV<CVariable>(CVariable(), Error::Interpreter::VARIABLE_DOES_NOT_EXIST);
	}

	/**
	* This function adds the passed variable to the list of variables.
	* 
	* @param pVariable	Variable, which should be added.
	* @return			Error message.
	*/
	short int addVariable(CVariable pVariable) {
		for (unsigned int i = 0; i < lVariables.size(); i++) {
			if (lVariables[i].getName() == pVariable.getName()) {
				//Variable's name does already exist:
				return Error::Interpreter::VARIABLE_NAME_ALREADY_IN_USE;
			}
		}
		lVariables.add(pVariable);
		return Error::SUCCESS;
	}
	
	/**
	* This function changes the value of the passed variable in the list of variables.
	* 
	* @param pVariable	Variable with new value, which should be changed in the list of variables.
	* @return			Error message.
	*/
	short int changeVariableValue(CVariable pVariable) {
		for (unsigned int i = 0; i < lVariables.size(); i++) {
			if (lVariables[i].getName() == pVariable.getName()) {
				//Found variable:
				if (lVariables[i].getType() != pVariable.getType()) {
					//Error: New value has different data-type:
					return Error::produceConvertError(pVariable.getType(), lVariables[i].getType());
				}
				lVariables.remove(i); //Remove variable with old value.
				lVariables.add(pVariable); //Add variable with new value.
				return Error::SUCCESS;
			}
		}
		return Error::Interpreter::VARIABLE_DOES_NOT_EXIST;
	}



public:
	/**
	* This function takes a list of abstract syntax trees, each of which HAS TO RESEMBLE a Lisp function.
	* 
	* @param pltASTs	List of abstract syntax trees, which resemble Lisp functions.
	* @return			Error message.
	*/
	CRV<CToken> interpret(CLinkedList<CAbstractSyntaxTree<CToken>> pltASTs) {
		//Each passed abstract syntax tree resembles a function:
		for (unsigned int i = 0; i < pltASTs.size(); i++) {
			//Add every function to the list of functions:
			CFunction newFunc;
			CRV<CToken> funcReturn = newFunc.evaluateFunctionSourceCode(pltASTs[i]);
			if (funcReturn.getErrorMessage() != Error::SUCCESS) {
				//An error occured:
				return funcReturn;
			}
			lFunctions.add(newFunc);
		}

		//Find the main function:
		for (unsigned int i = 0; i < lFunctions.size(); i++) {
			if (lFunctions[i].getName() == Keyword::MAIN_FUNCTION) {
				//Found main function:
				if (lFunctions[i].getParameterAmount() != 0) {
					//The function has too many parameters:
					return CRV<CToken>(CToken(), Error::Interpreter::MAIN_FUNCTION_HAS_PARAMETERS);
				}
				else if (lFunctions[i].getReturnType() != Token::U_VOID) {
					//The function does not have "void" as return type:
					return CRV<CToken>(CToken(), Error::Interpreter::MAIN_FUNCTION_HAS_INCORRECT_RETURN_TYPE);
				}
				//Interpret each expression from the Lisp main-function:
				CLinkedList<CAbstractSyntaxTree<CToken>> ltMainExpressions; //Stores every expression of the main-function.
				ltMainExpressions.addAll(lFunctions[i].getExpressions());
				for (unsigned int j = 0; j < ltMainExpressions.size(); j++) {
					CRV<CToken> rvEval = interpretExpression(ltMainExpressions[j]); //Interprets the current expression.
					if (rvEval.getErrorMessage() != Error::SUCCESS) {
						//An error occured:
						return rvEval;
					}
				}
				//Every expression was successfully interpreted:
				return CRV<CToken>(CToken(), Error::SUCCESS);
			}
		}
		//Main function does not exist:
		return CRV<CToken>(CToken(), Error::Interpreter::MISSING_MAIN_FUNCTION);
	}



private:
	/**
	* This function can interpret an abstract syntax tree.
	* 
	* @param pAST	Abstract syntax tree, which is supposed to be evaluated by the interpreter.
	* @return		Error message.
	*/
	CRV<CToken> interpretExpression(CAbstractSyntaxTree<CToken> pAST) {
		CRV<CToken> rvEval;
		//Evaluate AST:
		if (pAST.getContent().getType() == Token::OPERATOR_ARITHMETIC || pAST.getContent().getType() == Token::OPERATOR_RELATIONAL || pAST.getContent().getType() == Token::OPERATOR_BOOL) {
			//Operation found:
			rvEval = evaluateOperation(pAST);
		}
		else if (pAST.getContent().getType() == Token::KEYWORD) {
			//Keyword found:
			rvEval = evaluateKeyword(pAST);
		}
		else if (pAST.getContent().getType() == Token::IDENTIFIER) {
			//Identifier found:
			rvEval = evaluateIdentifier(pAST);
		}
		else {
			//Incorrect token found:
			return CRV<CToken>(rvEval.getContent(), Error::Interpreter::SYNTAX_I);
		}

		//Return result of evaluation:
		return rvEval;
	}



private:
	/**
	* This function is used to evaluate an arithmetic, relational or boolean operation.
	* The head node of the AST must resemble either an arithmetic-, a relational- or a boolean operator.
	* 
	* @param pAST	Abstract syntax tree, which represents the operation.
	* @return		Return value of the operation as token.
	*/
	CRV<CToken> evaluateOperation(CAbstractSyntaxTree<CToken> pAST) {
		if (!pAST.hasSubTrees()) {
			//The AST has no subtrees -> The current node should be returned:
			return CRV<CToken>(pAST.getContent(), Error::SUCCESS);
		}

		CToken headNode = pAST.getContent(); //Represents the headNode.

		CLinkedList<CToken> lOperands; //Stores the operands of the arithmetic operation.
		CLinkedList<CAbstractSyntaxTree<CToken>> lNodeSubtrees; //Stores the subtrees.
		lNodeSubtrees.addAll(pAST.getSubTrees());
		for (unsigned int i = 0; i < lNodeSubtrees.size(); i++) {
			CAbstractSyntaxTree<CToken> currentAST;
			currentAST = lNodeSubtrees[i];
			if (currentAST.getContent().getType() == Token::U_INT || currentAST.getContent().getType() == Token::U_DOUBLE || currentAST.getContent().getType() == Token::U_BOOL || currentAST.getContent().getType() == Token::U_CHAR || currentAST.getContent().getType() == Token::U_STRING) {
				//Found integer, double or string (string only if operator is "+"):
				lOperands.add(currentAST.getContent());
			}
			else if (currentAST.getContent().getType() == Token::IDENTIFIER) {
				//Found variablename or function call:
				CRV<CToken> rvEval = interpretExpression(currentAST); //Stores the result of the evaluation of the identifier.
				if (rvEval.getErrorMessage() != Error::SUCCESS) {
					//An error occured:
					return rvEval;
				}
				lOperands.add(rvEval.getContent());
			}
			else {
				//Another abstraxt syntax tree needs to be evaluated:
				CRV<CToken> rv_eval = evaluateOperation(currentAST);
				if (rv_eval.getErrorMessage() != Error::SUCCESS) {
					//An error occured:
					return rv_eval;
				}
				lOperands.add(rv_eval.getContent());
			}
		}

		if (headNode.getType() == Token::OPERATOR_ARITHMETIC) {
			//Arithmetic operation:
			if (headNode.getLexeme() == "+") {
				//Additon -> Check wether the operands can be added:
				short int nReturnType = lOperands[0].getType();
				if (nReturnType == Token::U_STRING || nReturnType == Token::U_CHAR) {
					//Every other operand must be string or character:
					for (unsigned int i = 1; i < lOperands.size(); i++) {
						if (lOperands[i].getType() != Token::U_STRING && lOperands[i].getType() != Token::U_CHAR) {
							//Error: Operands are not of the same type:
							return CRV<CToken>(lOperands[i], Error::Interpreter::OPERANDS_ARE_OF_DIFFERENT_TYPE);
						}
					}
					nReturnType = Token::U_STRING; //Needs to be string, since either added characters or strings, make up a string in the end! \(^_^)/
				}
				else if (nReturnType == Token::U_INT || nReturnType == Token::U_DOUBLE) {
					//Every other operand must be integer or double:
					for (unsigned int i = 1; i < lOperands.size(); i++) {
						if (lOperands[i].getType() != Token::U_INT && lOperands[i].getType() != Token::U_DOUBLE) {
							//Error: Operands are not of the same type:
							return CRV<CToken>(lOperands[i], Error::Interpreter::OPERANDS_ARE_OF_DIFFERENT_TYPE);
						}
						if (lOperands[i].getType() == Token::U_DOUBLE) {
							nReturnType = Token::U_DOUBLE; //When integer and double, or multiple doubles are added, a double-type value must be returned.
														 //If only integers are present, this value will simply stay as integer!
						}
					}
				}
				else if (nReturnType == Token::U_BOOL) {
					//Error: Cannot add multiple booleans:
					return CRV<CToken>(lOperands[0], Error::Interpreter::CANNOT_ADD_BOOLEANS);
				}
				else {
					//Error: Incorrect token encountered -> Syntax error:
					return CRV<CToken>(lOperands[0], Error::Interpreter::SYNTAX_I);
				}

				//Add the operands:
				string sReturnValue = ""; //Represents the value, which should be returned as token.
				if (nReturnType == Token::U_STRING) {
					//A string needs to be created:
					for (unsigned int i = 0; i < lOperands.size(); i++) {
						sReturnValue += lOperands[i].getLexeme();
					}
				}
				else {
					//A number (either integer or double) needs to be created:
					sReturnValue = lOperands[0].getLexeme(); //First number makes string.
					for (unsigned int i = 1; i < lOperands.size(); i++) {
						if (nReturnType == Token::U_INT) {
							sReturnValue = to_string(stoi(sReturnValue) + stoi(lOperands[i].getLexeme()));
						}
						else {
							sReturnValue = to_string(stod(sReturnValue) + stod(lOperands[i].getLexeme()));
						}
					}
				}
				return CRV<CToken>(CToken(sReturnValue, nReturnType, lOperands[0].getFilename(), lOperands[0].getLine()), Error::SUCCESS);
			}
			
			else if (headNode.getLexeme() == "-") {
				//Subtraction -> check wether all operators are numeric values:
				short int nReturnType = Token::U_INT; //Stores, wether return value is integer or double.
				for (unsigned int i = 0; i < lOperands.size(); i++) {
					if (lOperands[i].getType() == Token::U_DOUBLE) {
						nReturnType = Token::U_DOUBLE;
					}
					else if (lOperands[i].getType() != Token::U_DOUBLE && lOperands[i].getType() != Token::U_INT) {
						//Found token, which resembles a non-numeric value:
						return CRV<CToken>(lOperands[i], Error::Interpreter::CANNOT_SUBTRACT_NON_NUMERIC_VALUES);
					}
				}

				//Subtract the numbers:
				string sReturnValue = lOperands[0].getLexeme(); //Resembles the return value of the subtraction.
				for (unsigned int i = 1; i < lOperands.size(); i++) {
					if (nReturnType == Token::U_INT) {
						sReturnValue = to_string(stoi(sReturnValue) - stoi(lOperands[i].getLexeme()));
					}
					else {
						sReturnValue = to_string(stod(sReturnValue) - stod(lOperands[i].getLexeme()));
					}
				}
				return CRV<CToken>(CToken(sReturnValue, nReturnType, lOperands[0].getFilename(), lOperands[0].getLine()), Error::SUCCESS);
			}

			else if (headNode.getLexeme() == "*") {
				//Multiplication -> check wether all operators are numeric values:
				short int nReturnType = lOperands[0].getType(); //Stores, wether an integer or double should be returned.
				for (unsigned int i = 0; i < lOperands.size(); i++) {
					if (lOperands[i].getType() != Token::U_INT && lOperands[i].getType() != Token::U_DOUBLE) {
						//Error: Cannot multiply with non-numeric-values:
						return CRV<CToken>(lOperands[i], Error::Interpreter::CANNOT_MULTIPLY_NON_NUMERIC_VALUES);
					}
					else if (lOperands[i].getType() == Token::U_DOUBLE) {
						nReturnType = Token::U_DOUBLE;
					}
				}

				//Multiply:
				string sReturnValue = ""; //Stores the result of the multiplication as string.
				sReturnValue = lOperands[0].getLexeme();
				for (unsigned int i = 1; i < lOperands.size(); i++) {
					if (nReturnType == Token::U_INT) {
						sReturnValue = to_string(stoi(sReturnValue) * stoi(lOperands[i].getLexeme()));
					}
					else if (nReturnType == Token::U_DOUBLE) {
						sReturnValue = to_string(stod(sReturnValue) * stod(lOperands[i].getLexeme()));
					}
				}
				return CRV<CToken>(CToken(sReturnValue, nReturnType, lOperands[0].getFilename(), lOperands[0].getLine()), Error::SUCCESS);
			}

			else if (headNode.getLexeme() == "/") {
				//Division -> check wether all operators are numeric values:
				for (unsigned int i = 0; i < lOperands.size(); i++) {
					if (lOperands[i].getType() != Token::U_INT && lOperands[i].getType() != Token::U_DOUBLE) {
						//Error: Cannot divide non-numeric values:
						return CRV<CToken>(lOperands[i], Error::Interpreter::CANNOT_DIVIDE_NON_NUMERIC_VALUES);
					}
					else if (i >= 1 && stod(lOperands[i].getLexeme()) == 0) {
						//Error: Cannot divide by zero:
						return CRV<CToken>(lOperands[i], Error::Interpreter::CANNOT_DIVIDE_BY_ZERO);
					}
				}

				//Divide:
				string sReturnValue = ""; //Stores the result of the division as string.
				sReturnValue = lOperands[0].getLexeme();
				for (unsigned int i = 1; i < lOperands.size(); i++) {
					sReturnValue = to_string(stod(sReturnValue) / stod(lOperands[i].getLexeme()));
				}
				return CRV<CToken>(CToken(sReturnValue, Token::U_DOUBLE, lOperands[0].getFilename(), lOperands[0].getLine()), Error::SUCCESS);
			}
		}
		
		else if (headNode.getType() == Token::OPERATOR_RELATIONAL) {
			//Relational operation:
			if (lOperands.size() != 2) {
				//Error: Relational operations do only work with exactly 2 operators:
				if (lOperands.size() < 2) {
					return CRV<CToken>(headNode, Error::Interpreter::NOT_ENOUGH_ARGUMENTS);
				}
				else {
					return CRV<CToken>(headNode, Error::Interpreter::TOO_MANY_ARGUMENTS);
				}
			}

			CToken firstOperand = lOperands[0]; //Stores the first operand.
			CToken secondOperand = lOperands[1]; //Stores the second operand.
			bool bResult = false; //Stores the result of the relational operation.

			if (headNode.getLexeme() == "=") {
				//Equal to:
				if (firstOperand.getType() != secondOperand.getType() && !((firstOperand.getType() == Token::U_INT && secondOperand.getType() == Token::U_DOUBLE) || (firstOperand.getType() == Token::U_DOUBLE && secondOperand.getType() == Token::U_INT))) {
					//Error operands are of different type:
					return CRV<CToken>(headNode, Error::produceConvertError(firstOperand.getType(), secondOperand.getType()));
				}
				if (firstOperand.getLexeme() == secondOperand.getLexeme()) {
					//Operands are identical:
					return CRV<CToken>(CToken("t", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
				}
				else {
					//Operands are not identical:
					return CRV<CToken>(CToken("nil", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
				}
			}
			else if (headNode.getLexeme() == "!") {
				//Not equal to:
				if (firstOperand.getType() != secondOperand.getType() && !((firstOperand.getType() == Token::U_INT && secondOperand.getType() == Token::U_DOUBLE) || (firstOperand.getType() == Token::U_DOUBLE && secondOperand.getType() == Token::U_INT))) {
					//Error operands are of different type:
					return CRV<CToken>(headNode, Error::produceConvertError(firstOperand.getType(), secondOperand.getType()));
				}
				if (firstOperand.getLexeme() != secondOperand.getLexeme()) {
					//Operands are not identical:
					return CRV<CToken>(CToken("t", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
				}
				else {
					//Operands are identical:
					return CRV<CToken>(CToken("nil", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
				}
			}
			else {
				//For every other relational operation, the operands must either be an integer or a double:
				if (firstOperand.getType() != Token::U_INT && firstOperand.getType() != Token::U_DOUBLE) {
					//Error: The first operand is no number:
					return CRV<CToken>(firstOperand, Error::produceConvertError(firstOperand.getType(), Token::U_DOUBLE));
				}
				else if (secondOperand.getType() != Token::U_INT && secondOperand.getType() != Token::U_DOUBLE) {
					//Error: The second operand is no number:
					return CRV<CToken>(secondOperand, Error::produceConvertError(secondOperand.getType(), Token::U_DOUBLE));
				}
				double nVal1 = stod(firstOperand.getLexeme()); //Stores the first operand as double-value.
				double nVal2 = stod(secondOperand.getLexeme()); //Stores the second operand as double-value.

				if (headNode.getLexeme() == ">") {
					//Greater than:
					if (nVal1 > nVal2) {
						//The first value is greater than the second value:
						return CRV<CToken>(CToken("t", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
					else {
						//The first value is not greater than the second value:
						return CRV<CToken>(CToken("nil", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
				}
				else if (headNode.getLexeme() == "<") {
					//Less than:
					if (nVal1 < nVal2) {
						//The first value is less than the second value:
						return CRV<CToken>(CToken("t", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
					else {
						//The first value is not less than the second value:
						return CRV<CToken>(CToken("nil", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
				}
				else if (headNode.getLexeme() == ">=") {
					//Greater or equal than:
					if (nVal1 >= nVal2) {
						//The first value is greater or equal than the second value:
						return CRV<CToken>(CToken("t", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
					else {
						//The first value is not greater or equal than the second value:
						return CRV<CToken>(CToken("nil", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
				}
				else if (headNode.getLexeme() == "<=") {
					//Less or equal than:
					if (nVal1 <= nVal2) {
						//The first value is less or equal than the second value:
						return CRV<CToken>(CToken("t", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
					else {
						//The first value is not less or equal than the second value:
						return CRV<CToken>(CToken("nil", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
				}
			}
		}

		else if (headNode.getType() == Token::OPERATOR_BOOL) {
			//Boolean operation:
			if (lOperands.size() < 2) {
				//Error: Boolean operation must contain at least two arguments:
				return CRV<CToken>(headNode, Error::Interpreter::NOT_ENOUGH_ARGUMENTS);
			}
			if (headNode.getLexeme() == "&") {
				//All operands must be "t":
				for (unsigned int i = 0; i < lOperands.size(); i++) {
					if (lOperands[i].getLexeme() == "nil") {
						//nil was found, return nil:
						return CRV<CToken>(CToken("nil", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
				}
				//nil was not found, return t:
				return CRV<CToken>(CToken("t", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
			}
			else {
				//At least one operand must be "t":
				for (unsigned int i = 0; i < lOperands.size(); i++) {
					if (lOperands[i].getLexeme() == "t") {
						//t was found, return t:
						return CRV<CToken>(CToken("t", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
					}
				}
				//t was not found, return nil:
				return CRV<CToken>(CToken("nil", Token::U_BOOL, headNode.getFilename(), headNode.getLine()), Error::SUCCESS);
			}
		}

		else {
			//Found invalid token:
			return CRV<CToken>(headNode, Error::Interpreter::INCORRECT_TOKEN);
		}

		//Return SUCCESS -> Only needed to eliminate compiler warning. \(^_^)/
		return CRV<CToken>(headNode, Error::SUCCESS);
	}
	


	/**
	* This function is used to evaluate a keyword.
	* The headnode of the AST must resemble a keyword.
	* 
	* @param pAST	Abstract syntax tree, which represents the keyword.
	* @return		Error message including the token which caused the error.
	*/
	CRV<CToken> evaluateKeyword(CAbstractSyntaxTree<CToken> pAST) {
		if (!pAST.hasSubTrees()) {
			//Erorr: The AST has no subtrees:
			return CRV<CToken>(pAST.getContent(), Error::Interpreter::EMPTY_AST_I);
		}

		CLinkedList<CAbstractSyntaxTree<CToken>> lASTs; //Stores the subtrees of the current AST.
		lASTs.addAll(pAST.getSubTrees());
		CToken headNode = pAST.getContent(); //Headnode stores the keyword, which should be evaluated.
		if (headNode.getType() != Token::KEYWORD) {
			//Error: Headnode is no keyword:
			return CRV<CToken>(headNode, Error::Interpreter::SYNTAX_I);
		}

		if (headNode.getLexeme() == Keyword::INT || headNode.getLexeme() == Keyword::DOUBLE || headNode.getLexeme() == Keyword::BOOL || headNode.getLexeme() == Keyword::CHAR || headNode.getLexeme() == Keyword::STRING) {
			//The keyword indicates a declaration of a new variable:
			CToken variableName; //Stores the name of the variable.
			CToken variableValue; //Stores the value of the new variable.
			//Initialize variable with standard value:
			if (headNode.getLexeme() == Keyword::INT) {
				variableValue = CToken{ "0", Token::U_INT, variableName.getFilename(), variableName.getLine() };
			}
			else if (headNode.getLexeme() == Keyword::DOUBLE) {
				variableValue = CToken{ "0.0", Token::U_DOUBLE, variableName.getFilename(), variableName.getLine() };
			}
			else if (headNode.getLexeme() == Keyword::BOOL) {
				variableValue = CToken{ "nil", Token::U_BOOL, variableName.getFilename(), variableName.getLine() };
			}
			else if (headNode.getLexeme() == Keyword::CHAR) {
				variableValue = CToken{ " ", Token::U_CHAR, variableName.getFilename(), variableName.getLine() };
			}
			else if (headNode.getLexeme() == Keyword::STRING) {
				variableValue = CToken{ "", Token::U_STRING, variableName.getFilename(), variableName.getLine() };
			}
			
			if (lASTs.size() < 1) {
				//Error: Variablename is not present in sourcecode:
				return CRV<CToken>(headNode, Error::Interpreter::MISSING_TOKEN);
			}
			if (lASTs[0].getContent().getType() != Token::IDENTIFIER) {
				//Error: Incorrect token found:
				return CRV<CToken>(lASTs[0].getContent(), Error::Interpreter::INCORRECT_TOKEN);
			}
			//Found variable name:
			variableName = lASTs.retrieve(0).getContent();
			if (lASTs.size() == 1) {
				//The variable will be initialized:
				CToken initializer = lASTs[0].getContent(); //Token, which resembles the value the variable will be assigned to.
				if (initializer.getType() == Token::U_INT || initializer.getType() == Token::U_DOUBLE || initializer.getType() == Token::U_BOOL || initializer.getType() == Token::U_CHAR || initializer.getType() == Token::U_STRING) {
					//Value can be used directly:
					if (initializer.getType() != variableValue.getType()) {
						//Error: Invalid datatype:
						return CRV<CToken>(initializer, Error::produceConvertError(initializer.getType(), variableValue.getType()));
					}
					variableValue = initializer;
				}
				else {
					//Need to evaluate the token:
					CRV<CToken> rvEval = interpretExpression(lASTs[0]);
					if (rvEval.getErrorMessage() != Error::SUCCESS) {
						//An error occured:
						return rvEval;
					}
					else if (rvEval.getContent().getType() != variableValue.getType()) {
						return CRV<CToken>(rvEval.getContent(), Error::produceConvertError(rvEval.getContent().getType(), variableValue.getType()));
					}
					variableValue = rvEval.getContent();
				}
			}
			else if (lASTs.size() > 1) {
				//There are too many arguments wit this declaration:
				return CRV<CToken>(headNode, Error::Interpreter::TOO_MANY_ARGUMENTS);
			}
			//Create a new variable:
			short int nErrorMessage = addVariable(CVariable(variableName.getLexeme(), variableValue.getLexeme(), variableValue.getType()));
			//Return result of creation of new variable:
			return CRV<CToken>(variableName, nErrorMessage);
		}

		else if (headNode.getLexeme() == Keyword::SET) {
			CToken variableName; //Stores the name of the variable.
			CToken variableValue; //Stores the value of the new variable.
			if (lASTs.size() < 2) {
				//Error: Not enough arguments:
				return CRV<CToken>(headNode, Error::Interpreter::NOT_ENOUGH_ARGUMENTS);
			}
			else if (lASTs.size() > 2) {
				//Error: Too many arguments:
				return CRV<CToken>(headNode, Error::Interpreter::TOO_MANY_ARGUMENTS);
			}
			else if (lASTs[0].getContent().getType() != Token::IDENTIFIER) {
				//Error: Incorrect token found:
				return CRV<CToken>(lASTs[0].getContent(), Error::Interpreter::INCORRECT_TOKEN);
			}
			//Found variable name:
			variableName = lASTs.retrieve(0).getContent();
			
			//Get the new value for the variable:
			CToken initializer = lASTs[0].getContent(); //Token, which resembles the value the variable will be assigned to.
			if (initializer.getType() == Token::U_INT || initializer.getType() == Token::U_DOUBLE || initializer.getType() == Token::U_BOOL || initializer.getType() == Token::U_CHAR || initializer.getType() == Token::U_STRING) {
				//Value can be used directly:
				variableValue = initializer;
			}
			else {
				//Need to evaluate the token:
				CRV<CToken> rvEval = interpretExpression(lASTs[0]);
				if (rvEval.getErrorMessage() != Error::SUCCESS) {
					//An error occured:
					return rvEval;
				}
				variableValue = rvEval.getContent();
			}
			//Change variable's value:
			short int nErrorMessage = changeVariableValue(CVariable(variableName.getLexeme(), variableValue.getLexeme(), variableValue.getType()));
			//Return result of changed value:
			return CRV<CToken>(variableName, nErrorMessage);
		}
	
		else if (headNode.getLexeme() == Keyword::PRINT || headNode.getLexeme() == Keyword::PRINTLN) {
			//Print every other element in the list:
			for (unsigned int i = 0; i < lASTs.size(); i++) {
				//Print every other element in the list:
				CAbstractSyntaxTree<CToken> current = lASTs[i]; //Stores the current AST:
				if (current.hasSubTrees ()) {
					//Further evaluation needed:
					CRV<CToken> rvEval = interpretExpression(current);
					if (rvEval.getErrorMessage() != Error::SUCCESS) {
						//An error occured:
						return rvEval;
					}
					cout << rvEval.getContent().getLexeme();
				}
				else if (current.getContent().getType() == Token::IDENTIFIER) {
					//Need to get the value of variable:
					CRV<CToken> rvEval = interpretExpression(current);
					if (rvEval.getErrorMessage() != Error::SUCCESS) {
						//An error occured:
						return rvEval;
					}
					cout << rvEval.getContent().getLexeme();
				}
				else if (current.getContent().getType() == Token::U_INT || current.getContent().getType() == Token::U_DOUBLE || current.getContent().getType() == Token::U_BOOL || current.getContent().getType() == Token::U_CHAR || current.getContent().getType() == Token::U_STRING) {
					//Element resembles value which can be printed:
					cout << current.getContent().getLexeme();
				}
				else {
					//Encountered invalid token:
					return CRV<CToken>(current.getContent(), Error::Interpreter::INCORRECT_TOKEN);
				}
			}
			if (headNode.getLexeme() == Keyword::PRINTLN) {
				cout << "\n";
			}
			return CRV<CToken>(headNode, Error::SUCCESS);
		}

		else if (headNode.getLexeme() == Keyword::IF) {
		//Evaluate if-statement:
		return ifStatement(pAST);
		}

		else if (headNode.getLexeme() == Keyword::WHILE) {
			//Evaluate while-loop:
			return whileLoop(pAST);
		}

		else if (headNode.getLexeme() == Keyword::RETURN) {
			//Evaluate return-statement:
			if (pAST.hasSubTrees()) {
				if (pAST.getSubTreeNumber() == 1) {
					//Return the ONLY subtree of the return statement:
					CAbstractSyntaxTree<CToken> tReturnValue = pAST.getSubTreeAtIndex(0);
					if (tReturnValue.hasSubTrees() || !(tReturnValue.getContent().getType() == Token::U_INT || tReturnValue.getContent().getType() == Token::U_DOUBLE || tReturnValue.getContent().getType() == Token::U_CHAR || tReturnValue.getContent().getType() == Token::U_BOOL || tReturnValue.getContent().getType() == Token::U_STRING)) {
						//Further evaluation needed for return value:
						CRV<CToken> rvEval = interpretExpression(tReturnValue);
						if (rvEval.getErrorMessage() != Error::SUCCESS) {
							//An error occured:
							return rvEval;
						}
						return CRV<CToken>(rvEval.getContent(), Error::Interpreter::INFO_RETURN_STATEMENT_ENCOUNTERED);
					}
					return CRV<CToken>(pAST.getSubTreeAtIndex(0).getContent(), Error::Interpreter::INFO_RETURN_STATEMENT_ENCOUNTERED);
				}
				else {
					//Too many values encountered which could be returned:
					return CRV<CToken>(headNode, Error::Interpreter::TOO_MANY_VALUES_TO_RETURN);
				}
			}
		}

		else {
			//Found invalid keyword:
			return CRV<CToken>(headNode, Error::Interpreter::INCORRECT_TOKEN);
		}
	}



	/**
	* This function is used to evaluate an identifier.
	* The headnode of this AST must resemble the identifier.
	*
	* @param pAST	Abstract syntax tree, which represents the identifier.
	* @return		Error message including content.
	*/
	CRV<CToken> evaluateIdentifier(CAbstractSyntaxTree<CToken> pAST) {
		//Find out if keyword resembles function call or variable:
		bool bIsVariable = false; //Stores wether the keyword resembles a variable or not.

		if (!pAST.hasSubTrees()) {
			//There are no subtrees, which could resemble passed arguments within a function call:
			for (unsigned int i = 0; i < lVariables.size(); i++) {
				if (lVariables[i].getName() == pAST.getContent().getLexeme()) {
					//Found variable name:
					bIsVariable = true;
					break;
				}
			}
		}


		if (bIsVariable) {
			//Identifier resembles variable name:
			CRV<CVariable> rvVariable = getVariable(pAST.getContent().getLexeme());
			if (rvVariable.getErrorMessage() != Error::SUCCESS) {
				//An error occured:
				return CRV<CToken>(pAST.getContent(), rvVariable.getErrorMessage());
			}
			//Variable was successfully returned:
			return CRV<CToken>(CToken(rvVariable.getContent().getValue(), rvVariable.getContent().getType(), pAST.getContent().getFilename(), pAST.getContent().getLine()), Error::SUCCESS);
		}

		else {
			//Identifier resembles a function name:
			CLinkedList<CToken> plPassedArguments;
			if (pAST.hasSubTrees()) {
				//The function call has arguments which are passed:
				for (unsigned int i = 0; i < pAST.getSubTrees().size(); i++) {
					CToken currentArg;
					//Find out if each argument has subtrees and needs further evaluation:
					if (pAST.getSubTreeAtIndex(i).hasSubTrees() || !(pAST.getSubTreeAtIndex(i).getContent().getType() == Token::U_INT || pAST.getSubTreeAtIndex(i).getContent().getType() == Token::U_DOUBLE || pAST.getSubTreeAtIndex(i).getContent().getType() == Token::U_CHAR || pAST.getSubTreeAtIndex(i).getContent().getType() == Token::U_BOOL || pAST.getSubTreeAtIndex(i).getContent().getType() == Token::U_STRING)) {
						//Further evaluation neccessary:
						CRV<CToken> rvEval;
						rvEval = interpretExpression(pAST.getSubTreeAtIndex(i));
						if (rvEval.getErrorMessage() != Error::SUCCESS) {
							//An error occured:
							return rvEval;
						}
						currentArg = rvEval.getContent();
					}
					else {
						//No further evaluation neccessary:
						currentArg = pAST.getSubTreeAtIndex(i).getContent();
					}

					//Add the current argument to the list of passed arguments:
					plPassedArguments.add(currentArg);
				}
			}
			
			//Call the function:
			return callFunction(pAST.getContent(), plPassedArguments);
		}
	}
	


private:
	/**
	* This function is used to evaluate an if-statement.
	* The headnode of the passed AST must resemble the if-keyword.
	* 
	* @param pAST	Abstract syntax tree, which resembles the if-statement
	* @return		Error message and token, which caused the error.
	*/
	CRV<CToken> ifStatement(CAbstractSyntaxTree<CToken> pAST) {
		if (pAST.getContent().getLexeme() != Keyword::IF) {
			//Error: Incorrect token found:
			return CRV<CToken>(pAST.getContent(), Error::Interpreter::INCORRECT_TOKEN);
		}
		else if (!pAST.hasSubTrees()) {
			//Error: The AST does not have any subtrees:
			return CRV<CToken>(pAST.getContent(), Error::Interpreter::EMPTY_AST_I);
		}

		CLinkedList<CAbstractSyntaxTree<CToken>> lSubTrees; //Stores the subtrees of the passed AST.
		lSubTrees.addAll(pAST.getSubTrees());
		if (!(lSubTrees.size() >= 1)) {
			//Error: AST does not contain a condition:
			return CRV<CToken>(pAST.getContent(), Error::Interpreter::MISSING_CONDITION);
		}
		CAbstractSyntaxTree<CToken> tCondition = lSubTrees[0]; //Stores the condition of the AST.
		bool bCondition = false; //Stores wether the statement's condition is true or false.
		if (tCondition.hasSubTrees()) {
			//Further evaluation of the condition needed:
			CRV<CToken> rvEval = interpretExpression(tCondition);
			if (rvEval.getErrorMessage() != Error::SUCCESS) {
				//An error occured:
				return rvEval;
			}
			CToken condToken = rvEval.getContent(); //Stores the returned token, which stores the boolean for the condition.
			if (condToken.getType() != Token::U_BOOL || (condToken.getType() == Token::U_INT && (stoi(condToken.getLexeme()) != 1 || stoi(condToken.getLexeme()) != 0)) || (condToken.getType() == Token::U_DOUBLE && (stod(condToken.getLexeme()) != 1 || stod(condToken.getLexeme()) != 0))) {
				//Error: Incorrect datatype encountered:
				return CRV<CToken>(rvEval.getContent(), Error::produceConvertError(rvEval.getContent().getType(), Token::U_BOOL));
			}
			if ((condToken.getType() == Token::U_BOOL && condToken.getLexeme() == "t") || (condToken.getType() == Token::U_INT && stoi(condToken.getLexeme()) == 1) || (condToken.getType() == Token::U_DOUBLE && stod(condToken.getLexeme()) == 1)) {
				//Condition is true:
				bCondition = true;
			}
		}
		else {
			//No further evaluation needed:
			CToken condToken = tCondition.getContent(); //Stores the returned token, which stores the boolean for the condition.
			if (condToken.getType() != Token::U_BOOL || (condToken.getType() == Token::U_INT && (stoi(condToken.getLexeme()) != 1 || stoi(condToken.getLexeme()) != 0)) || (condToken.getType() == Token::U_DOUBLE && (stod(condToken.getLexeme()) != 1 || stod(condToken.getLexeme()) != 0))) {
				//Error: Incorrect datatype encountered:
				return CRV<CToken>(tCondition.getContent(), Error::produceConvertError(tCondition.getContent().getType(), Token::U_BOOL));
			}
			if ((condToken.getType() == Token::U_BOOL && condToken.getLexeme() == "t") || (condToken.getType() == Token::U_INT && stoi(condToken.getLexeme()) == 1) || (condToken.getType() == Token::U_DOUBLE && stod(condToken.getLexeme()) == 1)) {
				//Condition is true:
				bCondition = true;
			}
		}
		
		CLinkedList<CVariable> lOldVariables; //Stores the variables before the if-statement.
		lOldVariables.addAll(lVariables);

		//Check wether the if- or else-expressions should be executed:
		if (!(lSubTrees.size() >= 2)) {
			//Error no body found:
			return CRV<CToken>(pAST.getContent(), Error::Interpreter::MISSING_BODY);
		}
		CLinkedList<CAbstractSyntaxTree<CToken>> ltBodyExpressions; //Stores the expressions of the if-statement's body.
		CAbstractSyntaxTree<CToken> lBodyAST; //Stores the AST whose expressions should be executed.
		if (!bCondition) {
			//The condition is false -> execute else-body (if it exists):
			if (!(lSubTrees.size() >= 3)) {
				//There is no else body -> terminate the execution:
				return CRV<CToken>(pAST.getContent(), Error::SUCCESS);
			}
			lBodyAST = lSubTrees[2];
		}
		else {
			//The condition is true -> execute main-body:
			lBodyAST = lSubTrees[1];
		}
		//Execute expressions:
		if (lBodyAST.getContent().getType() != Token::BRANCH) {
			//Only one expression is represented in the body:
			CRV<CToken> rvEval = interpretExpression(lBodyAST); //Interpret the expression.
			if (rvEval.getErrorMessage() != Error::SUCCESS) {
				//An error occured:
				return rvEval;
			}
		}
		else {
			//Multiple expressions need to be evaluated:
			CLinkedList<CAbstractSyntaxTree<CToken>> ltBodyExpressions; //Stores the expressions of the if-statement's body.
			ltBodyExpressions.addAll(lBodyAST.getSubTrees());
			for (unsigned int i = 0; i < ltBodyExpressions.size(); i++) {
				CRV<CToken> rvEval = interpretExpression(ltBodyExpressions[i]); //Execute expression.
				if (rvEval.getErrorMessage() != Error::SUCCESS) {
					//An error occured:
					return rvEval;
				}
			}
		}
		//Every expression was successfully executed -> Restore the old variables:
		for (unsigned int i = 0; i < lOldVariables.size(); i++) {
			for (unsigned int j = 0; j < lVariables.size(); j++) {
				if (lOldVariables[i].getName() == lVariables[j].getName() && lOldVariables[i].getValue() != lVariables[j].getValue()) {
					//Found identical variable with different value:
					lOldVariables.remove(i); //Remove old variable.
					lOldVariables.add(lVariables[j], i); //Add new variable.
				}
			}
		}
		lVariables.clear();
		lVariables.addAll(lOldVariables); //Restore old variables.
		return CRV<CToken>(pAST.getContent(), Error::SUCCESS);
	}



	/**
	* This function is used to evaluate a while-loop
	* The headnode of the passed AST must resemble the while-keyword.
	* 
	* @param pAST	Abstract syntax tree, which resembles the while-loop
	* @return		Error message and token, which caused the error.
	*/
	CRV<CToken> whileLoop(CAbstractSyntaxTree<CToken> pAST) {
		if (pAST.getContent().getLexeme() != Keyword::WHILE) {
			//Error: Incorrect token found:
			return CRV<CToken>(pAST.getContent(), Error::Interpreter::INCORRECT_TOKEN);
		}
		else if (!pAST.hasSubTrees()) {
			//Error: The AST does not have any subtrees:
			return CRV<CToken>(pAST.getContent(), Error::Interpreter::EMPTY_AST_I);
		}

		CLinkedList<CAbstractSyntaxTree<CToken>> lSubTrees; //Stores the subtrees of the passed AST.
		lSubTrees.addAll(pAST.getSubTrees());
		if (!(lSubTrees.size() >= 1)) {
			//Error: AST does not contain a condition:
			return CRV<CToken>(pAST.getContent(), Error::Interpreter::MISSING_CONDITION);
		}
		CAbstractSyntaxTree<CToken> tCondition = lSubTrees[0]; //Stores the condition of the AST.
		bool bCondition = false; //Stores wether the loop's condition is true or false.
		if (tCondition.hasSubTrees()) {
			//Further evaluation of the condition needed:
			CRV<CToken> rvEval = interpretExpression(tCondition);
			if (rvEval.getErrorMessage() != Error::SUCCESS) {
				//An error occured:
				return rvEval;
			}
			CToken condToken = rvEval.getContent(); //Stores the returned token, which stores the boolean for the condition.
			if (condToken.getType() != Token::U_BOOL || (condToken.getType() == Token::U_INT && (stoi(condToken.getLexeme()) != 1 || stoi(condToken.getLexeme()) != 0)) || (condToken.getType() == Token::U_DOUBLE && (stod(condToken.getLexeme()) != 1 || stod(condToken.getLexeme()) != 0))) {
				//Error: Incorrect datatype encountered:
				return CRV<CToken>(rvEval.getContent(), Error::produceConvertError(rvEval.getContent().getType(), Token::U_BOOL));
			}
			if ((condToken.getType() == Token::U_BOOL && condToken.getLexeme() == "t") || (condToken.getType() == Token::U_INT && stoi(condToken.getLexeme()) == 1) || (condToken.getType() == Token::U_DOUBLE && stod(condToken.getLexeme()) == 1)) {
				//Condition is true:
				bCondition = true;
			}
		}
		else {
			//No further evaluation needed:
			CToken condToken = tCondition.getContent(); //Stores the returned token, which stores the boolean for the condition.
			if (condToken.getType() != Token::U_BOOL || (condToken.getType() == Token::U_INT && (stoi(condToken.getLexeme()) != 1 || stoi(condToken.getLexeme()) != 0)) || (condToken.getType() == Token::U_DOUBLE && (stod(condToken.getLexeme()) != 1 || stod(condToken.getLexeme()) != 0))) {
				//Error: Incorrect datatype encountered:
				return CRV<CToken>(tCondition.getContent(), Error::produceConvertError(tCondition.getContent().getType(), Token::U_BOOL));
			}
			if ((condToken.getType() == Token::U_BOOL && condToken.getLexeme() == "t") || (condToken.getType() == Token::U_INT && stoi(condToken.getLexeme()) == 1) || (condToken.getType() == Token::U_DOUBLE && stod(condToken.getLexeme()) == 1)) {
				//Condition is true:
				bCondition = true;
			}
		}

		CLinkedList<CVariable> lOldVariables; //Stores the variables before the execution of the while-loop.
		lOldVariables.addAll(lVariables);

		//Loop:
		while (true) {
			bCondition = false;
			if (tCondition.hasSubTrees()) {
				//Further evaluation of the condition needed:
				CRV<CToken> rvEval = interpretExpression(tCondition);
				if (rvEval.getErrorMessage() != Error::SUCCESS) {
					//An error occured:
					return rvEval;
				}
				CToken condToken = rvEval.getContent(); //Stores the returned token, which stores the boolean for the condition.
				if (condToken.getType() != Token::U_BOOL || (condToken.getType() == Token::U_INT && (stoi(condToken.getLexeme()) != 1 || stoi(condToken.getLexeme()) != 0)) || (condToken.getType() == Token::U_DOUBLE && (stod(condToken.getLexeme()) != 1 || stod(condToken.getLexeme()) != 0))) {
					//Error: Incorrect datatype encountered:
					return CRV<CToken>(rvEval.getContent(), Error::produceConvertError(rvEval.getContent().getType(), Token::U_BOOL));
				}
				if ((condToken.getType() == Token::U_BOOL && condToken.getLexeme() == "t") || (condToken.getType() == Token::U_INT && stoi(condToken.getLexeme()) == 1) || (condToken.getType() == Token::U_DOUBLE && stod(condToken.getLexeme()) == 1)) {
					//Condition is true:
					bCondition = true;
				}
			}
			else {
				//No further evaluation needed:
				CToken condToken = tCondition.getContent(); //Stores the returned token, which stores the boolean for the condition.
				if (condToken.getType() != Token::U_BOOL || (condToken.getType() == Token::U_INT && (stoi(condToken.getLexeme()) != 1 || stoi(condToken.getLexeme()) != 0)) || (condToken.getType() == Token::U_DOUBLE && (stod(condToken.getLexeme()) != 1 || stod(condToken.getLexeme()) != 0))) {
					//Error: Incorrect datatype encountered:
					return CRV<CToken>(tCondition.getContent(), Error::produceConvertError(tCondition.getContent().getType(), Token::U_BOOL));
				}
				if ((condToken.getType() == Token::U_BOOL && condToken.getLexeme() == "t") || (condToken.getType() == Token::U_INT && stoi(condToken.getLexeme()) == 1) || (condToken.getType() == Token::U_DOUBLE && stod(condToken.getLexeme()) == 1)) {
					//Condition is true:
					bCondition = true;
				}
			}
			if (!bCondition) {
				//The condition is false -> End the iteration of the while-loop:
				break;
			}

			//Get the body's statements:
			if (lSubTrees.size() < 2) {
				//Error: The body is missing:
				return CRV<CToken>(pAST.getContent(), Error::Interpreter::MISSING_BODY);
			}
			else if (lSubTrees.size() > 2) {
				//Error: The statement has too many arguments:
				return CRV<CToken>(lSubTrees[2].getContent(), Error::Interpreter::TOO_MANY_ARGUMENTS);
			}
			CAbstractSyntaxTree<CToken> tBodyAST; //Stores the AST of the body.
			tBodyAST = lSubTrees[1];
			//Execute expressions:
			if (tBodyAST.getContent().getType() != Token::BRANCH) {
				//Only one expression needs to be evaluated:
				CRV<CToken> rvEval = interpretExpression(tBodyAST);
				if (rvEval.getErrorMessage() != Error::SUCCESS) {
					//An error occured:
					return rvEval;
				}
			}
			else {
				//Multiple expressions need to be evaluated:
				CLinkedList<CAbstractSyntaxTree<CToken>> ltBodyExpressions; //Stores the expressions of the loop's body.
				ltBodyExpressions.addAll(tBodyAST.getSubTrees());
				for (unsigned int i = 0; i < ltBodyExpressions.size(); i++) {
					CRV<CToken> rvEval = interpretExpression(ltBodyExpressions[i]); //Execute current expression.
					if (rvEval.getErrorMessage() != Error::SUCCESS) {
						//An error occured:
						return rvEval;
					}
				}
			}
			//Every expression was successfully evaluated -> Restore old variables:
			for (unsigned int i = 0; i < lOldVariables.size(); i++) {
				for (unsigned int j = 0; j < lVariables.size(); j++) {
					if (lOldVariables[i].getName() == lVariables[j].getName() && lOldVariables[i].getValue() != lVariables[j].getValue()) {
						//Found identical variable with different value:
						lOldVariables.remove(i); //Remove old variable.
						lOldVariables.add(lVariables[j], i); //Add new variable.
					}
				}
			}
			lVariables.clear();
			lVariables.addAll(lOldVariables); //Restore old variables.
			//Continue with next iteration...
		}
		//After execution of statements:
		return CRV<CToken>(pAST.getContent(), Error::SUCCESS);
	}



private:
	/**
	* This function is used to call the LISP-function which has the same name as the passed
	* argument psFunctionName. The parameter plArguments resembles a list of LISP-arguments
	* that are passed onto the LISP-function.
	* If the LISP-function is executed without occuring errors, this function returns the LISP-
	* function's return value. If any errors are caused, the causing token is returned with
	* an appropriate erorr message.
	* 
	* @param pFunctionName	Name of the LISP-function.
	* @param plArguments	List of arguments, that are passed onto the LISP-function.
	* @return				The return value of the function or error message.
	*/
	CRV<CToken> callFunction(CToken pFunctionName, CLinkedList<CToken> plArguments) {
		//Find the function:
		CFunction function;
		bool bFunctionExists = false;
		for (unsigned int i = 0; i < lFunctions.size(); i++) {
			if (lFunctions[i].getName() == pFunctionName.getLexeme()) {
				//Found function at index i:
				function = lFunctions[i];
				bFunctionExists = true;
				break;
			}
		}
		if (!bFunctionExists) {
			//The called function does not exist:
			return CRV<CToken>(pFunctionName, Error::Interpreter::FUNCTION_DOES_NOT_EXIST);
		}
		
		//Check wether the function's parameter are identical with the passed arguments:
		CLinkedList<CVariable> lFunctionParameters; //Stores the parameters of the function.
		if (function.getParameterAmount() != plArguments.size()) {
			//Incorrect number of arguments are passed:
			return CRV<CToken>(pFunctionName, Error::Interpreter::INCORRECT_NUMBER_OF_ARGUMENTS_PASSED);
		}
		
		for (unsigned int i = 0; i < plArguments.size(); i++) {
			if (function.getParameters()[i].getType() != plArguments[i].getType()) {
				//Error: The passed type is not identical with the parameter type:
				return CRV<CToken>(plArguments[i], Error::produceConvertError(plArguments[i].getType(), function.getParameters()[i].getType()));
			}
			lFunctionParameters.add(CVariable(function.getParameters()[i].getName(), plArguments[i].getLexeme(), plArguments[i].getType()));
		}
		
		//Save the variables of the currently executed function:
		CLinkedList<CVariable> lOldVariables; //Stores the variables of the current function.
		lOldVariables.addAll(lVariables);
		lVariables.clear();
		lVariables.addAll(lFunctionParameters);
		
		//Execute the function's expressions:
		CLinkedList<CAbstractSyntaxTree<CToken>> lExpressions;
		CToken returnValue;
		lExpressions.addAll(function.getExpressions());
		for (unsigned int i = 0; i < lExpressions.size(); i++) {
			CRV<CToken> rvEval = interpretExpression(lExpressions[i]);
			if (rvEval.getErrorMessage() != Error::SUCCESS && rvEval.getErrorMessage() != Error::Interpreter::INFO_RETURN_STATEMENT_ENCOUNTERED) {
				//An error occured:
				return rvEval;
			}
			else if (rvEval.getErrorMessage() == Error::Interpreter::INFO_RETURN_STATEMENT_ENCOUNTERED) {
				//The function's execution was terminated through a return statement:
				returnValue = rvEval.getContent();
				break;
			}
		}
		
		if (function.getReturnType() == Token::U_VOID) {
			//No return value expected -> eventually returned values are ignored (ONLY TEMPORARY):
			//Restore variables of old function call:
			lVariables.clear();
			lVariables.addAll(lOldVariables);
			return CRV<CToken>(pFunctionName, Error::SUCCESS);
		}
		
		//Process the return value:
		if (returnValue.getType() != function.getReturnType()) {
			//The returned value's type is not identical with the function's return type:
			return CRV<CToken>(returnValue, Error::produceConvertError(returnValue.getType(), function.getReturnType()));
		}
		
		//Restore variables of old function call:
		lVariables.clear();
		lVariables.addAll(lOldVariables);
		
		//Return the return value:
		return CRV<CToken>(returnValue, Error::SUCCESS);
	}

};
