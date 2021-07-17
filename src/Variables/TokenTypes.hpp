/*
FILE:		TokenTypes.hpp

REMARKS:	The type of every token is stored in this file.
*/
#pragma once

using namespace std;



/**
* This enumeration contains every token-type that can be evaluated by the Interpreter.
*/
enum Token : const short int {
	UNKNOWN = 0, //Unknown token.
	PARENTHESES_CLOSED = 1, //Closed parentheses.
	PARENTHESES_OPENED = 2, //Opened parentheses.
	U_INT = 3, //Integer value.
	U_DOUBLE = 4, //Double value.
	U_STRING = 5, //String value.
	U_CHAR = 6, //Character value.
	U_BOOL = 7, //Boolean value.
	IDENTIFIER = 8, //Identifier (of a function, variable, class, etc...).
	KEYWORD = 9, //Keyword.
	OPERATOR_ARITHMETIC = 10, //Arithmetic operator (+, -, *, /).
	OPERATOR_RELATIONAL = 11, //Relational operator (!, =, >=, <=, >, <).
	OPERATOR_BOOL = 12, //Boolean operator (&, |).
	BRANCH = 13 //Represents a branch in the AST (Not part of the actual sourcecode!).
};



/**
* THIS FUNCTION ONLY SERVES FOR DEBUG PURPOSES.
* It prints the type of a token in written form, and not just a number.
* 
* @param pnTokenType	Type of the token.
*/
string printTokenType(short int pnTokenType) {
	switch (pnTokenType) {
	case 0:
		return "Unknown";
	case 1:
		return "Parenthesis_Closed";
	case 2:
		return "Parenthesis_Opened";
	case 3:
		return "Integer";
	case 4:
		return "Double";
	case 5:
		return "String";
	case 6:
		return "Character";
	case 7:
		return "Boolean";
	case 8:
		return "Identifier";
	case 9:
		return "Keyword";
	case 10:
		return "Arithmetic_Operator";
	case 11:
		return "Relational_Operator";
	case 12:
		return "Boolean_Operator";
	case 13:
		return "Branch";
	default:
		return "Unknown";
	}
}
