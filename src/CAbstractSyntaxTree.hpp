/*
FILE:		CAbstractSyntaxTree.hpp

REMARKS:	This file contains the class "CAbstractSyntaxTree", which is used to implement an abstract syntax tree.
*/
#pragma once

#include <iostream>

#include "CLinkedList.hpp"

using namespace std;



/**
* Objects of this class represent a node of an abstract syntax tree in C++.
* 
* @author	ChosenChris
* @version	28.05.2021
*/
template<class T>
class CAbstractSyntaxTree {
private:
	/**
	* This CLinkedList represents a list of every subtree of the current node.
	*/
	CLinkedList<CAbstractSyntaxTree<T>> lSubTreesObj;
	
	/**
	* Represents the content of the current node.
	*/
	T content;



public:
	/**
	* This defaul constructor does literally nothing and is just in place in order not to get an error message when used as template-type
	* of an CRV.
	* I highly recommend NOT to use this constructor.
	*/
	CAbstractSyntaxTree() {
		//Does nothing...
	}

	/**
	* This constructor creates a new abstract syntax tree with the passed object as it's content. The created
	* abstract syntax tree does not have any subtrees.
	* 
	* @param pContent	Content of the tree's current node.
	*/
	CAbstractSyntaxTree(T pContent) {
		content = pContent;
	}

	/**
	* This constructor creates a new abstract syntax tree with the passed object as it's content.
	* 
	* @param pContent		Content of the tree's current node.
	* @param plSubTreesObj	List with the current node's subtrees.
	*/
	CAbstractSyntaxTree(T pContent, CLinkedList<CAbstractSyntaxTree<T>> plSubTreesObj) {
		content = pContent;
		lSubTreesObj.addAll(plSubTreesObj);
	}



public:
	/**
	* This function returns the content of the current node.
	* 
	* @return	Content of this node.
	*/
	T getContent() {
		return content;
	}

	/**
	* This function returns a CLinkedList with every subtree of the current node.
	* 
	* @return	List of every subtree of the current node.
	*/
	CLinkedList<CAbstractSyntaxTree<T>> getSubTrees() {
		return lSubTreesObj;
	}

	/**
	* This function returns the current node's subtree at the specified index. If the index is out of bounds, a nullptr is returned.
	* 
	* @param pnIndex	Index of the subtree in the CLinkedList
	* @return			Subtree at the specified index.
	*/
	CAbstractSyntaxTree<T> getSubTreeAtIndex(unsigned int pnIndex) {
		if (pnIndex < lSubTreesObj.size() - 1) {
			return lSubTreesObj[pnIndex];
		}
		return nullptr;
	}



public:
	/**
	* This functions returns, wether the current node does have any subtrees (true) or not (false).
	* 
	* @return	Wether the current node has any subtrees.
	*/
	bool hasSubTrees() {
		if (lSubTreesObj.empty()) {
			return false;
		}
		else {
			return true;
		}
	}

	/**
	* This function returns the number of the current node's subtrees.
	* 
	* @return	Number of subtrees.
	*/
	unsigned int getSubTreeNumber() {
		if (!hasSubTrees()) {
			return 0;
		}
		else {
			lSubTreesObj.size();
		}
		return lSubTreesObj.size();
	}

};
