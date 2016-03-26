#ifndef TERMINABLE_H
#define TERMINABLE_H

#include <Azul.h>
using namespace std;

class SceneManager;

class Terminable{

	friend class TerminableManager;
	friend class TerminationDumpster;

public:
	typedef void (*ExternalManagerFunction)(Terminable*);


	/**\brief Removes a GameObject from internal Management and into an external manager.
	\ingroup METHODS

	\Note Once GameObjects are set to an external management, they must be registered to Update and
	Draw by the user in their Reinstantiation function!

	Game Objects are cleaned up by an internal function if they are not deleted by the user.
	Even though this is convenient, this may not always be wanted. Sometimes a user will wish
	to manage GameObjects by themselves. This method can be used to set a GameObject to be managed
	by an external Manager.

	\param exFunc exFunc is the function pointer that handles the GameObjects in the external manager.

	\par Example:
	The Following code shows a Recycling Bomb Factory, calling SetExternalManager on the instances
	it created and then setting the ExternalManager to be a function in the factory.

	First.. The .h File.
	\code
	#ifndef RECYCLINGBOMBFACTORY_H
#define RECYCLINGBOMBFACTORY_H

#include <P2Engine.h>
#include <stack>
class Bomb;
class RecyclingBombFactory{


private:
	static RecyclingBombFactory * factoryInstance; //Factory set up as singleton
	RecyclingBombFactory(){} //prevent constructor
	RecyclingBombFactory(RecyclingBombFactory&){} //prevent copy-construction
	RecyclingBombFactory& operator = (const RecyclingBombFactory&){} //prevent assignment
	~RecyclingBombFactory();

	static RecyclingBombFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingBombFactory();
		}
		return *factoryInstance;
	}


	//stack to hold Recycled bombs
	std::stack<Bomb*> recycledItems;
	

public:
	//responsible for spawning bullets
	static Bomb& CreateBomb(float,float,float);
	static void RecycleBomb(Terminable* b);

};



#endif RECYCLINGENEMYEXPLOSIONFACTORY_H
	\endcode

	Then the .cpp file

	\code
	#include "RecyclingBombFactory.h"
#include "Bomb.h"


//declare static members used in .h
RecyclingBombFactory * RecyclingBombFactory::factoryInstance =NULL;

RecyclingBombFactory::~RecyclingBombFactory(){
	//deletes every EnemyExplosion in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}

	delete factoryInstance;
}



Bomb& RecyclingBombFactory::CreateBomb(float xPos,float yPos, float zPos){
	
	cout<<"RBF:Bombs in recycle bin "<<Instance().recycledItems.size()<<endl;
	Bomb* bomb;
		
	//if the recyclebin is empty create a Bomb
	if ( Instance().recycledItems.empty() )
	{
	
		 
		bomb = new Bomb(xPos,yPos,zPos);
			
		// Tell P2Engine to let the GameObject by managed by this factory.
		bomb->SetExternalManagement( RecycleBomb);
	}
	else
	{
		//sets bomb to the top of the recycle stack (grab a previously used bomb)
		bomb = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

	}
	//respawn the bomb
	bomb->Initialize(xPos,yPos,zPos);
	

	return *bomb;
	
}

void RecyclingBombFactory::RecycleBomb(Terminable* bomb)
{
	//adds bomb to recycle stack after casting it to Bomb type
	Instance().recycledItems.push( (Bomb*) bomb);
	
}

	\endcode

	In the Bomb the Initialize() looks like so....

	\code

	void Bomb::Initialize(float x, float y, float z){
		
		//user is responsible for reinitializing registration to
		//both  drawing and updating!
		RegisterToDraw(); 
		RegisterForUpdate();

		SetAlarm(AlarmID_0,10);
		Matrix temp = Matrix();
		bombScale.set( SCALE, 1.0f, 1.0f, 1.0f);
		bombRot.set( ROT_XYZ, 0, 0, 0);
		bombPos = Vect(x,y,z);
		temp = bombScale * bombRot * Matrix( TRANS, bombPos );
		SetGameObjectWorld(temp);
		RegisterCollision(this);
	}

	\endcode


	*/
	void SetExternalManagement(ExternalManagerFunction exFunc);

	/**
	\brief Function that returns wheater the Terminable has an external manager (returning true) or not (returning false.) 
	*/
	bool HasExternalManager();

	/**\brief Mark Terminable for Destruction.
	\ingroup METHODS
	This method begins the process of removing the Terminable from the P2 Engine. This will also Trigger the Terminable's OnTermination() callback.

	\par Example:
	The Following code shows MarkForDestroy() being called on a ship and Asteroid after a ship collides with an asteroid.
	\code
	void Ship::Collision(Asteroid* other){

		this->MarkForDestroy();
		other->MarkForDestroy();

	}
	\endcode

	*/
	void MarkForDestroy();

protected:

	Terminable();
	Terminable( const Terminable & rhs){ rhs;}
	Terminable& operator = (const Terminable& rhs){ rhs;}
	virtual ~Terminable(){}

	/**
	\brief This method sends the Terminable to the ExternalManager via a function pointer
	extFunc.
	*/
	void SendToExternalManager();

	/**
	\brief This method is used to deregister the terminable from Automated functions.

	\note The GameObject class has been Automatically registered for Updating and Drawing,
	therefore DeregisterFromAutomated() must deregister the GameObject from updating and drawing.
	The GameObject class has reimplemented this function.
	*/
	virtual void DeregisterFromAutomated(){}

	/**
	\brief This method is used to register the terminable to Automated functions.

	\note The GameObject class has been Automatically registered for Updating and Drawing,
	therefore RegisterFromAutomated() must Register the GameObject from updating and drawing.
	The GameObject class has reimplemented this function.
	*/
	virtual void RegisterForAutomated(){}

	/**\brief Callback for termination event.
	\ingroup DEFAULTEVENTS
	Upon the destruction of a Terminable via MarkForDestroy, this method is called. A class derived from the Terminable class should implement this method to deregister itself from registered event managers such as the input manager.

	\par Example:
	The following code shows a Ship's (derived from GameObject and therefore Terminable as well...) OnTerminate event method;
	\code
	void Ship::OnTerminate(){

		delete graphicsObject; // clean up any needed memory here
		DeregisterForInput(AZUL_KEY::KEY_H,KeyEvent::KeyDown); //deregister any registered inputs
		DeregisterForUpdate(); // registers the ship from the update Manager

		DeregisterCollision<Ship>(this); //deregister from Collision 

	}
	\endcode

	\note Deregistering from Collision should always be called in the OnTerminate() event;
	*/
	virtual void OnTerminate(){}

	/**
	\brief Bool that holds whether a Terminable is managed Externally or not.
	*/
	bool hasExternalManagement;

	/**
	\brief Bool that holds wheter it has been markedForTermination or not.
	*/
	bool markedForTermination;

	/**
	\brief Function pointer to the function that SendToExternalManager() should send the
	Terminable to.
	*/
	ExternalManagerFunction extFunc;
};
#endif