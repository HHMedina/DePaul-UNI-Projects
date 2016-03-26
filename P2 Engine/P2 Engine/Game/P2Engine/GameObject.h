#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Collidable.h"
#include "Inputable.h"
#include <Updatable.h>
#include <Drawable.h>
#include <Terminable.h>
#include <Alarmable.h>
#include "SceneManager.h"
#include "GraphicsObjectMaker.h"

class GameObject:public Collidable, 
				public Inputable,
				public Updatable,//
				public Drawable,//
				public Alarmable,
				public Terminable{//


public:

	
	/**
	\ingroup METHODS
	\brief This method is used to set the world matrices for both the collision model and the graphicsObject 

	This method is used to set the world matrices for both the collision model and the graphicsObject.
	This allows the user to set the graphics and collision mesh at the same time. 

	\param newWorld is the new world matrix for the collision model and graphics object to be set to.
	*/
	void SetGameObjectWorld(Matrix& newWorld ){
		if(graphicsObject!=NULL){
			graphicsObject->setWorld(newWorld);
		}
		if(this->collisionModel!=NULL){
			colWorld = newWorld;
		}
	}

	/**
	\ingroup METHODS
	\brief This method allows the user to set the graphicsObject and Collision Model from the same graphics model passed in.

	Most of the time, a player will use the a graphicsObject to represent the gameobject visually as well as using the same model for
	collisions, so this method allows the user to set both from the same graphicsObject passed in. This calls delete on the previous
	graphicsObject of the gameObject.

	\param gObj gObj is the graphicsObject to set the graphicsObject of the GameObject to. This also passes in the model for the collision mesh.
	*/
	void SetGraphicAndCollisionModel(GraphicsObject* gObj){
		if(graphicsObject!=NULL){
			GraphicsObjectMaker::RemoveGraphicsObject(graphicsObject);
		}//frees up previous space
		graphicsObject = gObj;
		collisionModel = gObj->getModel();
	}

protected:

	GameObject(){
		RegisterForAutomated();
		graphicsObject=NULL;
		collisionModel=NULL;
	}
	GameObject( const GameObject & rhs){ rhs;}
	GameObject& operator = (const GameObject& rhs){ rhs;}
	virtual ~GameObject(){}
			
		
	//Alarms
	//virtual void Alarm0(){}
	//virtual void Alarm1(){}
	//virtual void Alarm2(){}

	/**
	\brief This method is called by the internal engine to automatically deregister the GameObject from the automated
	events. This also cancels all alarms in case the GameObject dies before an alarm is set off.

	*/
	void DeregisterFromAutomated(){
		CancelAlarm(AlarmID_0);
		CancelAlarm(AlarmID_1);
		CancelAlarm(AlarmID_2);
		SceneManager::DeregisterForUpdating(this);
		SceneManager::DeregisterForDrawing(this);
	}

	/**
	\brief This method is called by the internal engine to automatically register the GameObject to the automated
	events.

	*/
	void RegisterForAutomated(){
		SceneManager::RegisterForUpdating(this);
		SceneManager::RegisterForDrawing(this);
	}

	

};




#endif

