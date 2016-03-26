#include <stdio.h>
#include "answerRegistry.h"

#define QuestionAnswer(x,y)  Answer::registerAnswer(1,x,y)

struct Cat
{
	Cat()
	:c0( 0x00 ), c1(0x11), c2(0x22)
	{}

	unsigned char c0;
	unsigned char c1;
	unsigned char c2;
};

struct Dog
{
	Dog()
	: d0(0xDD00), d1(0xDD11), d2(0xDD22)
	{}

	unsigned short d0;
	unsigned short d1;
	unsigned short d2;
};

struct Bird
{
	Bird()
	: b0(0xBB000000), b1(0xBB000011), b2(0xBB000022)
	{}

	unsigned int b0;
	unsigned int b1;
	unsigned int b2;
};

struct petStore
{
	Cat		morris;
	Dog		fido;
	Bird	poly;
};

enum ErrorCode
{
	Found,
	NotFound
};

ErrorCode UnsignedCharTraversal( unsigned char* const begin, const unsigned char* const end, int& out){
	unsigned char* temp = begin;
	int counter = 0;
	while (temp<end)
	{
		if (temp==end){
			out = counter;
			return ErrorCode::Found;
		}
		else
		{
			temp++;
			counter++;
		}
	}

	return ErrorCode::NotFound;
}

void Students_Casting()
{
	petStore		ps;
	unsigned int	*r;
	unsigned short	*s;
	unsigned char	*p;
	unsigned int   val;

	// Example:
	// Using only:  
	//             unsigned char *p
	//             initialize p with ps
	// Using only p, find the values: 
	//      question 0) morris.c1

	p = &ps.morris.c1;
	QuestionAnswer( 0, p[0] );

	// Now repeat the pattern above { use the QuestionAnswer( index, val ) macro } 

	// Using only:  
	//             unsigned int *r
	//             initialize r with ps
	// Using only r, find the values: 
	//      question 1)  b0 
	//      question 2)  b1
	//      question 3)  b2 

	r = &ps.poly.b0;

	QuestionAnswer(1, r[0]);
	QuestionAnswer(2, r[1]);
	QuestionAnswer(3, r[2]);

	// Using only:  
	//             unsigned short *s
	//             initialize s with ps
	// Using only s, find the values: 
	//      question 4)  d0 
	//      question 5)  d1
	//      question 6)  d2 

	s = &ps.fido.d0;

	QuestionAnswer(4, s[0]);
	QuestionAnswer(5, s[1]);
	QuestionAnswer(6, s[2]);
	
	// Using only:  
	//             unsigned char *p
	//             initialize p with ps
	// Using only p, find the values: 
	//      question 7)  c0 
	//      question 8)  c1
	//      question 9)  c2 
		
	p = &ps.morris.c0;

	QuestionAnswer(7, p[0]);
	QuestionAnswer(8, p[1]);
	QuestionAnswer(9, p[2]);


	// For the next set of questions
	//
	// You can cast and create temp variables to help you answer questions correctly.
	//          p <- the starting address of ps
	//          s <- the starting address of ps
	//          r <- the starting address of ps


	//      question 10)  addr of ps

	//using unsigned int because pointer is 4 bytes, therefore 4 bytes unsigned is needed.
	unsigned int* addr = (unsigned int *) &ps; // Get an unsigned int pointer that points to the first byte of ps
	unsigned int addrLoc = (unsigned int) addr;//convert the address location of addr to an unsigned int (giving the address as am unsigned int value)
	QuestionAnswer(10,addrLoc);
	
	/*
	Memory Layout of Petstore:
	
		char c0
		char c1
		char c2
		short d0
		short d1
		short d2
		int b0
		int b1
		int b2

		00 aa aa aa
		00 00 00 00
		00 00 00 00
		00 00 00 00
		00 00 00 00
		00 00 00 00
		size = 6*4 = 24 bytes
	
	*/

	//int result = 0;

	//      question 11)  number of unsigned chars to c1
	//      question 12)  number of unsigned chars to d0
	//      question 13)  number of unsigned chars to b2;
	
	unsigned char* from = (unsigned char*) addr;
	unsigned char* to = (unsigned char*) &ps.morris.c1;
	unsigned int result = to - from;
	QuestionAnswer(11, result);

	to = (unsigned char*)&ps.fido.d0;
	result = to - from;
	QuestionAnswer(12, result);
	
	to = (unsigned char*)&ps.poly.b2;
	result = to - from;
	QuestionAnswer(13, result);

	//      question 14)  number of unsigned shorts to the first c2
	//      question 15)  number of unsigned shorts to the first d2
	//      question 16)  number of unsigned shorts to b1;
	unsigned short* from2 = (unsigned short*)addr;

	unsigned short* to2 = (unsigned short*)&ps.morris.c2;
	result = (to2 - from2);
	QuestionAnswer(14, result);
	
	to2 = (unsigned short*)&ps.fido.d2;
	result = (to2 - from2);
	QuestionAnswer(15, result);

	to2 = (unsigned short*)&ps.poly.b1;
	result = (to2 - from2);
	QuestionAnswer(16, result);

	//      question 17)  number of unsigned ints to the first c0
	//      question 18)  number of unsigned ints to the first d0
	//      question 19)  number of unsigned ints to the first b0;

	unsigned int* from3 = (unsigned int*)addr;

	unsigned int* to3 = (unsigned int*)&ps.morris.c0;
	result = (to3 - from3);
	QuestionAnswer(17, result);

	to3 = (unsigned int*)&ps.fido.d0;
	result = (to3 - from3);
	QuestionAnswer(18, result);

	to3 = (unsigned int*)&ps.poly.b0;
	result = (to3 - from3);
	QuestionAnswer(19, result);


}