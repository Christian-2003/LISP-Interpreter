/*
FILE:		CTokenizer.hpp

REMARKS:	This file contains the class "CTokenizer", which is used to split up the source code into Tokens.
*/
#pragma once

#include <iostream>
#include <algorithm>

#include "CLinkedList.hpp"
#include "CToken.hpp"
#include "CRV.hpp"
#include "Variables/TokenTypes.hpp"
#include "Variables/ErrorMessages.hpp"
#include "Variables/KeywordTypes.hpp"

using namespace std;



/**
* Any object of this class can split up the source code into Tokens.
* 
* @author	Christian-2003
* @version	30.05.2021
*/
class CTokenizer {
private:
	/**
	* This represents the source code as a string.
	*/
	string sSourceCode;
	
	/**
	* This represents the filename, the current sourcecode originates from.
	*/
	string sFilename;

	/**
	* This represents the source code as a CLinkedList of CTokens.
	*/
	CLinkedList<CToken> lTokensObj;

	/**
	* This stores the current line number.
	*/
	unsigned int nCurrentLine;



private:
	/**
	* This function is used to tokenize every type of brackets, that might be present in the source code.
	*
	* @return	Error message.
	*/
	short int tokenizeParenthesis() {
		if (sSourceCode.length() <= 0) {
			//No sourcecode available:
			return Error::Tokenizer::SOURCECODE_NOT_LONG_ENOUGH;
		}
		if (sSourceCode[0] == '(') {
			//Found opened parenthesis:
			lTokensObj.add(CToken(string(1, sSourceCode[0]), Token::PARENTHESES_OPENED, sFilename, nCurrentLine));
		}
		else if (sSourceCode[0] == ')') {
			//Found closed parenthesis:
			lTokensObj.add(CToken(string(1, sSourceCode[0]), Token::PARENTHESES_CLOSED, sFilename, nCurrentLine));
		}
		sSourceCode.erase(0, 1); //Remove the parenthesis.
		return Error::SUCCESS;
	}

	/**
	* This function is used to tokenize identifiers or keywords.
	*
	* @return	Error message
	*/
	short int tokenizeIdentifierAndKeyword() {
		if (sSourceCode.length() <= 0) {
			//No sourcecode available:
			return Error::Tokenizer::SOURCECODE_NOT_LONG_ENOUGH;
		}
		string sString = ""; //Stores the identifier or keyword.
		for (unsigned int i = 0; i < sSourceCode.length(); i++) {
			char chCurrentCharacter = sSourceCode[i];
			if (chCurrentCharacter == ' ' || chCurrentCharacter == '(' || chCurrentCharacter == ')' || chCurrentCharacter == ';' || chCurrentCharacter == '\n') {
				//Current character marks the end of the identifier / keyword:
				break;
			}
			sString += chCurrentCharacter;
		}
		if (sString == Keyword::INT || sString == Keyword::DOUBLE || sString == Keyword::BOOL || sString == Keyword::CHAR || sString == Keyword::STRING || sString == Keyword::SET || sString == Keyword::DEFINE || sString == Keyword::IF || sString == Keyword::WHILE || sString == Keyword::PRINT || sString == Keyword::PRINTLN || sString == Keyword::RETURN) {
			lTokensObj.add(CToken(sString, Token::KEYWORD, sFilename, nCurrentLine));
		}
		else {
			string sCopyString = sString;
			for_each(sCopyString.begin(), sCopyString.end(), [](char& c) {c = ::tolower(c); }); //Convert string to lowercase.
			if (sCopyString == "t" || sCopyString == "nil") {
				lTokensObj.add(CToken(sCopyString, Token::U_BOOL, sFilename, nCurrentLine));
				sSourceCode.erase(0, sCopyString.length()); //Remove identifier / keyword from source code.
				return Error::SUCCESS;
			}
			lTokensObj.add(CToken(sString, Token::IDENTIFIER, sFilename, nCurrentLine));
		}
		sSourceCode.erase(0, sString.length()); //Remove identifier / keyword from source code.
		return Error::SUCCESS;
	}

