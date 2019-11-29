//============================================================================
// Name        : EclipseR.cpp
// Author      : Cameron Ray
// Version     :
// Copyright   : Your copyright notice
// Description : Eclipse csv maker
//============================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "EclipseR.h"

#include "Eclipse.h"
#include "ResizeableArray.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "avlTree.h"

using namespace std;

ResizeableArray<Eclipse> arr;
LinkedList<Eclipse> list;
HashTable<Eclipse> hashTable;
avlTree<Eclipse> tree;

int totalEclipses;
int validEclipses;
int eclipsesMemory;
string header;

/*
 * Helper method for quickSort
 */
void swap(Eclipse* a, Eclipse* b)
{
	Eclipse t = *a;
	*a = *b;
	*b = t;
}

/*
 * Month partition is the same as partition, but compares
 * months(1-c in string) instead of column directly.
 */
int monthPartition(Eclipse arr[], int low, int high, int column) {
	string pivot = arr[high].getMonth();    // pivot
	int i = (low - 1);  // Index of smaller element

	for (int j = low; j <= high- 1; j++) {
		// If current element is smaller than or
		// equal to pivot
		if (arr[j].getMonth().compare(pivot) <= 0) {
			i++;    // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (Eclipse arr[], int low, int high, int column)
{
	string pivot = arr[high].getColumn(column);    // pivot
	int i = (low - 1);  // Index of smaller element

	for (int j = low; j <= high- 1; j++) {
		// If current element is smaller than or
		// equal to pivot
		if (arr[j].getColumn(column).compare(pivot) <= 0) {
			i++;    // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index
  quicksorting method taken from:
  http://www.geeksforgeeks.org/quick-sort/
 */
void quickSort(Eclipse arr[], int low, int high, int column)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
           at right place */
		int pi;

		switch (column) {
		case 4:
			pi = monthPartition(arr, low, high, column);
			break;
		default:
			pi = partition(arr, low, high, column);
		}

		// Separately sort elements before
		// partition and after partition
		quickSort(arr, low, pi - 1, column);
		quickSort(arr, pi + 1, high, column);
	}
}

/*
 * Find method using linear or binary search,
 * whether they are unsorted or sorted.
 */
void find(int column, string key, int sorted) {
	int counter = 0;
	int loop = true;

	//if already sorted then binary, else linear
	if (sorted == 0) {
		//linear search
		for (int i = 0; i < arr.currentIndex; ++i) {
			if (arr.arr[i].getColumn(column) == key) {
				cout << arr.arr[i].cleanLine;
				counter += 1;
			}
		}
		cout << "Eclipses found: " << counter << endl;
	} else {

		//binary seach (finding first instance of item)
		int start = 0;
		int end = arr.currentIndex;
		int mid;
		while (start < end) {
			mid = (start + end) / 2;
			if (arr.arr[mid].getColumn(column).compare(key) >= 0)
				end = mid;
			else
				start = mid + 1;
		}

		int j = start;

		//small linear search to find all duplicates then output them
		while (loop == true) {
			if (arr.arr[j].getColumn(column) == key) {
				cout << arr.arr[j].cleanLine;
				counter += 1;
			} else {
				loop = false;
			}
			j++;
		}
		cout << "Eclipses found: " << counter << endl;
	}

}

void createHash() {
	//hashTable = new HashTable<Eclipse>;
	hashTable.~HashTable();

	//sets amount of buckets
	hashTable.addSize(validEclipses / 5);

	Node<Eclipse>* tempEcl = list.head;


	for (int i = 1; i <= validEclipses; i++) {
		hashTable.Insert(tempEcl->getData(), tempEcl->getPosition());
		tempEcl = tempEcl->getNext();
	}
}

void readData(string entireFile) {
	string line;
	int tempIndex = -1;
	int tempLength;

	//gets first good line
	tempLength = entireFile.find('\n', 0);
	line = entireFile.substr(tempIndex + 1, (tempLength - tempIndex));
	tempIndex = tempLength;

	//while there is still another line left, loop
	while (tempIndex != -1) {
		Eclipse e = Eclipse(line);
		totalEclipses += 1;

		if (!e.isError) {
			validEclipses += 1;
			if (list.add(e, stoi(e.parsedData[1]))) {
				eclipsesMemory += 1;
				root = tree.insert(root, e, stoi(e.parsedData[1]), e.cleanLine);
			}
		}
		//gets next line
		tempLength = entireFile.find('\n', tempIndex + 1);
		if (tempLength != -1) {
			line = entireFile.substr(tempIndex + 1, (tempLength - tempIndex));
		}
		tempIndex = tempLength;
	}

	tempLength = 0;
	tempIndex = -1;
	line = "";

	createHash();
}


bool fileExists(string fileName)
{
	//checks file existence
	ifstream infile;
	infile.open(fileName);
	return infile.good();
}

void purgeData(string purgeFile)
{
	//TODO delete for tree
	string line;
	int tempIndex = -1;
	int tempLength;

	//gets first good line
	tempLength = purgeFile.find('\n', 0);
	line = purgeFile.substr(tempIndex + 1, (tempLength - tempIndex));
	tempIndex = tempLength;

	//while there is still another line left, loop
	while (tempIndex != -1) {
		Eclipse e = Eclipse(line);
		totalEclipses += 1;

		if (!e.isError) {
			if (!list.removeAt(stoi(e.parsedData[1]))) {
				cerr << "Could not find catalog number: " << stoi(e.parsedData[1]);
			} else {
				tree.deleteItem(root, stoi(e.parsedData[1]));
			}
		}

		//gets next line and resets columns and error boolean
		tempLength = purgeFile.find('\n', tempIndex + 1);
		if (tempLength != -1) {
			line = purgeFile.substr(tempIndex + 1, (tempLength - tempIndex));
		}
		tempIndex = tempLength;
	}

	createHash();
}

int main() {
	//input loop
	string fileName;
	ifstream fileReader;
	ofstream fileWriter;
	string startFile;
	int tempIndex = 0;
	int i = 0;
	bool stopLoop = false;
	bool flag = false;

	//data manipulation loop vars
	char choice;
	string input;
	string input2;
	string output;
	string tempStr;
	int sortedColumn = 0;
	string mergeFile;
	string purgeFile;
	string line;

	//tries filename until correct
	while (stopLoop == false) {
		//gets input until nothing or correct
		getline(cin, fileName);

		//determines valid input
		if (fileName == "") {
			if (startFile.empty()) {
				cout << "Goodbye.";
				exit(true);
			}
			stopLoop = true;
		} else {
			fileReader.open(fileName);
			if (!fileReader.good()) {
				cerr << "File is not available.\n";
			} else {
				while (!fileReader.eof()) {
					header = startFile.substr(0, tempIndex);
					header += '\n';

					//puts entire file into a string for ease of use later
					for (string line; getline(fileReader, line); )
					{
						if (i < 10) {
							header += line;
							header += '\n';

						} else {
							startFile += line;
							startFile += '\n';
						}
						i++;
					}
					i = 0;
				}
				fileReader.clear();
				fileReader.close();
			}
		}
	}

	//adds given data to linked list
	readData(startFile);

	//resets loop boolean
	stopLoop = false;


	//creates the array.
	Node<Eclipse>* tempNode = list.head;
	Eclipse eOut = tempNode->getData();

	while (flag != true) {
		arr.Add(eOut);
		if (tempNode != list.tail) {
			tempNode = tempNode->getNext();
			eOut = tempNode->getData();
		} else {
			flag = true;
		}
	}
	flag = false;


	//data manipulation loop
	while (stopLoop != true) {
		cout << "F(ind) O(utput) Q(uit) S(ort) M(erge) P(urge) C(atalog) H(ash) L(ist) (p)R(e) (pos)T?\n";
		cin >> choice;

		switch (choice) {
		case 'F':
			cout << "Select data field(1-18):\n";
			cin >> input;

			try {
				stoi(input);
			} catch (...) {
				cerr << "Invalid data field" << endl;
				break;
			}
			cout << "Query: \n";
			cin >> input2;

			if (stoi(input) == 1) {
				cout << hashTable.Find(1).cleanLine;
				cout << "Eclipses found: 1\n";
			} else {
				find(stoi(input), input2, sortedColumn);
			}


			break;
		case 'O':	//Output case
			cout << "File Name?\n";
			cin.ignore();
			getline(cin, input);

			if (input != "") {
				if (!fileExists(input)) {
					cerr << "File is not available.\n";
					break;
				} else {
					fileWriter.open(input);
					fileWriter << header;
					for (int i = 0; i < arr.currentIndex; ++i) {
						fileWriter << arr.arr[i].getCleanLine();
					}
				}
			} else {
				cout << header;
				for (int i = 0; i < arr.currentIndex; ++i) {
					cout << arr.arr[i].getCleanLine();
				}
			}
			cout << "Data lines read: " << totalEclipses << "; Valid eclipses read: " << validEclipses << "; Eclipses in memory: " << eclipsesMemory << endl;
			break;
		case 'Q':
			stopLoop = true;	//break out of the loop to end application
			break;
		case 'S':
			cout << "Select data field(1-18):\n";
			cin >> input;

			try {
				stoi(input);
			} catch (...) {
				cerr << "Invalid data field" << endl;
				break;
			}
			quickSort(arr.arr, 0, arr.currentIndex, stoi(input));
			sortedColumn = stoi(input);
			break;
		case 'M':
			//if doesnt exist, add, else replace
			cout << "Input merge file:\n";
			cin.ignore();
			getline(cin, fileName);

			//determines valid input
			if (fileName == "") {
				cerr << "File is not available.\n";
			} else {
				fileReader.open(fileName);
				if (!fileReader.good()) {
					cerr << "File is not available.\n";
				} else {
					while (!fileReader.eof()) {
						//puts entire file into a string for ease of use later
						for (string line; getline(fileReader, line); )
						{
							if (i >= 10) {
								mergeFile += line;
								mergeFile += '\n';
							}
							i++;
						}
						i = 0;
					}
					//close reader and  merge the eclipses
					fileReader.clear();
					fileReader.close();
					readData(mergeFile);

					//recreates the array.
					Node<Eclipse>* tempNodeM = list.head;
					Eclipse eOutM = tempNodeM->getData();


					arr.arr = new Eclipse[arr.arraySize];


					while (flag != true) {
						arr.Add(eOutM);
						if (tempNodeM != list.tail) {
							tempNodeM = tempNodeM->getNext();
							eOutM = tempNodeM->getData();
						} else {
							flag = true;
						}
					}
					flag = false;

				}
			}
			break;
		case 'P':
			//if exists in linked list, delete
			//new resizeable array created
			//gets input until nothing or correct
			cout << "Input purge file:\n";
			cin.ignore();
			getline(cin, fileName);

			//determines valid input
			if (fileName == "") {
				cerr << "File is not available.\n";
			} else {
				fileReader.open(fileName);
				if (!fileReader.good()) {
					cerr << "File is not available.\n";
				} else {
					while (!fileReader.eof()) {
						//puts entire file into a string for ease of use later
						for (string line; getline(fileReader, line); )
						{
							if (i >= 10) {
								purgeFile += line;
								purgeFile += '\n';
							}
							i++;
						}
						i = 0;
					}
					//close reader and  purge the eclipses
					fileReader.clear();
					fileReader.close();
					purgeData(purgeFile);


					//recreates the array.
					Node<Eclipse>* tempNodeP = list.head;
					Eclipse eOutP = tempNodeP->getData();

					arr.arr = new Eclipse[arr.arraySize];

					while (flag != true) {
						arr.Add(eOutP);
						if (tempNodeP != list.tail) {
							tempNodeP = tempNodeP->getNext();
							eOutP = tempNodeP->getData();
						} else {
							flag = true;
						}
						i++;
					}
					flag = false;
				}
			}
			break;
		case 'C':
		{
			//output header then tree, then validity
			cout << header;

			tree.inorder(root);
			cout << "Data lines read: " << totalEclipses << "; Valid eclipses read: " << validEclipses << "; Eclipses in memory: " << eclipsesMemory << endl;
			break;
		}
		case 'H' :
		{
			Node<Eclipse>* tempNodeH;
			Eclipse eOutH;

			for (int i = 0; i < hashTable.hashSize; i++) {
				cout << "Bucket: " << i << endl;
				tempNodeH = hashTable.itemArr[i].head;
				eOutH = tempNodeH->getData();
				flag = false;

				if (tempNodeH != NULL) {
					while (flag != true) {
						cout << eOutH.cleanLine;
						if (tempNodeH != hashTable.itemArr[i].tail) {
							tempNodeH = tempNodeH->getNext();
							eOutH = tempNodeH->getData();
						} else {
							flag = true;
						}
					}
				} else {
					cout << "NULL\n";
				}
				cout << endl;
			}
			break;
		}
		case 'L' :
		{
			//output header then linked list, then validity
			cout << header;

			Node<Eclipse>* tempNodeL = list.head;
			Eclipse eOutL = tempNodeL->getData();

			while (flag != true) {
				cout << eOutL.cleanLine;
				if (tempNodeL != list.tail) {
					tempNodeL = tempNodeL->getNext();
					eOutL = tempNodeL->getData();
				} else {
					flag = true;
				}
			}
			cout << "Data lines read: " << totalEclipses << "; Valid eclipses read: " << validEclipses << "; Eclipses in memory: " << eclipsesMemory << endl;
			break;
		}
		case 'R':
		{
			//pre-order traversal of array
			tree.preorder(root);
			break;
		}

		case 'T':
		{
			//post-order traversal of array
			tree.postorder(root);
			break;
		}
		}
	}
	//exiting message
	cout << "Have a nice day.";
	exit(true);

	//obligatory return statement
	return 0;
}
