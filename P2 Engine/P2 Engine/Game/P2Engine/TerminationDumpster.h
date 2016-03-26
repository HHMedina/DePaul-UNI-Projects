#ifndef TERMINATIONDUMPSTER_H
#define TERMINATIONDUMPSTER_H

#include <list>
using namespace std;

class Terminable;

class TerminationDumpster{

	friend class TerminableManager;
	friend class Scene;
	friend class SceneManager;
private:
	
	//should not be allowed to be used by user
	TerminationDumpster( const TerminationDumpster & rhs){ rhs; }
	TerminationDumpster& operator = (const TerminationDumpster& rhs){ rhs;return *this;}
	TerminationDumpster(){
		dumpsterList = list<Terminable*>();
	}
	~TerminationDumpster();

	/**
	\brief This method immediately removes a Terminable from the dumpsterList and calls delete on it.
	\param target target is the Terminable to be immediately removed from the dumpster.
	*/
	void DestroyNow(Terminable* target);

	/**
	\brief This method registers Terminables into the dumpster that should be automatically cleaned up at
	the end of a scene or the game.

	\param registrant registrant is the Terminable to be registered to the dumpster.
	*/
	void RegisterToDumpster(Terminable* registrant);

	/**
	\brief This method deregisters Terminables from the dumpster so it is not considered for
	automatic clean up.

	\param registrant is the Terminable to be deregistered from the TerminationDumpster.
	*/
	void DeregisterFromDumpster(Terminable* registrant);

	/**
	\brief List that holds the Terminables for automatic clean up.
	*/
	list<Terminable*> dumpsterList;
};
#endif