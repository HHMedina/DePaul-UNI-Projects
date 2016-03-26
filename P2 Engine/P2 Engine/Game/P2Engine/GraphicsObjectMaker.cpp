#include "GraphicsObjectMaker.h"
#include "AssetManager.h"
#include <assert.h>

GraphicsObjectMaker* GraphicsObjectMaker::instance =NULL;


//=====================
// Private Definitions
//=====================
GraphicsObjectMaker::~GraphicsObjectMaker(){

	while(!this->allGraphicsObjects.empty()){
		delete this->allGraphicsObjects.front();
		allGraphicsObjects.pop_front();
	}

	instance = NULL;
}

GraphicsObjectFlatTexture* GraphicsObjectMaker::privCreateGraphicsObjectFlatTextureWithDefaults(Model* model){

	
	Texture* texArray[MAXTEXTURECOUNT]; //maximum of 4 textures
	for(int i =0; i<MAXTEXTURECOUNT;++i){
		texArray[i] =GpuTexture::DefaultTexture(); //set to pink default
	}

	//load all Textures associated with this
	for(int i =0; i<model->numTextures;++i){
		//load texture into AssetManager
		AssetManager::LoadTexture(model->getTextureName(i),model->getTextureName(i));

		//keep track of textures
		texArray[i] = AssetManager::GetTexture(model->getTextureName(i));
	}

	//store graphics object before sending it out.

	GraphicsObjectFlatTexture* temp = new GraphicsObjectFlatTexture(model,texArray[0],texArray[1],texArray[2],texArray[3]);
	this->allGraphicsObjects.push_back(temp);
	return (GraphicsObjectFlatTexture*)temp;
}

GraphicsObjectFlatTexture* GraphicsObjectMaker::privCreateGraphicsObjectFlatTexture(Model* model,Texture* t1,Texture* t2 ,Texture* t3 ,Texture* t4 ){

	
	GraphicsObjectFlatTexture* temp = new GraphicsObjectFlatTexture(model,t1,t2,t3,t4);
	this->allGraphicsObjects.push_back(temp);
	return (GraphicsObjectFlatTexture*)temp;
	
}

GraphicsObjectWireFrame* GraphicsObjectMaker::privCreateGraphicsObjectWireFrame(Model* model){

	GraphicsObjectWireFrame* temp  = new GraphicsObjectWireFrame(model);
	this->allGraphicsObjects.push_back(temp);
	return (GraphicsObjectWireFrame*) temp;

}

GraphicsObjectWireFrame* GraphicsObjectMaker::privCreateGraphicsObjectWireFrame(Model* model,Vect& color ){

	//Store Created Object
	GraphicsObjectWireFrame* temp = new GraphicsObjectWireFrame(model);
	temp->color = color;
	return (GraphicsObjectWireFrame*)temp;

}

GraphicsObjectColorNoTexture* GraphicsObjectMaker::privCreateGraphicsObjectColorNoTexture(Model* model){

	GraphicsObject* temp = new GraphicsObjectColorNoTexture(model);
	this->allGraphicsObjects.push_back(temp);
	return (GraphicsObjectColorNoTexture*) temp;

}

void GraphicsObjectMaker::privRemoveGraphicsObject(GraphicsObject* grphObj){
	
	list<GraphicsObject*>::iterator it =allGraphicsObjects.begin();
	while(it!=allGraphicsObjects.end()){
		if(*it == grphObj){
			delete (*it);
			allGraphicsObjects.remove(grphObj);
			return;
		}
		++it;
	}
}

//=====================
// Static Definitions
//=====================

GraphicsObjectFlatTexture* GraphicsObjectMaker::CreateGraphicsObjectFlatTextureWithDefaults(Model* model){
	return Instance()->privCreateGraphicsObjectFlatTextureWithDefaults(model);
}

GraphicsObjectFlatTexture* GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(Model* model,Texture* t1,Texture* t2 ,Texture* t3 ,Texture* t4 ){
	return Instance()->privCreateGraphicsObjectFlatTexture(model,t1,t2,t3,t4);
}

GraphicsObjectWireFrame* GraphicsObjectMaker::CreateGraphicsObjectWireFrame(Model* model){
	return Instance()->privCreateGraphicsObjectWireFrame(model);
}

GraphicsObjectWireFrame* GraphicsObjectMaker::CreateGraphicsObjectWireFrame(Model* model,Vect& color ){
	return Instance()->privCreateGraphicsObjectWireFrame(model,color);
}

GraphicsObjectColorNoTexture* GraphicsObjectMaker::CreateGraphicsObjectColorNoTexture(Model* model){
	return Instance()->privCreateGraphicsObjectColorNoTexture(model);
}

void GraphicsObjectMaker::RemoveGraphicsObject(GraphicsObject* grphObj){
	Instance()->privRemoveGraphicsObject(grphObj);
}

void GraphicsObjectMaker::DeleteMe(){
	delete instance;
}

