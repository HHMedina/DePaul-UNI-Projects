#include "AssetManager.h"
#include <iostream>
#include <string>
#include <assert.h>


AssetManager * AssetManager::instance =NULL;

/*
These functions are related to Model Handling
*/
void AssetManager::LoadModel(const char* const fileName,const char* const assetName){
	
	/*
		This Section checks to see if there is already in the element of the map
		with they key and then deletes it. Maps have elements with unique keys and
		will replace the element with the new item. Due to the elements being pointer
		the memory needs to be freed before pointer reassignment.
	*/
	Model * tempModel = Instance()->mDataBase[assetName];

	if(tempModel != NULL){ 
		
		delete tempModel; //clean up memory (override)
		tempModel=NULL;
		
	}

	//creates an Azul Model from an FBX file (will override Previous Model)
	string command ="ConverterDebug " +string(fileName) + " > DebugInfo.txt" ;
	system(command.c_str());
	
	//Creates the Azul file (with extenstion for azul model) and puts it into asset
	//Database
	string azulFormatName = fileName;
	azulFormatName.erase(strlen(fileName)-3,strlen(fileName)-1);
	azulFormatName+="azul";
	Instance()->mDataBase[assetName] = GpuModel::Create(azulFormatName.c_str());
	//there is no way to really give back an error since Azul asserts and gives back
	//vague message in console.
}

Model * AssetManager::GetModel(const char* const assetName){

	Model* tempModel = Instance()->mDataBase[assetName];
	
	assert(tempModel && "AssetManager:GetModel: Model could not be Found. Retrieval failed.");
	
	return tempModel;
	
}

/*
These functions are related to Texture Handling
*/
void AssetManager::LoadTexture(const char* const fileName,const char* const assetName){

	Texture * tempTex = Instance()->tDataBase[assetName];

	

	if(tempTex != NULL){
		delete tempTex; //clean up memory (override)
		tempTex=NULL;
	}
	Instance()->tDataBase[assetName] = GpuTexture::Create(fileName);

	//there is no way to really give back an error since Azul asserts and gives back
	//vague message in console.
}

Texture * const AssetManager::GetTexture(const char* const assetName){
	Texture* tempTex = Instance()->tDataBase[assetName];
	
	assert(tempTex!=NULL && "AssetManager:GetTexture: Texture Could not be retrieved. Not Found.");

	return tempTex;
	
}

void AssetManager::UnloadAssets(){

	//deallocates the models and textures
	map<string,Model*,comparer>::iterator mIt = Instance()->mDataBase.begin();
	while(mIt!=Instance()->mDataBase.end()){
		delete mIt->second;
		mIt = Instance()->mDataBase.erase(mIt);
	}

	map<string,Texture*,comparer>::iterator tIt = Instance()->tDataBase.begin();
	while(tIt!=Instance()->tDataBase.end()){
		delete tIt->second;
		tIt = Instance()->tDataBase.erase(tIt);
	}

}

void AssetManager::DeleteMe(){
	Instance()->UnloadAssets();
	delete instance;
}

AssetManager::~AssetManager(){
	
	instance =NULL;

}