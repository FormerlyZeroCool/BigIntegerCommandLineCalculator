/*
 * largint.cpp
 *
 *  Created on: Oct 17, 2019
 *      Author: andrew
 */

#include "largeint.h"
//Will run as long as divisor is less than 2^15
divResult largeInt::divideList(unsortedList<short> dividend,short divisor,short radix)
{
	unsortedList<short> quotient;
	short current;
	short remainder = 0;
	bool addZero = false;
	for(iterator<short> it = dividend.begin();it != dividend.end(); it++)
	{
		current = *it+(remainder*radix);
		if(current != 0)
		{
			addZero = true;
		}
		if(addZero)
		{
			quotient.insertTail(current/divisor);
			remainder = current % divisor;
		}
	}
	return divResult(quotient,remainder);
}

bigDivResult largeInt::divideTwoLargeInt(largeInt divisor,largeInt dividend,short radix)
{
	short originalThisRadix = radix;

	bool isNegLocal = false;

	if(divisor.isNegative())
		isNegLocal = !isNegLocal;
	if(dividend.isNegative())
		isNegLocal = !isNegLocal;

	largeInt portionOfDividend("");
	portionOfDividend.setRadix(2);
	iterator<short> divIt = dividend.number.begin();
	unsortedList<short> result;
	int insertCount = 0;
	divisor.setRadix(2);
	dividend.setRadix(2);

	while(!dividend.number.isEmpty())
	{
		while(divIt != nullptr && portionOfDividend < divisor)
		{
			if(!portionOfDividend.isZero() || *divIt != 0)
				portionOfDividend.number.insertTail(*divIt);
			divIt++;
			dividend.number.removeHead();
			if(insertCount==0)
			result.insertTail(0);
			else
				insertCount--;

			std::cout<<"Building Portion Of Dividend: ";
			portionOfDividend.printNum();
			std::cout<<std::endl;

		}
		if(!portionOfDividend.isZero() //&& divisor <= portionOfDividend
		)
		{
			std::cout<<"Portion Of Dividend: ";
			portionOfDividend.printNum();
			std::cout<<std::endl<<"divisor: ";
			divisor.printNum();
			std::cout<<std::endl;
			result.insertTail(1);
			largeInt remainder = portionOfDividend-divisor;
			portionOfDividend.number.emptyList();

			///trim subtraction result
			remainder.trim();

			std::cout<<std::endl<<"remainder: ";
			remainder.printNum();
			std::cout<<std::endl;
			//insert remainder in head of portionofdividend
			if(!remainder.isZero())
			{
				iterator<short> remainderIt = remainder.number.rbegin();
				while(remainderIt != remainder.number.rend())
				{
					portionOfDividend.number.insertHead(*remainderIt);
					remainderIt--;
				}
				//portionOfDividend.trim();
				std::cout<<"Portion Of Dividend after subtraction, and rebuilding: ";
				portionOfDividend.printNum();
				std::cout<<std::endl;
			}
			portionOfDividend.trim();
			insertCount++;
		}
		else if(divIt == nullptr)
		{
			if (portionOfDividend < divisor || portionOfDividend.isZero())
			{
				result.insertTail(0);
			}
			dividend = largeInt("");
		}
	}

	largeInt intResult = largeInt(result,isNegLocal,2);
	intResult.setRadix(originalThisRadix);
	portionOfDividend.setRadix(originalThisRadix);
	return bigDivResult(intResult.number,isNegLocal,portionOfDividend.number,isNegLocal);
}
unsortedList<short> largeInt::addTwoLists(unsortedList<short> &summand1,unsortedList<short> &summand2,short radix)
{
	unsortedList<short> endSum;
	iterator<short> other = summand2.rbegin();
	iterator<short> num = summand1.rbegin();
	short carry = 0;
	short sum = 0;
	while(other != nullptr && num != nullptr)
	{
		sum = (*other+*num+carry)%radix;
		carry = (*other+*num+carry)/radix;
		endSum.insertHead(sum);
		other--;
		num--;
	}
	while(other != nullptr)
	{
		endSum.insertHead((*other+carry)%radix);
		carry = (*other+carry)/radix;
		other--;
	}
	while(num != nullptr)
	{
		endSum.insertHead((*num+carry)%radix);
		carry = (*num+carry)/radix;
		num--;
	}
	if(carry)
		endSum.insertHead(1);
	return endSum;
}

