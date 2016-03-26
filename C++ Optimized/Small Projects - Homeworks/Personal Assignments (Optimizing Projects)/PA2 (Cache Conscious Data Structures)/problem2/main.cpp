#include <iostream>
#include "TestDataStruct.h"
#include "../problem1/TestDataStruct.h"
void printComparisonMEM(void* ptr, int newSizeIn, int oldSizeIn, const char* const newName, const char* const oldName, FILE*& outFile){
	
	int i = 0;
	int numPads = 0;
	int columnCounter = 0;
	unsigned char* reader = (unsigned char*)ptr;
	fprintf(outFile, "Information for (%s)...\n\n\tMemory Layout (where 0xaa is padding):\n\n\t%#x: ", newName, reader);
	while (i<newSizeIn){//still bytes left to print

		if (reader[i] == 0xaa){
			numPads++;
		}

		//print the byte
		fprintf(outFile, "%02x ", reader[i]);
		i++;

		columnCounter++;
		if (columnCounter>3 && i<newSizeIn){
			fprintf(outFile, "\n\t%#x: ", reader + i);
			columnCounter = 0;
		}
	}

	fprintf(outFile, "\n\n\t(%s): Size: %i bytes.\tNumber of Padding: %i",newName, newSizeIn, numPads);
	fprintf(outFile, "\n\t(%s): Size: %i bytes.", oldName, oldSizeIn);
	fprintf(outFile, "\n\n");

}

int main(){

	FILE * outFile;
	fopen_s(&outFile, "Output.txt", "w");// openfile

	char* a = new char[sizeof(ReworkedDataStructs::Test_A)];
	memset(a, 0xaa, sizeof(ReworkedDataStructs::Test_A));
	ReworkedDataStructs::Test_A* input = new(a)ReworkedDataStructs::Test_A;
	printComparisonMEM(input, sizeof(ReworkedDataStructs::Test_A),sizeof(Test_A),"Reworked Test_A", "Test_A", outFile);
	delete input;

	char* b = new char[sizeof(ReworkedDataStructs::Test_B)];
	memset(b, 0xaa, sizeof(ReworkedDataStructs::Test_B));
	ReworkedDataStructs::Test_B* input2 = new(b)ReworkedDataStructs::Test_B;
	printComparisonMEM(input2, sizeof(ReworkedDataStructs::Test_B),sizeof(Test_B), "Reworked Test_B", "Test_B", outFile);
	delete input2;
	
	char* c = new char[sizeof(ReworkedDataStructs::Test_C)];
	memset(c, 0xaa, sizeof(ReworkedDataStructs::Test_C));
	ReworkedDataStructs::Test_C* input3 = new(c)ReworkedDataStructs::Test_C;
	printComparisonMEM(input3, sizeof(ReworkedDataStructs::Test_C),sizeof(Test_C), "Reworked Test_C", "Test_C", outFile);
	delete input3;

	
	char* d = new char[sizeof(ReworkedDataStructs::Test_D)];
	memset(d, 0xaa, sizeof(ReworkedDataStructs::Test_D));
	ReworkedDataStructs::Test_D* input4 = new(d)ReworkedDataStructs::Test_D;
	printComparisonMEM(input4, sizeof(ReworkedDataStructs::Test_D),sizeof(Test_D),"Reworked Test_D", "Test_D", outFile);
	delete input4;

	
	char* e = new char[sizeof(ReworkedDataStructs::Test_E)];
	memset(e, 0xaa, sizeof(ReworkedDataStructs::Test_E));
	ReworkedDataStructs::Test_E* input5 = new(e)ReworkedDataStructs::Test_E;
	printComparisonMEM(input5, sizeof(ReworkedDataStructs::Test_E), sizeof(Test_E), "Reworked Test_E", "Test_E", outFile);
	delete input5;

	char* f = new char[sizeof(ReworkedDataStructs::Test_F)];
	memset(f, 0xaa, sizeof(ReworkedDataStructs::Test_F));
	ReworkedDataStructs::Test_F* inputf = new(f)ReworkedDataStructs::Test_F;
	printComparisonMEM(inputf, sizeof(ReworkedDataStructs::Test_F), sizeof(Test_F), "Reworked Test_F", "Test_F", outFile);
	delete inputf;

	char* g = new char[sizeof(ReworkedDataStructs::Test_G)];
	memset(g, 0xaa, sizeof(ReworkedDataStructs::Test_G));
	ReworkedDataStructs::Test_G* inputg = new(g)ReworkedDataStructs::Test_G;
	printComparisonMEM(inputg, sizeof(ReworkedDataStructs::Test_G), sizeof(Test_G), "Reworked Test_G", "Test_G", outFile);
	delete inputg;

	char* vectP = new char[sizeof(ReworkedDataStructs::vect)];
	memset(vectP, 0xaa, sizeof(ReworkedDataStructs::vect));
	ReworkedDataStructs::vect* inputVect = new(vectP)ReworkedDataStructs::vect;
	printComparisonMEM(inputVect, sizeof(ReworkedDataStructs::vect), sizeof(vect), "Reworked vect", "vect", outFile);
	delete inputVect;

	char* h = new char[sizeof(ReworkedDataStructs::Test_H)];
	memset(h, 0xaa, sizeof(ReworkedDataStructs::Test_H));
	ReworkedDataStructs::Test_H* inputH = new(h)ReworkedDataStructs::Test_H;
	printComparisonMEM(inputH, sizeof(ReworkedDataStructs::Test_H), sizeof(Test_H), "Reworked Test_H", "Test_H", outFile);
	delete inputH;

	char* i = new char[sizeof(ReworkedDataStructs::Test_I)];
	memset(i, 0xaa, sizeof(ReworkedDataStructs::Test_I));
	ReworkedDataStructs::Test_I* inputI = new(i)ReworkedDataStructs::Test_I;
	printComparisonMEM(inputI, sizeof(ReworkedDataStructs::Test_I), sizeof(Test_I), "Reworked Test_I", "Test_I", outFile);
	delete inputI;

	char* j = new char[sizeof(ReworkedDataStructs::Test_J)];
	memset(j, 0xaa, sizeof(ReworkedDataStructs::Test_J));
	ReworkedDataStructs::Test_J* inputJ = new(j)ReworkedDataStructs::Test_J;
	printComparisonMEM(inputJ, sizeof(ReworkedDataStructs::Test_J), sizeof(Test_J), "Reworked Test_J", "Test_J", outFile);
	delete inputJ;


	fclose(outFile);//close file
	return 0;
}