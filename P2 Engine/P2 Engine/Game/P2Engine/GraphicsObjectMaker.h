#ifndef GRAPHICSOBJECTMAKER_H
#define GRAPHICSOBJECTMAKER_H

#include <Azul.h>
#include <map>
#include <list>
using namespace std;

class GraphicsObjectMaker{

	friend class Game;
	friend class GameObject;
	friend class Drawable;
private:

	/**
	\brief Static pointer to singleton
	*/
	static GraphicsObjectMaker * instance;//This is a singleton
	
	static GraphicsObjectMaker* Instance(){
		if(instance==nullptr){
			instance= new GraphicsObjectMaker();
		}
		return instance;

	}

	GraphicsObjectMaker(){}
	GraphicsObjectMaker( const GraphicsObjectMaker & rhs){ rhs;}
	GraphicsObjectMaker& operator = (const GraphicsObjectMaker& rhs){ rhs;}
	~GraphicsObjectMaker();

	/**
	\brief holds references to all of the grpahics objects created.

	This list holds a list to all of the graphics objects created so that it can clean up all of the graphics objects
	on removal, as well as being able to remove a specific graphics object when a player no longer needs it.
	*/
	std::list<GraphicsObject*> allGraphicsObjects;

	static const int MAXTEXTURECOUNT =4;
	
	//====================================================================
	// Private Methods called by static methods
	//====================================================================

	GraphicsObjectFlatTexture* privCreateGraphicsObjectFlatTextureWithDefaults( Model* model);

	
	GraphicsObjectFlatTexture* privCreateGraphicsObjectFlatTexture( Model* model,Texture* t1 =NULL,Texture* t2 =NULL,Texture* t3 =NULL,Texture* t4 =NULL);
	
	
	GraphicsObjectWireFrame* privCreateGraphicsObjectWireFrame(Model* model);

	
	GraphicsObjectWireFrame* privCreateGraphicsObjectWireFrame(Model* model,Vect& color);

	
	GraphicsObjectColorNoTexture* privCreateGraphicsObjectColorNoTexture(Model* model);

	void privRemoveGraphicsObject(GraphicsObject* grphObj);

	//==========================================================================
	// Should be private since users should not have to worry about removing from
	// the list of graphics object. If the user uses "new" to create a graphics
	// object, he or she is now in charge of managing the memory of the 
	// graphicsObject. This Method is used by the Internal Engine.
	//===========================================================================
	
	/**
	\brief This method removes and deletes a GraphicsObject from the GraphicsObjectMaker.

	This method is called by the internal engine when it sets a graphicsObject with the supplied 
	methods for setting a graphicsObject. 
	*/
	static void RemoveGraphicsObject(GraphicsObject* grphObj);
	
	//=====================================================================
	// Used to Call delete on Singleton
	//=====================================================================

	/**
	\brief Used by Game to delete the GraphicsObjectMaker
	*/
	static void DeleteMe();

public:
	
	//======================================================
	//	These functions are used to create graphical objects
	//======================================================

	/**
	\ingroup GRAPHICSOBJECTMAKER
	\brief Creates and returns a textured GraphicsObject with the model's default textures.
		
		This function will create and return a GraphicsObject with the default textures that were stated in the
		model.

	\note the GraphicObjectMaker keeps a reference to all of the graphicsObjects it makes so a user is not required to 
	delete graphicsObjects made with this function. Therefore, it is recommended that a user use this function, instead of
	creating a graphicsObject with new, because then memory management becomes the responsibility of the user!

	\param model model is the Model that will be used along with its default textures to return a GraphicsObject.

	\par The following example shows a clown class setting its graphics and collisionModel.

	\code

	Clown::Clown(){

		SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTextureWithDefaults(AssetManager::GetModel("ClownModel")));
		
		//setting world omitted.
		//registering for collision omitted
	}

	void Clown::Draw(){
		RenderGraphics(); //dont forget to draw it!
	}

	Clown::~Clown(){
		//GameObject is not responible for deleting its graphics object because this
		function was used!
	}

	\endcode
	*/
	static GraphicsObjectFlatTexture* CreateGraphicsObjectFlatTextureWithDefaults( Model* model);