	/**
	* This function is used to tokenize operators.
	*
	* @return	Error message
	*/
	short int tokenizeOperator() {
		if (sSourceCode.length() <= 0) {
			//No sourcecode available:
			return Error::Tokenizer::SOURCECODE_NOT_LONG_ENOUGH;
		}
		if (sSourceCode[0] == '+' || sSourceCode[0] == '*' || sSourceCode[0] == '/') {
			//Found arithmetic operator:
			lTokensObj.add(CToken(string(1, sSourceCode[0]), Token::OPERATOR_ARITHMETIC, sFilename, nCurrentLine));
			sSourceCode.erase(0, 1); //Remove arithmetic operator from sourcecode.
		}
		else if (sSourceCode[0] == '-') {
			if (sSourceCode.length() >= 2 && isdigit(sSourceCode[1])) {
				//Minus sign belongs to a negative number:
				tokenizeAtom();
			}
			else {
				//Found arithmetic operator:
				lTokensObj.add(CToken(string(1, sSourceCode[0]), Token::OPERATOR_ARITHMETIC, sFilename, nCurrentLine));
				sSourceCode.erase(0, 1); //Remove arithmetic operator from sourcecode.
			}
		}
		else if (sSourceCode[0] == '!' || sSourceCode[0] == '=' || sSourceCode[0] == '>' || sSourceCode[0] == '<') {
			if (sSourceCode.length() >= 2 && (sSourceCode[0] == '>' || sSourceCode[0] == '<') && sSourceCode[1] == '=') {
				//Found relational operator >= or <=:
				lTokensObj.add(CToken(string(1, sSourceCode[0]) + string(1, sSourceCode[1]), Token::OPERATOR_RELATIONAL, sFilename, nCurrentLine));
				sSourceCode.erase(0, 2); //Remove relational operator from sourcecode.
				return Error::SUCCESS;
			}
			//Found relational operator:
			lTokensObj.add(CToken(string(1, sSourceCode[0]), Token::OPERATOR_RELATIONAL, sFilename, nCurrentLine));
			sSourceCode.erase(0, 1); //Remove relational operator from sourcecode.
		}
		else if (sSourceCode[0] == '&' || sSourceCode[0] == '|') {
			//Found boolean operator:
			lTokensObj.add(CToken(string(1, sSourceCode[0]), Token::OPERATOR_BOOL, sFilename, nCurrentLine));
			sSourceCode.erase(0, 1); //Remove boolean operator from sourcecode.
		}
		return Error::SUCCESS;
	}

