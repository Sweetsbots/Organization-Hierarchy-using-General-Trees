/*
This project was written by Eric Sweet to fulfill the requirements for CS3100 Project 2

The space requirement for the tree is (3p+2s)n where p is the supervisor, leftChild and rightSibling pointers
and 2s is the title and name strings and n is the number of nodes
*/

#pragma once

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

#define TREENODEPTR TreeNode*
#define TREENULLPTR NULL
class TreeNode {
private:

public:
	//designs the TreeNode and sets pointers to nullptr
	string title;
	string name;
	TreeNode *supervisor;
	TreeNode *leftChild;
	TreeNode *rightSibling;
	TreeNode(string newTitle, string newName) {
		title = newTitle;
		name = newName;
		supervisor = nullptr;
		leftChild = nullptr;
		rightSibling = nullptr;
	}
};
class OrgTree
{
private:
	//create nodes of root and head and a size variable
	TreeNode * root;
	TreeNode * head;
	unsigned int size = 0;

	/*
		Recursive printing method to print the subTree to the screen with proper spacing

		Theta of n
	*/
	void printSubTreeRecursive(TREENODEPTR subTreeRoot, int count) {
		if (!subTreeRoot) {
			return;
		}
		else {
			for (int iterator = 0; iterator < count; iterator++) {
				cout << "     ";
			}
			cout << subTreeRoot->title << ", " << subTreeRoot->name << endl;
			count++;
			printSubTreeRecursive(subTreeRoot->leftChild, count);
			count--;
			printSubTreeRecursive(subTreeRoot->rightSibling, count);
		}

	}

