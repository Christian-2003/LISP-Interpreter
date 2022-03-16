/*
FILE:		CLog.hpp

REMARKS:	This file contains the class "CLog" which can be used to create a log for the interpreter.
*/
#pragma once
#pragma warning(disable: 4996) //Disable deprecation warning (Occurs when using 'ctime').

#include <ctime>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "CToken.hpp"
#include "Variables/TokenTypes.hpp"

using namespace std;



/**
* This enum contains the types for the different log entries.
*/
enum LogEntryType : const short int {
	ERR = 1,	//Type for error entries.
	INF = 2,	//Type for information entries.
	WARN = 3,	//Type for warning entries.
	FATAL = 4	//Type for fatal error.
};


/**
* This enum contains the 
*/
enum LogEntryComponent : const short int {
	UNSPECIFIED = 0,	//For unknown components.
	TERMINAL = 1,		//For the terminal.
	FILE_SYSTEM = 2,	//For the file system.
	TOKENIZER = 3,		//For the tokenizer.
	PARSER = 4,			//For the parser.
	INTERPRETER = 5		//For the interpreter.
};



/**
* Any instance of this class can be used to write a log for the interpreter.
*/
class CLog {
private:
	/**
	* This String contains the filename of the log:
	* DATEDAY.DATEMONTH.DATEYEAR_HOUR.MINUTE.SECOND.log
	*/
	string filename;

	/**
	* Reference to the logFile. This file will be opened when the constructor of this class
	* is called and closed when calling the method "close".
	*/
	fstream logFile;

	/**
	* Stores the raw system time.
	*/
	time_t rawSystemTime;

	/**
	* Structure stores the important values of the system time.
	*/
	struct tm* timeTable;

	/**
	* Whether the logger should actually log or not.
	*/
	bool log;



private:
	/**
	* This method converts the passed time to a String that contains it's date. The date will
	* be converted to the following format: 'DD.MM.YYYY'. This is the typical format used in German,
	* and since I'm from Germany, I don't care if you do not like this. Your problem lmao.
	*/
	string convertDateToString() {
		int day = timeTable->tm_mday;
		int month = timeTable->tm_mon + 1;
		int year = timeTable->tm_year + 1900;
		string dateString = "";
		//Fill the remaining digits with zero if the day is less than 9:
		if (day <= 9) {
			dateString += "0" + to_string(day);
		}
		else {
			dateString += to_string(day);
		}
		dateString += ".";
		//Fill the remaining digits with zero if the month is less than 9:
		if (month <= 9) {
			dateString += "0" + to_string(month);
		}
		else {
			dateString += to_string(month);
		}
		dateString += ".";
		//Add the year to the String:
		dateString += to_string(year);

		//Return the date as a String:
		return dateString;
	}

	/**
	* This method converts the passed time to a String that contains it's time. The time will
	* be converted to the following format: 'HH:MM:SS'. This is the typical format used in German,
	* and since I'm from Germany, I don't care if you do not like this. Your problem lmao.
	*/
	string convertTimeToString() {
		int hour = timeTable->tm_hour;
		int minute = timeTable->tm_min;
		int second = timeTable->tm_sec;
		string timeString = "";
		//Fill the remaining digits with zero if the hour is less than 9:
		if (hour <= 9) {
			timeString += "0" + to_string(hour);
		}
		else {
			timeString += to_string(hour);
		}
		timeString += ":";
		//Fill the remaining digits with zero if the minute is less than 9:
		if (minute <= 9) {
			timeString += "0" + to_string(minute);
		}
		else {
			timeString += to_string(minute);
		}
		timeString += ":";
		//Fill the remaining digits with zero if the second is less than 9:
		if (second <= 9) {
			timeString += "0" + to_string(second);
		}
		else {
			timeString += to_string(second);
		}

		//Return the time as a String:
		return timeString;
	}



	/**
	* This method refreshes the time.
	*/
	void refreshTime() {
		rawSystemTime = time(0);
		timeTable = localtime(&rawSystemTime);
	}



private:
	/**
	* This method generates the type from the passed short integer.
	* 
	* @param _type	Type for the log entry.
	* @return		Type for the log entry as String.
	*/
	string generateType(short int _type) {
		if (_type == LogEntryType::ERR) {
			//Erorr type:
			return "ERROR";
		}
		else if (_type == LogEntryType::INF) {
			//Info type:
			return "INFO";
		}
		else if (_type == LogEntryType::WARN) {
			//Warning type:
			return "WARNING";
		}
		else if (_type == LogEntryType::FATAL) {
			//Fatal error type:
			return "FATAL ERROR";
		}
		//Unknown type:
		return "UNKNOWN";
	}

