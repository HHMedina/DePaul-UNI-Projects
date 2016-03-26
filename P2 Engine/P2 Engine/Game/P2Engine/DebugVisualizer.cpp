#include "DebugVisualizer.h"
#include "GraphicsObjectMaker.h"

DebugVisualizer* DebugVisualizer::instance =NULL;
Vect DebugVisualizer::DEFAULTCOLOR = Vect(0,0,0);
void DebugVisualizer::Initialize(){
	GetInstance();

	GetInstance().wfSphere = GraphicsObjectMaker::CreateGraphicsObjectWireFrame(GpuModel::Create("BoundingSphere.azul"));
	GetInstance().wfBox = GraphicsObjectMaker::CreateGraphicsObjectWireFrame(GpuModel::Create("BoundingBox.azul"));
}

void DebugVisualizer::ShowPoint(const Vect& pos, const Vect& color, float markRadius){

	Matrix temp = Matrix(SCALE,markRadius,markRadius,markRadius) * Matrix(TRANS,pos);
	GetInstance().AddData(GetInstance().wfBox,temp,color);
}

void DebugVisualizer::ShowSphere(const Vect& center, float radius, const Vect& color ){
	Matrix temp = Matrix(SCALE,radius,radius,radius) * Matrix(TRANS,center);
	GetInstance().AddData(GetInstance().wfSphere,temp,color);
}

void DebugVisualizer::ShowAABB(const Vect& minPoint, const Vect& maxPoint, const Vect& color){

	Vect scale = maxPoint-minPoint;
	Vect pos = (maxPoint+minPoint)* .5f;
	Matrix temp = Matrix(SCALE,scale[x],scale[y],scale[z]) * Matrix(TRANS,pos[x],pos[y],pos[z]);
	GetInstance().AddData(GetInstance().wfBox,temp,color);

}

void DebugVisualizer::ShowOBB(const Vect& minPoint, const Vect& maxPoint, const Matrix& worldMat, const Vect& color){
	
	Vect scale = (maxPoint-minPoint);
	Vect pos = (maxPoint+minPoint)* .5f;
	Matrix temp = Matrix(SCALE,scale[x],scale[y],scale[z]) * Matrix(TRANS,pos[x],pos[y],pos[z])*worldMat;
	GetInstance().AddData(GetInstance().wfBox,temp,color);
	//scaleM*rotationM*translationM*myOwner->colWorld
}

void DebugVisualizer::RenderVisuals(){

	visualData* temp;

	while(!GetInstance().visualRequests.empty()){

		temp = GetInstance().visualRequests.front();
		GetInstance().visualRequests.pop_front();

		(*temp).gObj->setWorld((*temp).worldMat);
		(*temp).gObj->color = (*temp).color;
		(*temp).gObj->Render();

		GetInstance().RecycleData(temp);

	}

}

void DebugVisualizer::AddData(GraphicsObjectWireFrame* go, const Matrix& mat, const Vect& color){

	visualData* tempData = GetInstance().GetNewData();

	tempData->gObj =go;
	tempData->worldMat = mat;
	tempData->color = color;

	visualRequests.push_back(tempData);
}

void DebugVisualizer::RecycleData(DebugVisualizer::visualData* target ){
	GetInstance().visDataPool.push(target);
}

DebugVisualizer::visualData* DebugVisualizer::GetNewData(){

	visualData* temp;

	if(!DebugVisualizer::GetInstance().visDataPool.empty()){

		temp = DebugVisualizer::GetInstance().visDataPool.top();
		DebugVisualizer::GetInstance().visDataPool.pop();

	}else{
		temp = new visualData();
	}
	return temp;
}
