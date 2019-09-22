// Optimized_Luckynumbers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono> 
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>


//get luckynumbers from 1,2,3...N interval inclusive
/*count the amount of luckynumbers in interval [1,N]
where 1<=N<=1 billion
luckynumber is any number which consists of only the digits 4 or 7.
e.g. 44,444, 474, 774, 747, 7777...
*/

unsigned int getIntFromEncoding(const std::string& uInt32BitStr) {
	/*loop the string to get the chars
	if the char is 0 => it will become 4
	if the char is 1 => it will become 7
	use libraryfunction to get the decimalvalue and return it
	*/
	static const size_t maxBits = 32;

	std::string tempcopy(uInt32BitStr);
	for (size_t i = 0; i < maxBits; i++) {
		if (tempcopy[i] == '0') {
			tempcopy[i] = '4';

		} else if (tempcopy[i]=='1') {
			tempcopy[i] = '7';
		}
	}
	//empty bits are those that arent used for either 0 or 1 on the mostSignificantSide of the uint32 bit number
	std::string cleanedFromEmptyBits; 
	for (size_t i = 0; i < maxBits; i++) {
		if (tempcopy[i] != '-') {
			cleanedFromEmptyBits += tempcopy[i];
		}
	}
	auto res = strtoul(cleanedFromEmptyBits.c_str(), 0, 10);
	//danger if overflows because of narrowingConversion from unsigned long into unsigned int, but 
	//inputs should be only 32bit unsigned numbers input string reference
	
	unsigned int returnableres =  static_cast<unsigned int>(res);
	return returnableres;
}

unsigned int getCustomDigitCount(const std::string& binaryUint32) {
	/*calculate how many real digits exists in the binaryUint32
	discounting emptybits '-'
	counting '0'
	counting '1'

	if digitCount returned into the caller, is one more than than the original N digitcount
	Then we stop looping
	*/
	static const int maxBits = 32;
	unsigned int digitcount = 0;
	for (size_t i = 0; i < maxBits; i++) {
		if (binaryUint32[i] == '0' || binaryUint32[i]=='1') {
			++digitcount;
		}
	}
	return digitcount;
}

std::string getEncodingFromInt(unsigned int uInt32Bit) {
	//encode the unsigned int into cppstring
	//where the emptybits are -, and other bits are of course 0 and 1
	//emptybits would be unnecessary bits of 32bit integer that aren't being used to represent the number, on the
	//left hand side of the mostSignificantBit
	
	static const size_t maxBits = 32;
	std::string tempstr;

	//put the binaryDigits into string form e.g. "100101"

	for (size_t i = 0; i < maxBits; i++) {
		auto temp1 = 0 != (uInt32Bit & (1 << i));
		if (temp1) {
			tempstr.insert(0, "1");
		} else {
			tempstr.insert(0, "0");
		}
	}
	
	//declare emptyBits on the lefthand side of the mostSignificantBit =1
	//so that we only utilize as many bits as required.
	for (size_t i = 0; tempstr[i] != '1'; i++) {
		tempstr[i] = '-';
	}
	int debug2 = 9;
	return tempstr;	
}

unsigned int getLuckyNumbers2(unsigned int N) {
	/*put the luckydigits into INCREASING ORDER like 1,2,3...
	that is a requirement for this algorithm to work in the first place*/
	static const size_t maxBits = 32;
	static const std::vector< unsigned int> luckydigits { 4, 7 }; 
	std::string luckycandidate;
	for (size_t i = 0; i < maxBits; i++) { //32bits of empty space into the string to initialize luckycandidate
		luckycandidate += '-'; //put a minus sign to represent empty space (not being used by 0 and neither by 1)
	}
	luckycandidate[maxBits - 1] = '0';

	/*because there are two luckydigits only, then we can represent the decimaldigits
	4 and 7 (the real lucky digits) as binarydigits (bits) instead
	
	This allows us to increment loop the luckycandidate variable. Luckycandidate's own 
	binarydigits represent the 4 and the 7, (0, and 1 respectively, also in increasing order)

	Then, we can get all the binarydigits(bits) and transform the bits back into the decimaldigits
	Then, when we have the decimaldigits stored e.g. into an int array, or vector of ints, we can compare if 
	the decimalcandidate_number is too large for the N comparison value
		=>if too large, stop loop and return the countedLuckies (countedLuckies is how many luckies we have found)
		=>if smaller or same, continue and increment countedLuckies

	This way has the benefit that it keeps the permutations of the luckynumbers in increasing order of size naturally
	due to the way that binarydigits, binary addition works. 01 => 11 => 100 etc.
	The code could theoretically be modified to work with other number bases such as base3,base4,base5 ... base10
	but then you would need to store the baseN_representation in somehow different way than to simply 
	rely on the binarydigits(bits).
	
	For bigger numberbases than base2, you maybe need to make a numberclass, that works like a digitalclock
	where there are digits for each place of the clocknumbers like seconds, tensOfSeconds etc...
	except that in this case the "digitalclock digits" are simply the allowed digits of the allowed particular 
	numberbase of the baseN_number*/

	unsigned int luckyint = 0, luckytemp=0,counter = 0;
	unsigned int luckiesTotal = 0;
	while (1){
		luckyint = getIntFromEncoding(luckycandidate); //get the intvalue of the enumerated luckynumber, from encoding
		if (luckyint < N) {
			counter++; //increment intCounter, from which we get the next enumerated luckynumber
			luckiesTotal++;
			luckycandidate = getEncodingFromInt(counter); //store the intCounter encoded format into luckycandidate
		} else {
			break;
		}
	}
	return luckiesTotal;
}





int main()
{
    std::cout << "Hello World!\n";
	auto luckycount = getLuckyNumbers2(10000);
	int debug3 = 5;
	


}
