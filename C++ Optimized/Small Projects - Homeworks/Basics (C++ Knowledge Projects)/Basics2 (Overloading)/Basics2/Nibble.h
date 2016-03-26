#ifndef NIBBLE_H
#define NIBBLE_H
#include <iostream>
class Nibble
{
public:
	// insert your code here

	Nibble()
		:data(0)
	{
	}

	Nibble(char _data)
		:data(_data & 0x0f)
	{

	}

	Nibble(const Nibble& rhs)
		:data(rhs.data & 0x0f)//%0x10)
	{
	}

	Nibble& operator =(const Nibble& rhs)
	{
		if (this != &rhs){
			data = rhs.data;
		}
		return *this;
	}

	~Nibble(){
		//no dynamically allocated data... 
	}

	const Nibble operator +(const Nibble& rhs)
	{
		return Nibble(data + rhs.data);
	}

	const Nibble operator +(const int rhs)
	{
		return Nibble(data + (char)rhs);
	}
	
	friend const Nibble operator + (const int rhs,const Nibble& lhs)
	{
		return Nibble(lhs.data + (char)rhs);
	}

	Nibble& operator +=(const Nibble& rhs)
	{
		data = (data + rhs.data) & 0x0f;
		return *this;
	}

	Nibble operator ~(){
		Nibble temp((~data) & 0x0f);
			
		return temp;
	}

	Nibble operator +(){ //return positive..
		char temp = data;
		if (data & 0x8){ //if the last digit is a 1 (meaning negative number)
			temp = ~temp;
			temp++;
			temp = temp & 0x0f; //remove any outside bits (5 to 8th bit)
		}

		return Nibble(temp);
	}

	Nibble operator ++(){
		
		char future = data+1;
		data = future & 0x0f;
		return Nibble(future);
	}

	Nibble  operator++(int){
		char prev = data;
		data = (data + 1) & 0x0f;
		
		return Nibble(prev & 0x0f);
	}

	Nibble operator <<(int k){

		char tempData = data;
		char fallOff; // represents the first nibble of the char Data (the fall off as a result of shifting)
		tempData = tempData << k;
		fallOff = tempData & 0xf0;//remove everything but the falloff
		fallOff = fallOff >> 4;//move the falloff to the beginning of a nibble 
		tempData = tempData & 0x0f;//remove the falloff from the shifted data
		return Nibble(tempData | fallOff);//returns the shifted data with the fall off combined at the end.... rotational shift
	}

	operator unsigned int ()
	{
		return unsigned int(data+5);
	}
	unsigned char getData();

private:
	unsigned char data;
};



#endif