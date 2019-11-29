/*
 * Eclipse.h
 *
 *  Created on: Sep 18, 2017
 *      Author: Cameron
 */

#ifndef ECLIPSE_H_
#define ECLIPSE_H_
#include <iostream>
#include <string>

class Eclipse {

	//friend operator
	friend std::ostream& operator << (std::ostream& os, const Eclipse&){
		return os << "this is an eclipse" << std::endl;
	};



public:
	Eclipse();
	Eclipse(std::string line);
	void CheckError();
	std::string getColumn(int column);
	virtual ~Eclipse();

	bool isError = false;
	std::string cleanLine;
	std::string line;
	int position = 0;
	std::string parsedData[20];
 	std::string month;

	//implementation of operator=
	Eclipse& operator= (const Eclipse& ecl)
	{
		isError = ecl.isError;
		line = ecl.line;
		month = ecl.month;
		cleanLine = ecl.cleanLine;

		for (int i = 0; i < 20; ++i) {
			parsedData[i] = ecl.parsedData[i];
		}

		// return the existing object so we can chain this operator
		return *this;
	}

	const std::string& getLine() const {
		return line;
	}

	void setLine(const std::string& line) {
		this->line = line;
	}

	const std::string& getCleanLine() const {
		return cleanLine;
	}

	const std::string& getMonth() const {
		return month;
	}

private:
	void checkError();

	bool isPartial = false;
	int numColumns = 1;
	int tempIndex = 0;
	int tempLength = 0;
	const int PARTIAL = 56;
};


#endif /* ECLIPSE_H_ */