	/**
	\ingroup GRAPHICSOBJECTMAKER
	\brief Creates and returns a textured GraphicsObject with user specified textures.
		
		This function will create and return a GraphicsObject with textures that a user specifies.


	\note the GraphicObjectMaker keeps a reference to all of the graphicsObjects it makes so a user is not required to 
	delete graphicsObjects made with this function. Therefore, it is recommended that a user use this function, instead of
	creating a graphicsObject with new, because then memory management becomes the responsibility of the user!

	\param model model is the Model that will be used to represent the graphicsObject in 3D space.
	\param t1 t1 is the Texture that will be used for the first slot of the model's textures.
	\param t2 t2 is the Texture that will be used for the second slot of the model's textures.
	\param t3 t3 is the Texture that will be used for the third slot of the model's textures.
	\param t4 t4 is the Texture that will be used for the fourth slot of the model's textures.

	\par The following example shows a clown class setting its graphics and collision model by getting a 
	textured GameObject with a sad clown texture, instead of the default (happy clown).

	\code

	Clown::Clown(){

		SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("ClownModel"),AssetManager::GetTexture("SadClownTexture")));
		
		//setting world omitted.
		//registering for collision omitted
	}

	void Clown::Draw(){
		RenderGraphics(); //dont forget to draw it!
	}

	Clown::~Clown(){
		//GameObject is not responible for deleting its graphics object because this
		function was used!
	}

	\endcode
	*/
	static GraphicsObjectFlatTexture* CreateGraphicsObjectFlatTexture( Model* model,Texture* t1,Texture* t2 =NULL,Texture* t3 =NULL,Texture* t4 =NULL);
	
	/**
	\ingroup GRAPHICSOBJECTMAKER
	\brief Creates and returns a wireframe GraphicsObject with a default blue color.
	
	\note the GraphicObjectMaker keeps a reference to all of the graphicsObjects it makes so a user is not required to 
	delete graphicsObjects made with this function. Therefore, it is recommended that a user use this function, instead of
	creating a graphicsObject with new, because then memory management becomes the responsibility of the user!

	\param model model is the Model that will be used to represent the graphicsObject in 3D space.


	\par The following example shows a clown class setting its graphics and collision model by getting a 
	wireframe GameObject of a clown model.

	\code

	Clown::Clown(){

		SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectWireFrame(AssetManager::GetModel("ClownModel")));
		
		//setting world omitted.
		//registering for collision omitted
	}

	void Clown::Draw(){
		RenderGraphics //dont forget to draw it!
	}

	Clown::~Clown(){
		//GameObject is not responible for deleting its graphics object because this
		function was used!
	}

	\endcode
	*/
	static GraphicsObjectWireFrame* CreateGraphicsObjectWireFrame(Model* model);

	/**
	\ingroup GRAPHICSOBJECTMAKER
	\brief Creates and returns a wireframe GraphicsObject with a user specified color.
		
	\note the GraphicObjectMaker keeps a reference to all of the graphicsObjects it makes so a user is not required to 
	delete graphicsObjects made with this function. Therefore, it is recommended that a user use this function, instead of
	creating a graphicsObject with new, because then memory management becomes the responsibility of the user!

	\param model model is the Model that will be used to represent the graphicsObject in 3D space.
	\param color color is the Vect that will be used to set the color of the wireframe.

	\par The following example shows a clown class setting its graphics and collision model by getting a 
	wireframe GameObject of a clown model with green lines.

	\code

	Clown::Clown(){

		SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectWireFrame(AssetManager::GetModel("ClownModel"), Vect(0,1,0) ));
		
		//setting world omitted.
		//registering for collision omitted
	}

	void Clown::Draw(){
		RenderGraphics(); //dont forget to draw it!
	}

	Clown::~Clown(){
		//GameObject is not responible for deleting its graphics object because this
		function was used!
	}

	\endcode
	*/
	static GraphicsObjectWireFrame* CreateGraphicsObjectWireFrame(Model* model,Vect& color);

	/**
	\ingroup GRAPHICSOBJECTMAKER
	\brief Creates and returns a solidColor GraphicsObject without textures.
	
	\note the GraphicObjectMaker keeps a reference to all of the graphicsObjects it makes so a user is not required to 
	delete graphicsObjects made with this function. Therefore, it is recommended that a user use this function, instead of
	creating a graphicsObject with new, because then memory management becomes the responsibility of the user!

	\param model model is the Model that will be used to represent the graphicsObject in 3D space.

	\par The following example shows a clown class setting its graphics and collision model by getting a 
	solidColor GameObject of a clown Model.

	\code

	Clown::Clown(){

		SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectColorNoTexture(AssetManager::GetModel("ClownModel") ));
		
		//setting world omitted.
		//registering for collision omitted
	}

	void Clown::Draw(){
		RenderGraphics(); //dont forget to draw it!
	}

	Clown::~Clown(){
		//GameObject is not responible for deleting its graphics object because this
		function was used!
	}

	\endcode
	*/
	static GraphicsObjectColorNoTexture* CreateGraphicsObjectColorNoTexture(Model* model);

};



#endif