#ifndef COLLIDABLEGROUP
#define COLLIDABLEGROUP

#include <set>
#include "CollidableGroupBase.h"
#include <iostream>

template<typename C>
class CollidableGroup: public CollidableGroupBase {
	
	friend class CollisionManager; //Allow Collision Manager private access
	friend class CollidableGroup; //Allow CollidableGroups private access to each other.
	
	template <class T>
	friend class CollisionSingleProcessor;
	template <class T, class T2>
	friend class CollisionPairProcessor;

public:
	
	typedef std::set<C*> CollidableCollection;
	
	/**
	\brief This method is used to register a Collidable of Class C into its corresponding CollidableGroup.
	\param Registrant Registrant is the collidable to be added to the CollidableGroup's collection.
	*/
	static void Register(C* Registrant){
		Instance().collideCol.insert(Registrant);
	}

	/**
	\brief This method is used to deregister a Collidable of Class C from its corresponding CollidableGroup.
	\param Deregistrant Deregistrant is the collidable to be removed from the CollidableGroup's collection.
	*/
	static void Deregister(C* Deregistrant){
		Instance().collideCol.erase(Deregistrant);
	}
	
	static CollidableGroup<C>& Instance(){
		if(!instance){
			instance = new CollidableGroup<C>;
		}
		return *instance;
	}

	
		

private:

	/**
	\brief This is the collections of Collidables of class C that have been registered for collision consideration.
	*/
	CollidableCollection collideCol;

	static CollidableGroup<C>* instance; //this is a singleton
	CollidableGroup<C>(){
		collideCol = std::set<C*>();
	}
	CollidableGroup<C>( const CollidableGroup<C> & ){}
	CollidableGroup<C>& operator = (const CollidableGroup<C>& ){ }
	virtual ~CollidableGroup<C>(){
		Instance().collideCol.clear();
		instance = NULL;
	}



};

template <typename C> 
CollidableGroup<C>* CollidableGroup<C>::instance=NULL;


#endif