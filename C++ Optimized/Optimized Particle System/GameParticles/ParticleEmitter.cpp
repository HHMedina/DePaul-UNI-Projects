#include "DO_NOT_MODIFY\Timer\Timer.h"
#include "DO_NOT_MODIFY\Timer\GlobalTimer.h"
#include "DO_NOT_MODIFY\OpenGL\OpenGLInterface.h"

#include <assert.h>

#include "ParticleEmitter.h"
#include "ParticleFactory.h"
#include "Settings.h"
#include "HotCold\HotParticle.h"
#include "HotCold\ColdParticleSide.h"

static const unsigned char squareColors[] = 
{
	255,  255,  000,  255,
	000,  255,  255,  255,
	000,  000,  000,  000,
	255,  000,  255,  255,
}; 

static const float squareVertices[] = 
{
	-0.015f,  -0.015f, 0.0f,
	 0.015f,  -0.015f, 0.0f,
	-0.015f,   0.015f, 0.0f,
	 0.015f,   0.015f, 0.0f,
};


ParticleEmitter::ParticleEmitter()
:	start_position( 0.0f, 0.0f, 0.0f ),
	start_velocity( 0.0f, 1.0f, 0.0f),
	start_scale(1.0f, 1.0f, 1.0f, 1.0f),
	max_life( MAX_LIFE ),
	max_particles( NUM_PARTICLES ),
	spawn_frequency( 0.0000001f ),
	last_spawn( globalTimer::getTimerInSec() ),
	last_loop(  globalTimer::getTimerInSec() ),
	last_active_particle( -1 ),
	vel_variance( 1.0f, 4.0f, 0.4f ),
	pos_variance( 1.0f, 1.0f, 1.0f ),
	scale_variance( 2.5),
	headParticle(0)
{
	// nothing to do
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter& rhs)
	: start_position(rhs.start_position),
	start_velocity(rhs.start_velocity),
	start_scale(rhs.start_scale),
	max_life(rhs.max_life),
	max_particles(rhs.max_particles),
	spawn_frequency(rhs.spawn_frequency),
	last_spawn(rhs.last_spawn),
	last_loop(rhs.last_loop),
	last_active_particle(rhs.last_active_particle),
	vel_variance(rhs.vel_variance),
	pos_variance(rhs.pos_variance),
	scale_variance(rhs.scale_variance),
	headParticle(0),
	transCamera(Matrix()),
	partTmp(Matrix()),
	camPosVect(Vect4D())

{
}

ParticleEmitter& ParticleEmitter::operator=(const ParticleEmitter& rhs)
{
	start_position = rhs.start_position;
	start_velocity = rhs.start_velocity;
	start_scale = rhs.start_scale;

	vel_variance = rhs.vel_variance;
	pos_variance = rhs.pos_variance; 
	
	max_life = rhs.max_life; 
	last_spawn = rhs.last_spawn;
	last_loop = rhs.last_loop;
	spawn_frequency = rhs.spawn_frequency;
	scale_variance = rhs.scale_variance;

	max_particles = rhs.max_particles;
	last_active_particle = rhs.last_active_particle;
	return *this;
}

ParticleEmitter::~ParticleEmitter()
{
	// do nothing
	//THERE IS A MEMORY LEAK HERE!
	//Walk the list and destroy all of the particles!!!
}


void ParticleEmitter::SpawnParticle()
{
	// create another particle if there are ones free
	if( last_active_particle < max_particles-1 )
	{
	
		// create new particle
		HotParticle *newParticle = ParticleFactory::GetParticle();//new Particle();

		// initialize the particle
		newParticle->Initialize(0.0f,start_position,start_velocity,start_scale);

		// apply the variance
		this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);

		// increment count
		last_active_particle++;

		// add to list
		this->addParticleToList( newParticle );

	}
}

void ParticleEmitter::update()
{
	// get current time
	float current_time = globalTimer::getTimerInSec();

	// spawn particles
	double time_elapsed = (double)current_time - (double)last_spawn; // needs to be double for precision
	// update
	while (spawn_frequency < time_elapsed)
	{
		// spawn a particle
		this->SpawnParticle();
		// adjust time
		time_elapsed -= (double)spawn_frequency;
		// last time
		last_spawn = current_time;
	}

	// total elapsed
	time_elapsed = (double)current_time - (double)last_loop;


	HotParticle *p = this->headParticle;
	// walk the particles

	while( p!= 0 )
	{
		// call every particle and update its position 
		p->Update((float)time_elapsed);

		// if it's live is greater that the max_life 
		// and there is some on the list
		// remove node
		if ((p->life > max_life) && (last_active_particle > 0))
		{
			// particle to remove
			HotParticle *s = p;// p->next;

			// need to squirrel it away.
			p=p->next;

			// remove last node
			this->removeParticleFromList( s );

			// update the number of particles
			last_active_particle--;
		}
		else
		{
			// increment to next point
			p = p->next;
		}
	}

	
	last_loop = current_time;
}
	   
void ParticleEmitter::addParticleToList(HotParticle *p )
{
	assert(p);
	if( this->headParticle == 0 )
	{ // first on list
		this->headParticle = p;
		p->next = 0;
		p->prev= 0;
	}
	else 
	{ // add to front of list
		headParticle->prev = p;
		p->next = headParticle;
		p->prev = 0;
		headParticle = p;
	}

}