	/**
	* This method generates the component from the passed short integer.
	*
	* @param _component	Component for the log entry.
	* @return			Component for the log entry as String.
	*/
	string generateComponent(short int _component) {
		if (_component == LogEntryComponent::TERMINAL) {
			return "Terminal";
		}
		else if (_component == LogEntryComponent::FILE_SYSTEM) {
			return "File system";
		}
		else if (_component == LogEntryComponent::TOKENIZER) {
			return "Tokenizer";
		}
		else if (_component == LogEntryComponent::PARSER) {
			return "Parser";
		}
		else if (_component == LogEntryComponent::INTERPRETER) {
			return "Interpreter";
		}
		return "Unknown";
	}



public:
	/**
	* The default constructor instantiates a new log. The name of the log will be the system's time of when
	* this constructor is called. Please make sure to "close()" the log afterwards.
	*/
	CLog() {
		//Generate name for the log-file:
		refreshTime();
		filename = "";
		log = false;
	}



public:
	/**
	* Returns the filename of the current log.
	* 
	* @return	Name of the file of the log.
	*/
	string getFilename() {
		return filename;
	}

	/**
	* Returns whether the logger is currently loggin or not.
	* 
	* @return	Whether the logger is logging or not.
	*/
	bool isLogging() {
		return log;
	}



public:
	/**
	* This method adds a custom entry to the log.
	* 
	* @param _event	Event to be added to the log. The String has to be formatted correctly.
	* @param _type	Type for the entry.
	*/
	void addEntry(string _entry, short int _component, short int _type) {
		if (!log) {
			//Logger shall not log:
			return;
		}
		refreshTime();
		string entry = "[" + convertDateToString() + " " + convertTimeToString() + "] "; //Stores the date and time.
		
		//Generate component and type:
		entry += "[" + generateComponent(_component) + "/" + generateType(_type) + "] ";

		//Add the custom entry:
		entry += _entry;

		//Add entry to log:
		logFile << entry << endl;
	}



	/**
	* This method adds an entry regarding a command input within the terminal:
	* 
	* @param _command	Command which was entered in the terminal.
	* @param _arguments	Arguments that were passed with the aforementioned command.
	*/
	void addEntry_terminalCommand(string _command, CLinkedList<string> _arguments) {
		if (!log) {
			//Logger shall not log:
			return;
		}
		refreshTime();
		string entry = "[" + convertDateToString() + " " + convertTimeToString() + "] "; //Stores the date and time.
		entry += "[" + generateComponent(LogEntryComponent::TERMINAL) + "/" + generateType(LogEntryType::INF) + "] ";
		entry += "Executing command \"" + _command + "\" ";
		if (_arguments.empty()) {
			//No arguments passed:
			entry += "without arguments.";
		}
		else {
			//Arguments were passed:
			entry += "with arguments: ";
			for (unsigned int i = 0; i < _arguments.size(); i++) {
				entry += "\"" + _arguments[i] + "\"";
				if (i < _arguments.size() - 1) {
					entry += ", ";
				}
			}
			entry += ".";
		}

		//Add entry to log:
		logFile << entry << endl;
	}

	/**
	* This method adds an entry regarding an incorrect argument that was passed with a
	* keyword in the terminal.
	* 
	* @param _command		Command, that was passed with the incorrect argument.
	* @param _arguments		Arguments that were passed.
	* @param _errorArgument	Incorrect argument that caused the exception.
	* @param _errorMessage	Error message to output in the log.
	*/
	void addEntry_terminalCommandArgException(string _command, CLinkedList<string> _arguments, string _errorArgument, string _errorMessage) {
		if (!log) {
			//Logger shall not log:
			return;
		}
		if (_arguments.empty()) {
			//No arguments were passed: Function should not have been called:
			return;
		}
		refreshTime();
		string entry = "[" + convertDateToString() + " " + convertTimeToString() + "] "; //Stores the date and time.
		entry += "[" + generateComponent(LogEntryComponent::TERMINAL) + "/" + generateType(LogEntryType::ERR) + "] ";
		entry += "Executing command \"" + _command + "\" ";
		entry += "with arguments: ";
		for (unsigned int i = 0; i < _arguments.size(); i++) {
			entry += "\"" + _arguments[i] + "\"";
			if (i < _arguments.size() - 1) {
				entry += ", ";
			}
		}
		//Add error argument and error message:
		entry += ": \"" + _errorArgument + "\": " + _errorMessage + ".";

		//Add entry to log:
		logFile << entry << endl;
	}

