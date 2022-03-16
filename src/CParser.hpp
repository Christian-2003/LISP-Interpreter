/*
FILE:		CParser.hpp

REMARKS:	This file contains the class "CParser", with which an abstract syntax tree can be created.
*/
#pragma once

#include <iostream>
#include <string>

#include "CAbstractSyntaxTree.hpp"
#include "CLinkedList.hpp"
#include "CRV.hpp"
#include "CToken.hpp"
#include "Variables/ErrorMessages.hpp"
#include "Variables/KeywordTypes.hpp"
#include "Variables/TokenTypes.hpp"

using namespace std;



/**
* This class is used to create an abstract syntax tree (of the type "CAbstractSyntaxTree") with a list of
* Tokens (of the type "CLinkedList<CToken>"). The abstract syntax tree can be traversed afterwards through
* another class.
* 
* @author	Christian-2003
* @version	01.06.2021
*/
class CParser {
private:
	/**
	* Represents a list of tokens, with which the abstract syntax tree is created afterwards.
	*/
	CLinkedList<CToken> lTokensObj;



public:
	/**
	* Default constructor of this class instantiates a new CParser.
	*/
	CParser() {
		//Does nothing...
	}



private:
	/**
	* This function parses a list of Atoms, which is represented through an AST.
	* 
	* @retrun	AST that was created with the list of tokens.
	*/
	CRV<CAbstractSyntaxTree<CToken>> parseList() {
		if (lTokensObj.empty()) {
			//No tokens available:
			return CRV<CAbstractSyntaxTree<CToken>>(CAbstractSyntaxTree<CToken>(), Error::Parser::SYNTAX_P);
		}

		//First token resembles the head of the AST:
		CToken headNode = lTokensObj.retrieve(0);
		if (headNode.getType() == Token::PARENTHESES_OPENED) {
			//Found a branch in the sourcecode:
			CLinkedList<CAbstractSyntaxTree<CToken>> lListExpressions; //Stores the ASTs of this list.
			//Parse every other expression, that is represented inside this branch:
			while (!lTokensObj.empty()) {
				CToken currentToken = lTokensObj[0];
				if (currentToken.getType() == Token::PARENTHESES_OPENED) {
					//Another expression needs to be parsed:
					lTokensObj.remove(0); //Remove opened parenthesis.
					CRV<CAbstractSyntaxTree<CToken>> currentAST = parseList();
					if (currentAST.getErrorMessage() != Error::Parser::EMPTY_AST_P && currentAST.getErrorMessage() != Error::SUCCESS) {
						//An error occured:
						return currentAST;
					}
					else if (currentAST.getErrorMessage() != Error::Parser::EMPTY_AST_P) {
						//Add the returned AST to the list:
						lListExpressions.add(currentAST.getContent());
					}
					continue;
				}
				else if (currentToken.getType() == Token::PARENTHESES_CLOSED) {
					//Parsed every expression inside this branch:
					lTokensObj.remove(0); //Remove closed parenthesis.
					break;
				}
				else {
					CRV<CAbstractSyntaxTree<CToken>> rv_atom = parseAtom(); //Parse the atom.
					if (rv_atom.getErrorMessage() != Error::SUCCESS) {
						//An error occured:
						return rv_atom;
					}
					lListExpressions.add(rv_atom.getContent());
				}
			}
			return CRV<CAbstractSyntaxTree<CToken>>(CAbstractSyntaxTree<CToken>(CToken(to_string(BRANCH), BRANCH, headNode.getFilename(), headNode.getLine()), lListExpressions), Error::SUCCESS);
		}
		else if (headNode.getType() == Token::PARENTHESES_CLOSED) {
			//The parsed list is empty:
			return CRV<CAbstractSyntaxTree<CToken>>(CAbstractSyntaxTree<CToken>(), Error::Parser::EMPTY_AST_P);
		}
		
		//Parse every other token, until a closed parenthesis is encountered:
		CLinkedList< CAbstractSyntaxTree<CToken>> lSubTrees; //Stores every subtree of the headNode.
		while (!lTokensObj.empty()) {
			CToken currentToken = lTokensObj[0];
			if (currentToken.getType() == Token::PARENTHESES_OPENED) {
				//Another list needs to be parsed:
				if (lTokensObj.size() >= 2 && lTokensObj[1].getType() != Token::PARENTHESES_OPENED) {
					lTokensObj.remove(0); //Remove parenthesis.
				}
				CRV<CAbstractSyntaxTree<CToken>> currentAST = parseList();
				if (currentAST.getErrorMessage() != Error::Parser::EMPTY_AST_P && currentAST.getErrorMessage() != Error::SUCCESS) {
					//An error occured:
					return currentAST;
				}
				else if (currentAST.getErrorMessage() != Error::Parser::EMPTY_AST_P) {
					//Add the returned AST to the list:
					lSubTrees.add(currentAST.getContent());
				}
			}
			else if (currentToken.getType() == Token::PARENTHESES_CLOSED) {
				//The currently parsed list ends with the current token:
				lTokensObj.remove(0); //Remove parenthesis.
				CAbstractSyntaxTree<CToken> listAST{ headNode, lSubTrees };
				return CRV<CAbstractSyntaxTree<CToken>>(listAST, Error::SUCCESS); //Return the AST.
			}
			else {
				CRV<CAbstractSyntaxTree<CToken>> rv_atom = parseAtom(); //Parse the atom.
				if (rv_atom.getErrorMessage() != Error::SUCCESS) {
					//An error occured:
					return rv_atom;
				}
				lSubTrees.add(rv_atom.getContent());
			}
		}
		//The parsed list does not end (no closed parenthesis):
		return CRV<CAbstractSyntaxTree<CToken>>(CAbstractSyntaxTree<CToken>(), Error::Parser::SYNTAX_P);
	}



