#ifndef DRAWABLEMANAGER_H
#define DRAWABLEMANAGER_H

#include <list>
using namespace std;

class Drawable;
class DrawableManager{

	friend class Scene;//allow Scene to acess private members

public:
		
	~DrawableManager();

	/**
	\brief This method registers a Drawable into the DrawableManager in order to
	allow it's Draw() callback to be called. The drawable is added to the activeDrawablesList.
	If it is already in the list, then do nothing.

	\param registrant registrant is the Drawable to be registered.
	*/
	void Register(Drawable* registrant);

	
	/**
	\brief This method deregisters a Drawable from the DrawableManager in order to 
	have it's Draw() callback from being called. The drawable is removed from the list.
	If it can not be found, do nothing.
	
	\param target target is the Drawable to be deregistered.
	*/
	void Deregister(Drawable* target);

private:

	DrawableManager();
	DrawableManager( const DrawableManager & rhs){ rhs;}
	DrawableManager& operator = (const DrawableManager& rhs){ rhs; return *this;}
	
	
	
	/**
	\brief This calls the Draw() function of every Drawable that is registered to the
	DrawableManager (in activeDrawablesList.)
	
	*/
	virtual void DrawAll();

	
	/**
	\brief The list that holds all the registered Drawables to have their Draw() method called on.
	
	*/
	list<Drawable*> activeDrawablesList;
	list<Drawable*>::iterator iterate;
};

#endif