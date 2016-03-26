#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math\Matrix.h"
#include "Math\Vect4D.h"

#include <list>

class HotParticle;

class  ParticleEmitter
{
public:
	ParticleEmitter();
	ParticleEmitter(const ParticleEmitter&);
	ParticleEmitter& operator=(const ParticleEmitter&);
	~ParticleEmitter();
	
	void SpawnParticle();
	void update();
	void draw();

	void addParticleToList(HotParticle *p );
	void removeParticleFromList( HotParticle *p );

	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);

private:
	void ParticleEmitter::ConcatMatricesForDraw(Matrix& output, const Vect4D& partScale, const Vect4D& camTrans, const Vect4D& partTrans, const float partRot);
	//added for efficiency = * // avoids creation of many temporaries
	Matrix transCamera;
	Matrix partTmp;
	Matrix cameraMatrix;

	Vect4D	start_position;//vect4d = 16 byte
	Vect4D	start_velocity;
	Vect4D  start_scale;

	Vect4D	vel_variance;//*
	Vect4D	pos_variance;//* 


	Vect4D camPosVect;// added for speed efficiency

	HotParticle *headParticle;//* * = 4 byte

	float	max_life; // float = 4 byte
	float	last_spawn;
	float	last_loop;
	float	spawn_frequency;
	float	scale_variance;//*

	int		max_particles;//int = 2 byte
	int		last_active_particle;

	

};

#endif // PARTICLEEMITTER_H
