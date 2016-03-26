/** \defgroup P2-API P2 API
\brief These commands are available to P2 Engine users
*/

/** \defgroup GAMEOBJECT GameObject
\ingroup P2-API
\brief All game entities will be derived from the GameObject class.
\note A GameObject is Derived from Terminable, Collidable, Updatable, Inputable, Drawable, and Alarmable, and will therefore absorb all of its methods.
*/

/** \defgroup AUDIBLE AudibleFunctions
\ingroup GAMEOBJECT
\brief These functions are relevant to anything that derived from the audible class
\note GameObjects are derived from Audible. Also, Audible sound's position is automated.

The following is an example of a missle that makes a whirling sound in the air.

\code
class Missle:public GameObject{
public:
	Missle(){
		SetAudibleSound("WhirlSound");
		SetAudibleLooped(true);//looping whirl
		PlayAudibleSound();//play upon creation
				
		//irrelevant functions omitted...
	}
	
	void Update(){
		misslePos += Vect(0,2,0); //missle moving up

		//since this sound is done via Audible Sound...position is automated!
		//whirlSound->SetSoundPosition(misslePos);//not needed!

		SetGameObjectWorld(misslePos,Vect(1.0f,1.0f,1.0f),Vect(0,0,0)); //sound set via this function!
	}

	void OnTerminate(){
		//whirlSound->Release(); Audible takes care of releasing its own sound! Not Needed!
	}

	Sound3D* whirlSound;

	Vect misslePos;
};

\endcode

*/

/** \defgroup SNDCON SoundConscious
\ingroup GAMEOBJECT
\brief These functions are relevant to anything that is is consicous to sound. (derived from SoundConscious)
*/



/** \defgroup SOUND Sound
\ingroup P2-API
\brief This module teaches the user how to use sound in the P2 Engine.
*/

/** \defgroup SOUNDSYS SoundSystemFunctions
\ingroup SOUND
\brief This module shows the global SoundSystem Functions.
*/


/** \defgroup USINGSOUNDS UsingSounds
\ingroup SOUND
\brief This module teaches the user how to use Sounds in the P2 Engine.
*/

/** \defgroup BOTHSOUNDS BothSoundTypes
\ingroup USINGSOUNDS
\brief These are the relevant features for both SoundTypes.
*/

/** \defgroup SOUND2D Sound2D
\ingroup USINGSOUNDS
\brief Sound2Ds are used for sounds played in 2D space.
*/

/** \defgroup SOUND3D Sound3D
\ingroup USINGSOUNDS
\brief Sound3Ds are used for sounds played in 3D space.
*/

/** \defgroup SOUND2DEX Sound2DExample
\ingroup USINGSOUNDS
\brief The following shows how to use Sound2D's.

The following shows an example of a robot making a 2D beep sound when the user presses they key 'P'.

\code

class Robot:public GameObject{
public:
	Robot(){
	
		beepSound = AssetManager::GetSound2DInstance("BeepSound");
		
		RegisterForInput(AZUL_KEY::KEY_P);
		
		//irrelevant functions omitted...
	}


	void KeyDown(AZUL_KEY key){
			
		if(key == AZUL_KEY::KEY_P){
			beepSound->Play();
		}
	}

	void OnTerminate(){
		beepSound->Release(); //dont forget to release the sound upon death
	}

	Sound2D* beepSound;
};

\endcode

*/

/** \defgroup SOUND3DEX Sound3DExample
\ingroup USINGSOUNDS
\brief The following shows how to use Sound3D's.

The following shows an example of a missle making a sound as it travels through the air.

\code

class Missle:public GameObject{
public:
	Missle(){
	
		whirlSound = AssetManager::GetSound2DInstance("WhirlSound");
		whirlSound->SetLooped(true); //whirling always while in the air
		whirlSound->Play();//play on creation/ launch of missle
				
		//irrelevant functions omitted...
	}
	
	void Update(){
		misslePos += Vect(0,2,0); //missle moving up

		//since this sound is not done via Audible Sound... the user is responsible for updating its position.
		whirlSound->SetSoundPosition(misslePos);

		SetGameObjectWorld(misslePos,Vect(1.0f,1.0f,1.0f),Vect(0,0,0));
	}

	void OnTerminate(){
		whirlSound->Release(); //dont forget to release the sound upon death!
	}

	Sound3D* whirlSound;

	Vect misslePos;
};

\endcode

*/

/**
\defgroup ASSETMANAGER AssetManager
\ingroup P2-API
\brief The AssetManager is used to allow the player to load models and texture into the game, while also allowing them to get a reference to that loaded resource.

*/


/**
\defgroup GRAPHICSOBJECTMAKER GraphicsObjectMaker
\ingroup P2-API
\brief The GraphicsObjectMaker is pretty much a factory that will create and return different types of Graphics Object.
	The GraphicsObjectMaker returns a new graphics object as well as offering more functionality than just creating a new 
	GraphicsObject.

\note The GraphicObjectMaker does not store graphics Object since they are said to be owned by the GameObjects. Having a 1:1 graphics object
per GameObjects allows the user to change the GameObjects graphicsObject without affecting other GameObjects graphics objects, even if they are
of the same class.
*/

/**
\defgroup TIMEMANAGER TimeManager
\ingroup P2-API
\brief The TimeManager is used to allow the player to get the frame time and to get the time since the start of the game.

*/

/**
\defgroup SCENE Scenes
\ingroup P2-API
\brief A scene is a base class from which all scenes should derive from.
*/

