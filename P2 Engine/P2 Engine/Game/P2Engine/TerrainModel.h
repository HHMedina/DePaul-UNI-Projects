#ifndef _TerrainModel
#define _TerrainModel

#include <P2Engine.h>
#include <tuple>
#include <list>

class TerrainModel : public GameObject
{
private:

	struct MaxAndMinPoints
	{
		MaxAndMinPoints(){

		}
		
		MaxAndMinPoints(const MaxAndMinPoints& rhs){
			this->MinPoint = rhs.MinPoint;
			this->MaxPoint = rhs.MaxPoint;
		}

		const MaxAndMinPoints &operator=(const MaxAndMinPoints &rhs){
			this->MaxPoint = rhs.MaxPoint;
			this->MinPoint = rhs.MinPoint;
			return *this;
		}

		MaxAndMinPoints(const Vect& min,const Vect& max){
			MinPoint =min;
			MaxPoint = max;
		}
		~MaxAndMinPoints(){}
		Vect MaxPoint;
		Vect MinPoint;
	};


	//for debugging
	Vect highLightPos;

	int imgWidth, imgHeigth,numVerts,numTris,numCells,numVertsPerSide,numCellsPerSide;
	float cellDimension,sideLength;

	GraphicsObject* pGObjFT;
	GraphicsObjectWireFrame* pGObjFT2;
	Matrix WorldMat;

	//placed here for debug drawing
	VertexStride_VUN* vertsArray;
	MaxAndMinPoints* minMaxData;

	GameObject* test;

	void CreateTerrainModel( char* heightmapFile, float Sidelength,float zeroHeight,float maxheight, int RepeatU, int RepeatV);
	void SaveTerrainModel( VertexStride_VUN* pVerts, int num_verts, TriangleIndex* tlist, int num_tri); 
	virtual void Update();

public:
	//static Vect highLightPos;
	//void SetHighLightPosition(const Vect& pos){
	//	highLightPos = pos;

	//}

	void HighLightCellOnGrid(Vect& indx, Vect& color){
		int index = (int)indx[z] *numCellsPerSide+ (int)indx[x];
		DebugVisualizer::ShowAABB(minMaxData[  index   ].MinPoint,minMaxData[  index  ].MaxPoint,color );
	}

	void HighLightCellsUnderNeathSphere(GameObject*  gObj){
		Vect position = gObj->GetCollisionVolume().GetCheapCenter();
		float sphereRadius =gObj->GetCollisionVolume().GetCheapSphereRadius();
		
		DebugVisualizer::ShowSphere(position,sphereRadius);
		float minX = position[x]-sphereRadius;
		float maxX = position[x]+sphereRadius;
		float minZ = position[z]-sphereRadius;
		float maxZ = position[z]+sphereRadius;

		Vect minCell = GetIndexFromWorldPosition(Vect(minX,0,minZ)); 
		Vect maxCell = GetIndexFromWorldPosition(Vect(maxX,0,maxZ));
		int xRangeToCheck = maxCell[x]-minCell[x];
		int zRangeToCheck = maxCell[z]-minCell[z];

		
		for(float j = minZ;j<maxZ;){
			for(float i = minX;i<maxX;){
				//under sphere
				Vect currentCell = GetIndexFromWorldPosition(Vect(i,0,j));

				int index = (int)currentCell[z] *numCellsPerSide+ (int)currentCell[x];
				//collides with sphere?
				if(P2Math::TestAABBSphereCollision(minMaxData[index].MinPoint,minMaxData[index].MaxPoint,position,sphereRadius)){

					//HighLightCellOnGrid(Vect(i,0,j),Vect(1,0,0));
					HighlightCellOnTerrainFromWorld(Vect(i,0,j),Vect(1,0,0));

				}else{
					//HighLightCellOnGrid(Vect(i,0,j),Vect(0,1,0));
					HighlightCellOnTerrainFromWorld(Vect(i,0,j),Vect(0,1,0));
				}
						
					
			
				
				i+=cellDimension;
			}
			
			j+=cellDimension;
		
		
		}

		
	}

	

	void HighlightCellOnTerrainFromWorld(const Vect& pos,const Vect& color){

		

		//cellDimension =  cellDimension ;
		Vect tempIndx= GetIndexFromWorldPosition(pos);

		//if(tempIndx[x] == -1||tempIndx[z] == -1){
		//	return;
		//}


		int index = (int)tempIndx[z] *numCellsPerSide+ (int)tempIndx[x];

		//DebugVisualizer::ShowPoint(Vect(vertsArray[(int)(tempIndx[y] * imgWidth)+(int)tempIndx[x]].x ,vertsArray[(int)(tempIndx[y] * imgWidth)+(int)tempIndx[x]].y,vertsArray[(int)(tempIndx[y] * imgWidth)+(int)tempIndx[x]].z),Vect(0,0,1),20);
		
		float halfSize = (sideLength/2); //+ (cellDimension/2);
		if(pos[x] < halfSize&&pos[x] > -halfSize&&pos[z] < halfSize&&pos[z] > -halfSize){
			DebugVisualizer::ShowAABB(minMaxData[  index   ].MinPoint,minMaxData[  index  ].MaxPoint,color );
		}
	}

	Vect GetIndexFromWorldPosition(const Vect& pos){

		//bring into local grid space (position)... used to acocunt for origin
		float posInLocalx =(sideLength/2.0f) + pos[x];
		float posInLocalz = (sideLength/2.0f)  + pos[z];//- (cellDimension/2)

		float calculatedFX = (float)(posInLocalx/cellDimension) ; //-1;
		float calculatedFZ = (float)(posInLocalz/cellDimension); //-1;
		calculatedFX;
		calculatedFZ;
		
		//if(calculatedFX<0||calculatedFZ<0){
			//negative indices return negative 
		//	return Vect(-1,-1,-1);
		//}
		
	
		//if((int)calculatedFX <0|| (int) calculatedFZ<0|| (int) calculatedFX > numCellsPerSide || (int)calculatedFZ>numCellsPerSide ){
			//if out of bounds return -1 vector
		//	return Vect(-1,-1,-1);
		//}else{
		return Vect( (int)calculatedFX ,0,(int)calculatedFZ);
		//}
		//return Vect((int)(pos[x]/cellDimension + (imgWidth/2.0f)) -1,0,(int)(pos[z]/cellDimension + (imgWidth/2.0f)) -1  );

	}
	TerrainModel( char* heightmapFile, float Sidelenght, float maxheight, float zeroHeight, char* TextureKey, int RepeatU, int RepeatV, GameObject* gameObj);
	~TerrainModel(){
		if(vertsArray){
			delete[] vertsArray;
		}
		if(minMaxData){
			delete[] minMaxData;
		}
	}
	virtual void Draw() override;

};


#endif _TerrainModel