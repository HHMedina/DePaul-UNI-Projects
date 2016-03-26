#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <list>

class Alarmable;
class Scene;
class Updatable;
class Drawable;
class Inputable;
class Terminable;
class Terrain;
enum AlarmID;
enum AZUL_KEY;
enum KeyEvent;

using namespace std;

class SceneManager{

	//allow private access to following classes only
	friend class TerminableManager;
	friend class Terminable;
	friend class Game;

public:
		
	
	/**
	\ingroup SCENEFUNCTIONS
	\brief methos called to set the first scene of the game.

	This method is used by the user to specify which scene should be launched when
	the game starts.

	\param targetScene targetScene is the scene that should be set as the start scene.

	\par An example of setting a scene is seen below.

	\code
	void Game::LoadGameContent(){
	// Model loading ---------------------------------------

		//Model loading omitted.

	// Texture Loading  --------------------------------

		//texture loading omitted.

		SceneManager::SetStartScene(new TutorialLevel1() );
	
}


	\endcode

	*/
	static void SetStartScene(Scene* targetScene);

	/**\brief Function used to change scenes.
	\ingroup SCENEFUNCTIONS
	This method is used to change to the next scene, by passing in what scene to go to.

	\param nScene nScene is pointer to a Scene that will be changed into.

	\par Example:
	The following code changes level when a Player (class) collides with a door.

	\code
	void Player::Collision(Door* other){

		SceneManager::ChangeScene( new HospitalInterior() );
	}
	\endcode

	*/
	static void ChangeScene(Scene* nScene);


	////////////////////////new stufff//////////////


	static void SetCurrentTerrain(const char* const);


	template<class C1>
	static void SetCollisionWithTerrain(){
		Instance().currentScene->GetCollisionManager().SetCollisionWithTerrain<C1>();
	}

	static Terrain* GetCurrentTerrain();

	////////////////////////////////////

	/**
	\brief Method used to register an Updatable in the current scene

	This method registers an Updatable, by calling the current scene's
	UpdatableManager's register().

	\param u u is the updatable to be registered for updating.
	*/
	static void RegisterForUpdating(Updatable* u);

	/**
	\brief Method used to deregister an Updatable in the current scene

	This method deregisters an Updatable, by calling the current scene's
	UpdatableManager's Deregister().

	\param u u is the updatable to be registered for updating.
	*/
	static void DeregisterForUpdating(Updatable* u);

	/**
	\brief Method used to register an Alarm in the current scene for an Alarmable.

	This method registers an Alarm for an Alarmable, by calling the current scene's
	AlarmManager's register().

	\param ptr ptr is the Alarmable whose alarm will be registered.
	\param id id is the Alarm number that will be registered.
	\param s s is the amount the alarm will be set to.
	*/
	static void RegisterForAlarm(Alarmable* ptr,AlarmID id,float s);

	/**
	\brief Method used to deregister an Alarm in the current scene for an Alarmable.

	This method deregisters an Alarm for an Alarmable, by calling the current scene's
	AlarmManager's deregister().

	\param ptr ptr is the Alarmable whose alarm will be deregistered.
	\param id id is the Alarm number that will be deregistered.

	*/
	static void DeregisterForAlarm(Alarmable* ptr,AlarmID id);
	
	//static void GetTimeRemainingOfAlarm(Alarmable* ptr,AlarmID id);

	static void AddTimeToAlarm(Alarmable* ptr,AlarmID id,float timeToAdd);

	static void SubtractTimeFromAlarm(Alarmable* ptr,AlarmID id,float timeToSubtract);

	/**
	\brief Method used to register a Drawable in the current scene.

	This method registers a Drawable, by calling the current scene's
	DrawableManager's register().

	\param d d is the Drawable to be registered.
	*/
	static void RegisterForDrawing(Drawable* d);