unsortedList<short> subtractTwoLists(unsortedList<short> minuend,unsortedList<short> subtrahend, short radix)
{
	iterator<short> oIt = subtrahend.rbegin();
	iterator<short> tIt = minuend.rbegin();
	iterator<short> walkerTIt = tIt;
	unsortedList<short> result;
	short cr = 0;
	while(oIt != nullptr && tIt != nullptr)
	{
		if(*tIt < *oIt)
		{
			walkerTIt = tIt;
			walkerTIt--;
			while(walkerTIt != minuend.begin() && *walkerTIt == 0)
			{
				walkerTIt--;
			}
			*walkerTIt -= 1;
			walkerTIt++;
			while(walkerTIt != minuend.rbegin() && walkerTIt != tIt)
			{
				*walkerTIt = radix-1;
				walkerTIt++;
			}
			*tIt = (radix+*tIt);
		}
		cr = *tIt - *oIt;
		result.insertHead(cr);
		oIt--;
		tIt--;
	}

	while(tIt != nullptr)
	{
		result.insertHead(*tIt);
		tIt--;
	}
	return result;

}

bool isEqualToZero(unsortedList<short> number)
{
	bool isZero = true;
	for(iterator<short> i = number.begin();i != nullptr;i++)
	{
		if(*i != 0)
			isZero = false;
	}
	return isZero;
}
void largeInt::trim()
{
	iterator<short> it = number.begin();
	while(it != nullptr && *it == 0)
	{
		it++;
		number.removeHead();
	}
}
largeInt largeInt::getBinVersion()
{
	divResult result(number,0);
	unsortedList<short> binRep;
	while(!isEqualToZero(result.quotient))
	{
		result = divideList(result.quotient,2,radix);
		binRep.insertHead(result.remainder);
	}

	largeInt binNum = largeInt(binRep,this->negative,2);
	return binNum;
}
bool largeInt::isZero()
{
	bool isZero = true;
	for(iterator<short> i = number.begin();i != nullptr;i++)
	{
		if(*i != 0)
			isZero = false;
	}
	return isZero;
}

void largeInt::setRadix(short newRadix)
{

	divResult result(number,0);
	unsortedList<short> convertedNum;
	while(!isEqualToZero(result.quotient))
	{
		result = divideList(result.quotient,newRadix,this->radix);
		convertedNum.insertHead(result.remainder);
	}

	this->number = convertedNum;
	this->radix = newRadix;

}
void largeInt::operator+=(largeInt summand)
{
	bool isNeg = false;
	unsortedList<short> result;
	if(summand.isNegative() && this->isNegative())
	{
		result = addTwoLists(this->number,summand.number,this->radix);
		isNeg = true;
	}
	else if(!summand.isNegative() && !this->isNegative())
	{
		result = addTwoLists(this->number,summand.number,this->radix);
	}
	else if(summand.isNegative() && !this->isNegative())
	{
		if(summand < *this)
		{
			result = subtractTwoLists(this->number,summand.number,radix);
		}
		else
		{
			result = subtractTwoLists(summand.number,this->number,radix);
			isNeg = true;
		}
	}
	else if(!summand.isNegative() && this->isNegative())
	{
		if(summand < *this)
		{
			result = subtractTwoLists(this->number,summand.number,radix);
			isNeg = true;
		}
		else
		{
			result = subtractTwoLists(summand.number,this->number,radix);
		}
	}
	largeInt data(result,isNeg);
	data.trim();
	*this = data;
}

largeInt largeInt::operator+(largeInt summand)
{
	bool isNeg = false;
	unsortedList<short> result;
	if(summand.isNegative() && this->isNegative())
	{
		result = addTwoLists(this->number,summand.number,this->radix);
		isNeg = true;
	}
	else if(!summand.isNegative() && !this->isNegative())
	{
		result = addTwoLists(this->number,summand.number,this->radix);
	}
	else if(summand.isNegative() && !this->isNegative())
	{
		if(summand < *this)
		{
			result = subtractTwoLists(this->number,summand.number,radix);
		}
		else
		{
			result = subtractTwoLists(summand.number,this->number,radix);
			isNeg = true;
		}
	}
	else if(!summand.isNegative() && this->isNegative())
	{
		if(summand < *this)
		{
			result = subtractTwoLists(this->number,summand.number,radix);
			isNeg = true;
		}
		else
		{
			result = subtractTwoLists(summand.number,this->number,radix);
		}
	}
	largeInt data(result,isNeg);
	data.trim();
	return data;
}

