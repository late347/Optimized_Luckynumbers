// Optimized_Luckynumbers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono> 
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>


namespace globals {
	static const unsigned int g_maxBits = 32;
}


//get luckynumbers from 1,2,3...N interval inclusive
/*count the amount of luckynumbers in interval [1,N]
where 1<=N<=1 billion
luckynumber is any number which consists of only the digits 4 or 7.
e.g. 44,444, 474, 774, 747, 7777...
*/

//the clear way to calculate the luckynumbers
//iterate thru all of them
// timecomplexity O(N), laggy with huge numbers
unsigned int dumbLucky(unsigned int num) {
	static const unsigned int lucky4 = 4, lucky7 = 7;
	while (num > 0) {
		if (!(num % 10 == lucky4 || num % 10 == lucky7))
			return 0;
		num = num / 10;
	}
	return 1;
}



//simple helper func to calculate powers of 2
unsigned int pow2(unsigned int n) {
	unsigned int val = 1;
	val = (1 << n);
	return val;
}


unsigned int getIntDigitCount(unsigned int n) {
	//note input should be between range [1, uintmax]
	//no input with zero
	unsigned int d = 0;
	if (n == 0) return 1;
	//basically O(1) here
	while (n > 0) {
		n /= 10;
		++d;
	}
	return d;
}

bool checkAllPermutationsFinished(const std::string& binstr) {
	for (auto& c : binstr) {
		if (c == '0') {
			return false;
		}
	}
	 return true; //if all the bits are 1's, then that's the break condition, 
	 //when all the permutations have been enumerated in that amount  of bits
	 //available in memory for that integer n
}

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

unsigned int refactored_getIntFromEncoding(const std::string& uInt32BitStr) {
	/*loop the string to get the chars
	if the char is 0 => it will become 4
	if the char is 1 => it will become 7
	use libraryfunction to get the decimalvalue and return it
	*/
	//static const unsigned int maxBits = 32;
	std::string tempcopy(uInt32BitStr); //copy is sadly needed becuz data will be modified here in the copy

	for (auto i = 0; i < globals::g_maxBits; i++) {
		if (tempcopy[i] == '0') {
			tempcopy[i] = '4';

		} else if (tempcopy[i] == '1') {
			tempcopy[i] = '7';
		}
	}

	//slightly refactored here to that cleanedFromEmptyBits is known size
	//and everything at zero
	std::string cleanedFromEmptyBits(globals::g_maxBits,'0');

	for (auto i = 0; i < globals::g_maxBits; i++) {
		if (tempcopy[i] != '-') {
			cleanedFromEmptyBits[i] = tempcopy[i];
		}
	}
	auto res = strtoul(cleanedFromEmptyBits.c_str(), 0, 10);
	//danger if overflows because of narrowingConversion from unsigned long into unsigned int, but 
	//inputs should be only 32bit unsigned numbers input string reference

	unsigned int returnableres = static_cast<unsigned int>(res);
	return returnableres;
}


//NOTE this is old un-used func
//unsigned int getCustomDigitCount(const std::string& binaryUint32) {
//	/*calculate how many real digits exists in the binaryUint32
//	discounting emptybits '-'
//	counting '0'
//	counting '1'
//
//	if digitCount returned into the caller, is one more than than the original N digitcount
//	Then we stop looping
//	*/
//	static const int maxBits = 32;
//	unsigned int digitcount = 0;
//	for (size_t i = 0; i < maxBits; i++) {
//		if (binaryUint32[i] == '0' || binaryUint32[i]=='1') {
//			++digitcount;
//		}
//	}
//	return digitcount;
//}