	/**
	* This function is used to tokenize any atom in lisp (Numbers, Booleans, strings, etc...).
	*
	* @return	Error message
	*/
	short int tokenizeAtom() {
		if (sSourceCode.length() <= 0) {
			//No sourcecode available:
			return Error::Tokenizer::SOURCECODE_NOT_LONG_ENOUGH;
		}

		if (sSourceCode[0] == '-' || isdigit(sSourceCode[0])) {
			//Found number:
			string sDigitsOfNumber(1, sSourceCode[0]); //Stores the number as string.
			bool bHasDecimalPoint = false; //Stores, wether the number has a decimal point.
			//Get the rest of the number:
			for (unsigned int i = 1; i < sSourceCode.length(); i++) {
				char currentDecimal = sSourceCode[i];
				if (isdigit(currentDecimal)) {
					sDigitsOfNumber += currentDecimal;
				}
				else if (currentDecimal == '.') {
					if (bHasDecimalPoint) {
						//ERROR: number already has a decimal point!
						return Error::Tokenizer::TOO_MANY_DECIMALS;
					}
					sDigitsOfNumber += currentDecimal;
					bHasDecimalPoint = true;
				}
				else {
					break;
				}
			}
			if (bHasDecimalPoint) {
				lTokensObj.add(CToken(sDigitsOfNumber, Token::U_DOUBLE, sFilename, nCurrentLine));
			}
			else {
				lTokensObj.add(CToken(sDigitsOfNumber, Token::U_INT, sFilename, nCurrentLine));
			}
			sSourceCode.erase(0, sDigitsOfNumber.length());
		}

		else if (sSourceCode[0] == '\"') {
			//Found String:
			sSourceCode.erase(0, 1); //Remove quotation marks.
			string sString = ""; //Stores the string.
			for (unsigned int i = 0; i < sSourceCode.length(); i++) {
				char currentCharacter = sSourceCode[i];
				if (currentCharacter == '\"') {
					//End of string:
					sSourceCode.erase(i, 1); //Remove quotation marks.
					break;
				}
				sString += currentCharacter;
			}
			lTokensObj.add(CToken(sString, Token::U_STRING, sFilename, nCurrentLine));
			sSourceCode.erase(0, sString.length()); //Remove String from sourcecode.
		}

		else if (sSourceCode[0] == '\'') {
			//Found character:
			sSourceCode.erase(0, 1); //Remove quotation marks.
			if (sSourceCode[0] == '\'') {
				//Next character resembles the next quotation marks: Error, no char found:
				return Error::Tokenizer::EMPTY_CHARACTER_ENCOUNTERED;
			}
			string sCharacter = "";
			sCharacter += sSourceCode[0];
			sSourceCode.erase(0, 1); //Remove the character from the source code.
			if (sSourceCode[0] != '\'') {
				//Next character does not represent the closed quotation marks: Error, no exit marks:
				return Error::Tokenizer::NO_EXIT_QUOTATION_MARK;
			}
			sSourceCode.erase(0, 1); //Remove quotation marks.
			lTokensObj.add(CToken(sCharacter, Token::U_CHAR, sFilename, nCurrentLine));
		}
		return Error::SUCCESS;
	}



public:
	/**
	* Default constructor of this class instantiates a new Tokenizer.
	*/
	CTokenizer() {
		sFilename = "";
		sSourceCode = "";
		nCurrentLine = 1;
	}



public:
	/**
	* This function starts the process of splitting up the source code into Tokens.
	*
	* @param psSourceCode	Source code, which is supposed to be split up into Tokens.
	* @param psFilename		Filename, from which the sourcecode originates.
	* @return				List of Tokens.
	*/
	CRV<CLinkedList<CToken>> tokenize(string psSourceCode, string psFilename) {
		sFilename = psFilename;
		sSourceCode = psSourceCode;
		//transform(sSourceCode.begin(), sSourceCode.end(), sSourceCode.begin(), ::tolower); //Transform input to only include lowercase characters.
		lTokensObj.clear();
		nCurrentLine = 1;

		while (!sSourceCode.empty()) {
			char chCurrent = sSourceCode[0];
			if (chCurrent == '-' || chCurrent == '+' || chCurrent == '/' || chCurrent == '*' || chCurrent == '!' || chCurrent == '=' || chCurrent == '<' || chCurrent == '>' || chCurrent == '&' || chCurrent == '|') {
				//Found operator:
				short int nErrorMessage = tokenizeOperator();
				if (nErrorMessage != Error::SUCCESS) {
					return CRV<CLinkedList<CToken>>(CLinkedList<CToken>(), nErrorMessage); //Returns an empty CLinkedList.
				}
			}
			else if (chCurrent == '(' || chCurrent == ')') {
				//Found parenthesis:
				short int nErrorMessage = tokenizeParenthesis();
				if (nErrorMessage != Error::SUCCESS) {
					return CRV<CLinkedList<CToken>>(CLinkedList<CToken>(), nErrorMessage); //Returns an empty CLinkedList.
				}
			}
			else if (isdigit(chCurrent) || chCurrent == '\"' || chCurrent == '\'') {
				//Found atom:
				short int nErrorMessage = tokenizeAtom();
				if (nErrorMessage != Error::SUCCESS) {
					return CRV<CLinkedList<CToken>>(CLinkedList<CToken>(), nErrorMessage); //Returns an empty CLinkedList.
				}
			}
			else if (chCurrent == ';') {
				//Found comment:
				for (unsigned int i = 0; i < sSourceCode.length(); i++) {
					if (sSourceCode[i] == '\n') {
						//Found the end of the line:
						sSourceCode.erase(0, i); //Remove the comment.
						//Don't need to switch into the next line, since this is done in the next iteration of the while loop.
						break;
					}
				}
			}
			else if (chCurrent == ' ' || chCurrent == '\t' || chCurrent == '\n') {
				//Found characters, that need to be erased:
				sSourceCode.erase(0, 1);
				if (chCurrent == '\n') {
					nCurrentLine++; //Switch into the next line.
				}
			}
			else {
				//Something else (probably an identifier or keyword) found:
				short int nErrorMessage = tokenizeIdentifierAndKeyword();
				if (nErrorMessage != Error::SUCCESS) {
					return CRV<CLinkedList<CToken>>(CLinkedList<CToken>(), nErrorMessage); //Returns an empty CLinkedList.
				}
			}
		}
		return CRV<CLinkedList<CToken>>(lTokensObj, Error::SUCCESS);
	}
};
