/*
 * Eclipses.cpp
 *
 *  Created on: Sep 18, 2017
 *      Author: camer
 */

#include "Eclipse.h"

#include <string>
using namespace std;

Eclipse::Eclipse() {
	isError = true;
}


Eclipse::Eclipse(string inputLine) {
	//default constructor
	line = inputLine;
	cleanLine = inputLine;
	CheckError();

}

void Eclipse::CheckError() {
	if (line.empty()) {
		isError = true;
	} else {
		//find out if the eclipse is partial
		if (line.at(PARTIAL) == 'P') {
			isPartial = true;
		} else {
			isPartial = false;
		}

		//go through every char in each line and make into csv lines
		for (unsigned int i = 0; i < line.length(); ++i) {
			if ((isspace(line[i]) && isspace(line[i+1]))) {
				line.erase(i,1);
				i -= 1;
			} else if (isspace(line[i])) {
				if (i < 1 || numColumns == 18) {
					line.erase(i,1);
					i -= 1;
				} else {
					line[i] = ',';
					numColumns += 1;	//add to column count with each seperating comma
				}
			}
		}

		//gets rid of last comma in partial eclipses
		if (isPartial) {
			line.erase(line.length() - 1, 1);
			numColumns -= 1;
		}

		//finds the index of the eclipse
		tempIndex = line.find(',');
		try {
		position = stoi(line.substr(0, tempIndex));
		} catch (...) {
			//never get here
		}
		parsedData[1] = line.substr(0, tempIndex);

		//checks for first type of error
		if (((!isPartial) && (numColumns == 18)) || ((isPartial) && numColumns == 16)) {
		} else {
			if (isPartial) {
				cerr << "Error in data row " << position << ": " << numColumns << " columns found. Should be 16." << endl;
			} else {
				cerr << "Error in data row " << position << ": " << numColumns << " columns found. Should be 18." << endl;
			}
			isError = true;
		}

		//checks for second type of error
		if (!isError) {
			//finds the data between the commas and adds to an array
			for (int i = 2; i <= numColumns; ++i) {
				tempLength = line.find(',', tempIndex + 1);
				if (tempLength != 0) {
					parsedData[i] = line.substr(tempIndex + 1, (tempLength - tempIndex - 1));
				}
				tempIndex = tempLength;

				//if it should be an int
				if (i == 1 || i == 2 || i == 3 || i == 5 || i == 7 ||
						i == 9 || i == 15 || i == 16 || (i == 17 && !isPartial)) {
					try {
						stoi(parsedData[i]);
					} catch (...) {
						cerr << "Error in data row " << position << ": Column " << i << " is not a whole number." << endl;
						isError = true;
					}
				}

				//if it should be a double
				if (i == 11 || i == 12) {
					try {
						stod(parsedData[i]);
					} catch (...) {
						cerr << "Error in data row " << position << ": Column " << i << " is not a decimal number." << endl;
						isError = true;
					}
				}
			}
		}

		//sets month
		if (parsedData[4] == "Jan") {
			month = "1";
		} else if (parsedData[4] == "Feb") {
			month = "2";
		} else if (parsedData[4] == "Mar") {
			month = "3";
		} else if (parsedData[4] == "Apr") {
			month = "4";
		} else if (parsedData[4] == "May") {
			month = "5";
		} else if (parsedData[4] == "Jun") {
			month = "6";
		} else if (parsedData[4] == "Jul") {
			month = "7";
		} else if (parsedData[4] == "Aug") {
			month = "8";
		} else if (parsedData[4] == "Sep") {
			month = "9";
		} else if (parsedData[4] == "Oct") {
			month = "a";
		} else if (parsedData[4] == "Nov") {
			month = "b";
		} else if (parsedData[4] == "Dec") {
			month = "c";
		} else {
			month = "d";
		}

	}
}

string Eclipse::getColumn(int column) {
	return parsedData[column];
}

Eclipse::~Eclipse() {
}
