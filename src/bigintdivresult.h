/*
 * bigintdivresult.h
 *
 *  Created on: Oct 24, 2019
 *      Author: andrew
 */

#ifndef SRC_BIGINTDIVRESULT_H_
#define SRC_BIGINTDIVRESULT_H_

#include "unsortedlist.hpp"

struct bigDivResult{
	UnsortedList<short> quotientNum;
	bool quotientIsNeg;
	UnsortedList<short> remainderNum;
	bool remainderIsNeg;
	bigDivResult(
	UnsortedList<short> quotientNum,
	bool quotientIsNeg,
	UnsortedList<short> remainderNum,
	bool remainderIsNeg)
	:quotientNum(quotientNum),quotientIsNeg(quotientIsNeg),remainderNum(remainderNum),remainderIsNeg(remainderIsNeg)
	{

	}
};


#endif /* SRC_BIGINTDIVRESULT_H_ */
