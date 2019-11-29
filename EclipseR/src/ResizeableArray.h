/*
 * ResizeableArray.h
 *
 *  Created on: Sep 25, 2017
 *      Author: camer
 */

#ifndef RESIZEABLEARRAY_H_
#define RESIZEABLEARRAY_H_
#include <string>

template<typename TheType>
class ResizeableArray {
public:
	ResizeableArray();
	ResizeableArray(int);
	virtual ~ResizeableArray();


	void SizeDoubler();
	void Remove();

	void Add(TheType);
	void AddAt(TheType, int);
	void ReplaceAt(TheType, int);
	void RemoveAt(TheType, int);

	int arraySize = 10;
	TheType* arr = new TheType[arraySize];
	int currentIndex = 0;

private:

};

template<typename TheType>
inline ResizeableArray<TheType>::ResizeableArray() {
	//default array size
	arraySize = 10;
}

template<typename TheType>
inline ResizeableArray<TheType>::ResizeableArray(int size) {
	//custom array size
	arraySize = size;
}

template<typename TheType>
inline void ResizeableArray<TheType>::SizeDoubler() {
	//make temp array
	TheType* tempArray = arr;

	//double size and make eclipses that size
	arraySize *= 2;
	arr = new TheType[arraySize];

	//set eclipses to tempArray values
	for (int i = 0; i < (arraySize / 2); ++i) {
		arr[i] = tempArray[i];
	}

	delete[] tempArray;

}

template<typename TheType>
inline void ResizeableArray<TheType>::Remove() {

	delete(&arr[currentIndex]);
	currentIndex -= 1;

	//checks for array size, if half then halve
	if (currentIndex == (arraySize / 2)) {
		//make temp array
		TheType *tempArray = arr;

		//halve the size and make eclipses that size
		arraySize /= 2;
		arr = new TheType[arraySize];

		//set eclipses to tempArray values
		for (int i = 0; i < (arraySize * 2); ++i) {
			arr[i] = tempArray[i];
		}

		//deletes the tempArray
		delete[] tempArray;
	}
}

template<typename TheType>
inline void ResizeableArray<TheType>::Add(TheType element) {
	//adds an element to the array
	if (currentIndex < arraySize) {
		arr[currentIndex] = element;
		currentIndex += 1;
	} else {
		ResizeableArray::SizeDoubler();
		arr[currentIndex] = element;
		currentIndex += 1;
	}

}

template<typename TheType>
inline void ResizeableArray<TheType>::AddAt(TheType element, int index) {
	//adds an element at specific index
	TheType temp;
	if (index < currentIndex) {
		temp = arr[index];
		for (int i = index; i < arraySize - 2; ++i) {
			arr[i] = temp;
			temp = arr[i+1];
		}
	} else {
		std::cerr << "Invalid argument for AddAt index : " << index << std::endl;
	}
}

template<typename TheType>
inline void ResizeableArray<TheType>::ReplaceAt(TheType element, int index) {
	//replace element at specific
	if (index < currentIndex) {
		arr[index] = element;
	} else {
		std::cerr << "Invalid argument for ReplaceAt index : " << index << std::endl;
	}
}

template<typename TheType>
inline void ResizeableArray<TheType>::RemoveAt(TheType element, int index) {
	//remove element at specific index
	if (index < currentIndex) {
		delete(&arr[index]);
		for (int i = index; i < arraySize - 2; ++i) {
			arr[i] = arr[i+1];
		}
	} else {
		//std::cerr << "Invalid argument for RemoveAt index : " << index << std::endl;
	}
}

template<typename TheType>
inline ResizeableArray<TheType>::~ResizeableArray() {
	delete(arr);
}


#endif /* RESIZEABLEARRAY_H_ */