	/**
	\brief Method used deregister a Drawable in the current scene.

	This method deregisters a Drawable, by calling the current scene's
	DrawableManager's deregister().

	\param d d is the Drawable to be deregistered.
	*/
	static void DeregisterForDrawing(Drawable* d);

	/**
	\brief Method used register an Inputable in the current scene.

	This method registers an Inputable, by calling the current scene's
	InputManager's register().

	\param in in is the Inputable to be registered.
	\param key key is the key to register for the Inputable.
	\param keyEvent keyEvent is the KeyEvent to register for.
	*/
	static void RegisterForInput(Inputable * in, AZUL_KEY key, KeyEvent keyEvent  );

	/**
	\brief Method used deregister an Inputable in the current scene.

	This method deregisters an Inputable, by calling the current scene's
	InputManager's deregister().

	\param in in is the Inputable to be deregistered.
	\param key key is the key to be deregistered for the Inputable.
	\param keyEvent keyEvent is the KeyEvent to deregister for.
	*/
	static void DeregisterForInput(Inputable * in, AZUL_KEY key , KeyEvent keyEvent );

	static void DeregisterAllInputKeys(Inputable * in);

	/**
	\brief Method used register a Terminable in the current scene.

	This method registers a Terminable, by calling the current scene's
	TerminableManager's register().

	\param ptr ptr is the Terminable to be registered for termination.
	*/
	static void RegisterForTermination(Terminable * ptr);
	
	
	/**
	\brief Method used register a CollisionPair in the current scene.

	This method registers a CollisionPair, by calling the current scene's
	SetCollisionPair();
	*/
	template<class C1, class C2>
	static void SetCollisionPair(){
		Instance().currentScene->GetCollisionManager().SetCollisionPair<C1,C2>();
	}

	template<class C>
	static void SetCollisionSelf(){
		Instance().currentScene->GetCollisionManager().SetCollisionSelf<C>();
	}

private:

	SceneManager(){currentScene=NULL;nextScene=NULL;}
	SceneManager( const SceneManager & rhs){ rhs;}
	SceneManager& operator = (const SceneManager& rhs){rhs;}
	~SceneManager();

	static SceneManager& Instance(){
		if(!instance){
			instance = new SceneManager();
		}
		return *instance;
	}

	/**
	\brief This is a singleton.
	*/
	static SceneManager* instance;//this is a singleton

	/**
	\brief pointer to the currentScene (active scene) of the game.
	*/
	Scene* currentScene;

	/**
	\brief pointer to the next scene.

	This pointer is used in determining if the SceneManager should switch levels.
	*/
	Scene* nextScene;

	/**
	\brief used to clean up the current scene.

	This method is used to clean up the active scene (current scene) when the game ends.
	*/
	void CleanUpCurrentScene();

	/**
	\brief This method is used to call the current scene's DrawScene().
	*/
	static void DrawCurrentScene();

	/**
	\brief This method is used to call the current scene's UpdateScene().
	*/
	static void UpdateCurrentScene();

	/**
	\brief This method is used to register a Terminable to the current scene's 
	TerminationDumpster.

	\param ptr ptr is Terminable to be registered in the current scene's TerminationDumpster
	*/
	static void RegisterInTerminationDumpster(Terminable * ptr);

	/**
	\brief This method is used to deregister a Terminable to the current scene's 
	TerminationDumpster.

	\param ptr ptr is Terminable to be deregistered in the current scene's TerminationDumpster
	*/
	static void DeregisterFromTerminationDumpster(Terminable * ptr);
	
	/**
	\brief this method starts the process of cleaning the memory used in the SceneManager
	as well as deleting the current scene.
	*/
	static void DeleteMe();

	/**
	\brief This method is used to call the Current Scene's TerminationDumpster and
	immediately destroy and object (release the memory used for it.)

	\param ptr ptr is the Terminable to be immediately Destroyed.

	*/
	static void DestroyNow(Terminable * ptr);
};




#endif