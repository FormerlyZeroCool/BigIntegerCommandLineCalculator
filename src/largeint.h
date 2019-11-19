/*
 * largeint.h
 *
 *  Created on: Oct 14, 2019
 *      Author: andrew
 */

#ifndef LARGEINT_H_
#define LARGEINT_H_
#include "unsortedlist.h"
#include <string>
#include <iostream>
#include <stack>
#include "bigintdivresult.h"


struct divResult{
	unsortedList<short> quotient;
	short remainder;
	divResult(unsortedList<short> q,short r):quotient(q),remainder(r){}
};
//Big Endian
class largeInt{
private:
	unsortedList<short> number;
	bool negative;
	short radix;
	bool isZero();
	void trim();
	long toLong();
	unsortedList<short> addTwoLists(unsortedList<short> &summand1,unsortedList<short> &summand2,short radix);
	unsortedList<short> subtractTwoLists(unsortedList<short> minuend,unsortedList<short> subtrahend, short radix);
	//Will run as long as divisor is less than 2^15
	divResult divideList(unsortedList<short> dividend,short divisor,short radix);
	//Should always run, but less efficiently
	bigDivResult divideTwoLargeInt(largeInt divisor,largeInt dividend,short radix);
public:

	largeInt getBinVersion();
	largeInt():negative(false),radix(10){}
	largeInt(std::string num);
	largeInt(int other);
	largeInt(unsortedList<short> num,bool isNegative = false,short radix = 10);
	largeInt operator^(largeInt power);
	void operator=(const largeInt& other);
	void operator=(int other);
	bool operator==(largeInt& other);
	bool operator!=(int other);
	largeInt operator-(largeInt subtrahend);
	void operator-=(largeInt subtrahend);
	void operator*=(largeInt multiplyer);
	largeInt operator*(largeInt multiplyer);
	largeInt operator/(largeInt dividend);
	largeInt operator/=(largeInt divisor);
	largeInt operator%(largeInt dividend);
	largeInt operator%=(largeInt dividend);
	void operator+=(largeInt summand);
	largeInt operator+(largeInt summand);
	bool operator<=(largeInt& o);
	bool operator<(largeInt&);
	bool operator>(largeInt&);
	bool operator>=(largeInt&);
	//setRadix() actually changes the data structure,
	//so always iterators instantiated after this will not point at the correct element
	void setRadix(short radix);
	bool isNegative();
	void insertHead(short);
	void insertTail(short);
	void printNum();
	friend std::ostream& operator<<(std::ostream &o,largeInt &l);
	friend std::istream& operator>>(std::istream &o,largeInt &l);
};

#endif /* LARGEINT_H_ */
