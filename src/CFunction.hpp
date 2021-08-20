/*
FILE:		CFunction.hpp

REMARKS:	Contains the "CFunction"-class which is used to model a Lisp-function.
*/
#pragma once

#include <iostream>

#include "CToken.hpp"
#include "CRV.hpp"
#include "CAbstractSyntaxTree.hpp"
#include "CLinkedList.hpp"
#include "CVariable.hpp"
#include "Variables/ErrorMessages.hpp"
#include "Variables/KeywordTypes.hpp"
#include "Variables/TokenTypes.hpp"

using namespace std;



/**
* This class can resemble a function in Lisp.
* 
* @author	ChosenChris
* @version	21.07.2021
*/
class CFunction {
private:
	/**
	* Stores the name of the function.
	*/
	string sName;

	/**
	* Stores the function's return value.
	*/
	short int nReturnValue;

	/**
	* Stores every parameter of the function.
	*/
	CLinkedList<CVariable> lParameters;

	/**
	* Stores every expression of the function as abstract syntax tree.
	*/
	CLinkedList<CAbstractSyntaxTree<CToken>> lExpressions;



public:
	/**
	* The default constructor of this class instantiates a new instance of this class.
	*/
	CFunction() {
		sName = "";
		nReturnValue = Token::U_VOID;
	}




public:
	/**
	* This function creates the Lisp-function based on the passed AST.
	* 
	* @param pAST	Abstract syntax tree, which resembles the function-definition in the Lisp sourceceode.
	* @return		Error message including the token which caused the error.
	*/
	CRV<CToken> evaluateFunctionSourceCode(CAbstractSyntaxTree<CToken> pAST) {
		//Headnode of the AST makes up the return value (e.g. "int" or "double"):
		CToken returnTypeNode = pAST.getContent(); //Stores the headNode of the passed AST.
		if (returnTypeNode.getLexeme() == Keyword::INT) {
			nReturnValue = Token::U_INT;
		}
		else if (returnTypeNode.getLexeme() == Keyword::DOUBLE) {
			nReturnValue = Token::U_DOUBLE;
		}
		else if (returnTypeNode.getLexeme() == Keyword::BOOL) {
			nReturnValue = Token::U_BOOL;
		}
		else if (returnTypeNode.getLexeme() == Keyword::CHAR) {
			nReturnValue = Token::U_CHAR;
		}
		else if (returnTypeNode.getLexeme() == Keyword::STRING) {
			nReturnValue = Token::U_STRING;
		}
		else if (returnTypeNode.getLexeme() == Keyword::R_VOID) {
			nReturnValue = Token::U_VOID;
		}
		else {
			//Incorrect return value encountered:
			return CRV<CToken>(returnTypeNode, Error::Interpreter::INCORRECT_TOKEN);
		}

		CLinkedList<CAbstractSyntaxTree<CToken>> lSubTrees; //Stores the subtrees of the passed AST.
		if (!pAST.hasSubTrees()) {
			//Error: Function is empty:
			return CRV<CToken>(returnTypeNode, Error::Interpreter::INCORRECT_FUNCTION_DEFINITION);
		}
		lSubTrees.addAll(pAST.getSubTrees());
		
		//First subtree resembles the function's name:
		if (lSubTrees[0].getContent().getType() != Token::IDENTIFIER || lSubTrees[0].hasSubTrees()) {
			//Error: First token does not resemble the function's name:
			return CRV<CToken>(lSubTrees[0].getContent(), Error::Interpreter::FUNCTION_NAME_IS_INCORRECT);
		}
		sName = lSubTrees[0].getContent().getLexeme();
		
		//Find out the function's parameters:
		if (lSubTrees.size() % 2 != 0 && lSubTrees.size() > 1) {
			//There is at least one parameter:
			CAbstractSyntaxTree<CToken> parameterAST = lSubTrees[1];
			if (parameterAST.getContent().getType() != Token::BRANCH) {
				//There is only one parameter:
				if (!parameterAST.hasSubTrees()) {
					//The parameter does not have a name:
					return CRV<CToken>(parameterAST.getContent(), Error::Interpreter::NOT_ENOUGH_ARGUMENTS);
				}
				else if (parameterAST.getSubTrees().size() > 1) {
					//The parameter somehow has too many arguments:
					return CRV<CToken>(parameterAST.getContent(), Error::Interpreter::TOO_MANY_ARGUMENTS);
				}
				short int nParameterType = Token::U_VOID;
				if (parameterAST.getContent().getLexeme() == Keyword::INT) {
					nParameterType = Token::U_INT;
				}
				else if (parameterAST.getContent().getLexeme() == Keyword::DOUBLE) {
					nParameterType = Token::U_DOUBLE;
				}
				else if (parameterAST.getContent().getLexeme() == Keyword::BOOL) {
					nParameterType = Token::U_BOOL;
				}
				else if (parameterAST.getContent().getLexeme() == Keyword::CHAR) {
					nParameterType = Token::U_CHAR;
				}
				else if (parameterAST.getContent().getLexeme() == Keyword::STRING) {
					nParameterType = Token::U_STRING;
				}
				 
				lParameters.add(CVariable(parameterAST.getSubTreeAtIndex(0).getContent().getLexeme(), "", nParameterType));
			}
			else {
				//There are multiple parameters:
				CLinkedList<CAbstractSyntaxTree<CToken>> ltParameters; //Stores the paraemeters as subtrees:
				ltParameters.addAll(parameterAST.getSubTrees());
				for (unsigned int i = 0; i < ltParameters.size(); i++) {
					CAbstractSyntaxTree<CToken> parameterAST = ltParameters[i];
					if (!parameterAST.hasSubTrees()) {
						//The parameter does not have a name:
						return CRV<CToken>(parameterAST.getContent(), Error::Interpreter::NOT_ENOUGH_ARGUMENTS);
					}
					else if (parameterAST.getSubTrees().size() > 1) {
						//The parameter somehow has too many arguments:
						cout << "DEBUG: 1" << endl;
						return CRV<CToken>(parameterAST.getContent(), Error::Interpreter::TOO_MANY_ARGUMENTS);
					}
					short int nParameterType = Token::U_VOID;
					if (parameterAST.getContent().getLexeme() == Keyword::INT) {
						nParameterType = Token::U_INT;
					}
					else if (parameterAST.getContent().getLexeme() == Keyword::DOUBLE) {
						nParameterType = Token::U_DOUBLE;
					}
					else if (parameterAST.getContent().getLexeme() == Keyword::BOOL) {
						nParameterType = Token::U_BOOL;
					}
					else if (parameterAST.getContent().getLexeme() == Keyword::CHAR) {
						nParameterType = Token::U_CHAR;
					}
					else if (parameterAST.getContent().getLexeme() == Keyword::STRING) {
						nParameterType = Token::U_STRING;
					}
					lParameters.add(CVariable(parameterAST.getSubTreeAtIndex(0).getContent().getLexeme(), "", nParameterType));
				}
			}
		}
		
		//Find out if the function has expressions:
		if (pAST.getSubTreeNumber() < 2) {
			//Function has no expressions:
			return CRV<CToken>(lSubTrees[0].getContent(), Error::Interpreter::MISSING_FUNCTION_EXPRESSION);
		}

		//Find out the function's expressions:
		CAbstractSyntaxTree<CToken> tExpression = lSubTrees[lSubTrees.size() - 1]; //Stores the function's expressions as parameters.
		if (tExpression.getContent().getType() == Token::BRANCH) {
			//There are multiple expressions:
			lExpressions.addAll(tExpression.getSubTrees()); //Add every subtree to the list of expressions.
		}
		else {
			//There is only one expression:
			lExpressions.add(tExpression);
		}
		
		// ----- THE FOLLOWING CODE IS ONLY FOR DEBUGGING -----
		//cout << "DEBUG: functionName=\"" << sName << "\", returnValue=" << nReturnValue << ", numOfParameters=" << lParameters.size() << ", numOfExpressions=" << lExpressions.size() << endl;
		// ----------------------------------------------------

		return CRV<CToken>(CToken(), Error::SUCCESS);
	}




public:
	/**
	* This function returns the Lisp-function's identifier.
	* 
	* @return	Name of the Lisp function.
	*/
	string getName() {
		return sName;
	}

	/**
	* This function returns the Lisp-function's return type.
	* 
	* @return	Return type of the Lisp function.
	*/
	short int getReturnType() {
		return nReturnValue;
	}

	/**
	* This function returns the Lisp-function's parameters.
	* 
	* @return	Parameters of the Lisp function.
	*/
	CLinkedList<CVariable> getParameters() {
		return lParameters;
	}

	/**
	* This function returns the Lisp-function's expressions.
	* 
	* @return	Expressions of the Lisp function.
	*/
	CLinkedList<CAbstractSyntaxTree<CToken>> getExpressions() {
		return lExpressions;
	}

	/**
	* This function returns the number of the Lispfunction's parameters.
	* 
	* @return	Number of parameters.
	*/
	unsigned int getParameterAmount() {
		return lParameters.size();
	}
};
