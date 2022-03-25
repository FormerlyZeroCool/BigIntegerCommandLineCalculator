/*
 * largeint.h
 *
 *  Created on: Oct 14, 2019
 *      Author: andrew
 */

#ifndef LARGEINT_H_
#define LARGEINT_H_
#include "unsortedlist.hpp"
#include <string>
#include <iostream>
#include <stack>
#include "bigintdivresult.h"


struct divResult{
	UnsortedList<short> quotient;
	short remainder;
	divResult(UnsortedList<short> q,short r):quotient(q),remainder(r){}
};
//Big Endian
class LargeInt{
private:
	UnsortedList<short> number;
	bool negative;
	short radix;
	bool isZero();
	void trim();
	long toLong();
	//Simple function to add two lists together cannot handle summand2+(-summand)
	UnsortedList<short> addTwoLists(UnsortedList<short> &summand1,UnsortedList<short> &summand2,short radix);
	//Simple subtraction function, cannot handle minuend<subtrahend
	UnsortedList<short> subtractTwoLists(UnsortedList<short> minuend,UnsortedList<short> subtrahend, short radix);
	//Will run as long as divisor is less than 2^15
	divResult divideList(UnsortedList<short> dividend,short divisor,short radix);
	//Should always run, but less efficiently
	bigDivResult divideTwoLargeInt(LargeInt divisor,LargeInt dividend,short radix);
public:

	LargeInt getBinVersion();
	LargeInt():negative(false),radix(10){}
	LargeInt(std::string num);
	LargeInt(int other);
	LargeInt(UnsortedList<short> num,bool isNegative = false,short radix = 10);
	LargeInt operator^(LargeInt power);
	void operator=(const LargeInt& other);
	void operator=(int other);
	bool operator==(LargeInt& other);
	bool operator!=(int other);
	LargeInt operator-(LargeInt subtrahend);
	void operator-=(LargeInt subtrahend);
	void operator*=(LargeInt multiplyer);
	LargeInt operator*(LargeInt multiplyer);
	LargeInt operator/(LargeInt dividend);
	LargeInt operator/=(LargeInt divisor);
	LargeInt operator%(LargeInt dividend);
	LargeInt operator%=(LargeInt dividend);
	void operator+=(LargeInt summand);
	LargeInt operator+(LargeInt summand);
	bool operator<=(LargeInt& o);
	bool operator<(LargeInt&);
	bool operator>(LargeInt&);
	bool operator>=(LargeInt&);
	//setRadix() actually changes the data structure,
	//so always iterators instantiated after this will not point at the correct element
	void setRadix(short radix);
	bool isNegative();
	void insertHead(short);
	void insertTail(short);
	void printNum();
	friend std::ostream& operator<<(std::ostream &o,LargeInt &l);
	friend std::istream& operator>>(std::istream &o,LargeInt &l);
};

#endif /* LARGEINT_H_ */
