/*
FILE:		main.cpp

REMARKS:	This file contains the "main()"-function, everything that is needed to execute sourcecode and everything for the UX.
*/
#include <iostream>
#include <string>
#include <Windows.h>
#include <cctype>

#include "CLinkedList.hpp"
#include "CAbstractSyntaxTree.hpp"
#include "CTokenizer.hpp"
#include "CToken.hpp"
#include "CParser.hpp"
#include "CFileScanner.hpp"
#include "CInterpreter.hpp"

using namespace std;
inline void execute(string, bool);
inline void printAST(CAbstractSyntaxTree<CToken>, int);
inline void printError(CToken, short int);



/**
* Changes the text color of the text in the terminal.
*
* @param pnColor	Color, to which the text should be changed.
*/
inline void SetTextColor(int pnColor) {
	HANDLE consoleHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHANDLE, pnColor);
}



/**
* This function executes sourcecode from a specific file.
*
* @param psFilename		File, in which the sourcecode is located.
*/
inline void execute(string psFilename, bool pbDebugMode) {
	//-+-+-+-+-+-+- READ THE FILE -+-+-+-+-+-+-
	CFileScanner fileScanner;
	CRV<string> rvFileContent = fileScanner.scan(psFilename);
	if (rvFileContent.getErrorMessage() != Error::SUCCESS) {
		//An error occured:
		printError(CToken(), rvFileContent.getErrorMessage());
		return;
	}
	//----- DEBUG BEGIN -----
	if (pbDebugMode) {
		SetTextColor(8);
		cout << "Sourcecode=\n" << rvFileContent.getContent() << endl;
	}
	//------ DEBUG END ------

	//-+-+-+-+-+-+- TOKENIZING -+-+-+-+-+-+-
	CTokenizer tokenizer;
	CRV<CLinkedList<CToken>> rvTokens = tokenizer.tokenize(rvFileContent.getContent(), psFilename);
	CLinkedList<CToken> tokens;
	if (rvTokens.getErrorMessage() != Error::SUCCESS) {
		//An error occured:
		printError(rvTokens.getContent()[0], rvTokens.getErrorMessage());
		return;
	}
	tokens.addAll(rvTokens.getContent());
	//----- DEBUG BEGIN -----
	if (pbDebugMode) {
		SetTextColor(8);
		cout << "TokenList=" << endl;
		for (unsigned int i = 0; i < tokens.size(); i++) {
			cout << "[" << tokens[i].getFilename() << "(" << tokens[i].getLine() << "), \"" << tokens[i].getLexeme() << "\": " << printTokenType(tokens[i].getType()) << "]" << endl;
			//cout << tokens[i].getFilename() << "\\" << tokens[i].getLine() << "\tLEXEME=\"" << tokens[i].getLexeme() << "\",\tTYPE=" << printTokenType(tokens[i].getType()) << endl;
		}
		cout << "\n";
	}
	//------ DEBUG END ------

	//-+-+-+-+-+-+- PARSING -+-+-+-+-+-+-
	CParser parser;
	CRV<CLinkedList<CAbstractSyntaxTree<CToken>>> rvAST = parser.parse(tokens);
	if (rvAST.getErrorMessage() != Error::SUCCESS) {
		//An error occured:
		printError(rvAST.getContent()[0].getContent(), rvAST.getErrorMessage());
		return;
	}
	CLinkedList<CAbstractSyntaxTree<CToken>> lASTs = rvAST.getContent();
	//----- DEBUG BEGIN -----
	if (pbDebugMode) {
		SetTextColor(8);
		for (unsigned int i = 0; i < lASTs.size(); i++) {
			cout << "AbstractSyntaxTree " << i << "=" << endl;
			printAST(lASTs[i], 0);
			cout << "\n";
		}
	}
	//------ DEBUG END ------

	//-+-+-+-+-+-+- INTERPRETING -+-+-+-+-+-+-
	CInterpreter interpreter;
	SetTextColor(7); //Change textcolor for output while interpreting.
	CRV<CToken> rvInterpreter;
	rvInterpreter = interpreter.interpret(lASTs);
	if (rvInterpreter.getErrorMessage() != Error::SUCCESS) {
		//An error occured:
		cout << '\n';
		printError(rvInterpreter.getContent(), rvInterpreter.getErrorMessage());
		return;
	}
}

