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
#include "CLog.hpp"

using namespace std;
void execute(string, bool);
void printAST(CAbstractSyntaxTree<CToken>, int);
void printError(CToken, short int);



/**
* Changes the text color of the text in the terminal.
*
* @param pnColor	Color, to which the text should be changed.
*/
void SetTextColor(int pnColor) {
	HANDLE consoleHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHANDLE, pnColor);
}



/**
* This function executes sourcecode from a specific file.
*
* @param psFilename		File, in which the sourcecode is located.
*/
void execute(string psFilename, bool pbDebugMode) {
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
		printError(rvTokens.getContent()[rvTokens.getContent().size() - 1], rvTokens.getErrorMessage());
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
void printAST(CAbstractSyntaxTree<CToken> pAST, int nDepth) {
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
void printError(CToken errorToken, short int pnErrorMessage) {
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
	cerr << Error::generateErrorMessage(pnErrorMessage) << endl;
}





/**
* This function is the starting point of the Interpreter and resembles the command prompt of the program.
*/
int main() {
	SetTextColor(7);
	cout << "==========================================================================================" << endl;
	cout << "LispInterpreter 2021 Developer Command Promt v1.9" << endl;
	cout << "Made by Christian-2003" << endl;
	cout << "==========================================================================================\n" << endl;
	string sFilePath = ""; //Stores the filepath (Can be changed through cd).
	bool bDebugStatus = false; //Indicates, wether advanced debug-information should be shown or not.
	bool logEvents = false; //Indicates, whether everything should be logged or not.
	logger.close(); //Close logger, since logging is deactivated normally.
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

		//Log entered command:
		logger.addEntry_terminalCommand(sCommand, lsArguments);

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
				cout << "LOG           Enables or disables the log for the interpreter." << endl;
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
				else if (sArgument == "log") {
					//Display information about log-command:
					SetTextColor(7);
					cout << "Shows whether the interpreter is currently logging it's events into a log. If the optional" << endl;
					cout << "argument is passed, the logging is enabled or disabled." << endl;
					cout << "LOG [log]" << endl;
					cout << "\t log - Optional argument indicates whether the interpreter should log it's" << endl;
					cout << "\t       events or not. If no argument is given, the command shows if logging" << endl;
					cout << "\t       is enabled or disabled.\n" << endl;
				}
				else {
					//Incorrect argument:
					SetTextColor(12);
					cerr << "The argument \"" << lsArguments[0] << "\" is incorrect.\n" << endl;
					logger.addEntry_terminalCommandArgException(sCommand, lsArguments, lsArguments[0], "Incorrect argument");
				}
			}
			else {
				SetTextColor(12);
				cerr << "Too many arguments. Type \"HELP help\" for further information.\n" << endl;
				logger.addEntry_terminalCommandException(sCommand, lsArguments, "Too many arguments");
			}
		}

		else if (sCommand == "exit" || sCommand == "close") {
			//Exit the application:
			if (logEvents) {
				//Logging beforehand:
				logger.addEntry("Exit terminal.", LogEntryComponent::TERMINAL, LogEntryType::INF);
				logger.addEntry("End session log.", LogEntryComponent::TERMINAL, LogEntryType::INF);
				logger.close();
			}
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
					logger.addEntry_terminalCommandArgException(sCommand, lsArguments, lsArguments[1], "Incorrect argument");
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
				logger.addEntry_terminalCommandException(sCommand, lsArguments, "Too many arguments");
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
				logger.addEntry_terminalCommandException(sCommand, lsArguments, "Too many arguments");
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

		else if (sCommand == "log") {
			//Enable or disable the log:
			if (lsArguments.empty()) {
				//Show whether the logging is activated or deactivated:
				if (logEvents) {
					//Logging is activated:
					SetTextColor(7);
					cout << "The interpreter is logging to \"" << logger.getFilename() << "\".\n" << endl;
				}
				else {
					//Logging is deactivated:
					cout << "The interpreter is not logging.\n" << endl;
				}
			}
			else if (lsArguments.size() == 1) {
				//(De-)Activate logging:
				string sArgument = lsArguments[0]; //Stores the argument.
				for_each(sArgument.begin(), sArgument.end(), [](char& c) {c = ::tolower(c); }); //Convert argument to lowercase.
				if (sArgument == "1" || sArgument == "true" || sArgument == "t") {
					//Enable logging:
					if (logEvents) {
						//Interpreter was logging beforehand -> Just continue logging:
						SetTextColor(7);
						cout << "The interpreter is already logging to \"" << logger.getFilename() << "\".\n" << endl;
					}
					else {
						//Enable logging:
						logEvents = true;
						logger.open(); //Begin logging.
						SetTextColor(7);
						cout << "The interpreter is now logging to \"" << logger.getFilename() << "\".\n" << endl;
						logger.addEntry("Begin session log...", LogEntryComponent::TERMINAL, LogEntryType::INF);
					}
				}
				else if (sArgument == "0" || sArgument == "false" || sArgument == "f") {
					//Disable logging:
					if (logEvents) {
						//Interpreter was logging beforehand:
						SetTextColor(7);
						cout << "The interpreter has stopped logging. Find the previous log at \"" << logger.getFilename() << "\".\n" << endl;
						logger.addEntry("End session log.", LogEntryComponent::TERMINAL, LogEntryType::INF);
					}
					else {
						//Interpreter was not logging beforehand.
						cout << "The interpreter is not logging.\n" << endl;
					}
					logEvents = false;
					logger.close();
				}
				else {
					//Incorrect argument passed:
					SetTextColor(12);
					cerr << "The argument \"" << lsArguments[0] << "\" is incorrect. Type \"HELP log\" for further information.\n" << endl;
					logger.addEntry_terminalCommandArgException(sCommand, lsArguments, lsArguments[0], "Incorrect argument");
					continue;
				}
			}
			else {
				//Incorrect number of arguments passed:
				SetTextColor(12);
				cerr << "Too many arguments. Type \"HELP log\" for further information.\n" << endl;
				logger.addEntry_terminalCommandException(sCommand, lsArguments, "Too many arguments");
			}
		}

		else {
			//Incorrect command:
			SetTextColor(12);
			cerr << "The command \"" << sInput << "\" is incorrect. Type \"HELP\" for further information.\n" << endl;
			logger.addEntry_terminalCommandException(sCommand, lsArguments, "Incorrect command");
			continue;
		}
	}
}
