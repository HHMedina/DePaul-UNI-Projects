#ifndef UPDATEABLEMANAGER_H
#define UPDATEABLEMANAGER_H

#include <Azul.h>
#include <list>
using namespace std;

class Updatable;

class UpdatableManager{

	friend class Scene;

public:
		
	/**
	\brief This method is used to register an Updatable into the UpdatableManager for Update() consideration.

	\param registrant registrant is the Updatable to be registered into the UpdatableManager.
	*/
	void Register(Updatable* registrant);

	/**
	\brief This method is used to deregister an Updatable from the UpdatableManager for Update() consideration.

	\param target target is the Updatable to be deregistered from the UpdatableManager.
	*/
	void Deregister(Updatable* target);

private:

	UpdatableManager();
	UpdatableManager( const UpdatableManager & rhs){ rhs;}
	UpdatableManager& operator = (const UpdatableManager& rhs){ rhs; return *this;}
	~UpdatableManager();
	
	/**
	\brief This method is called at everyframe by the Scene and calls the Update() callback on all of the Updatables
	in the activeUpdatablesList.
	*/
	void UpdateAll();

	/**
	\brief The list of Registered Updatables for callback consideration.
	*/
	list<Updatable*> activeUpdatablesList;
	list<Updatable*>::iterator iterate;
};




#endif