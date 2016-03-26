#include <assert.h>
/*
constantsArr = array holding the constansts from A_0 to A_N
xVal = X Value
nTerms = The number of terms to sum. For the assignment nterms == 6 will give
		y = A_0 + A_1X + A_2X^2 + .... + A_5X^5.
*/
float CalculateEquation(const int* constantsArr, const int nTerms, const float xVal){
	
	assert(constantsArr != 0 && "Null was passed in via constantsArr parameter of CalculateEquation(const int* constantsArr, const int arrLength, const float xVal).");

	float counter = 1; // each step of the formula is A*x^n where the counter is the value of X^n on the nth step
	float sum = 0; //holds the sum

	for (int i = 0; i<nTerms; i++){//using hard coded 6 because formula is only for 6 terms
		sum += constantsArr[i] * counter;//for each term multiply x^n with the corresponding constant
		counter *= xVal;//multiply the current x^n by x to get x^n+1... the next value
	}

	return sum;
}