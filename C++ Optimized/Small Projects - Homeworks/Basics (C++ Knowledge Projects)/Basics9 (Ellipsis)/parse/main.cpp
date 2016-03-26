#include <stdio.h>
#include <string.h>
// Add functions and includes as needed

enum ChunkType
{
	VERTS_TYPE,
	NORMS_TYPE,
	ANIM_TYPE,
	TEXTURE_TYPE,
	UV_TYPE
};

enum ChunkValidationError
{
	InvalidChunk,
	ValidChunk
};

ChunkValidationError CheckIfValidChunkType(const char*  const comp)
{
	if (strcmp(comp, "VERTS_TYPE") == 0 || strcmp(comp, "NORMS_TYPE") == 0 || strcmp(comp, "ANIM_TYPE")== 0 || strcmp(comp, "TEXTURE_TYPE") == 0 || strcmp(comp, "UV_TYPE") == 0  )
	{

		return ValidChunk;
	}
	return InvalidChunk;

}

int main(int argc, char *argv[])
{


	FILE* tFile = new FILE;
	fopen_s(&tFile, "est.txt", "w");

	fprintf(tFile, "%s\n", argv[1]);
	
	if (argc == 5)
	{
		for (int i = 1; i < argc; i=i+2){ // every second argument should be a -option ( -t or -n)
						
			if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-T") == 0)
			{
				if (InvalidChunk == CheckIfValidChunkType(argv[i+1]))
				{

					fprintf(tFile, "Invalid ChunkType \n");
					return - 1;
				}
				
			}
			else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "-N") == 0)
			{
				if (strlen(argv[i+1])>19)
				{
					return -1; // invalid chunk name
				}
			}
			else
			{
				fprintf(tFile, "Invalid format \n");
				return -1; //formatting not followed... second argument is not -t or -n
				
			}
		}

		return 0;
	
	} else//doesnt have 5 arguments (doesnt follow format)
 	{
		return -1;
		fprintf(tFile, "Invalid number arguments \n");
	}


	
	return -3;
}