/**
\defgroup SCENEEXPLAINATION Creating a Level
\ingroup SCENE

\par Creating a scene
In order for a user to use a scene, he or she must derive their own scene from
the Scene class. The Scene class itself if just a base class that defines what a scene's functionality is.
One important thing to note is that the Scene class has a pure virtual function that each derived scene must reimplement.
This function is SetUp(). All SetUp does is define what is inside of the scene, a scene also defines what types can collide
with another, through using the SetCollisionPair() function. For further information please look up SetCollisionPair() in the
Scene Module. 

For example, if I wanted to create a Tutorial Scene for my game, I would have it derive from Scene. For instance, 
my TutorialLevel1.h would currently look like the following.

\code
#ifndef TUTORIALLEVEL1_H
#define TUTORIALLEVEL1_H

#include <P2Engine.h>
using namespace std;

class TutorialLevel1: public Scene{


public:

	TutorialLevel1(){}
	TutorialLevel1( const TutorialLevel1 & rhs){ rhs;}
	TutorialLevel1& operator = (const TutorialLevel1& rhs){ rhs;return *this;}
	~TutorialLevel1(){
	}

	virtual void SetUp();

};
#endif

\endcode

The .cpp would look as follows:


\code
#include "TutorialLevel1.h"

void TutorialLevel1::SetUp(){



}

\endcode


*/


/**
\defgroup INITIALSCENESETUP Setting the initial scene
\ingroup SCENE

Every game you create must have at least one scene, and must have a starting scene, 
or else the P2Engine will have no idea where to start. In order to set the initial scene,
the user has to use the function SceneManager::SetStartScene() in the P2EngineContent.cpp's
LoadGameContent() which is located in the User Files folder.

\par Below is an example of setting the initial scene, where the scene is the same scene from the "Creating a Level" section in the Scene module of the API.

\code

void Game::LoadGameContent(){ // this is located in the P2EngineContent.cpp in the User Files
	
	//make sure to load all the assets you will be using through out all of the levels.
	AssetManager::LoadModel("space_frigate.fbx","SpaceShip");
	AssetManager::LoadTexture("grid.tga","grid");

	SceneManager::SetStartScene(new  TutorialLevel1()); //sets the first scene to the TutorialLevel1
	
}

\endcode


*/


/**
\defgroup SETTINGUPCOLLISIONPAIRS Setting the scene's collisions
\ingroup SCENE

In the P2Engine, the user must declare what pair wise collisions should be considered for a scene. For example,
a user might not want to have Player types collide with Water Types, but may want Boat Types to collide with the Water Types.

\par Below is an example of setting the collision pairs in a level.

\code

void Level1::SetUp(){

	Player* p1= new Player();
	Bullet* b1 = new Bullet();
	
	
	SetCollisionPair<Player,Bullet>();
}

\endcode
\note A user should not set collision pair with the Bullet type and the Player Type and a collision pair with the Player Type and the Bullet Type. Only one is needed and should be done.

There are also cases where a player wishes to be able to have objects of one kind to be able to collide with other instances of the same class.
For these situations, instead of using SetCollisionPair<C1,C2>(), the user should use SetCollisionSelf<C>(). This will let the collision system
know that the user wants a Collidable of type C to collide with other Collidables of the same type.

\par Below is an example of a user specifing a gameobject to be able to collide within its own group.
	
\code

	void DodgeBallLevel1::SetUp(){

		Ball* b1= new Ball();
		Ball* b2 = new Ball();
	
	
		SetCollisionSelf<Ball>(); // allows balls to be considered for consideration
	}

\endcode


*/

/** \defgroup SCENEFUNCTIONS Scene Functions
\ingroup SCENE
\brief These are the events that can be performed by a scene.
\note All derived classes from scene will inherit these functions.
*/


/** \defgroup DEFAULTEVENTS Default Events
\ingroup GAMEOBJECT
\brief These are the events that a GameObject is set to automatically recieve.
\note A GameObject is Derived from Terminable, Collidable, Updatable, Inputable, Drawable, and Alarmable, and will therefore absorb all of its methods.
*/

/** \defgroup DRAWING Drawing
\ingroup GAMEOBJECT
\brief These are the methods that are related to drawing.
\note A GameObject is Derived from Terminable, Collidable, Updatable, Inputable, Drawable, and Alarmable, and will therefore absorb all of its methods.
*/


/** \defgroup COLLISION Collision
\ingroup GAMEOBJECT
\brief These are the methods related to Collisions.
\note A GameObject is Derived from Terminable, Collidable, Updatable, Inputable, Drawable, and Alarmable, and will therefore absorb all of its methods.
\note In order for collisions to work, a user must have set the collision pair between the two desired
	collidables with SetCollisionPair() and must have those GameObjects be registered into the collision system
	by using RegisterCollision().
*/


/** \defgroup UPDATING Updating
\ingroup GAMEOBJECT
\brief These are the methods that are related to Updating.
\note A GameObject is Derived from Terminable, Collidable, Updatable, Inputable, Drawable, and Alarmable, and will therefore absorb all of its methods.
*/

/** \defgroup INPUT Input
\ingroup GAMEOBJECT
\brief These are the methods that are related to keyboard input.
\note A GameObject is Derived from Terminable, Collidable, Updatable, Inputable, Drawable, and Alarmable, and will therefore absorb all of its methods.
*/

/** \defgroup ALARM Alarm
\ingroup GAMEOBJECT
\brief These are the methods that are related to the alarm system.
\note A GameObject is Derived from Terminable, Collidable, Updatable, Inputable, Drawable, and Alarmable, and will therefore absorb all of its methods. Alarms for GameObjects will automatically be canceled upon death. 
*/

/** \defgroup METHODS Methods
\ingroup GAMEOBJECT

\brief Methods used to control the GameObject.
\note A GameObject is Derived from Terminable, Collidable, Updatable, Inputable, Drawable, and Alarmable, and will therefore absorb all of its methods.
*/