	/*
	Recursive printing method to print the subTree to a file with proper spacing

	Theta of n
	*/
	void printToFileSubTreeRecursive(TREENODEPTR subTreeRoot, ofstream &outputFile) {
		if (!subTreeRoot) {
			return;
		}
		else {
			outputFile << subTreeRoot->title << ", " << subTreeRoot->name << endl;
			printToFileSubTreeRecursive(subTreeRoot->leftChild, outputFile);
			outputFile << ")" << endl;
			printToFileSubTreeRecursive(subTreeRoot->rightSibling, outputFile);
		}

	}
public:
	OrgTree() {
	}
	/*
	Adds a new root to the tree, if a root
	already exists, it makes a new root and then sets
	the previous root as its child

	Theta of n
	*/
	void addRoot(string title, string name) {
		TreeNode *root = new TreeNode(title, name);
		TreeNode *temp;
		if (size > 0) {
			temp = this->root;
			this->root = root;
			this->root->leftChild = temp;
			temp->supervisor = this->root;
		}
		else {
			this->root = root;
		}
		size++;

	}
	/*
	Get's the size of the tree

	Runtime of Theta 1
	*/
	unsigned int getSize() {
		return size;
	}
	/*
	Gets the root of the tree

	runtime of Theta 2
	*/
	TREENODEPTR getRoot() {
		if (!root) return TREENULLPTR;
		return root;
	}
	/*
	gets the leftmostChild of a specific node

	Runtime of Theta 2
	*/
	TREENODEPTR leftmostChild(TREENODEPTR node) {
		if (!node->leftChild) {
			return TREENULLPTR;
		}
		return node->leftChild;
	}
	/*
	Returns the rightSibling of a specific node

	Runtime of Theta 2
	*/
	TREENODEPTR rightSibling(TREENODEPTR node) {
		if (!node->rightSibling) {
			return TREENULLPTR;
		}
		return node->rightSibling;
	}
	/*
		Prints the first node and then prints the rest of the tree if it has a sibling

		Runtime Theta of n
	*/
	void printSubTree(TREENODEPTR subTreeRoot) {
		cout << subTreeRoot->title << ", " << subTreeRoot->name << endl;
		int counter = 1;
		if (subTreeRoot->leftChild) {
			printSubTreeRecursive(subTreeRoot->leftChild, counter);
		}
	}
	/*
	This function checks to make sure the root title is not the one to be found,
	if it is then it returns the root, else it calls on the recursive findNode function

	It is in Theta of n
	*/
	TREENODEPTR find(string title) {
		TreeNode *foundNode = nullptr;
		//if roots title is the title, return root
		if (root->title == title) {
			return root;
		}
		foundNode = findNode(root, title);
		return foundNode;
	}
	/*
	This function recursively finds a node using the title
	of the node it is trying to find

	It is in Theta of n
	*/
	TREENODEPTR findNode(TREENODEPTR node, string title) {
		if (node != nullptr) {
			//if the nodes title is title return the node
			if (node->title == title) {
				return node;
			}
			else {
				//call findNode recursive
				TREENODEPTR returner = findNode(node->leftChild, title);
				//if the returner is not null, return it
				if (returner) {
					return returner;
				}
				//return the found node if not found by the left child
				return findNode(node->rightSibling, title);
			}
		}
		//if node is nullptr return nullptr
		else {
			return TREENULLPTR;
		}
	}
	/*
	This program reads in user input from a file and then adds
	the people to the general tree until there are no more people to add

	This function is in Theta of n
	*/
	bool read(string filename) {
		ifstream inputFile;
		string str;
		string paranthese = ")";
		int counter = 0;
		string blank = "";
		char comma = ',';
		char space = ' ';
		TreeNode *head = nullptr;
		inputFile.open(filename);
		if (!inputFile) {
			return 0;
		}
		else {
			while (inputFile) {
				getline(inputFile, str);
				//if a paranthese is found, make the head the supervisor
				if (paranthese.compare(str) == 0) {
					//if the head doesnt have a supervisor, then its the root and we can end the function
					if (!head->supervisor) {
						inputFile.close();
						return 1;
					}
					head = head->supervisor;

					//this is to make sure the program doesnt read in an extra line on accident
				}
				else if (blank.compare(str) == 0) {
					break;
				}
				else {
					counter = 0;
					string newTitle, newName;
					int count = 0;
					int nameCount = 0;
					/*
					Splits the string into the title and into the name
					*/
					for (int iterator = 0; iterator < str.length(); iterator++) {
						char temp = str[iterator];
						//if the char is a comma, start working on the name
						if (comma == temp) {
							count++;
						}
						else if (count == 0) {
							newTitle += temp;
						}
						else {
							//if it is the space after the comma, ignore it
							if (space == temp && nameCount == 0) {
								nameCount++;
							}
							else {
								newName += temp;
							}
						}
					}
					//if size is 0 it adds the root
					if (size == 0) {
						addRoot(newTitle, newName);
						head = getRoot();
					}
					//hire to the head and then make the head the new hire
					else {
						hire(head, newTitle, newName);
						head = find(newTitle);

					}
				}
			}

			inputFile.close();
			return 1;
		}
	}
	/*
	This function writes the tree to a given file name and then closes the file

	This method is in Theta of n
	*/
	void write(string filename) {
		ofstream outputFile(filename);
		//goes to recursive print function
		printToFileSubTreeRecursive(getRoot(), outputFile);
		outputFile.close();
	}
	/*
	This method hires a person to work under the given manager

	This method is in Theta of n
	*/
	void hire(TREENODEPTR manager, string newTitle, string newName) {
		TreeNode *newHire, *currentPerson, *supervisor;
		if (!manager) return;
		supervisor = manager;
		currentPerson = manager;
		newHire = new TreeNode(newTitle, newName);
		//if the person has no left child, make the hire left child
		if (!currentPerson->leftChild) {
			currentPerson->leftChild = newHire;
			newHire->supervisor = supervisor;
		}
		//else iterate through the leftChilds rightSiblings and then add the node
		else {
			currentPerson = currentPerson->leftChild;
			while (currentPerson->rightSibling) {
				currentPerson = currentPerson->rightSibling;
			}
			currentPerson->rightSibling = newHire;
			newHire->supervisor = supervisor;
		}
		size++;
	}
	/*
	This function fires a specified title, makes the children of the
	title the children of the titles supervisor and checks for bounds and uses the
	find function

	It is in Theta of n
	*/
	bool fire(string formerTitle) {
		//verify the root is not the person being fired, if it is return 0
		if (root->title == formerTitle) return 0;

		//search for the person
		TreeNode *firedPerson = find(formerTitle);
		TreeNode *stillLooking = nullptr;
		TreeNode *amend = nullptr;
		//return false if the firedPerson was not found
		if (!firedPerson) return 0;
		size--;
		//make a tree node of the firedPersons right sibling
		if (firedPerson->leftChild) {
			TreeNode * currentRight = firedPerson->rightSibling;
			TreeNode * previous = firedPerson->supervisor->leftChild;
			//iterate through currentRight
			while (currentRight) {
				currentRight = currentRight->rightSibling;
			}
			//iterate through rightSiblings
			while (previous->rightSibling != firedPerson) {
				previous = previous->rightSibling;
			}
			//if firedPerson has a right sibling keep adding
			if (firedPerson->rightSibling) {
				stillLooking = firedPerson->rightSibling;
			}
			while (stillLooking) {
				previous->rightSibling = stillLooking;
				previous = stillLooking;
				stillLooking = stillLooking->rightSibling;
			}
			//iterate if firedPerson has a left child
			if (firedPerson->leftChild) {
				currentRight = firedPerson->leftChild;
				currentRight->supervisor = firedPerson->supervisor;
				amend = firedPerson->supervisor->leftChild;
			}
			//if amend is the firedPerson, make the firedPerson supervisor left child previous and then update the supervisor
			if (amend == firedPerson) {
				firedPerson->supervisor->leftChild = previous;
				previous->supervisor = firedPerson->supervisor;
			}
			else {
				//make previous' right sibling the current right
				previous->rightSibling = currentRight;
				//iterate
				previous = previous->rightSibling;
				//if currentRight has a rightSibling
				if (currentRight->rightSibling) {
					//iterate
					currentRight = currentRight->rightSibling;
					//loop through updating supervisors and adding on to previous' rightSibling
					while (currentRight) {
						currentRight->supervisor = previous->supervisor;
						previous->rightSibling = currentRight;
						previous = previous->rightSibling;
						currentRight = currentRight->rightSibling;
					}
				}
			}
		}
		else {
			//iterate throught the supervisors children until you find the firedPerson
			//then make the node right before the firedPerson's rightSibling a nullptr
			TreeNode *iterate = firedPerson->supervisor->leftChild;
			while (iterate->rightSibling != firedPerson) {
				iterate = iterate->rightSibling;
			}
			iterate->rightSibling = nullptr;
		}
		delete firedPerson;
		return 1;
	}
	~OrgTree() {

	}
};

