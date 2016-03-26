#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <Azul.h>
#include <map>

using namespace std;

class AssetManager{

	static struct comparer
	{
		public:
		bool operator()(const std::string x, const std::string y)
		{
			return x.compare(y)>0;
		}
	};

	friend class Game;

private:

	
	/**
	\brief Static pointer to singleton
	*/
	static AssetManager * instance;//This is a singleton

	/**
	\brief Map that acts as a database for storing Models
	*/
	map<string, Model*,comparer> mDataBase;
	
	/**
	\brief Map that acts as a database for storing Textures
	*/
	map<string, Texture*,comparer> tDataBase;

	static AssetManager* Instance(){
		if(instance==nullptr){
			instance= new AssetManager();
		}
		return instance;

	}

	AssetManager(){}
	AssetManager( const AssetManager & rhs){ rhs;}
	AssetManager& operator = (const AssetManager& rhs){ rhs;}
	~AssetManager();

	/**
	\brief This function calls delete on all assets inside of the texture database and
	the model database.
	*/
	void UnloadAssets();
	
	/**
	\brief This function calls UnloadAssets, deletes itself by calling delete on the static pointer,
	and sets the pointer instance to NULL, in order to allow reinstantiation of singleton.
	*/
	static void DeleteMe();

public:
	
	/**
	\ingroup ASSETMANAGER

	\note an asset should be loaded by using the P2EngineContent.cpp's LoadGameContent() located in the User Files folder.
	\brief This method loads a file into the game.
	
	This method loads a model located in the "assets" folder and then loads it into the game's
	asset database as a Model.
	
	\param fileName fileName is the name (with extension) of the file to be loaded.
	\param assetName assetName is the name the user sets to the asset, which will be used when loading assets.


	\par Example:

	The following is an example of a user loading a ship model and texture, as well as a turtle model and texture.

	\code

	void Game::LoadGameContent(){ //located in P2EngineContent.cpp in the User Files Folder.
	// Model loading ---------------------------------------

		// Load external models -------------------------------
		AssetManager::LoadModel("space_frigate.fbx","SpaceShipModel");
		AssetManager::LoadModel("turtle.fbx","TurtleModel");
		
		// Texture Loading  --------------------------------
		AssetManager::LoadTexture("space_frigate.tga","ShipTexture");
		AssetManager::LoadTexture("turtle.tga","TurtleTexture");

		SceneManager::SetStartScene(new  Level1()); // set the starting scene of the game. Important! See the Scene Module.
	
	}
	\endcode

	*/
	static void LoadModel(const char* const fileName,const char* const  assetName);
	
	/**
	\ingroup ASSETMANAGER

	\brief This method gets a pointer to the wanted Model asset.
	
	This method returns a pointer to a Model which is retrived via assetname.
	If this method does not find the assset, it will fail. If it does, check to see
	if you have loaded the asset correctly.
	
	\param assetName assetName is the name of the Model the user wants a reference to.


	\par Example:

	The following is an example of a user making a graphicsObject out of a model and a texture
	to visually represent a Ship GameObject.

	\code

	Ship::Ship(){

		SetGraphicsObject( GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShipModel"),AssetManager::GetTexture("ShipTexture")));

	}
	\endcode

	*/
	static Model* GetModel(const char* const assetName);
		

	/**
	\ingroup ASSETMANAGER

	\note an asset should be loaded by using the P2EngineContent.cpp's LoadGameContent() located in the User Files folder.
	\brief This method loads a file into the game.
	
	This method loads a image file located in the "assets" folder and then loads it into the game's
	asset database as a texture.
	
	\param fileName fileName is the name (with extension) of the file to be loaded.
	\param assetName assetName is the name the user sets to the asset, which will be used when loading assets.


	\par Example:

	The following is an example of a user loading a ship model and texture, as well as a turtle model and texture.

	\code

	void Game::LoadGameContent(){ //located in P2EngineContent.cpp in the User Files Folder.
	// Model loading ---------------------------------------

		// Load external models -------------------------------
		AssetManager::LoadModel("space_frigate.fbx","SpaceShipModel");
		AssetManager::LoadModel("turtle.fbx","TurtleModel");
		
		// Texture Loading  --------------------------------
		AssetManager::LoadTexture("space_frigate.tga","ShipTexture");
		AssetManager::LoadTexture("turtle.tga","TurtleTexture");

		SceneManager::SetStartScene(new  Level1()); // set the starting scene of the game. Important! See the Scene Module.
	
	}
	\endcode

	*/
	static void LoadTexture(const char* const fileName,const char* const  assetName);
	
	/**
	\ingroup ASSETMANAGER

	\brief This method gets a pointer to the wanted Texture asset.
	
	This method returns a pointer to a Texture which is retrived via asset name.
	If this method does not find the assset, it will use a purple texture instead.
	
	\param assetName assetName is the name of the Texture the user wants a reference to.

	\note For an examaple, please see GetModel() , in which the user uses GetModel() and GetTexture()
	to set the visual representation of a Ship GameObject.

	*/
	static Texture* const GetTexture(const char* const assetName);

};



#endif