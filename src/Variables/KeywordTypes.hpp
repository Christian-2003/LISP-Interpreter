/*
FILE:		KeywordTypes.hpp

REMARKS:	Every keyword is stored in this file.
*/
#pragma once

#include <iostream>

using namespace std;



namespace Keyword {
	/**
	* Keyword to declare a new integer variable.
	*/
	const string INT = "int";

	/**
	* Keyword to declare a new double variable.
	*/
	const string DOUBLE = "double";

	/**
	* Keyword to declare a new boolean variable.
	*/
	const string BOOL = "boolean";

	/**
	* Keyword to declare a new character variable.
	*/
	const string CHAR = "char";

	/**
	* Keyword to declare a mew string variable.
	*/
	const string STRING = "string";

	/**
	* Keyword to change a variable's value.
	*/
	const string SET = "set";

	/**
	* Keyword to define a new function.
	*/
	const string DEFINE = "defun";

	/**
	* Keyword for an if-statement.
	*/
	const string IF = "if";

	/**
	* Keyword for a while-loop.
	*/
	const string WHILE = "while";

	/**
	* Keyword to print anything into the console.
	*/
	const string PRINT = "print";

	/**
	* Keyword to print anything into the console and add a line break afterwards.
	*/
	const string PRINTLN = "println";
}
