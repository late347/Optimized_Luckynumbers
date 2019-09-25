// Optimized_Luckynumbers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono> 
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>

typedef unsigned long long ull;

namespace globals {
	static const size_t g_maxBits = 64;
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
// this function below actually just checks if 
// a single number num == luckynumber
size_t dumbLucky(size_t num) {
	static const size_t lucky4 = 4, lucky7 = 7;
	while (num > 0) {
		if (!(num % 10 == lucky4 || num % 10 == lucky7))
			return 0;
		num = num / 10;
	}
	return 1;
}



//simple helper func to calculate powers of 2
size_t pow2(size_t n) {
	size_t val = 1;
	val = ((size_t)1 << n);
	return val;
}


size_t getIntDigitCount(size_t n) {
	//note input should be between range [1, uintmax]
	//no input with zero
	size_t d = 0;
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


size_t refactored_getIntFromEncoding(const std::string& uInt32BitStr) {
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
	//auto res = strtoul(cleanedFromEmptyBits.c_str(), 0, 10);
	auto res0 = strtoull(cleanedFromEmptyBits.c_str(), 0, 10);
	//danger if overflows because of narrowingConversion from unsigned long into unsigned int, but 
	//inputs should be only 32bit unsigned numbers input string reference

	return res0;
}





bool getBitPosition(const size_t& number, const size_t& bitPos) {
	bool bit = 0 != (number & ((size_t)1 << bitPos));
	return bit;
}

std::string refactored_getEncodingFromInt(size_t number, size_t unavailableBits) {
	//encode the unsigned int into cppstring
	//where the unavailableBits are -, and other bits are of course 0 and 1
	//unavailableBits would be "forbidden bits" so that the other bits are "memorybits"
	//so to re-cap, memorybits are any bits that are allowedmemory from 32bit int
	//and unavailableBits are any bits that are unavailablememory from 32bit int

	//static const unsigned int maxBits = 32;
	std::string tempstr(globals::g_maxBits,'0');

	//put the binaryDigits into string form e.g. "100101"

	for (auto i = 0; i < globals::g_maxBits; i++) {
		auto temp1 = getBitPosition(number, i);
		if (temp1) {
			//tempstr.insert(0, "1");
			tempstr[globals::g_maxBits - (size_t)1 - ( size_t)i] = '1';
		} else {
			//tempstr.insert(0, "0");
			tempstr[globals::g_maxBits - ( size_t)1 - ( size_t)i] = '0';
		}
	}
	int debug88 = 88;//only for debugbreakpoints
	//declare emptyBits on the lefthand side of the mostSignificantBit =1
	//so that we only utilize as many bits as required.
	for (auto i = 0; i < unavailableBits; i++) {
		tempstr[i] = '-';
	}
	int debug9 = 9;//only for debugbreakpoints
	return tempstr;
}







size_t refactored_getLuckyNumbers3(size_t N) {

	//static const unsigned int maxBits = 32;
	std::string luckycandidate(globals::g_maxBits, '-');
	const size_t n_digitcount = getIntDigitCount(N);
	
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
	const size_t unavailableBits = globals::g_maxBits - n_digitcount;

	/*format a binary number with only k memorybits available
	where k = digitCountOfN, and initialize all available bits in memory to 0 value*/
	for (auto i = 0; i < globals::g_maxBits; i++) {
		if (i < unavailableBits) {
			luckycandidate[i] = '-'; //put a minus sign to represent unavailable storage bits (not being used by 0 and neither by 1)
		} else {
			luckycandidate[i] = '0';
		}
	}

	luckycandidate[globals::g_maxBits - 1] = '0';
	size_t luckyint = 0, permutationCounter = 0;
	size_t luckiesTotal = 0, loopcounter=0;
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
	for (auto i = 1; i < n_digitcount; i++) {
		luckiesTotal += pow2(i);
	}
	int debug67 = 67; //only for debugbreakpoints

	while (looping) {

		luckyint = refactored_getIntFromEncoding(luckycandidate); //get the intvalue of the enumerated luckynumber, from encoding
		++loopcounter;
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
	printf("algoloop ran ull% times\n",loopcounter);
	return luckiesTotal;

}





int main()
{
	size_t mediuminput = 999'999'987;
	size_t largeinput =  18446744073709551611;
	size_t N = largeinput;
    std::cout << "Hello World!\n";
	auto start = std::chrono::high_resolution_clock::now();
	auto foundLuckiesCount = refactored_getLuckyNumbers3(N);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "N=" << N << ", luckynumbers_count was: " << foundLuckiesCount << ", duration was us="<< duration.count() <<std::endl;
	



	// probably commenc out this dumbLucky calculation if you have large inputsizes N
	// it will be super laggy
	// bearable but laggy  choice for comparison for size of N might be N == 888'888'888
	/*unsigned int dumbLuckyCount = 0;
	 start = std::chrono::high_resolution_clock::now();
	for (unsigned int j = 1; j <= N; ++j) {
		dumbLuckyCount += dumbLucky(j);
	}
	 stop = std::chrono::high_resolution_clock::now();
	 duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "dumbLucky count was: " << dumbLuckyCount << ", duration was us=" << duration.count() << std::endl;
	*/



}
