#ifndef SCENE_H
#define SCENE_H

#include <map>
using namespace std;


class Alarmable;

//needed for non pointer declaration of manager members
#include "InputManager.h"
#include "DrawableManager.h"
#include "UpdatableManager.h"
#include "AlarmManager.h"
#include "CollisionManager.h"
#include "TerminableManager.h"
#include "TerminationDumpster.h"
#include "TerrainManager.h"

class Scene{

	friend class SceneManager;

public:
		
	virtual ~Scene();
	

protected:	

	//dissalow creation of scene base class (abstract type)
	Scene();
	Scene( const Scene & rhs){ rhs;}
	Scene& operator = (const Scene& rhs){ rhs;return *this;}

	//Each scene has its own drawable, Inputable, and Updatable Managers

	/**
	\brief A Scene's instance of an UpdatableManager. 
	*/
	UpdatableManager updatableManager;

	/**
	\brief A Scene's instance of a DrawableManager. 
	*/
	DrawableManager drawableManager;

	/**
	\brief A Scene's instance of an InputManager. 
	*/
	InputManager inputManager;

	/**
	\brief A Scene's instance of an AlarmManager. 
	*/
	AlarmManager alarmManager;

	/**
	\brief A Scene's instance of a CollisionManager. 
	*/
	CollisionManager collisionManager;

	/**
	\brief A Scene's instance of a TerminableManager. 
	*/
	TerminableManager terminableManager;

	/**
	\brief A Scene's instance of a TerminationDumpster. 
	*/
	TerminationDumpster terminationDumpster;

	TerrainManager terrainManager;

	/**
	\brief Method which calls the DrawableManager's DrawAll() method.
	*/
	void DrawScene();

	/**
	\brief Method which updates the state of the scene.

	This method calls inputManager.Update(), alarmManager. UpDateAlarms(),
	updatableManager.UpdateAll(), collisionManager.ProcessCollisions(), and
	terminableManager.ProcessTerminations() in that order.
	*/
	void UpdateScene();

	//Members for Register and Deregistering in Managers
	//Note: Private in order to disallow acess to scene besides through
	//scene manager's current scene.

	/**
	\brief Returns the scene's instance of an UpdatableManager.
	*/
	UpdatableManager& GetUpdatableManager();

	TerrainManager& GetTerrainManager();

	/**
	\brief Returns the scene's instance of a DrawableManager.
	*/
	DrawableManager& GetDrawableManager();

	/**
	\brief Returns the scene's instance of an InputManager.
	*/
	InputManager& GetInputManager();

	/**
	\brief Returns the scene's instance of an AlarmManager.
	*/ 
	AlarmManager& GetAlarmManager();

	/**
	\brief Returns the scene's instance of a TerminableManager.
	*/
	TerminableManager& GetTerminableManager();

	/**
	\brief Returns the scene's instance of a TerminationDumpster.
	*/
	TerminationDumpster& GetTerminationDumpster();

	/**
	\brief Returns the scene's instance of a CollisionManager.
	*/
	CollisionManager& GetCollisionManager();

	////new stufffff//////


	void SetCurrentTerrain(const char* const assetName);

	/////

	/**\brief Sets the collision pairs for a scene.
	\ingroup SCENEFUNCTIONS
	This method will allow the user to specify which pair wise collisions should be considered by the collision system.
	This method should be called in the SetUp() method of a derived Scene class.

	\par Example:
	The Following code shows setting a collision pair between WaterBalloon GameObjects and Clown GameObjects.
	\code
	void CarnivalLevel::SetUp(){

   Clown* bobo = New Clown();
   Clown* sadSally = new Clown();

   BadKid* dennis = new BadKid();
   WaterBalloon* wBalloon = new WaterBalloon();
    
    SetCollisionPair<WaterBalloon,Clown>();
}
	\endcode

	\note A user should not set collision pair between  WaterBalloon type and the Clown Type and between the Clown Type and WaterBalloon Type. Only one is needed and should be done.

	*/
	template <class C1, class C2>
	void SetCollisionPair(){
		collisionManager.SetCollisionPair<C1,C2>();
	}


	/**\brief Sets the considerations for Self-class collisions.
	\ingroup SCENEFUNCTIONS
	This method will allow the user to specify that a Collidable of Class C can collide with other Collidables of
	the same class.

	\par Example:
	The Following code shows setting a collision pair between Marble GameObjects.
	\code
	void MarblesGameLevel1::SetUp(){

    Marble* marble1 = new Marble();
    Marble* marble2 = new Marble();
       
    SetCollisionSelf<Marble>(); // make MArbles able to collide with each other.
}
	\endcode
	*/
	template <class C>
	void SetCollisionSelf(){
		collisionManager.SetCollisionSelf<C>();
	}



	template <class C>
	void SetCollisionWithTerrain(){
		collisionManager.SetCollisionWithTerrain<C>();
	}


	/**\brief Function used to initialize a scene.
	\ingroup SCENEFUNCTIONS
	This method is used to initialize a scene, so any GameObjects, or scoring, should be initialized here.
	This method is called when the game starts and when a scene changes, calling the SetUp() of the new scene.

	\par Example:
	The Following code the SetUp() for a scene which is supposed to have a desert terrain and
	a tank (controlled by the player)

	\code
	void DesertLevel::SetUp(){

		Tank* player = new Tank();
		Terrain* desert = new DesertTerrain();
    
		SetCollisionPair<Bullet,Tank>(); // in this level, there will be multiple tanks who shoot each other with bullets
	}
	\endcode

	\note Every derived level is forced to reimplement the SetUp() method.
	*/
	virtual void SetUp()=0;


	/**\brief Callback method called at the end of a scene.
	\ingroup SCENEFUNCTIONS

	This method is a callback that is called when a scene gets ended, whether it be by changing, scenes or ending the game.

	\note The user must reimplement this function if he or she wants actions taken on a scene's end.

	*/
	virtual void OnSceneEnd(){}

};
#endif