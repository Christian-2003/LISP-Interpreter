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
	* This function can interpret an abstract syntax tree.
	* 
	* @param pAST	Abstract syntax tree, which is supposed to be evaluated by the interpreter.
	* @return		Error message.
	*/
	CRV<CToken> interpret(CAbstractSyntaxTree<CToken> pAST) {
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
				CRV<CToken> rvEval = interpret(currentAST); //Stores the result of the evaluation of the identifier.
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
					CRV<CToken> rvEval = interpret(lASTs[0]);
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
				CRV<CToken> rvEval = interpret(lASTs[0]);
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
					CRV<CToken> rvEval = interpret(current);
					if (rvEval.getErrorMessage() != Error::SUCCESS) {
						//An error occured:
						return rvEval;
					}
					cout << rvEval.getContent().getLexeme();
				}
				else if (current.getContent().getType() == Token::IDENTIFIER) {
					//Need to get the value of variable:
					CRV<CVariable> rvGetVar = getVariable(current.getContent().getLexeme());
					if (rvGetVar.getErrorMessage() != Error::SUCCESS) {
						//An error occured:
						return CRV<CToken>(current.getContent(), rvGetVar.getErrorMessage());
					}
					cout << rvGetVar.getContent().getValue();
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
		
		else {
			//Found invalid keyword:
			return CRV<CToken>(headNode, Error::Interpreter::INCORRECT_TOKEN);
		}
	}
	


	/**
	* This function is used to evaluate an identifier.
	* The headnode of this AST must resemble the identifier.
	*/
	CRV<CToken> evaluateIdentifier(CAbstractSyntaxTree<CToken> pAST) {
		if (pAST.hasSubTrees()) {
			//Identifier resembles function call:
			return CRV<CToken>(pAST.getContent(), Error::Interpreter::INCORRECT_TOKEN);
			//TODO: Implement functions and function calls!
		}
		else {
			//Identifier reesembles variable name:
			CRV<CVariable> rvVariable = getVariable(pAST.getContent().getLexeme());
			if (rvVariable.getErrorMessage() != Error::SUCCESS) {
				//An error occured:
				return CRV<CToken>(pAST.getContent(), rvVariable.getErrorMessage());
			}
			//Variable was successfully returned:
			return CRV<CToken>(CToken(rvVariable.getContent().getValue(), rvVariable.getContent().getType(), pAST.getContent().getFilename(), pAST.getContent().getLine()), Error::SUCCESS);
		}
	}
};