	/**
	* This function parses a single atom.
	* 
	* @retrun	AST that was created with the single atom.
	*/
	CRV<CAbstractSyntaxTree<CToken>> parseAtom() {
		if (lTokensObj.empty()) {
			//No tokens available:
			return CRV<CAbstractSyntaxTree<CToken>>(CAbstractSyntaxTree<CToken>(), Error::Parser::SYNTAX_P);
		}

		CToken atomToken = lTokensObj.retrieve(0); //Stores the token of the current atom.
		if (atomToken.getType() == Token::PARENTHESES_CLOSED || atomToken.getType() == Token::PARENTHESES_OPENED) {
			//Atom can never be opened or closed parenthesis:
			return CRV<CAbstractSyntaxTree<CToken>>(CAbstractSyntaxTree<CToken>(), Error::Parser::ATOM_CAN_NEVER_BE_PARENTHESES);
		}

		return CRV<CAbstractSyntaxTree<CToken>>(CAbstractSyntaxTree<CToken>(atomToken), Error::SUCCESS); //Return the token as AST.
	}



public:
	/**
	* This function starts a recursive algorithm which creates an abstract syntax tree, and returns said tree through a CRV.
	* 
	* @param plTokensObj	List of tokens, with which the AST should be created.
	* @return				AST that was created with the passed list of tokens.
	*/
	CRV<CLinkedList<CAbstractSyntaxTree<CToken>>> parse(CLinkedList<CToken> plTokensObj) {
		logger.addEntry("Begin syntactical analysis (Parser).", LogEntryComponent::PARSER, LogEntryType::INF);
		lTokensObj.clear();
		lTokensObj.addAll(plTokensObj);

		CLinkedList<CAbstractSyntaxTree<CToken>> lASTs; //Stores every abstract syntax tree of the source code.
		while (!lTokensObj.empty()) {
			CToken firstToken = lTokensObj.retrieve(0); //Removes first token (Should be opened parenthesis).
			if (firstToken.getType() != Token::PARENTHESES_OPENED) {
				//First token is no opened parenthesis -> Syntax error:
				logger.addEntry_tokenException(firstToken, LogEntryComponent::PARSER, LogEntryType::FATAL, Error::Parser::SYNTAX_P, Error::generateErrorMessage(Error::Parser::SYNTAX_P));
				return CRV<CLinkedList<CAbstractSyntaxTree<CToken>>>(CLinkedList<CAbstractSyntaxTree<CToken>>(), Error::Parser::SYNTAX_P); //Return empty list.
			}
			CRV<CAbstractSyntaxTree<CToken>> rv_parseList = parseList(); //Parses the list.
			if (rv_parseList.getErrorMessage() != Error::SUCCESS) {
				//An error occured:
				logger.addEntry_tokenException(rv_parseList.getContent().getContent(), LogEntryComponent::PARSER, LogEntryType::FATAL, rv_parseList.getErrorMessage(), Error::generateErrorMessage(rv_parseList.getErrorMessage()));
				return CRV<CLinkedList<CAbstractSyntaxTree<CToken>>>(CLinkedList<CAbstractSyntaxTree<CToken>>(), rv_parseList.getErrorMessage());
			}
			lASTs.add(rv_parseList.getContent());
		}

		logger.addEntry("End syntactical analysis (Parser) successfully.", LogEntryComponent::PARSER, LogEntryType::INF);
		return CRV<CLinkedList<CAbstractSyntaxTree<CToken>>>(lASTs, Error::SUCCESS); //Return every ast.
	}
};
