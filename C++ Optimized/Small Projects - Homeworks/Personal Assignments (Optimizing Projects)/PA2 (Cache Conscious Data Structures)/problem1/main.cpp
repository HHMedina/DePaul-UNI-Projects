#include <iostream>
#include <string>
#include "TestDataStruct.h"

void printME(void* ptr, int sizeIn, const char* const name,FILE*& outFile){


	int i = 0;
	int numPads = 0;
	int columnCounter = 0;
	unsigned char* reader = (unsigned char*)ptr;
	fprintf(outFile,"Information for (%s)...\n\n\tMemory Layout (where 0xaa is padding):\n\n\t%#x: ", name,reader);
	while (i<sizeIn){//still bytes left to print

		if (reader[i] == 0xaa){
			numPads++;
		}

		//print the byte
		fprintf(outFile, "%02x ", reader[i]);
		i++;

		columnCounter++;
		if (columnCounter>3 && i<sizeIn){
			fprintf(outFile, "\n\t%#x: ",reader+i);
			columnCounter = 0;
		}
	}

	fprintf(outFile, "\n\n\tSize: %i bytes.\tNumber of Padding: %i", sizeIn, numPads);
	fprintf(outFile, "\n\n");

}


int main(){

	FILE * outFile;
	fopen_s(&outFile, "Output.txt", "w");// openfile

	char* a = new char[sizeof(Test_A)];
	memset(a, 0xaa, sizeof(Test_A));
	Test_A* input = new(a)Test_A;
	printME(input, sizeof(Test_A), "Test_A",outFile);
	delete input;

	char* b = new char[sizeof(Test_B)];
	memset(b, 0xaa, sizeof(Test_B));
	Test_B* input2 = new(b)Test_B;
	printME(input2, sizeof(Test_B), "Test_B",outFile);
	delete input2;

	char* c = new char[sizeof(Test_C)];
	memset(c, 0xaa, sizeof(Test_C));
	Test_C* input3 = new(c)Test_C;
	printME(input3, sizeof(Test_C), "Test_C",outFile);
	delete input3;

	char* d = new char[sizeof(Test_D)];
	memset(d, 0xaa, sizeof(Test_D));
	Test_D* input4 = new(d)Test_D;
	printME(input4, sizeof(Test_D), "Test_D",outFile);
	delete input4;

	char* e = new char[sizeof(Test_E)];
	memset(e, 0xaa, sizeof(Test_E));
	Test_E* input5 = new(e)Test_E;
	printME(input5, sizeof(Test_E), "Test_E",outFile);
	delete input5;

	char* f = new char[sizeof(Test_F)];
	memset(f, 0xaa, sizeof(Test_F));
	Test_F* inputf = new(f)Test_F;
	printME(inputf, sizeof(Test_F), "Test_F",outFile);
	delete inputf;

	char* g = new char[sizeof(Test_G)];
	memset(g, 0xaa, sizeof(Test_G));
	Test_G* inputg = new(g)Test_G;
	printME(inputg, sizeof(Test_G), "Test_G",outFile);
	delete inputg;

	char* vectP = new char[sizeof(vect)];
	memset(vectP, 0xaa, sizeof(vect));
	vect* inputVect = new(vectP)vect;
	printME(inputVect, sizeof(vect), "vect",outFile);
	delete inputVect;

	char* h = new char[sizeof(Test_H)];
	memset(h, 0xaa, sizeof(Test_H));
	Test_H* inputH = new(h)Test_H;
	printME(inputH, sizeof(Test_H), "Test_H",outFile);
	delete inputH;

	char* i = new char[sizeof(Test_I)];
	memset(i, 0xaa, sizeof(Test_I));
	Test_I* inputI = new(i)Test_I;
	printME(inputI, sizeof(Test_I), "Test_I",outFile);
	delete inputI;

	char* j = new char[sizeof(Test_J)];
	memset(j, 0xaa, sizeof(Test_J));
	Test_J* inputJ = new(j)Test_J;
	printME(inputJ, sizeof(Test_J), "Test_J",outFile);
	delete inputJ;


	fclose(outFile);//close file

	//printf("%d \n",sizeof(Test_A));
	//printf("%d",sizeof(Test_B));
	return 0;
}