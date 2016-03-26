#include <stdio.h>
#include <cstdarg>
// Add functions and includes as needed


int SecondMax(int count, ...)
{
	// Do your magic here:
	count;
	va_list list;
	va_start(list,count);

	if (count>1){ 

		int greatest = va_arg(list, int);
		int secondGreatest = va_arg(list, int);
		if (greatest<secondGreatest)//check if swap needed
		{
			int temp = secondGreatest;
			secondGreatest = greatest;
			greatest = temp;
		}

		for (int i = 0; i<count - 2;i++)//check for other greatestmax and second greatest max
		{
			int contender = va_arg(list, int);
			
			if (contender>greatest)
			{
				secondGreatest = greatest;
				greatest = contender;
			} 
			else if (contender>secondGreatest && contender!=greatest)
			{
				secondGreatest = contender;
			}
		}

		return secondGreatest;
	} 
	else if (count == 1)
	{
		return va_arg(list, int); //return the only param
	} else
	{
		return 0;
	}

	
}