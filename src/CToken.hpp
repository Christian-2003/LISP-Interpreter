/*
FILE:		CToken.hpp

REMARKS:	This file contains the class "CToken", which is used to represent a Token.
*/
#pragma once

#include <iostream>

using namespace std;


/**
* Objects of this type represent a Token, with which the Interpreter can work.
* 
* @author	Christian-2003
* @version	30.05.2021
*/
class CToken {
private:
	/**
	* This represents the content of this Token.
	*/
	string sLexeme;

	/**
	* This resembles the type of the current Token.
	*/
	short int nType;

	/**
	* This represents the filename, from which the token is from.
	*/
	string sFilename;

	/**
	* This represents the line, in which the token is located.
	*/
	unsigned int nLine;



public:
	/**
	* This default constructor creates a new Token, without any content or type.
	*/
	CToken() {
		sLexeme = "";
		nType = 0;
		sFilename = "";
		nLine = 0;
	}

	/**
	* This constructor instantiates a new Token with the passed values as lexeme and type.
	*
	* @param psLexeme	Lexeme of this Token.
	* @param pnType		Type of this Token.
	*/
	CToken(string psLexeme, int pnType) {
		sLexeme = psLexeme;
		nType = pnType;
		sFilename = "";
		nLine = 0;
	}

	/**
	* This constructor instantiates a new Token with the passed values as lexeme, type, filename and line.
	*
	* @param psLexeme	Lexeme of this token.
	* @param pnType		Type of this token.
	* @param psFilename	Name of the file, the token originates from.
	* @param pnline		Line number, from which the token originates.
	*/
	CToken(string psLexeme, short int pnType, string psFilename, unsigned int pnLine) {
		sLexeme = psLexeme;
		nType = pnType;
		sFilename = psFilename;
		nLine = pnLine;
	}



public:
	/**
	* Returns the type of the current Token.
	* 
	* @return	Type of the Token.
	*/
	short int getType() {
		return nType;
	}

	/**
	* Returns the lexeme of the current Token.
	* 
	* @return	Lexeme of the Token.
	*/
	string getLexeme() {
		return sLexeme;
	}

	/**
	* Returns the filename, the token originates from.
	* 
	* @return	Filename of the token.
	*/
	string getFilename() {
		return sFilename;
	}

	/**
	* Returns the line, the token originates from.
	* 
	* @return	Line of the token.
	*/
	unsigned int getLine() {
		return nLine;
	}
};
