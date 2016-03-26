#include "TerrainModel.h"
#include <P2Engine.h>
#include <assert.h>
#include <string>
#include <vector>
#include <assert.h>
#include "AssetManager.h"
#include "DebugVisualizer.h"
#include "GLTools.h"
#include "GL\glew.h"


// Note: the heightmap file should not be a pre-loaded texture from the texture manager
TerrainModel::TerrainModel( char* heightmapFile, float Sidelength, float maxheight, float zeroHeight, char* TextureKey, int RepeatU, int RepeatV, GameObject* gameObj)
{
	sideLength = Sidelength;
	
	test = gameObj;
	

	// Create the model 
	// *** and save it to an .azul file ***
	// *** and loads in the the Model Manager ***
	CreateTerrainModel( heightmapFile, Sidelength, zeroHeight,maxheight, RepeatU, RepeatV);

	// and then load it back in through the model manager... Because it's easier than modding AZUL
	//forced azulterrain
	Model* terrainModel = GpuModel::Create("AZULTERRAIN.azul");

	// Scale and adjust position if needed
	Matrix temp = Matrix();
	Matrix tankTopScale;
	tankTopScale.set( SCALE, 1, 1, 1);
	Matrix tankTopRot;
	tankTopRot.set( ROT_XYZ, 0,0 ,0);
	Vect tankTopPos;
	tankTopPos = Vect(0,0,0);
	WorldMat = tankTopScale * tankTopRot * Matrix( TRANS, tankTopPos );

	// Connect the model to a graphics object
	pGObjFT = new GraphicsObjectFlatTexture( terrainModel , AssetManager::GetTexture(TextureKey)	);
	pGObjFT->setWorld( WorldMat );

	//*  
	// This is for debugging only
	pGObjFT2 = new GraphicsObjectWireFrame( terrainModel); TextureKey;
	pGObjFT2->setWorld( WorldMat );
	//*/
}

void TerrainModel::Draw()
{
	
		
	pGObjFT->Render();
	//pGObjFT2->Render();  // Debug tool only
}

