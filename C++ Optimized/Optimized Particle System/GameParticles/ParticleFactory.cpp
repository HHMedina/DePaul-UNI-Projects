#include "ParticleFactory.h"
#include "../HotCold/HotParticle.h"
#include "HotCold\ColdParticleSide.h"
ParticleFactory* ParticleFactory::instance = 0;

void ParticleFactory::FillPool(unsigned int number)
{
	GetInstance().contingHolder = new HotParticle[number];
	GetInstance().contingHolderCold = new ColdParticleSide[number];
	
	HotParticle* temp = GetInstance().contingHolder;
	ColdParticleSide* cTemp = GetInstance().contingHolderCold;
	for (unsigned int i = 0; i < number; i++){
		temp->next = temp + 1;
		temp->prev = temp - 1;
		
		temp->coldSide = cTemp + i;
		temp++;

	}
	GetInstance().contingHolder->prev = 0;
	GetInstance().contingHolder[number - 1].next = 0;

	temp = GetInstance().contingHolder;

	while (temp != 0)
	{
	
		GetInstance().particlePool.push(temp);

		temp = temp->next;
	}

	/*
	for (unsigned int i = 0; i < number; i++)
	{
		GetInstance().particlePool.push(new Particle());
	}
	*/
}

HotParticle* ParticleFactory::GetParticle()
{
	HotParticle * temp  = GetInstance().particlePool.front();
	GetInstance().particlePool.pop();
	GetInstance().addParticleToList(temp);
	return temp;

}

void ParticleFactory::DestroyParticle(HotParticle* target)
{
	GetInstance().removeParticleFromList(target);
}

ParticleFactory::ParticleFactory()
	:OutOfFactory(0), contingHolder(0), contingHolderCold(0)
{
}

void ParticleFactory::addParticleToList(HotParticle *p)
{
	assert(p);
	if (GetInstance().OutOfFactory == 0)
	{ // first on list
		GetInstance().OutOfFactory = p;
		p->next = 0;
		p->prev = 0;
	}
	else
	{ // add to front of list
		GetInstance().OutOfFactory->prev = p;
		p->next = GetInstance().OutOfFactory;
		p->prev = 0;
		GetInstance().OutOfFactory = p;
	}

}

void ParticleFactory::removeParticleFromList(HotParticle *p)
{

	// make sure we are not screwed with a null pointer
	assert(p);

	if (p->prev == 0 && p->next == 0)
	{ // only one on the list
		GetInstance().OutOfFactory = 0;
	}
	else if (p->prev == 0 && p->next != 0)
	{ // first on the list
		p->next->prev = 0;
		GetInstance().OutOfFactory = p->next;
	}
	else if (p->prev != 0 && p->next == 0)
	{ // last on the list 
		p->prev->next = 0;
	}
	else//( p->next != 0  && p->prev !=0 )
	{ // middle of the list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}
	//add to stack
	GetInstance().particlePool.push(p);

}

ParticleFactory::~ParticleFactory()
{
	delete[] GetInstance().contingHolder;
	delete[] GetInstance().contingHolderCold;
	
}