/**
* This function is only used for debugging purposes.
* It prints a ast to the console.
*
* @param pAST	AST
* @param nDepth	Depth of the current AST
*/
inline void printAST(CAbstractSyntaxTree<CToken> pAST, int nDepth) {
	//Print the current node:
	if (pAST.getContent().getType() == BRANCH) {
		cout << "[BRANCH]:" << endl;
	}
	else {
		cout << "[\"" << pAST.getContent().getLexeme() << "\", " << printTokenType(pAST.getContent().getType()) << "]" << endl;
		//cout << pAST.getContent().getLexeme() << endl;
	}

	if (pAST.hasSubTrees()) {
		//Print subtrees if they exist:

		//Print every subtree of the current AST:
		CLinkedList<CAbstractSyntaxTree<CToken>> treeList;
		treeList.addAll(pAST.getSubTrees());
		for (unsigned int i = 0; i < treeList.size(); i++) {
			for (int j = 0; j <= nDepth; j++) {
				cout << "   ";
			}
			printAST(treeList[i], nDepth + 1);
		}
	}
}

/**
* This function prints the error message to the console.
*
* @param errorToken	Token, which caused the occuring error.
*/
inline void printError(CToken errorToken, short int pnErrorMessage) {
	if (pnErrorMessage == Error::SUCCESS || pnErrorMessage == Error::Parser::EMPTY_AST_P) {
		//No error occured:
		return;
	}
	SetTextColor(12);
	//Check wether information about token does exist:
	if (errorToken.getFilename() != "") {
		cerr << errorToken.getFilename() << "(" << errorToken.getLine() << "): \"" << errorToken.getLexeme() << "\": ";
	}
	cerr << "error " << pnErrorMessage << ":" << endl;
	//Print the error message:
	switch (pnErrorMessage) {
	case Error::FileScanner::ERROR_FILESCANNER_FILE_DOES_NOT_EXIST:
		cerr << "The file does not exist." << endl;
		break;
	case Error::Tokenizer::TOO_MANY_DECIMALS:
		cerr << "The number has too many decimal points." << endl;
		break;
	case Error::Tokenizer::SOURCECODE_NOT_LONG_ENOUGH:
		cerr << "The sourcecode is not long enough." << endl;
		break;
	case Error::Tokenizer::EMPTY_CHARACTER_ENCOUNTERED:
		cerr << "Encountered char-type value with no content." << endl;
		break;
	case Error::Tokenizer::NO_EXIT_QUOTATION_MARK:
		cerr << "Missing quotation marks." << endl;
		break;
	case Error::Parser::SYNTAX_P:
		cerr << "Encountered syntax error while parsing." << endl;
		break;
	case Error::Parser::ATOM_CAN_NEVER_BE_PARENTHESES:
		cerr << "Encountered parentheses as atom." << endl;
		break;
	case Error::Interpreter::SYNTAX_I:
		cerr << "Encountered syntax error while interpreting." << endl;
		break;
	case Error::Interpreter::OPERANDS_ARE_OF_DIFFERENT_TYPE:
		cerr << "Encountered operands of different type, that are not compatible with each other." << endl;
		break;
	case Error::Interpreter::CANNOT_ADD_BOOLEANS:
		cerr << "Cannot add multiple boolean values." << endl;
		break;
	case Error::Interpreter::CANNOT_SUBTRACT_NON_NUMERIC_VALUES:
		cerr << "Cannot subtract multiple non-numeric values." << endl;
		break;
	case Error::Interpreter::CANNOT_MULTIPLY_NON_NUMERIC_VALUES:
		cerr << "Cannot multiply multiple non-numeric values." << endl;
		break;
	case Error::Interpreter::CANNOT_DIVIDE_NON_NUMERIC_VALUES:
		cerr << "Cannot divide multiple non-numeric values." << endl;
		break;
	case Error::Interpreter::CANNOT_DIVIDE_BY_ZERO:
		cerr << "Encountered division through zero." << endl;
		break;
	case Error::Interpreter::VARIABLE_DOES_NOT_EXIST:
		cerr << "Variable does not exist." << endl;
		break;
	case Error::Interpreter::VARIABLE_NAME_ALREADY_IN_USE:
		cerr << "Variable with this name is already in use." << endl;
		break;
	case Error::Interpreter::EMPTY_AST_I:
		cerr << "Encountered empty abstract syntax tree." << endl;
		break;
	case Error::Interpreter::MISSING_TOKEN:
		cerr << "Missing token." << endl;
		break;
	case Error::Interpreter::INCORRECT_TOKEN:
		cerr << "Encountered incorrect token." << endl;
		break;
	case Error::Interpreter::TOO_MANY_ARGUMENTS:
		cerr << "Too many arguments." << endl;
		break;
	case Error::Interpreter::NOT_ENOUGH_ARGUMENTS:
		cerr << "Not enough arguments." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_INT_TO_DOUBLE:
		cerr << "Cannot convert integer to double." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_INT_TO_BOOL:
		cerr << "Cannot convert integer to boolean." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_INT_TO_CHAR:
		cerr << "Cannot convert integer to character." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_INT_TO_STRING:
		cerr << "Cannot convert integer to string." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_DOUBLE_TO_INT:
		cerr << "Cannot convert double to integer." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_DOUBLE_TO_BOOL:
		cerr << "Cannot convert double to boolean." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_DOUBLE_TO_CHAR:
		cerr << "Cannot convert double to character." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_DOUBLE_TO_STRING:
		cerr << "Cannot convert double to string." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_BOOL_TO_INT:
		cerr << "Cannot convert boolean to integer." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_BOOL_TO_DOUBLE:
		cerr << "Cannot convert boolean to double." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_BOOL_TO_CHAR:
		cerr << "Cannot convert boolean to character." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_BOOL_TO_STRING:
		cerr << "Cannot convert boolean to string." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_CHAR_TO_INT:
		cerr << "Cannot convert character to integer." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_CHAR_TO_DOUBLE:
		cerr << "Cannot convert character to double." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_CHAR_TO_BOOL:
		cerr << "Cannot convert character to boolean." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_CHAR_TO_STRING:
		cerr << "Cannot convert character to string." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_STRING_TO_INT:
		cerr << "Cannot convert string to integer." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_STRING_TO_DOUBLE:
		cerr << "Cannot convert string to double." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_STRING_TO_BOOL:
		cerr << "Cannot convert string to boolean." << endl;
		break;
	case Error::Interpreter::CANNOT_CONVERT_STRING_TO_CHAR:
		cerr << "Cannot convert string to character." << endl;
		break;
	case Error::Interpreter::MISSING_CONDITION:
		cerr << "Missing condition of statement." << endl;
		break;
	case Error::Interpreter::MISSING_BODY:
		cerr << "Missing body of statement." << endl;
		break;
	case Error::Interpreter::INCORRECT_FUNCTION_DEFINITION:
		cerr << "Incorrect function definition." << endl;
		break;
	case Error::Interpreter::FUNCTION_NAME_IS_INCORRECT:
		cerr << "Incorrect function name." << endl;
		break;
	case Error::Interpreter::MISSING_MAIN_FUNCTION:
		cerr << "LISP main-function is missing." << endl;
		break;
	case Error::Interpreter::MAIN_FUNCTION_HAS_PARAMETERS:
		cerr << "LISP main-function has too many parameters." << endl;
		break;
	case Error::Interpreter::MAIN_FUNCTION_HAS_INCORRECT_RETURN_TYPE:
		cerr << "LISP main-function has incorrect return type." << endl;
		break;
	case Error::Interpreter::MISSING_FUNCTION_EXPRESSION:
		cerr << "Function has no expressions." << endl;
		break;
	case Error::Interpreter::FUNCTION_DOES_NOT_EXIST:
		cerr << "The called function does not exist." << endl;
		break;
	case Error::Interpreter::INCORRECT_NUMBER_OF_ARGUMENTS_PASSED:
		cerr << "An incorrect number of arguments is passed." << endl;
		break;
	case Error::Interpreter::TOO_MANY_VALUES_TO_RETURN:
		cerr << "LISP function can only return up to exactly one argument." << endl;
		break;
	default:
		cerr << "Encountered unknown error." << endl;
		break;
	}
}