std::string getEncodingFromInt(unsigned int uInt32Bit, unsigned int unavailableBits) {
	//encode the unsigned int into cppstring
	//where the unavailableBits are -, and other bits are of course 0 and 1
	//unavailableBits would be "forbidden bits" so that the other bits are "memorybits"
	//so to re-cap, memorybits are any bits that are allowedmemory from 32bit int
	//and unavailableBits are any bits that are unavailablememory from 32bit int
	
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
	int debug0 = 88;//only for debugbreakpoints
	//declare emptyBits on the lefthand side of the mostSignificantBit =1
	//so that we only utilize as many bits as required.
	for (size_t i = 0; i < unavailableBits; i++) {
		tempstr[i] = '-';
	}
	int debug2 = 9;//only for debugbreakpoints
	return tempstr;	
}


bool getBitPosition(const unsigned int& u32Integer, const unsigned int& bitPos) {
	bool bit = 0 != (u32Integer & (1 << bitPos));
	return bit;
}

std::string refactored_getEncodingFromInt(unsigned int uInt32Bit, unsigned int unavailableBits) {
	//encode the unsigned int into cppstring
	//where the unavailableBits are -, and other bits are of course 0 and 1
	//unavailableBits would be "forbidden bits" so that the other bits are "memorybits"
	//so to re-cap, memorybits are any bits that are allowedmemory from 32bit int
	//and unavailableBits are any bits that are unavailablememory from 32bit int

	//static const unsigned int maxBits = 32;
	std::string tempstr(globals::g_maxBits,'0');

	//put the binaryDigits into string form e.g. "100101"

	for (auto i = 0; i < globals::g_maxBits; i++) {
		auto temp1 = getBitPosition(uInt32Bit, i);
		if (temp1) {
			//tempstr.insert(0, "1");
			tempstr[globals::g_maxBits - 1 - i] = '1';
		} else {
			//tempstr.insert(0, "0");
			tempstr[globals::g_maxBits - 1 - i] = '0';
		}
	}
	int debug88 = 88;//only for debugbreakpoints
	//declare emptyBits on the lefthand side of the mostSignificantBit =1
	//so that we only utilize as many bits as required.
	for (unsigned int i = 0; i < unavailableBits; i++) {
		tempstr[i] = '-';
	}
	int debug9 = 9;//only for debugbreakpoints
	return tempstr;
}



//NOTE THIS FUNC IS BUGGED
//unsigned int getLuckyNumbers2(unsigned int N) {
//	/*put the luckydigits into INCREASING ORDER like 1,2,3...
//	that is a requirement for this algorithm to work in the first place*/
//	static const size_t maxBits = 32;
//	static const std::vector< unsigned int> luckydigits { 4, 7 }; 
//	std::string luckycandidate;
//	
//	
//	for (size_t i = 0; i < maxBits; i++) { //32bits of empty space into the string to initialize luckycandidate
//		luckycandidate += '-'; //put a minus sign to represent empty space (not being used by 0 and neither by 1)
//	}
//	luckycandidate[maxBits - 1] = '0';
//
//	/*because there are two luckydigits only, then we can represent the decimaldigits
//	4 and 7 (the real lucky digits) as binarydigits (bits) instead
//	
//	This allows us to increment loop the luckycandidate variable. Luckycandidate's own 
//	binarydigits represent the 4 and the 7, (0, and 1 respectively, also in increasing order)
//
//	Then, we can get all the binarydigits(bits) and transform the bits back into the decimaldigits
//	Then, when we have the decimaldigits stored e.g. into an int array, or vector of ints, we can compare if 
//	the decimalcandidate_number is too large for the N comparison value
//		=>if too large, stop loop and return the countedLuckies (countedLuckies is how many luckies we have found)
//		=>if smaller or same, continue and increment countedLuckies
//
//	This way has the benefit that it keeps the permutations of the luckynumbers in increasing order of size naturally
//	due to the way that binarydigits, binary addition works. 01 => 11 => 100 etc.
//	The code could theoretically be modified to work with other number bases such as base3,base4,base5 ... base10
//	but then you would need to store the baseN_representation in somehow different way than to simply 
//	rely on the binarydigits(bits).
//	
//	For bigger numberbases than base2, you maybe need to make a numberclass, that works like a digitalclock
//	where there are digits for each place of the clocknumbers like seconds, tensOfSeconds etc...
//	except that in this case the "digitalclock digits" are simply the allowed digits of the allowed particular 
//	numberbase of the baseN_number*/
//
//	unsigned int luckyint = 0, luckytemp=0,counter = 0;
//	unsigned int luckiesTotal = 0, customDigitCount=1;
//	unsigned int n_digitcount = getIntDigitCount(N);
//	const unsigned int unavailableBits = maxBits - n_digitcount;
//
//	bool looping = true;
//	//customDigitCount <= n_digitcount && checkAllPermutationsFinished(luckycandidate)
//	while ( looping  ){
//		luckyint = getIntFromEncoding(luckycandidate); //get the intvalue of the enumerated luckynumber, from encoding
//		customDigitCount = getIntDigitCount(luckyint);
//		if (luckyint <= N) {
//			luckiesTotal++;
//		} 
//		
//		if (customDigitCount > n_digitcount && checkAllPermutationsFinished(luckycandidate)) {
//			looping=false;
//		}
//
//		counter++; //increment intCounter, from which we get the next enumerated luckynumber
//		luckycandidate = getEncodingFromInt(counter, unavailableBits); //store the intCounter encoded format into luckycandidate
//		int debug5 = 5;
//	}
//
//
//
//	return luckiesTotal;
//}