// Creates the actual model mesh
void TerrainModel::CreateTerrainModel( char* heightmapFile, float Sidelength,float zeroHeight,float maxheight, int RepeatU, int RepeatV)
{
	

	
	// Using GLTools to read in the file
	int icomp;
	unsigned int dtype;
	GLbyte* imgData = gltReadTGABits( heightmapFile, &imgWidth, &imgHeigth, &icomp, &dtype);
	assert( imgWidth == imgHeigth); // We need square images for heightmaps
	
	/// Insert much work to create and the model

	/*
	Notes:

	img width * img height = number of verts since grid points are "in the middle of the pixel"


	*/

	cellDimension =Sidelength/(imgWidth-1);
	numVerts = (imgHeigth)*(imgWidth);
	numVertsPerSide = (imgHeigth);
	numTris = (numVertsPerSide-1) * (numVertsPerSide-1) * 2;
	numCellsPerSide = (numVertsPerSide-1) ;
	numCells = numCellsPerSide*numCellsPerSide;
	//create vertex list for plane in image scale (1 pixel = one azul unit) (with position in world space
	vertsArray = new VertexStride_VUN[numVerts];
	minMaxData = new MaxAndMinPoints[numCellsPerSide*numCellsPerSide];

	float uvSegment = 1.0f/(imgHeigth-1.0f);

	//create flat mesh
	for(int i = 0; i<imgWidth; ++i){
		for(int j = 0; j<imgWidth; ++j){
		//get the position value for each vert
			VertexStride_VUN * temp = &vertsArray[(i*numVertsPerSide) + j];
			
			

			//set all uneccessary to 0
			temp->txt =0;
			temp->nx=0;
			temp->ny=0;
			temp->nz=0;

			//calculate the position of the vertex
			//cout<<(-numVertsPerSide/2.0f);
			temp->x = ((-numVertsPerSide/2.0f) +.5f + j ) * cellDimension;//
			temp->z = ((-numVertsPerSide/2.0f) +.5f + i) * cellDimension;//flipped to face +z axis
			
			//set UVs
			temp->u = (j*uvSegment)*RepeatU;
			temp->v = (numVertsPerSide- i)*uvSegment*RepeatV;
			
			//calculation y from image data
			unsigned char imgDataTemp =  static_cast<unsigned char>( imgData[ ((imgHeigth-1)-i)* (imgHeigth*3) + ((j*3)+2) ] );//(i*(imgHeigth*3)) + ((j*3)+2)] );
			imgDataTemp = imgDataTemp;
			float adjustedForHeight = zeroHeight + ( (((float)imgDataTemp)/255.0f) * maxheight );
			adjustedForHeight=adjustedForHeight;

			
			temp->y =adjustedForHeight;
		
		}
	}

		



	//end vertex list
	
	
	//create triangle list
	TriangleIndex* tList = new TriangleIndex[numTris];
	int currentCell = 0;
	//int testCounter=0;
	for(int i = 0; i<numCellsPerSide; ++i){
		for(int j = 0; j<numCellsPerSide; ++j){
			


			
			VertexStride_VUN * topLeft = &vertsArray[(i*numVertsPerSide) + j];
			VertexStride_VUN * topRight = &vertsArray[(i*numVertsPerSide) + (j+1)];
			VertexStride_VUN * bottomLeft = &vertsArray[((i+1)*numVertsPerSide) + j];
			VertexStride_VUN * bottomRight = &vertsArray[((i+1)*numVertsPerSide) + (j+1)];


			//find min and max points
			Vect min = Vect(topLeft->x,topLeft->y ,topLeft->z );
			Vect max = Vect(topLeft->x,topLeft->y ,topLeft->z );

			min = P2Math::MinVect(min,Vect(topRight->x,topRight->y ,topRight->z));
			max = P2Math::MaxVect(max,Vect(topRight->x,topRight->y ,topRight->z));

			min = P2Math::MinVect(min,Vect(bottomLeft->x,bottomLeft->y ,bottomLeft->z));
			max = P2Math::MaxVect(max,Vect(bottomLeft->x,bottomLeft->y ,bottomLeft->z));

			min = P2Math::MinVect(min,Vect(bottomRight->x,bottomRight->y ,bottomRight->z));
			max = P2Math::MaxVect(max,Vect(bottomRight->x,bottomRight->y ,bottomRight->z));
			//get the min and max points (in my case its the top left corner and bottom right corner of each cell
			minMaxData[(i*(numCellsPerSide)) + j] = MaxAndMinPoints(min, max);
			//testCounter++;
			//(i*imgHeigth) + j
			///////


			TriangleIndex* currentTriangle = &tList[currentCell];


			//top triangle
			currentTriangle->v0 = (i*numVertsPerSide) + j;//top left
			currentTriangle->v1 = ((i+1)*numVertsPerSide +j);//bottom left
			currentTriangle->v2 = (i*numVertsPerSide) + (j+1);//top right
			
			++currentCell;
			currentTriangle = &tList[currentCell];

			//bottom triangle
			currentTriangle->v0 = ((i+1)*numVertsPerSide +j);//bottom left
			currentTriangle->v1 = ((i+1)*numVertsPerSide) + (j+1);//bottom right
			currentTriangle->v2 = (i*numVertsPerSide) + (j+1);//top right

			++currentCell;
		}
	}
	//end triangle formation

	
	SaveTerrainModel(vertsArray,imgHeigth*imgWidth,tList,numTris);

}

