/*
 * LinkedList.h
 *
 *  Created on: Oct 26, 2017
 *      Author: camer
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <iostream>
using namespace std;

template <class T> class Node
{
public:
	T getData() const {
		return data;
	}

	void setData(T data) {
		this->data = data;
	}

	Node<T>*& getNext() {
		return next;
	}

	void setNext(Node<T>* next) {
		this->next = next;
	}

	int getPosition() const {
		return position;
	}

	void setPosition(int position) {
		this->position = position;
	}

private:
	T data; //the object information
	Node* next; //pointer to the next node element
	int position;
};

template <class T> class LinkedList
{

public:

	Node<T> *head, *tail, *temp;

	LinkedList()
	{
		head = NULL;
		tail = NULL;
		temp = NULL;
	}

	~LinkedList()
	{

	}

	bool removeAt(int position) {
		bool flag = false;
		Node<T>* tempRemover = new Node<T>;
		tempRemover->setPosition(position);
		Node<T>* tempRemovee = new Node<T>;
		tempRemovee = head;

		while (flag != true) {
			if (tempRemovee == tail) {
				return false;
			} else if (tempRemover->getPosition() == tempRemovee->getPosition()) {
				tempRemovee = tempRemovee->getNext();
				flag = true;
			} else {
				tempRemovee = tempRemovee->getNext();
			}
		}

		return true;
	}

	void replaceAt(T info, int position) {
		bool flag = false;
		temp = new Node<T>;
		temp->setPosition(position);
		temp->setData(info);

		Node<T>* temp2 = new Node<T>;
		temp2 = head;

		while (flag != true) {
			if (temp->getPosition() == temp2->getPosition()) {
				temp2 = temp;
			} else {
				temp2 = temp2->getNext();
			}
		}


	}

	//Method adds info to the end of the list
	bool add(T info, int position)
	{
		if(head == NULL) //if list is currently empty
		{
			head = new Node<T>; //Create new node of type T
			head->setData(info);
			head->setPosition(position);
			tail = head;

		}
		else //if not empty find where it goes
		{
			temp = new Node<T>;
			temp->setPosition(position);
			temp->setData(info);
			bool flag = false;

			//if catalog number is larger than the tail, add to tail
			if (temp->getPosition() > tail->getPosition()) {
				temp->setData(info);
				tail->setNext(temp);
				tail = tail->getNext();
			} else {
				Node<T>* tempAfter = new Node<T>;
				Node<T>* tempBefore = new Node<T>;
				tempAfter = head;

				//finds nodes which our new one needs to be before and after
				while(flag != true) {
					if (temp->getPosition() > tempAfter->getPosition()) {
						tempBefore = tempAfter;
						tempAfter = tempAfter->getNext();
					} else if (temp->getPosition() == tempAfter->getPosition()){
						//replaces node with duplicate
						tempBefore->setNext(temp);
						temp->setNext(tempAfter->getNext());
						return false;
					} else {
						if (tempAfter == head) {
							temp->setNext(tempAfter);
							head = temp;
						} else {
							//sets temp node into the linked list
							tempBefore->setNext(temp);
							temp->setNext(tempAfter);
						}
						flag = true;
					}
				}
			}

		}
		return true;
	}
};
#endif /* LINKEDLIST_H_ */