bool largeInt::isNegative()
{
	return negative;
}
void largeInt::operator*=(largeInt multiplyer)
{
	int product = 1;
	int carry = 0;
	int count = 0;
	bool isNeg = false;
	//setSign
	if(multiplyer.negative)
		isNeg = !isNeg;
	if(this->negative)
		isNeg = !isNeg;
	unsortedList<short> sumOfProducts;

	for(iterator<short>  oIt = multiplyer.number.rbegin();oIt != nullptr;oIt--)
	{
		short oNum = *oIt;
		count++;
		unsortedList<short> intProduct;
		for(int i = 1;i < count;i++)
		{
			intProduct.insertTail(0);
		}
		for(iterator<short> tIt = this->number.rbegin();tIt != nullptr;tIt--)
		{
			short tNum = *tIt;
			product = ((oNum)*(tNum)+carry)%radix;
			carry = ((oNum)*(tNum)+carry)/radix;
			intProduct.insertHead(product);
		}
		if(carry != 0)
		{
			intProduct.insertHead(carry);
			carry = 0;
		}
		sumOfProducts = addTwoLists(sumOfProducts,intProduct,radix);

	}

	 number = sumOfProducts;
	 negative = isNeg;
}
largeInt largeInt::operator*(largeInt multiplyer)
{

	int product = 1;
	int carry = 0;
	int count = 0;
	bool isNeg = false;
	//setSign
	if(multiplyer.negative)
		isNeg = !isNeg;
	if(this->negative)
		isNeg = !isNeg;
	unsortedList<short> sumOfProducts;

	for(iterator<short>  oIt = multiplyer.number.rbegin();oIt != nullptr;oIt--)
	{
		short oNum = *oIt;
		count++;
		unsortedList<short> intProduct;
		for(int i = 1;i < count;i++)
		{
			intProduct.insertTail(0);
		}
		for(iterator<short> tIt = this->number.rbegin();tIt != nullptr;tIt--)
		{
			short tNum = *tIt;
			product = ((oNum)*(tNum)+carry)%radix;
			carry = ((oNum)*(tNum)+carry)/radix;
			intProduct.insertHead(product);
		}
		if(carry != 0)
		{
			intProduct.insertHead(carry);
			carry = 0;
		}
		sumOfProducts = addTwoLists(sumOfProducts,intProduct,radix);

	}
	return largeInt(sumOfProducts,isNeg,radix);
}

largeInt largeInt::operator/(largeInt divisor)
{
	bigDivResult result = divideTwoLargeInt(divisor,*this,this->radix);

	return largeInt(result.quotientNum,result.quotientIsNeg,this->radix);
}

largeInt largeInt::operator%(largeInt divisor)
{
	bigDivResult result = divideTwoLargeInt(divisor,*this,this->radix);

	return largeInt(result.remainderNum,result.remainderIsNeg,this->radix);
}

void largeInt::operator-=(largeInt subtrahend)
{
	bool isNeg = false;
		unsortedList<short> result;
		if(subtrahend.isNegative() && !this->isNegative())
		{
			result = addTwoLists(this->number,subtrahend.number,this->radix);
		}
		else if(!subtrahend.isNegative() && this->isNegative())
		{
			result = addTwoLists(this->number,subtrahend.number,this->radix);
			isNeg = true;
		}
		else if(!subtrahend.isNegative() && !this->isNegative())
		{
			if(subtrahend < *this)
			{
				result = subtractTwoLists(this->number,subtrahend.number,radix);
			}
			else
			{
				result = subtractTwoLists(subtrahend.number,this->number,radix);
				isNeg = true;
			}
		}
		else if(subtrahend.isNegative() && this->isNegative())
		{
			if(subtrahend < *this)
			{
				result = subtractTwoLists(this->number,subtrahend.number,radix);
				isNeg = true;
			}
			else
			{
				result = subtractTwoLists(subtrahend.number,this->number,radix);
			}
		}
		largeInt data(result,isNeg);
		data.trim();
		*this = data;
}
largeInt largeInt::operator-(largeInt subtrahend)
{
	bool isNeg = false;
	unsortedList<short> result;
	if(subtrahend.isNegative() && !this->isNegative())
	{
		result = addTwoLists(this->number,subtrahend.number,this->radix);
	}
	else if(!subtrahend.isNegative() && this->isNegative())
	{
		result = addTwoLists(this->number,subtrahend.number,this->radix);
		isNeg = true;
	}
	else if(!subtrahend.isNegative() && !this->isNegative())
	{
		if(subtrahend <= *this)
		{
			result = subtractTwoLists(this->number,subtrahend.number,radix);
		}
		else
		{
			result = subtractTwoLists(subtrahend.number,this->number,radix);
			isNeg = true;
		}
	}
	else if(subtrahend.isNegative() && this->isNegative())
	{
		if(subtrahend < *this)
		{
			result = subtractTwoLists(this->number,subtrahend.number,radix);
			isNeg = true;
		}
		else
		{
			result = subtractTwoLists(subtrahend.number,this->number,radix);
		}
	}
	largeInt data(result,isNeg,this->radix);

	data.trim();
	return data;

}

