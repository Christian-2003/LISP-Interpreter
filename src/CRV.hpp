/*
FILE:		CRV.hpp

REMARKS:	This file contains the class "CRV", which is used for a function to return a value with extra information.
*/
#pragma once

#include <iostream>

using namespace std;



/**
* The class "CRV" is used to return values of from a function and includes extra information.
* RV means "Return value".
* 
* @param <T>	Type of the objects that can be returned with this class.
* 
* @author		Christian-2003
* @version		30.05.2021
*/
template<class T>
class CRV {
private:
	/**
	* Represents the content of the RV.
	*/
	T content;

	/**
	* Represents the error message, which should be returned with this return value.
	*/
	short int nErrorMessage;



public:
	/**
	* Default constructor instantiates a new RV with the error message 0 ("SUCCESS").
	*/
	CRV() {
		nErrorMessage = 0;
	}

	/**
	* Constructor instantiates a new RV with the passed values as content and error message.
	* 
	* @param pContent		Content of the RV.
	* @param pnErrorMessage	Error message of the RV.
	*/
	CRV(T pContent, short int pnErrorMessage) {
		content = pContent;
		nErrorMessage = pnErrorMessage;
	}



public:
	/**
	* Returns the content of this RV.
	* 
	* @return	Content.
	*/
	T getContent() {
		return content;
	}

	/**
	* Returns the error message of this RV.
	* 
	* @return	Error message.
	*/
	short int getErrorMessage() {
		return nErrorMessage;
	}
};