#ifndef HOTPARTICLE_H
#define HOTPARTICLE_H
#include <memory>
#include "../MemSystem/mem.h"

class ColdParticleSide;
class Particle;
class Vect4D;
#include "Vect4D.h"
class HotParticle
{
public:

	friend class ParticleFactory;
	friend class ParticleEmitter;
	HotParticle();
	~HotParticle();
	void DestroyMe();
	void operator = (const HotParticle&);

	void* operator new(const size_t size)
	{
		return memSystem::Malloc(size); 
		//
	}

		void operator delete(void* ptr)
	{
		memSystem::Free(ptr);
		//
	}

	void* operator new[](const size_t size)
	{
		return memSystem::Malloc(size); 
		//
	}

		void operator delete[](void* ptr)
	{
		memSystem::Free(ptr);
		//
	}

private:
	HotParticle(const HotParticle& ){}

	void Update(const float time_elapsed);
	void Initialize(float _life, const Vect4D& start_Pos, const Vect4D& start_Vel, const Vect4D& _scale);
	
	Vect4D	position;
	Vect4D	velocity;
	Vect4D	scale;
	float	life;
	HotParticle *next;
	HotParticle *prev;
	ColdParticleSide* coldSide;


};

#endif