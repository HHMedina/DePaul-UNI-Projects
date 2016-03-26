#include "Terrain.h"
#include "AssetManager.h"
#include "P2Math.h"
#include <assert.h>
#include <string>
#include <vector>
#include "Collidable.h"

Terrain::Terrain( const char* const heightmapFile, const char* const assetName, const float Sidelength, const float maxheight, const float zeroHeight, const char* const TextureKey, const int RepeatU, const int RepeatV){
	vertsArray=NULL;
	minMaxData =NULL;
	terrainGrphObject=NULL;
	sideLength = Sidelength;

	// Create the model 
	// *** and save it to an .azul file ***
	CreateTerrainModel( heightmapFile,assetName, Sidelength, zeroHeight,maxheight, RepeatU, RepeatV);

	// and then load it back in through the model manager... Because it's easier than modding AZUL
	//forced azulterrain
	string temp = assetName + string(".azul");
	AssetManager::LoadAzulModel(temp.c_str(),assetName);
	//Model* terrainModel = GpuModel::Create("AZULTERRAIN.azul");

	// Connect the model to a graphics object
	terrainGrphObject = new GraphicsObjectFlatTexture( AssetManager::GetModel(assetName) , AssetManager::GetTexture(TextureKey)	);
}

Terrain::Terrain(const Terrain& ){}

const Terrain& Terrain::operator =(const Terrain&){return *this;}

Terrain::~Terrain(){
	if(vertsArray){
		delete vertsArray;
	}
	if(minMaxData){
		delete minMaxData;
	}
}

void Terrain::Draw(){
	terrainGrphObject->Render();
}

Vect Terrain::GetCellIndexFromWorldPosition(const Vect& pos){

	
	//bring into local grid space (position)... used to acocunt for origin
	float posInLocalx =(sideLength/2.0f) + pos[x];
	float posInLocalz = (sideLength/2.0f)  + pos[z];

	float calculatedFX = (float)(posInLocalx/cellDimension) ; 
	float calculatedFZ = (float)(posInLocalz/cellDimension); 
				
	return Vect( (int)calculatedFX ,0,(int)calculatedFZ);

}

void Terrain::GetLikelyCandidates(Collidable* colObj,list<MaxAndMinPoints>::iterator& startOut,list<MaxAndMinPoints>::iterator& endOut){
	likeyCandidates.clear(); //clear any previous searches
	


	Vect position = colObj->GetCheapSphereCenter();
	float sphereRadius =colObj->GetCheapSphereRadius();
		
	//DebugVisualizer::ShowSphere(position,sphereRadius);
	float minX = position[x]-sphereRadius;
	float maxX = position[x]+sphereRadius;
	float minZ = position[z]-sphereRadius;
	float maxZ = position[z]+sphereRadius;

	Vect minCell = GetCellIndexFromWorldPosition(Vect(minX,0,minZ)); 
	Vect maxCell = GetCellIndexFromWorldPosition(Vect(maxX,0,maxZ));
	int xRangeToCheck = maxCell[x]-minCell[x];
	int zRangeToCheck = maxCell[z]-minCell[z];
			
	for(float j = minZ;j<maxZ;){
		for(float i = minX;i<maxX;){
			//under sphere
			Vect currentCell = GetCellIndexFromWorldPosition(Vect(i,0,j));//GetIndexFromWorldPosition(Vect(i,0,j));
			//if it is within terrain..add tolikely candidates
			if(IsWithinTerrain( Vect(i,0,j) )){
				//int index = (int)currentCell[z] *numCellsPerSide+ (int)currentCell[x];
				int index = P2Math::ArrayIndexFrom2DTo1D((int)currentCell[z],(int)currentCell[x],numCellsPerSide);
				likeyCandidates.push_back(minMaxData[index]);
			}

			i+=cellDimension;
		}
			
		j+=cellDimension;
		
		
	}

	///set the iterators to the start and end of internally maintained list (prevents unncessessary construction)
	startOut = likeyCandidates.begin();
	endOut = likeyCandidates.end();
}

bool Terrain::IsWithinTerrain(const Vect& pos){
	float halfSize = (sideLength/2);
	if(pos[x] < halfSize&&pos[x] > -halfSize&&pos[z] < halfSize&&pos[z] > -halfSize){

		return true;
	}
	return false;
}

void Terrain::CreateTerrainModel(const char* const heightmapFile, const char* const assetName, const float Sidelength,const float zeroHeight,const float maxheight, const int RepeatU, const int RepeatV){

	
	// Using GLTools to read in the file
	int icomp;
	unsigned int dtype;
	GLbyte* imgData = gltReadTGABits( heightmapFile, &imgWidth, &imgHeigth, &icomp, &dtype);
	assert( imgWidth == imgHeigth); // We need square images for heightmaps
	
	/// Insert much work to create and the model
	
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

	//create Vertex list
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
			temp->x = ((-numVertsPerSide/2.0f) +.5f + j ) * cellDimension;
			temp->z = ((-numVertsPerSide/2.0f) +.5f + i) * cellDimension;
			
			//set UVs
			temp->u = (j*uvSegment)*RepeatU;
			temp->v = (numVertsPerSide- i)*uvSegment*RepeatV;
			
			//calculation y from image data
			unsigned char imgDataTemp =  static_cast<unsigned char>( imgData[ ((imgHeigth-1)-i)* (imgHeigth*3) + ((j*3)+2) ] );
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

			Vect topRightVect = Vect(topRight->x,topRight->y ,topRight->z);
			Vect bottomLeftVect = Vect(bottomLeft->x,bottomLeft->y ,bottomLeft->z);
			Vect bottomRightVect = Vect(bottomRight->x,bottomRight->y ,bottomRight->z);

			min = P2Math::MinVect(min,topRightVect);
			max = P2Math::MaxVect(max,topRightVect);

			min = P2Math::MinVect(min,bottomLeftVect);
			max = P2Math::MaxVect(max,bottomLeftVect);

			min = P2Math::MinVect(min,bottomRightVect);
			max = P2Math::MaxVect(max,bottomRightVect);
			//get the min and max points (in my case its the top left corner and bottom right corner of each cell
			minMaxData[(i*(numCellsPerSide)) + j] = MaxAndMinPoints(min, max);
			
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

	
	SaveTerrainModel(assetName, vertsArray,imgHeigth*imgWidth,tList,numTris);

}

// Saves the model to file in the Azul format 
// (realistically, the filename should be parametrized)
void Terrain::SaveTerrainModel( const char* const assetName, VertexStride_VUN* pVerts, int num_verts, TriangleIndex* tlist, int num_tri)
{
	const char* const TerrainName = assetName;
	string temp = "../Assets/" + string(assetName) + ".azul";
	const char* const TerrainFilename = temp.c_str();

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
