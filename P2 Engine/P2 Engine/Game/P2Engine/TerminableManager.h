#ifndef TERMINABLEMANAGER_H
#define TERMINABLEMANAGER_H

#include <list>

using namespace std;

class Terminable;

class TerminableManager{

	friend class Scene;
	friend class Terminable;

public:

	/**
	\brief Method used to Register a Terminable for termination into the TerminableManager

	\param ptr ptr is the Terminable to be Registered into the TerminableManager for termination.
	*/
	void Register(Terminable* ptr);
private:


	
	TerminableManager(){}
	TerminableManager( const TerminableManager & rhs){ rhs;}
	TerminableManager& operator = (const TerminableManager&){return *this;}
	~TerminableManager();	

	/**
	\brief Method used to deregister a Terminable for termination into the TerminableManager

	\param ptr ptr is the Terminable to be deregistered from the TerminableManager for termination.
	*/
	void Deregister(Terminable* ptr);

	/**
	\brief Method called at every Scene Update that handles deleting Terminables or sending them
	to an external manager if they have one.
	*/
	void processTerminations();

	/**
	\brief List that hold the Terminables to be processed for Termination consideration.
	*/
	list<Terminable*> terminateList;
};

#endif