#ifndef PARTICLEFACTORY_H
#define PARTICLEFACTORY_H

#include <list>
#include <queue>
#include <assert.h>
class Particle;
class HotParticle;
class ColdParticleSide;


class ParticleFactory
{
public:
	ParticleFactory();
	~ParticleFactory();

	static void FillPool(unsigned int number);
	static HotParticle* GetParticle();
	static void DestroyParticle(HotParticle* target);
	static void DestroyMe()
	{
		delete instance;
		instance = 0;
	}



private:

	static ParticleFactory& GetInstance()
	{
		if (!instance)
		{
			instance = new ParticleFactory();
		}
		return *instance;
	}



	std::queue<HotParticle*> particlePool;

	static ParticleFactory* instance;

	HotParticle* OutOfFactory;

	HotParticle* contingHolder;

	ColdParticleSide* contingHolderCold;

	void removeParticleFromList(HotParticle *p);

	void addParticleToList(HotParticle *p);
};




#endif