	/**
	* This method adds an entry regarding an incorrect command that was entered in
	* the terminal.
	* 
	* @param _command		Incorrect command.
	* @param _arguments		Arguments that were passed with the command.
	* @param _errorMessage	Error message to output in the log.
	*/
	void addEntry_terminalCommandException(string _command, CLinkedList<string> _arguments, string _errorMessage) {
		if (!log) {
			//Logger shall not log:
			return;
		}
		refreshTime();
		string entry = "[" + convertDateToString() + " " + convertTimeToString() + "] "; //Stores the date and time.
		entry += "[" + generateComponent(LogEntryComponent::TERMINAL) + "/" + generateType(LogEntryType::ERR) + "] ";
		entry += "Passed command \"" + _command + "\"";
		if (!_arguments.empty()) {
			entry += " with arguments: ";
			for (unsigned int i = 0; i < _arguments.size(); i++) {
				entry += "\"" + _arguments[i] + "\"";
				if (i < _arguments.size() - 1) {
					entry += ", ";
				}
			}
		}
		//Add error message:
		if (_errorMessage.length() != 0) {
			entry += ": " + _errorMessage + ".";
		}

		//Add entry to log:
		logFile << entry << endl;
	}



	/**
	* This method adds an entry regarding a extraxted token of the tokenizer.
	*
	* @param _token	Token which was extracted.
	*/
	void addEntry_tokenizerAddToken(CToken _token) {
		if (!log) {
			//Logger shall not log:
			return;
		}
		refreshTime();
		string entry = "[" + convertDateToString() + " " + convertTimeToString() + "] "; //Stores the date and time.

		//Generate component and type:
		entry += "[" + generateComponent(LogEntryComponent::TOKENIZER) + "/" + generateType(LogEntryType::INF) + "] ";

		//Add token to log:
		entry += "[" + _token.getFilename() + "(" + to_string(_token.getLine()) + ")]: Extract token \"" + _token.getLexeme() + "\" of type " + printTokenType(_token.getType()) + ".";

		//Add entry to log:
		logFile << entry << endl;
	}

	/**
	* This method adds an entry regarding a extraxted token of the tokenizer.
	*
	* @param _token			Token which was extracted.
	* @param _component		Component, in which the exception occured.
	* @param _type			Type of the occuring exception.
	* @param _errorCode		Error code that was generated.
	* @param _errorMessage	Error message that belongs to the error code.
	*/
	void addEntry_tokenException(CToken _token, short int _component, short int _type, short int _errorCode, string _errorMessage) {
		if (!log) {
			//Logger shall not log:
			return;
		}
		refreshTime();
		string entry = "[" + convertDateToString() + " " + convertTimeToString() + "] "; //Stores the date and time.

		//Generate component and type:
		entry += "[" + generateComponent(_component) + "/" + generateType(_type) + "] ";

		//Add token to log:
		entry += "[" + _token.getFilename() + "(" + to_string(_token.getLine()) + ")]: Token \"" + _token.getLexeme() + "\" of type " + printTokenType(_token.getType());

		//Add error type and message:
		entry += " caused error " + to_string(_errorCode);
		if (_errorMessage.empty()) {
			//No error message passed:
			entry += ".";
		}
		else {
			entry += ": " + _errorMessage;
		}

		//Add entry to log:
		logFile << entry << endl;
	}



public:
	/**
	* This method closes the log. After this method is called, the log will not be continued.
	*/
	void close() {
		logFile.close(); //Close the file.
		log = false;
	}

	/**
	* Begins logging to a log. The name of the log will be the system's time of when
	* this method is called. Please make sure to "close()" the log afterwards.
	*/
	void open() {
		//Generate name for the log-file:
		refreshTime();
		string timeForFilename = convertTimeToString();
		replace(timeForFilename.begin(), timeForFilename.end(), ':', '.'); //Replace ':' with '.', since ':' is not allowed with filenames on MS Windows.
		filename = convertDateToString() + "_" + timeForFilename + ".log";

		logFile.open(filename, ios::out | ios::app); //Open the file.
		log = true;
	}

};



/**
* Global object can log the events of the interpreter.
*/
CLog logger;
