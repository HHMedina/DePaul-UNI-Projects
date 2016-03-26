#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <Azul.h>
#include <map>
#include "GraphicsObjectMaker.h"

using namespace std;

class SceneManager;

class Drawable{
	friend class DrawableManager;

public:

	/**\brief Registers Drawable For Drawing.
	\ingroup DRAWING
	This function registers a Drawable for Drawing.

	\par Example:
	The following code shows setting up a ship for drawing in it's constructor;
	\code
	Ship::Ship(){

		//set's the gameObject's graphicsObject
		SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShip"),AssetManager::GetTexture("ShipTex")));
	
		//sets up initial rotation,scale and position in the world
		Matrix world = Matrix();
		Matrix ShipScale = Matrix();
		Matrix ShipRot = Matrix();
		Vect ShipPos;
		world.set(IDENTITY);
		ShipScale.set( SCALE, 0.5f, 0.5f, 0.5f);
		ShipRot.set( ROT_XYZ, 0, 0, 0);
		ShipPos.set(0, 20, -40);
		world *= ShipScale * ShipRot * Matrix( TRANS, ShipPos );

		SetGameObjectWorld(world); // set GameObject's (ship) graphic model and collision model's world transform to world;
	
		this->RegisterToDraw();

	}
	\endcode
	\note In order to actually draw you must call RenderGraphics() in Ship::Draw() just as below.
	\code
	 void Ship::Draw(){
		RenderGraphics();
	}


	\endcode

	\note GameObjects are set to automatically register to draw upon creation, but the example illustrates how to register for drawing if need be.
	*/
	void RegisterToDraw();
	
	/**\brief Deregisters Drawable For Drawing.
	\ingroup DRAWING
	This function Deregisters a Drawable for Drawing.

	\par Example:
	The following code shows Deregistering drawing for a ninja when picking up a stealth power up.
	\code
	void Ninja::Collision(StealthPowerUp* other){
	
		this->DeregisterToDraw();// make ninja invisible

		other->MarkForDestroy(); // destroy powerup
		
	}
	\endcode
	
	*/
	void DeregisterToDraw();
		

	/**\brief Method for setting the visual representation of a Drawable
	\ingroup DRAWING
	This function sets the visual representation (data member graphicsObject ) of the drawable.
	
	\note This function calls delete on previous GraphicsObject to free up memory.

	\param newGObj newGObj is the GraphicsObject that the Drawable's graphicsObject member will be set to.

	\par Example:
	The following code shows a Ninja's visual representation being set to a GraphicsObject with a Fat Ninja Model and Fat Ninja Texture.
	\code
	Ninja::Ninja(){

		//sets the graphical representation only
		SetGraphicsObject( GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("FatNinjaModel"),AssetManager::GetTexture("FatNinaTexture")));
	}
	\endcode
	
	Remember to call the actual draw, like so!

	\code
	void Ninja::Draw(){
		RenderGraphics(); //renders the ninja which has a fatninja model and texture
	}
	\endcode
	
	*/
	void SetGraphicsObject(GraphicsObject* newGObj){
		if(graphicsObject!=NULL){
			GraphicsObjectMaker::RemoveGraphicsObject(graphicsObject);
		}//frees up previous space
		graphicsObject = newGObj;
	}

	
protected:

	Drawable(){
		//Set graphicsObject to Null
		graphicsObject =NULL;
	}
	Drawable( const Drawable & rhs){ rhs;}
	Drawable& operator = (const Drawable& rhs){ rhs;}
	virtual ~Drawable(){
		//does not delete graphicsObject because it's dereference is not owned by
		//the Drawable. It is owned by whoever sets it.
	}

	


	/**\brief Method for drawing graphics objects.
	\ingroup DEFAULTEVENTS
	This function allows the drawing of graphics objects. Every Drawable has the ability to draw things onto the screen, but is usually
	called to draw it's graphicsObject member.

	\par Example:
	The following code shows a Ship drawings its graphicsObject member (visual representaiton of itself).
	\code
	void Ship::Draw(){
		RenderGraphics();
	}
	\endcode

	Where, in this case, the graphicsObject has been set in the constructor.

	\code
	Ship::Ship(){

		//sets the graphical representation along with the collider mesh from the graphical object
		SetGraphicAndCollisionModel( GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShip"),AssetManager::GetTexture("ShipTex")));
	}

	\endcode
	
	*/
	virtual void Draw(){}

	/**\brief Method for drawing graphics objects.
	\ingroup DEFAULTEVENTS
	This method calls the visual representation of the gameobject to be drawn onto the screen.

	\par Example:
	The following code shows a Ship calling RenderGraphics() to have its visual representation appear on the screen.
	\code
	void Ship::Draw(){
		RenderGraphics();
	}
	\endcode

	Where, in this case, the graphic of the ship has been set in the constructor.

	\code
	Ship::Ship(){

		//sets the graphical representation along with the collider mesh from the graphical object
		SetGraphicAndCollisionModel( GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShip"),AssetManager::GetTexture("ShipTex")));
	}

	\endcode
	
	*/
	void RenderGraphics(){
		graphicsObject->Render();
	}

	/**
	\brief Visual representation of a Drawable. GraphicsObject class object.
	*/
	GraphicsObject* graphicsObject;


};
#endif