// Saves the model to file in the Azul format 
// (realistically, the filename should be parametrized)
void TerrainModel::SaveTerrainModel( VertexStride_VUN* pVerts, int num_verts, TriangleIndex* tlist, int num_tri)
{
	char* TerrainName = "AZULTERRAIN";
	char* TerrainFilename = "../Assets/AZULTERRAIN.azul";

	//// Write the data to a file ----------------------------
	FileHandle fh;
	FileError  ferror;
	std::vector<std::string> FBX_textNames; // (Ed) purposefully empty to be consistent 

	// 1) Create a blank header

		// // write the data
		AzulFileHdr  azulFileHdr;
		strcpy_s(azulFileHdr.objName, OBJECT_NAME_SIZE, TerrainName);

	// 2)  Write the data (Header, Buffers) to the files

	// write the header (Take 1) 

		// create the file, write the header
		ferror = File::open(fh, TerrainFilename, FILE_READ_WRITE );
		assert( ferror == FILE_SUCCESS );

		// write the data
		ferror = File::write( fh, &azulFileHdr, sizeof(azulFileHdr) );
		assert( ferror == FILE_SUCCESS );

	// update the header: numTextures, textureNamesOffset
		azulFileHdr.numTextures = FBX_textNames.size();

		// update the offset
		ferror = File::tell( fh, azulFileHdr.textureNamesOffset );
		assert( ferror == FILE_SUCCESS );

	
	std::vector<std::string>::iterator FBX_textNames_iterator;

	int i=0;
	for( FBX_textNames_iterator = FBX_textNames.begin(); 
		    FBX_textNames_iterator != FBX_textNames.end();
		    FBX_textNames_iterator++ )
	{
		const char *ss = (*FBX_textNames_iterator).c_str();
		// write the vertex data
		ferror = File::write( fh, ss, strlen(ss) + 1);
		assert( ferror == FILE_SUCCESS );
		i++;
	}

	// update the header: numVerts, VertBufferOffset

		// update the number of verts
		azulFileHdr.numVerts = num_verts;
   
		// update the offset
		ferror = File::tell( fh, azulFileHdr.vertBufferOffset );
		assert( ferror == FILE_SUCCESS );

	// write the vertex data
	ferror = File::write( fh, pVerts, num_verts * sizeof(VertexStride_VUN) );
	assert( ferror == FILE_SUCCESS );

	// update the header: numTriList, triListBufferOffset

		// update the number of verts
		azulFileHdr.numTriangles = num_tri;

		// update the offset
		ferror = File::tell( fh, azulFileHdr.triangleListBufferOffset );
		assert( ferror == FILE_SUCCESS );

	// write the triListBuffer data
	ferror = File::write( fh, tlist, num_tri * sizeof(TriangleIndex) );
	assert( ferror == FILE_SUCCESS );
	    
	// write the header (Take 2) now with updated data

		azulFileHdr.textureNamesInBytes = azulFileHdr.vertBufferOffset - azulFileHdr.textureNamesOffset;

		// reset to the beginning of file
		ferror = File::seek( fh, FILE_SEEK_BEGIN, 0 );
		assert( ferror == FILE_SUCCESS );

		// write the buffer
		ferror = File::write( fh, &azulFileHdr, sizeof(azulFileHdr) );
		assert( ferror == FILE_SUCCESS );

	// All done - bye bye
	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );
}

void TerrainModel::Update(){

	//HighlightCellOnTerrainFromWorld(test->position,Vect(0,0,0));
	HighLightCellsUnderNeathSphere(test);
	/*
	for(int i = 0; i<numVertsPerSide; ++i){
		for(int j = 0; j<numVertsPerSide; ++j){
			VertexStride_VUN* temp = &vertsArray[(i*numVertsPerSide) + j];
			DebugVisualizer::ShowPoint(Vect(temp->x,temp->y,temp->z));
		}
	}
	*/
	//for(int i =0; i<numCells;i++){
	//	DebugVisualizer::ShowAABB(minMaxData[i].MinPoint, minMaxData[i].MaxPoint );

	//}

	//list<MaxAndMinPoints>::iterator it = minMaxData.begin();

	//while(it != minMaxData.end()){

	//	DebugVisualizer::ShowAABB((*it).MinPoint,(*it).MaxPoint );

	//	++it;
	//}

}