//NOTE this is older versions of the algorithm but it still works, pretty much
//it was later refactored a bit into nicer and cleaner looking code
unsigned int getLuckyNumbers3(unsigned int N) {

	static const size_t maxBits = 32;
	static const std::vector< unsigned int> luckydigits{ 4, 7 };
	std::string luckycandidate;

	unsigned int n_digitcount = getIntDigitCount(N);
	const unsigned int unavailableBits = maxBits - n_digitcount; 

	/*format a binary number with only k memorybits available
	where k = digitCountOfN, and initialize all available bits in memory to 0 value*/
	for (size_t i = 0; i < maxBits; i++) { 
		if (i < unavailableBits) {
			luckycandidate += '-'; //put a minus sign to represent unavailable storage bits (not being used by 0 and neither by 1)
		} else {
			luckycandidate += '0';
		}
	}

	luckycandidate[maxBits - 1] = '0';
	unsigned int luckyint = 0, luckytemp = 0, counter = 0;
	unsigned int luckiesTotal = 0, luckyDigitCount = 1;
	bool looping = true;

	/*count the actual luckynumbers in 1digit, 2digit 3digit numbers
	where digit is smaller than the actual digitcount of N, let's imagine
	that N would have digitcount=4 at this point*/
	for (unsigned int i = 1; i < n_digitcount; i++) {
		//luckiesTotal += round(pow(2,i));
		luckiesTotal += pow2(i);
	}
	int debug67 = 67; //only for debugbreakpoints

	while (looping) {

		luckyint = getIntFromEncoding(luckycandidate); //get the intvalue of the enumerated luckynumber, from encoding
		luckyDigitCount = getIntDigitCount(luckyint);

		if (luckyint <= N) {
			luckiesTotal++;
		}

		if (checkAllPermutationsFinished(luckycandidate)) {
			looping = false;
		}

		counter++; //increment intCounter, from which we get the next enumerated luckynumber
		luckycandidate = getEncodingFromInt(counter, unavailableBits); //store the intCounter into encoded format in luckycandidate
		int debug5 = 5;//only for debugbreakpoints

	}



	return luckiesTotal;

}



