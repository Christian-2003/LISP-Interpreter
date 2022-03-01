/*
FILE:		CLinkedList.hpp

REMARKS:	This file contains the implementation of a LinkedList (subclass of the "list"-class of the C++ STL).
*/
#pragma once

#include <list>
#include <stdexcept>

using namespace std;



/**
* Objects of this class represent a LinkedList in C++.
* This class resembles a subclass of the C++ STL's "list"-class.
* 
* @param <T>	Type of the objects that are stores in the list.
* 
* @author		Christian-2003
* @version		27.05.2021
*/
template<class T>
class CLinkedList : public list<T> {
public:
	/**
	* This overloaded operator allows to access any element in the LinkedList through the specified index. If the index is out of range or
	* the passed index cannot be applied to the list, an exception is thrown.
	* 
	* @param pnIndex	Index of the element in the list.
	* @return			Element at the specified index.
	*/
	T operator[](unsigned int pnIndex) {
		if (pnIndex > this->size() - 1) {
			//The passed index is out of range:
			throw invalid_argument("index out of range");
		}
		else {
			try {
				typename list<T>::iterator listIterator = this->begin();
				advance(listIterator, pnIndex);
				return *listIterator;
			}
			catch (invalid_argument e) {
				throw e;
			}
		}
	}



public:
	/**
	* This function adds another element to the list. It works similar to the "push_back()"-function.
	* 
	* @param pContent	Element, which should be added to the end of the list.
	*/
	void add(T pContent) {
		this->push_back(pContent);
	}

	/**
	* This function adds another element at the specified position to the list. If the passed position is out of range, the element is
	* added to the end of the list.
	* 
	* @param pContent	Element, which should be added to the list.
	* @param pnIndex	Position at which the element should be inserted.
	*/
	void add(T pContent, unsigned int pnIndex) {
		if (pnIndex > this->size() - 1) {
			//Index is out of range:
			this->push_back(pContent);
		}
		else {
			typename list<T>::iterator listIterator = this->begin();
			advance(listIterator, pnIndex);
			this->insert(listIterator, pContent);
		}
	}



public:
	/**
	* This function adds every element of the passed list to the end of the current list.
	* 
	* @param pContents	Elements, which shoudld be added to the current list.
	*/
	void addAll(CLinkedList<T> pContents) {
		for (T current : pContents) {
			this->push_back(current);
		}
	}



public:
	/**
	* This function removes the element at the specified index in the list. If the passed index is out of range, no element is removed.
	*
	* @param pnIndex	Index of the element, that should be removed.
	*/
	void remove(unsigned int pnIndex) {
		if (pnIndex > this->size() - 1) {
			//Index is out of range:
			return;
		}
		typename list<T>::iterator listIterator = this->begin();
		advance(listIterator, pnIndex);
		this->erase(listIterator); //Removes the element.
	}

	/**
	* This function removes the element at the specified index in the list. If the passed index is out of range, no element is removed.
	*
	* @param pnIndex	Index of the element, that should be removed.
	*/
	T retrieve(unsigned int pnIndex) {
		typename list<T>::iterator listIterator = this->begin();
		advance(listIterator, pnIndex);
		T elementRemoved = *listIterator;
		this->erase(listIterator); //Removes the element.
		return elementRemoved;
	}
};
