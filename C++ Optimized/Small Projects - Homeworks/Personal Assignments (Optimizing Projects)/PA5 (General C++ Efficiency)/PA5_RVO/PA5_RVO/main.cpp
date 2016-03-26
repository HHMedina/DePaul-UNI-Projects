// main.cpp : Defines the entry point for the console application.
//

void StressTest( );

#include "PA5_B.h"
#include "timer.h"
#include "output.h"

int main()
{

	// Create the IO for this program, that's WHERE YOU PUT YOUR NAME, please change it.
	io::create("Hector Medina");

	// Initialize timer
	timer::initTimer();

	// Create a timer objects
	timer t1;

	// start timer
	t1.tic();

	for (int i = 0; i < 5000000; i++)
	{
	   StressTest();
	}

	// stop timer
	t1.toc();

	// log the time for the implicit creation
    fprintf(io::getHandle(),"Debug Mode\n");
	fprintf(io::getHandle(),"Run without RVO: %f ms\n", t1.timeInSeconds()* 1000.0f );

	return(0);
}

