#include <assert.h>
#include "answerRegistry.h"



#define QuestionAnswer(x,y)  Answer::registerAnswer(0,x,y)

unsigned char data[] =
{ 
  0xAB, 0xCD, 0x12, 0x3F,
  0x33, 0xB5, 0xD3, 0x35,
  0x23, 0x24, 0x01, 0xFE,
  0xCD, 0x33, 0x44, 0x55,
  0x66, 0x03, 0x75, 0x33,
  0x29, 0x55, 0x22, 0x11,
  0x56, 0x88, 0xA9, 0x13,
  0x14, 0x82, 0x68, 0x26 
};


void Students_PointerWalk()
{
	unsigned char  *p;  // char are 8-bits wide
	unsigned int   *r;  // ints are 32-bits wide
	unsigned short *s;  // shorts are 16-bits wide

	unsigned int val;   // answer value

	// Sample Question 1
	// follow the pattern for the whole walking pointer test

	p = &data[0];       

	val = p[0];
	QuestionAnswer( 0 , val );

	// Now repeat for the pointer test
	// 0-23 question in total for this part

	//Q1
	val = p[3];
	QuestionAnswer(1, val);

	//Q2
	val = p[5];
	QuestionAnswer(2, val);

	//Q3
	p = p + 12;

	val = *(p);
	QuestionAnswer(3, val);

	//Q4
	val = p[2];
	QuestionAnswer(4, val);

	//Q5
	val = *p++;
	QuestionAnswer(5, val);

	//Q6
	p += 6;

	val = *--p;
	QuestionAnswer(6, val);

	//Q7
	val = p[5];
	QuestionAnswer(7, val);

	//Q8
	p = p + 2;

	val = *p++;
	QuestionAnswer(8, val);

	//Q9
	val = *(p+3);
	QuestionAnswer(9, val);

	//Q10
	p = 5 + p;

	val = *(p++);
	QuestionAnswer(10, val);

	//Q11
	val = *(--p);
	QuestionAnswer(11, val);

	//Q12
	r = (unsigned int*) &data[0];

	val = *(r);//should be 3F 12 CD AB
	QuestionAnswer(12, val);

	//Q13
	val = *(r+5);//should be 11 22 55 29
	QuestionAnswer(13, val);

	//Q14
	r++;

	val = *r++;//should be 35 D3 B5 33
	QuestionAnswer(14, val);

	
	//Q15
	r = r + 2;

	val = r[2];//should be 13 A9 88 56
	QuestionAnswer(15, val);

	//Q16
	r = r + 1;

	val = r[0];//should be 11 22 55 29
	QuestionAnswer(16, val);

	//Q17
	s = (unsigned short*) r;

	val = s[-2];//should be 03 66
	QuestionAnswer(17, val);

	//Q18
	s = s-3;

	val = s[2];//should be 33 75 or 13173 in decimal 
	QuestionAnswer(18, val);

	//Q19
	s += 5;

	val = *(s+3);//should be 26 68 
	QuestionAnswer(19, val);

	//Q20
	val = *(s);//should be 56 88
	QuestionAnswer(20, val);

	//Q21
	p = (unsigned char*) s;

	val = *(p+3);//should be 13
	QuestionAnswer(21, val);

	//Q22
	p += 5;

	val = p[-9];//should be 29
	QuestionAnswer(22, val);

	//Q22
	--p;
	
	val = p[0];//should be 14
	QuestionAnswer(23, val);

	// Make sure you look at the memory window 
	// and understand the test thoroughly

   

}


