#include "output.h"
#include <set>
#include <functional>
#include <algorithm>

struct ZipCode
{
	char s[4];
	int  zip;
};

ZipCode data[] =
{
	{"Bug", 0xABCD},
	{"Dog", 0x1331},
	{"Cat", 0x8844},
	{"Pig", 0xBB77},
	{"Bee", 0xFFEE}
};


struct secondCharCompare {
	bool operator() (const ZipCode& leftZip, const ZipCode& rightZip) const{
		
		return leftZip.s[1] < rightZip.s[1];
	}
};

struct compareToZipInt {

	compareToZipInt(const int& lookUpVal )
		:myInt(lookUpVal)
	{}

	bool operator() (const ZipCode& _zipCode) const{

		return _zipCode.zip == myInt;
	}
	const int& myInt;

private:
	compareToZipInt& operator =(const compareToZipInt& rhs);
};

void PrintSet(const char * const name,const std::set<ZipCode,secondCharCompare> setToPrint)
{
	int counter = 0;
	fprintf(io::getHandle(), "\n%s\n\n",name);
	for (std::set<ZipCode, secondCharCompare>::iterator it = setToPrint.begin(); it != setToPrint.end();it++)
	{
		fprintf(io::getHandle(), "        zippy[%d]: s:%s  zip:0x%x \n", counter, it->s, it->zip);
		counter++;
	}
}

void PrintSetMemberInformation(const char * const sectionName, const std::set<ZipCode,secondCharCompare>::iterator& zip)
{
	fprintf(io::getHandle(), "\n%s\n\n",sectionName);
	fprintf(io::getHandle(), "        zippy: s:%s\n",zip->s);
	fprintf(io::getHandle(), "                zip:%#x\n", zip->zip);
	fprintf(io::getHandle(), "                addr: %#p\n\n", zip);
}


void Problem_3( )
{
	// create file
	io::create( "Hector Medina", "problem3.txt" );

	// Original data
	
	ZipCode *p = &data[0];
	fprintf(io::getHandle(),"original\n\n");
	for( int i = 0; i < 5; i++)
	{
		fprintf(io::getHandle(),"        zippy[%d]: s:%s  zip:0x%x \n", i, p->s, p->zip  );
		p++;
	}

	// a) insert the data into a stl:set in the order given
	//    use a compare function that sorts with the 2nd letter of the string 
	//    print it to the file (begin to end) order
	std::set<ZipCode,secondCharCompare> zippy(data,data+5);
	PrintSet("stl: insert into a set\n\tcompare function sorts 2nd character of string",zippy);
	
	// b)  Find node {"Bee", 0xFFEE} in the set
	//     use the complete node in the search
	//     once found print the iterator's address and it's content
	std::set<ZipCode, secondCharCompare>::iterator it = zippy.find({ "Bee", 0xFFEE });
	PrintSetMemberInformation("stl: find node using complete node : {\"Bee\", 0xFFEE }",it);
	
	// c)  Find the the string name associated with this zip data '0xBB77'
	//     Do not use the complete node, only the zip data
	//     Print the node contents and iterator address
	//
	//     NOTE: you can only use STL: set  - no maps!
	//
	//     hint:  use a custom predicate
	it = std::find_if(zippy.begin(), zippy.end(),compareToZipInt(0xbb77));
	PrintSetMemberInformation("stl: find node using only the zip data: 0xBB77 ", it);


	// bye bye
	fprintf(io::getHandle(),"\n");
	io::destroy();
}