bool largeInt::operator<(largeInt& o)
{
	bool isLessThan = false;
	o.trim();
	this->trim();
	if(number.getLength() < o.number.getLength())
	{
		isLessThan = true;
	}
	else if (number.getLength() == o.number.getLength())
	{
		iterator<short> tIt = number.begin();
		iterator<short> oIt = o.number.begin();
		bool checking = true;
		while(!isLessThan && checking && tIt != nullptr && oIt != nullptr)
		{
			if(*tIt < *oIt)
				isLessThan = true;
			else if(*tIt > *oIt)
				checking = false;
			*tIt++;
			*oIt++;
		}
	}
	return isLessThan;
}
bool largeInt::operator<=(largeInt& o)
{
	bool isLessThan = false;
	o.trim();
	this->trim();
	if(number.getLength() <= o.number.getLength())
	{
		isLessThan = true;
	}
	else if (number.getLength() == o.number.getLength())
	{
		iterator<short> tIt = number.begin();
		iterator<short> oIt = o.number.begin();
		bool checking = true;
		while(!isLessThan && checking && tIt != nullptr && oIt != nullptr)
		{
			if(*tIt < *oIt)
				isLessThan = true;
			else if(*tIt > *oIt)
				checking = false;
			*tIt++;
			*oIt++;
		}
	}
	if(!isLessThan)
	{
		isLessThan = this->number == o.number;
	}
	return isLessThan;
}
void largeInt::printNum()
{
	if(negative)
	{
		std::cout<<'-';
	}
	for(iterator<short> i = number.begin();i != nullptr;i++)
		if(*i>9)
		{
			std::cout<<char(*i+55);
		}
		else
		std::cout<<*i;
	if(number.isEmpty())
		std::cout<<0;
}

bool largeInt::operator==(largeInt& other)
{
	return (other.number == this->number && this->radix == other.radix);
}

void largeInt::operator=(const largeInt& other)
{
	this->negative = other.negative;
	this->number = other.number;
}
void largeInt::operator=(int other)
{
	number.emptyList();
	while(other != 0)
	{
		number.insertHead(other%10);
		other /= 10;
	}
}
largeInt::largeInt(int other)
{
	radix = 10;
	if(other < 0)
		negative = true;
	else
		negative = false;
	number.emptyList();
	while(other != 0)
	{
		number.insertHead(other%10);
		other /= 10;
	}
}

bool largeInt::operator!=(int other)
{
	iterator<short> it = number.rbegin();
	bool equal = true;
	while(other != 0 && equal)
	{
		if(other%10 != *it)
			equal = false;
		other /= 10;
		it--;
	}
	return equal;
}

largeInt::largeInt(unsortedList<short> num,bool isNegative,short radix):negative(isNegative)
{
	this->number = num;
	this->radix = radix;
}
largeInt::largeInt(std::string num):negative(false),radix(10)
{
	int len = num.length();
	char c;
	for(int i = 0;i < len;i++)
	{
		c = num[i];
		if(c == '-')
			negative = true;
		else if(c >= 48 && c <58)
			number.insertTail(c-48);
	}

}

void largeInt::insertHead(short d)
{
	number.insertHead(d);
}
void largeInt::insertTail(short d)
{
	number.insertTail(d);
}