void ParticleEmitter::removeParticleFromList( HotParticle *p )
{
	// make sure we are not screwed with a null pointer
	assert(p);

	if( p->prev == 0 && p->next == 0  )
	{ // only one on the list
		this->headParticle = 0;
	}
	else if( p->prev == 0 && p->next != 0  )
	{ // first on the list
		p->next->prev = 0;
		this->headParticle = p->next;
	}
	else if( p->prev!= 0 && p->next == 0 )
	{ // last on the list 
		p->prev->next = 0;
	}
	else//( p->next != 0  && p->prev !=0 )
	{ // middle of the list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}
	p->DestroyMe();//send back to factory/memory pool
	p = 0;

}

void ParticleEmitter::ConcatMatricesForDraw( Matrix& output, const Vect4D& partScale, const Vect4D& camTrans, const Vect4D& partTrans, const float partRot)
{
	/*
	SxSxCOS								-SySxSIN				0			0
	SxSySIN								SySyCOS					0			0
	0										0					SzSz		0
	Sx(COS(X1+X2)+SIN(Y1+Y2))	Sy(COS(X1+X2)+SIN(Y1+Y2))		Sz(Z1+Z2)	1.0f
	*/

	//this matrix represents partTmp after ALL transformations
	// Matrix partTmp; 
	//partTmp = scaleMatrix;
	//partTmp *= transCamera;
	//partTmp *= transParticle;
	//partTmp *= rotParticle;
	//partTmp *= scaleMatrix;
	// 

	float resCos = cosf(partRot);
	float resSin = sinf(partRot);

	output.mm0 = _mm_set_ps(0.0f, 0.0f, partScale.x * partScale.y * -resSin,   partScale.x * partScale.x * resCos);
	output.mm1 = _mm_set_ps(0.0f, 0.0f, partScale.y * partScale.y * resCos,    partScale.x * partScale.y * resSin);
	output.mm2 = _mm_set_ps(0.0f, partScale.z * partScale.z, 0.0f, 0.0f);
	output.mm3 = _mm_set_ps(1.0f, 
							partScale.z*(camTrans.z + partTrans.z), 
							partScale.y* ((-resSin * (camTrans.x + partTrans.x)) + (resCos * (camTrans.y + partTrans.y) ) ),
							partScale.x* ((resCos * (camTrans.x + partTrans.x)) + (resSin * (camTrans.y + partTrans.y))));
}

void ParticleEmitter::draw()
{
	
	// get the camera matrix from OpenGL
	//glGetDoublev(GL_MODELVIEW_MATRIX, reinterpret_cast<double*>(&cameraMatrix));
	glGetFloatv(GL_MODELVIEW_MATRIX, reinterpret_cast<float*>(&cameraMatrix));
	

	//can this stay here?
	// OpenGL goo... don't worrry about this
		glVertexPointer(3, GL_FLOAT, 0, squareVertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
		glEnableClientState(GL_COLOR_ARRAY);
	
	// get the position from this matrix
	cameraMatrix.get(Matrix::MATRIX_ROW_3, &camPosVect);
	
	// iterate throught the list of particles
	HotParticle * it = headParticle;
	while( it != 0)
	{
		// set total transformation of particle
		//this creates a single matrix representative of
		//all transformations done onto the particle.
		//removes the uneeded multiplications within 
		//regular multiplications of matrices
		ConcatMatricesForDraw(partTmp,it->scale,camPosVect,it->position,it->coldSide->rotation);

		// set the transformation matrix
		glLoadMatrixf(reinterpret_cast<float*>(&(partTmp)));
				
		// draw the trangle strip
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		it = it->next;
				
	}

	// clears or flushes some internal setting, used in debug, but is need for performance reasons
	// magic...  really it's magic.
	GLenum glError = glGetError();
	glError;
}


void ParticleEmitter::Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
	// Add some randomness...

	// Ses it's ugly - I didn't write this so don't bitch at me
	// Sometimes code like this is inside real commerical code ( so now you know how it feels )
	
	// x - variance
	
	float var = static_cast<float>(rand() % 1000) * 0.001f;
	int sign = rand() % 2;
	float *t_pos = reinterpret_cast<float*>(&pos);
	float *t_var = &pos_variance[x];
	if(sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;
	

	// y - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = rand() % 2;
	t_pos++;
	t_var++;
	if(sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;
	
	// z - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = rand() % 2;
	t_pos++;
	t_var++;
	if(sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;
	
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = rand() % 2;
	
	// x  - add velocity
	t_pos = &vel[x];
	t_var = &vel_variance[x];
	if(sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;
	
	// y - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = rand() % 2;
	t_pos++;
	t_var++;
	if(sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;
	
	// z - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = rand() % 2;
	t_pos++;
	t_var++;
	if(sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;
	
	// correct the sign
	var = 2.0f * static_cast<float>(rand() % 1000) * 0.001f;
	sign = rand() % 2;
	
	if(sign == 0)
	{
		var *= -1.0f;
	}
	sc *= var;
	
	
}


// End of file