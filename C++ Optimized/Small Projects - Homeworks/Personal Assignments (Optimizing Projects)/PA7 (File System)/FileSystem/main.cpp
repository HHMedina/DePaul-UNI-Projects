// LoadInPlace.cpp : Defines the entry point for the console application.
//
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "List.h"
#include "Node.h"
#include "output.h"



/*Node Data Members

Node	*next;
Node	*prev;
int   x;
int   y;
int   z;
int	key;
*/


enum ReadWriteBinFileError{

	OpenFileFailure,
	WriteToFileSuccess,
	WritingError,
	ReadingError,
	EmptyList,
	ConstructFromFileSuccess

};

struct LListFileHeader
{
	LListFileHeader()
		:numNodes(0), blockSize(0)
	{
	}
	
	LListFileHeader&  operator =(const LListFileHeader& rhs)
	{
		this->blockSize = rhs.blockSize;
		this->numNodes = rhs.numNodes;
		return *this;
	}


	~LListFileHeader(){}

	unsigned int numNodes;
	unsigned int blockSize;

private:
	
};

ReadWriteBinFileError WriteToFile(const Node* headList, const char* const fileName)
{

	/*
	This section correspondings to 1) Create a new Binary file
	
	This section first writes the file information in a header then writes the
	Node data, one node at a time.
	*/
	if (headList != 0){//check for empty list

		FILE* myFile;

		if (fopen_s(&myFile, fileName, "wb") == 0){//if file open successfully

			//get the blockSize and number of nodes and store the information in a header at the top
			LListFileHeader thisHeader;
			const Node* temp = headList;
			while (temp!=0)
			{
				thisHeader.numNodes++;
				thisHeader.blockSize += sizeof(Node) - (sizeof(Node*) * 2);
				temp = temp->next;
			}

			//write header to file
			if (fwrite(&thisHeader, sizeof(LListFileHeader), 1, myFile) != 1){
				return WritingError;
			}


			//only store the int x, int y, int z, and int key, since nodes will be allcoated using new. (pointers will be overwritten)
			temp = headList;
			while (temp != 0){//while there are still nodes left to write

				//write x,y,z,and key to file.
				if (fwrite(&temp->x, sizeof(int), 4, myFile) != 4){
					return WritingError;
				}
				temp = temp->next;
			}

			fclose(myFile);
			return WriteToFileSuccess;
		}
		else
		{
			return OpenFileFailure;
		}
	}
	else
	{
		return EmptyList;
	}
}

ReadWriteBinFileError ConstructList(Node** outHead, const char* const fileName)
{
	FILE* myFile;
	if (fopen_s(&myFile, fileName, "rb") == 0){//if file open successfully

		
		/*
			This section correspondings to 2) Read the binary file

			It loads the header first and then the rest of the file based on the
			reported block size. 
		*/

		//read the header information
		LListFileHeader thisHeader;
		if (fread_s(&thisHeader, sizeof(LListFileHeader), sizeof(LListFileHeader), 1, myFile) != 1){
			return ReadingError;
		}

		int* loadedData = new int[thisHeader.numNodes * (sizeof(Node) - (sizeof(Node*) * 2)) ];//pointers are omitted
		
		if (fread_s(loadedData, thisHeader.blockSize, thisHeader.blockSize, 1, myFile) != 1)
		{
			return ReadingError;
		}

		/*
			This section correspondings to 3) Recreate the linked list from your loaded data

			It uses the loadedData and the information from the loaded header to create corresponding
			nodes with the corresponding data read from the binary file
		*/

		unsigned int numIntsInEachNode = 4;

		//create head first
		*outHead = new Node();

		//set pointers to null initially
		(*outHead)->prev = 0;
		(*outHead)->next = 0;

		(*outHead)->x = loadedData[0];
		(*outHead)->y = loadedData[1];
		(*outHead)->z = loadedData[2];
		(*outHead)->key = loadedData[3];

		Node* temp = *outHead;
		unsigned int offSet;
		for (unsigned int i = 1; i < thisHeader.numNodes; i ++)//create the rest of the nodes
		{
			temp->next = new Node();

			//set next of new node to null..since no default constructor
			temp->next->next = 0;
			temp->next->prev = temp;

			offSet = i * numIntsInEachNode;

			//set node's data to corresponding information in the read data
			temp->next->x = loadedData[offSet+0]; 
			temp->next->y = loadedData[offSet + 1];
			temp->next->z = loadedData[offSet + 2];
			temp->next->key = loadedData[offSet + 3];

			temp = temp->next;
		}


		delete[] loadedData;
		fclose(myFile);
		return ConstructFromFileSuccess;
	}
	else
	{
		return OpenFileFailure;
	}
}

void PrintList(Node*& head)
{
	Node* temp = head;
	while (temp != 0)
	{
		temp->dumpNode();
		temp = temp->next;
	}
}

int main()
{
	// Create the IO for this program, that's WHERE YOU PUT YOUR NAME, please change it.
	io::create("Hector Medina");

	// Do not modify this function, you need to leave this routine alone.
	// It initializes the data if a very disjointed way do not modify it!
	List::CreateLinkedList();

	// Output Original Nodes
	fprintf(io::getHandle(), "Original nodes in Memory\n\n");
	List::dumpList();

	// SECTION 1:  Under grads only
	//        1) Create a new Binary file
	//             a) open new binary file
	//             b) write code to copy the node data to a binary file
	//             c) close file
	//        2) Read the Binary file
	//             a) open file
	//             b) write code to load(read) data from your binary file
	//        3) Recreate the linked list from your loaded data
	//             a) add functions to the linked list to help with this task
	//             b) after loaded and linked, close file
	//        4) Print the linked list data, (like sample) 
	//             a) obviously your pointers will be different, but not your data
	//             b) append to existing output file


	const char* const fileName = "MyBinFile";

	if (WriteToFile(List::getListHead(), fileName) != WriteToFileSuccess)
	{
		assert(0 && "Could not Write list to file.");
	}

	Node* constructedHead = 0;

	/*
		This section corresponds to 3 and 4
	*/
	if (ConstructList(&constructedHead, fileName) != ConstructFromFileSuccess)
	{
		assert(0 && "List Could Not Be Constructed.");
	}

	/*
		This section corresponds to 4) Print the linked list data
	*/
	fprintf(io::getHandle(), "New linked list after loading it back from a file\n\n");
	PrintList(constructedHead);

	return 0;
}