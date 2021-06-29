/*
 * largint.cpp
 *
 *  Created on: Oct 17, 2019
 *      Author: andrew
 */

#include "largeint.h"

//Will run as long as divisor is less than 2^15
divResult LargeInt::divideList(UnsortedList<short> dividend,short divisor,short radix)
{
	UnsortedList<short> quotient;
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

bigDivResult LargeInt::divideTwoLargeInt(LargeInt divisor,LargeInt dividend,short radix)
{
	short originalThisRadix = radix;

	bool isNegLocal = false;

	if(divisor.isNegative())
		isNegLocal = !isNegLocal;
	if(dividend.isNegative())
		isNegLocal = !isNegLocal;

	LargeInt portionOfDividend("");
	portionOfDividend.setRadix(2);
	UnsortedList<short> result;
	int insertCount = 0;
	divisor.setRadix(2);
	dividend.setRadix(2);

	iterator<short> divIt = dividend.number.begin();
	while(!dividend.number.isEmpty())
	{
		//std::cout<<"Dividend = "<<dividend<<std::endl;
		//std::cout<<"Portion of Dividend = "<<portionOfDividend<<std::endl;
		//std::cout<<"Divisor = "<<divisor<<std::endl;
		while(divIt != nullptr && (portionOfDividend < divisor))
		{
			if(!portionOfDividend.isZero() || *divIt != 0)
				portionOfDividend.insertTail(*divIt);
			divIt++;
			dividend.number.removeHead();
			if(insertCount==0)
				result.insertTail(0);
			else
				insertCount--;

		}
		if(!portionOfDividend.isZero() && divisor <= portionOfDividend
		)
		{
			result.insertTail(1);
			portionOfDividend = ((portionOfDividend-divisor));

			portionOfDividend.trim();
			insertCount++;
		}
		else if(divIt == nullptr)
		{
			if (portionOfDividend < divisor || portionOfDividend.isZero())
			{
				result.insertTail(0);
			}
			dividend = LargeInt("");
		}
	}

	LargeInt intResult(result,isNegLocal,2);
	if(2 !=  originalThisRadix)
		intResult.setRadix(originalThisRadix);
	if(2 != radix)
		portionOfDividend.setRadix(originalThisRadix);
	return bigDivResult(intResult.number,isNegLocal,portionOfDividend.number,isNegLocal);
}
UnsortedList<short> LargeInt::addTwoLists(UnsortedList<short> &summand1,UnsortedList<short> &summand2,short radix)
{
	UnsortedList<short> endSum;
	iterator<short> other = summand2.rbegin();
	iterator<short> num = summand1.rbegin();
	short carry = 0;
	short sum = 0;
	while(other != nullptr && num != nullptr)
	{
		sum = (*other + *num + carry) % radix;
		carry = (*other + *num + carry) / radix;
		endSum.insertHead(sum);
		other--;
		num--;
	}
	while(other != nullptr)
	{
		endSum.insertHead((*other + carry) % radix);
		carry = (*other + carry) / radix;
		other--;
	}
	while(num != nullptr)
	{
		endSum.insertHead((*num + carry) % radix);
		carry = (*num + carry) / radix;
		num--;
	}
	if(carry)
		endSum.insertHead(1);
	return endSum;
}

UnsortedList<short> LargeInt::subtractTwoLists(UnsortedList<short> minuend,UnsortedList<short> subtrahend, short radix)
{
	iterator<short> oIt = subtrahend.rbegin();
	iterator<short> tIt = minuend.rbegin();
	iterator<short> walkerTIt = tIt;
	UnsortedList<short> result;
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

bool isEqualToZero(UnsortedList<short> number)
{
	bool isZero = true;
	for(iterator<short> i = number.begin();i != nullptr;i++)
	{
		if(*i != 0)
			isZero = false;
	}
	return isZero;
}
void LargeInt::trim()
{
	iterator<short> it = number.begin();
	while(it != nullptr && *it == 0)
	{
		it++;
		number.removeHead();
	}
}
LargeInt LargeInt::getBinVersion()
{
	divResult result(number,0);
	UnsortedList<short> binRep;
	while(!isEqualToZero(result.quotient))
	{
		result = divideList(result.quotient,2,radix);
		binRep.insertHead(result.remainder);
	}

	LargeInt binNum = LargeInt(binRep,this->negative,2);
	return binNum;
}
bool LargeInt::isZero()
{
	bool isZero = true;
	for(iterator<short> i = number.begin();i != nullptr;i++)
	{
		if(*i != 0)
			isZero = false;
	}
	return isZero;
}

void LargeInt::operator+=(LargeInt summand)
{
	bool isNeg = false;
	UnsortedList<short> result;
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
	LargeInt data(result,isNeg);
	data.trim();
	*this = data;
}

LargeInt LargeInt::operator+(LargeInt summand)
{
	bool isNeg = false;
	UnsortedList<short> result;
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
	LargeInt data(result,isNeg);
	data.trim();
	return data;
}

bool LargeInt::isNegative()
{
	return negative;
}
void LargeInt::operator*=(LargeInt multiplyer)
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
	UnsortedList<short> sumOfProducts;

	for(iterator<short>  oIt = multiplyer.number.rbegin();oIt != nullptr;oIt--)
	{
		short oNum = *oIt;
		count++;
		UnsortedList<short> intProduct;
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
LargeInt LargeInt::operator*(LargeInt multiplyer)
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
	UnsortedList<short> sumOfProducts;

	for(iterator<short>  oIt = multiplyer.number.rbegin();oIt != multiplyer.number.rend();oIt--)
	{
		short oNum = *oIt;
		count++;
		UnsortedList<short> intProduct;
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
	return LargeInt(sumOfProducts,isNeg,radix);
}

LargeInt LargeInt::operator/(LargeInt divisor)
{
	bigDivResult result = divideTwoLargeInt(divisor,*this,this->radix);

	return LargeInt(result.quotientNum,result.quotientIsNeg,this->radix);
}

LargeInt LargeInt::operator/=(LargeInt divisor)
{
	bigDivResult result = divideTwoLargeInt(divisor,*this,this->radix);
	*this = LargeInt(result.quotientNum,result.quotientIsNeg,this->radix);
	return *this;
}

LargeInt LargeInt::operator%(LargeInt divisor)
{
	bigDivResult result = divideTwoLargeInt(divisor,*this,this->radix);

	return LargeInt(result.remainderNum,result.remainderIsNeg,this->radix);
}
LargeInt LargeInt::operator%=(LargeInt divisor)
{
	bigDivResult result = divideTwoLargeInt(divisor,*this,this->radix);
	*this = LargeInt(result.remainderNum,result.remainderIsNeg,this->radix);
	return *this;
}

void LargeInt::operator-=(LargeInt subtrahend)
{
	bool isNeg = false;
		UnsortedList<short> result;
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
		LargeInt data(result,isNeg);
		data.trim();
		*this = data;
}
LargeInt LargeInt::operator-(LargeInt subtrahend)
{
	bool isNeg = false;
	UnsortedList<short> result;
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
	LargeInt data(result,isNeg,this->radix);

	data.trim();
	return data;

}

bool LargeInt::operator<(LargeInt& o)
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

LargeInt LargeInt::operator^(LargeInt power)
{
		LargeInt dat("1");
		LargeInt powerBin = power.getBinVersion();
		iterator<short> powBinIt = powerBin.number.begin();

		if(*powBinIt == 1)
		{
		 	dat = *this;
		}
		powBinIt++;
		while(powBinIt != nullptr)
		{
			if(*powBinIt == 0)
			{
				dat *= dat;
			}
			else
			{
				dat = dat * dat * *this;
			}
			powBinIt++;
		}
		return dat;
}

bool LargeInt::operator>(LargeInt& o)
{
	bool isGreaterThan = false;
	o.trim();
	this->trim();
	if(number.getLength() > o.number.getLength())
	{
		isGreaterThan = true;
	}
	else if (number.getLength() == o.number.getLength())
	{
		iterator<short> tIt = number.begin();
		iterator<short> oIt = o.number.begin();
		bool checking = true;
		while(!isGreaterThan && checking && tIt != nullptr && oIt != nullptr)
		{
			if(*tIt > *oIt)
				isGreaterThan = true;
			else if(*tIt < *oIt)
				checking = false;
			*tIt++;
			*oIt++;
		}
	}
	return isGreaterThan;
}
bool LargeInt::operator>=(LargeInt& o)
{
	bool isGreaterThan = false;
	o.trim();
	this->trim();
	if(number.getLength() > o.number.getLength())
	{
		isGreaterThan = true;
	}
	else if (number.getLength() == o.number.getLength())
	{
		iterator<short> tIt = number.begin();
		iterator<short> oIt = o.number.begin();
		bool checking = true;
		while(!isGreaterThan && checking && tIt != nullptr && oIt != nullptr)
		{
			if(*tIt > *oIt)
				isGreaterThan = true;
			else if(*tIt < *oIt)
				checking = false;
			*tIt++;
			*oIt++;
		}
	}
	if(!isGreaterThan)
	{
		isGreaterThan = this->number == o.number;
	}
	return isGreaterThan;
}
bool LargeInt::operator<=(LargeInt& o)
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
	if(!isLessThan)
	{
		isLessThan = this->number == o.number;
	}
	return isLessThan;
}

std::istream& operator>>(std::istream &i,LargeInt &l)
{
	std::string data;
	i>>data;
	l = data;
	return i;
}
std::ostream& operator<<(std::ostream &o,LargeInt &l)
	{
	std::stack<char> num;
	long count = 0;
	for(iterator<short> i = l.number.rbegin();i != nullptr;i--)
	{
		if(count % 3 == 0 && count != 0)
		{
			num.push(',');
		}
		num.push(*i<10?*i+48:*i+55);
		count++;
	}
		if(l.negative)
		{
			o<<'-';
		}
		char n;
		while(!num.empty())
		{
			n = num.top();
			num.pop();
			o<<n;
		}
		if(l.number.isEmpty())
			o<<0;

		return o;
	}

void LargeInt::setRadix(short newRadix)
{

	divResult result(number,0);
	UnsortedList<short> convertedNum;
	while(!isEqualToZero(result.quotient))
	{
		result = divideList(result.quotient,newRadix,this->radix);
		convertedNum.insertHead(result.remainder);
	}

	this->number = convertedNum;
	this->radix = newRadix;

}
void LargeInt::printNum()
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

bool LargeInt::operator==(LargeInt& other)
{
	return (other.number == this->number && this->radix == other.radix);
}

void LargeInt::operator=(const LargeInt& other)
{
	this->negative = other.negative;
	this->number = other.number;
}
void LargeInt::operator=(int other)
{
	number.emptyList();
	if(other < 0)
	{
		this->negative = true;
		other *= -1;
	}
	while(other != 0)
	{
		number.insertHead(other%10);
		other /= 10;
	}
}
LargeInt::LargeInt(int other)
{
	radix = 10;
	if(other < 0)
	{
		this->negative = true;
		other *= -1;
	}
	else
		negative = false;
	number.emptyList();
	while(other != 0)
	{
		number.insertHead(other%10);
		other /= 10;
	}
}

bool LargeInt::operator!=(int other)
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

LargeInt::LargeInt(UnsortedList<short> num,bool isNegative,short radix):negative(isNegative)
{
	this->number = num;
	this->radix = radix;
}
LargeInt::LargeInt(std::string num):negative(false),radix(10)
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

void LargeInt::insertHead(short d)
{
	number.insertHead(d);
}
void LargeInt::insertTail(short d)
{
	number.insertTail(d);
}
long LargeInt::toLong()
{
	long multiplier = 1;
	long sum = 0;
	LargeInt one(1);
	for(iterator<short> i = number.rbegin();i != nullptr; i--)
	{
		sum += multiplier* *i;
		multiplier *= 10;
	}
	return sum;
}