unsigned int refactored_getLuckyNumbers3(unsigned int N) {

	//static const unsigned int maxBits = 32;
	std::string luckycandidate(globals::g_maxBits, '-');
	const unsigned int n_digitcount = getIntDigitCount(N);
	
	// allow only n_digitcount amount of bits to be used in the
	// luckycandidate to represent the bits an unsignedinteger (inside the cppstring)
	// the rest of the 32bits are padded with '-' characters such as if 
	// n_digitcount == 7, we would have 7 bits of memory available and using zero padding
	// in the form of -------------------------0000000
	// the padding with '-' chars makes it easier to extract the permutations and their integer values,
	// according to an encoding/decoding scheme such that 0->4, and 1->7, so that we are able to enumerate all
	// the permutations using only the permutationcounter in binary to increment and enumerate the permutations
	// then we store the permutationcounter value into cppstring 
	// the first enumerated permutations would be 4444444, 4444474, 4444477 etc...
	const unsigned int unavailableBits = globals::g_maxBits - n_digitcount;

	/*format a binary number with only k memorybits available
	where k = digitCountOfN, and initialize all available bits in memory to 0 value*/
	for (unsigned int i = 0; i < globals::g_maxBits; i++) {
		if (i < unavailableBits) {
			luckycandidate[i] = '-'; //put a minus sign to represent unavailable storage bits (not being used by 0 and neither by 1)
		} else {
			luckycandidate[i] = '0';
		}
	}

	luckycandidate[globals::g_maxBits - 1] = '0';
	unsigned int luckyint = 0, permutationCounter = 0;
	unsigned int luckiesTotal = 0;
	bool looping = true;

	/*count the actual luckynumbers in 1digit, 2digit 3digit etc... numbers
	where we count until including the (n_digitcount -1) amount of
	smaller digit numbers
	counting is done similarly if you imagine that luckynumbers are basically
	like passwords (permutation with repetition) so that you only have
	two choices available for each character, either 4 or 7, so that this means that
	in a three character password (3digit luckynumber) there are actually 2*2*2 luckynumbers
	in the 3digit range of luckynumbers (this helps, if we know for sure that n_digitcount is a
	4digit number or larger digit number itself)

	all 3digit numbers are smaller than 4digit numbers
	all 2digit numbers are smaller than 3digit numbers etc...
	keep the process going with this style of combinatorics counting.
	*/
	for (unsigned int i = 1; i < n_digitcount; i++) {
		luckiesTotal += pow2(i);
	}
	int debug67 = 67; //only for debugbreakpoints

	while (looping) {

		luckyint = refactored_getIntFromEncoding(luckycandidate); //get the intvalue of the enumerated luckynumber, from encoding

		if (luckyint <= N) {
			++luckiesTotal;
		}

		if (checkAllPermutationsFinished(luckycandidate)) {
			looping = false;
		}

		++permutationCounter; //increment intCounter, from which we get the next enumerated luckynumber
		luckycandidate = refactored_getEncodingFromInt(permutationCounter, unavailableBits); //store the intCounter into encoded format in luckycandidate
		int debug5 = 5;//only for debugbreakpoints

	}

	return luckiesTotal;

}





int main()
{
	unsigned int N = 1000'000'000-1;
    std::cout << "Hello World!\n";
	auto start = std::chrono::high_resolution_clock::now();
	auto foundLuckiesCount = refactored_getLuckyNumbers3(N);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "N=" << N << ", luckynumbers_count was: " << foundLuckiesCount << ", duration was us="<< duration.count() <<std::endl;
	

	unsigned int dumbLuckyCount = 0;
	 start = std::chrono::high_resolution_clock::now();
	for (unsigned int j = 1; j <= N; ++j) {
		dumbLuckyCount += dumbLucky(j);
	}
	// After function call 
	 stop = std::chrono::high_resolution_clock::now();
	// Subtract stop and start timepoints and 
	// cast it to required unit. Predefined units 
	// are nanoseconds, microseconds, milliseconds, 
	// seconds, minutes, hours. Use duration_cast() 
	// function. 
	 duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	



	std::cout << "dumbLucky count was: " << dumbLuckyCount << ", duration was us=" << duration.count() << std::endl;




}
