#ifndef COLDPARTICLENODE_H
#define COLDPARTICLENODE_H

#include "Vect4D.h"
#include "Matrix.h"
#include "../MemSystem/mem.h"

class ColdParticleSide
{
	friend class HotParticle;
	friend class ParticleEmitter;
public:
	ColdParticleSide();
	~ColdParticleSide();

	void* operator new(const size_t size)
	{
		return memSystem::Malloc(size); //memSystem::Malloc(size); //_aligned_malloc(size, 16);
		//
	}

		void operator delete(void* ptr)
	{
		memSystem::Free(ptr);//memSystem::Free(ptr);//_aligned_free(ptr);
		//
	}

	void* operator new[](const size_t size)
	{
		return memSystem::Malloc(size); //return memSystem::Malloc(size); //memSystem::Malloc(size); //_aligned_malloc(size, 16);
		//
	}

	void operator delete[](void* ptr)
	{
		memSystem::Free(ptr); //memSystem::Free(ptr);//memSystem::Free(ptr);//_aligned_free(ptr);
		//
	}

private:

	ColdParticleSide(const ColdParticleSide&){}
	void operator = (const ColdParticleSide&){}
	
	float	rotation;
	float	rotation_velocity;
};

#endif