/**
* This function is the starting point of the Interpreter and resembles the command prompt of the program.
*/
int main() {
	SetTextColor(7);
	cout << "==========================================================================================" << endl;
	cout << "LispInterpreter 2021 Developer Command Promt v1.0" << endl;
	cout << "Made by ChosenChris" << endl;
	cout << "==========================================================================================\n" << endl;
	string sFilePath = ""; //Stores the filepath (Can be changed through cd).
	bool bDebugStatus = false; //Indicates, wether advanced debug-information should be shown or not.
	//While loop repeats forever:
	while (true) {
		SetTextColor(15);
		cout << "interpreter>";
		string sInput = ""; //Stores the user's input.
		SetTextColor(14);
		getline(cin, sInput); //Reads the user's input.
		string sInputCopy = sInput; //Stores a copy of the user's input.
		SetTextColor(15);
		string sCommand = ""; //Stores the command, which was typed by the user.
		CLinkedList<string> lsArguments; //Stores every argument (substring between spaces) of the input.
		if (sInputCopy.find(' ') != string::npos) {
			//Argument found:
			sCommand = sInputCopy.substr(0, sInputCopy.find(' '));
			sCommand = sCommand;
			sInputCopy.erase(0, sInputCopy.find(' ') + 1); //Remove the command from the input.
			//Filter arguments:
			while (sInputCopy.find(' ') != string::npos) {
				string sCurrent = sInputCopy.substr(0, sInputCopy.find(' ')); //Creates substr.
				sInputCopy.erase(0, sInputCopy.find(' ') + 1); //Remove the command from the input.
				lsArguments.add(sCurrent);
			}
			lsArguments.add(sInputCopy);
		}
		else {
			//No arguments available:
			sCommand = sInputCopy;
		}
		for_each(sCommand.begin(), sCommand.end(), [](char& c) {c = ::tolower(c); }); //Convert command to lowercase.

		//Evaluate command:
		if (sCommand == "help") {
			//Show every different command:
			if (lsArguments.empty()) {
				//Show commands:
				SetTextColor(7);
				cout << "For more information on a specific command, type \"HELP command-name.\"" << endl;
				cout << "HELP          Shows help information for commands." << endl;
				cout << "EXIT          Exits the interpreter." << endl;
				cout << "EXECUTE       Executes sourcecode from a file." << endl;
				cout << "CLS           Clears the content of the terminal." << endl;
				cout << "CD            Shows the name of the current directory, or changes it." << endl;
				cout << "\n";
			}
			else if (lsArguments.size() == 1) {
				string sArgument = lsArguments[0]; //Stores the argument.
				for_each(sArgument.begin(), sArgument.end(), [](char& c) {c = ::tolower(c); }); //Convert argument to lowercase.
				if (sArgument == "help") {
					//Display information about help-command:
					SetTextColor(7);
					cout << "Shows help information for commands." << endl;
					cout << "HELP [command]" << endl;
					cout << "\tcommand - Shows help information for the specified command.\n" << endl;
				}
				else if (sArgument == "exit") {
					//Display information about help-command:
					SetTextColor(7);
					cout << "Exits the interpreter.\n" << endl;
				}
				else if (sArgument == "execute") {
					//Display information about execute-command:
					SetTextColor(7);
					cout << "Executes sourcecode from a file." << endl;
					cout << "EXECUTE [path] [debugMode]" << endl;
					cout << "\tpath      - File (and path), in which the sourcecode is located." << endl;
					cout << "\tdebugMode - Optional argument indicates wether debug-information should be shown" << endl;
					cout << "\t            while executing the sourcecode.\n" << endl;
				}
				else if (sArgument == "cls") {
					//Display information about cls-command:
					SetTextColor(7);
					cout << "Clears the content of the terminal.\n" << endl;
				}
				else if (sArgument == "cd") {
					//Display information about cd-command:
					SetTextColor(7);
					cout << "Shows the name of the current directory, or changes it." << endl;
					cout << "CD [directory]" << endl;
					cout << "\tdirectory - Optional argument resembles the new directory. If no argument" << endl;
					cout << "\t            is given, the current directory is shown.\n" << endl;
				}
				else {
					//Incorrect argument:
					SetTextColor(12);
					cerr << "The argument \"" << lsArguments[0] << "\" is incorrect.\n" << endl;
				}
			}
			else {
				SetTextColor(12);
				cerr << "Too many arguments. Type \"HELP help\" for further information.\n" << endl;
			}
		}

		else if (sCommand == "exit" || sCommand == "close") {
			//Exit the application:
			break;
		}

		else if (sCommand == "execute") {
			//Execute sourcecode:
			string sFile = "";
			bool bDebugMode = false;
			if (lsArguments.size() == 2) {
				//File and debug mode are passed:
				if (sFilePath != "") {
					sFile = sFilePath + "/" + lsArguments[0];
				}
				else {
					sFile = lsArguments[0];
				}
				string sArgument = lsArguments[1]; //Stores the argument.
				for_each(sArgument.begin(), sArgument.end(), [](char& c) {c = ::tolower(c); }); //Convert argument to lowercase.
				if (sArgument == "1" || sArgument == "true") {
					//Debug-mode is true:
					bDebugMode = true;
				}
				else if (sArgument == "0" || sArgument == "false") {
					//Debug-mode is false:
					bDebugMode = false;
				}
				else {
					//Incorrect argument:
					SetTextColor(12);
					cerr << "The argument \"" << lsArguments[1] << "\" is incorrect. Type \"HELP execute\" for further information.\n" << endl;
					continue;
				}
			}
			else if (lsArguments.size() == 1) {
				//Only the file is passed:
				if (sFilePath != "") {
					sFile = sFilePath + "/" + lsArguments[0];
				}
				else {
					sFile = lsArguments[0];
				}
				bDebugMode = bDebugStatus;
			}
			else if (lsArguments.size() > 2) {
				//Error: too many arguments:
				SetTextColor(12);
				cerr << "Too many arguments. Type \"HELP execute\" for further information.\n" << endl;
				continue;
			}
			//Execute code:
			execute(sFile, bDebugMode);
			cout << "\n" << endl;
		}

		else if (sCommand == "cls") {
			//Clear the console:
			system("CLS");
		}

		else if (sCommand == "cd") {
			//cd-command:
			if (lsArguments.size() > 1) {
				//Error: Too many arguments:
				SetTextColor(12);
				cerr << "Too many arguments. Type \"HELP cd\" for further information.\n" << endl;
				continue;
			}
			if (lsArguments.empty()) {
				//No argument -> Show filepath:
				SetTextColor(7);
				if (sFilePath == "") {
					//No specified file path:
					cout << "Current filepath: No specified path.\n" << endl;
				}
				else {
					//Show specified file path:
					cout << "Current filepath: " << sFilePath << "\n" << endl;
				}
			}
			else {
				//Change filepath:
				sFilePath = lsArguments[0];
				SetTextColor(7);
				cout << "The file path has been changed to "<< sFilePath << ".\n" << endl;
			}
		}

		else {
			//Incorrect command:
			SetTextColor(12);
			cerr << "The command \"" << sInput << "\" is incorrect. Type \"HELP\" for further information.\n" << endl;
			continue;
		}
	}
}
