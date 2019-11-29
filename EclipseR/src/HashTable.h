/*
 * HashTable.h
 *
 *  Created on: Nov 9, 2017
 *      Author: camer
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_
#include <iostream>
#include <string>
#include "LinkedList.h"

using namespace std;


template <class T> class HashTable
{
public:

	int hashSize = 0; // the size of the hash table (how many cells it has)
	int totElems = 0; // holds the total number of elements in the entire table
	int* bucketSize = 0;
	LinkedList<T>* itemArr = new LinkedList<T>[hashSize];

	HashTable()
	{

	}/* End of HashTable */

	void addSize(int hashSze) {
		hashSize = hashSze;
		itemArr = new LinkedList<T>[hashSize];
		bucketSize = new int[hashSize];
		for(int x=0; x < hashSize; ++x)
		{
			itemArr[x].head = NULL;
			bucketSize[x] = 0;
		}
	}

	bool IsEmpty(int key)
	{
		if(key >=0 && key < hashSize)
		{
			return itemArr[key].head == NULL;
		}
		return true;
	}/* End of IsEmpty */

	int Hash(int preKey)
	{
		return abs(preKey % hashSize);
	} /* End of Hash */

	void Insert(T newItem, int preKey)
	{
		int key = Hash(preKey);
		Append(key, newItem, preKey);
	}/* End of Insert */

	void Append(int key, T newItem, int preKey)
	{
		itemArr[key].add(newItem, preKey);

		++bucketSize[key];
		++totElems;

	}/* End of Append */

	int getHashSize()
	{
		return hashSize;
	}/* End of TableSize */

	int getTotElems()
	{
		return totElems;
	}/* End of TotalElems */

	int getBucketSize(int key)
	{
		return(!IsEmpty(key)) ? bucketSize[key]:0;
	}/* End of BucketSize */

	T Find(int preKey)
	{
		int key = Hash(preKey);
		bool stopLoop = false;
		Node<T>* temp;

		if(!IsEmpty(key)) {
			temp  = itemArr[key].head;

			while (stopLoop != true) {
				if (temp != itemArr[key].tail){
					temp = temp->getNext();
					if (temp->getPosition() == preKey) {
						stopLoop = true;
					}
				} else {
					stopLoop = true;
				}
			}
		}
		return temp->getData();
	}/* End of Count */

	~HashTable()
	{
		delete[] itemArr;
		delete[] bucketSize;
	}/* End of ~HashTable */

};


#endif /* HASHTABLE_H_ */
