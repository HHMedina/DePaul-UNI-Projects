#include "HotParticle.h"
#include "ColdParticleSide.h"
#include "Matrix.h"
#include "Vect4D.h"
#include "Enum.h"
#include "../ParticleFactory.h"
HotParticle::HotParticle()
	:life(0.0f), position(0.0f, 0.0f, 0.0f, 1.0f), velocity(0.0f, 0.0f, 0.0f, 1.0f),
	scale(1.0f, 1.0f, 1.0f, 1.0f)
{
}

HotParticle::~HotParticle()
{
}

void HotParticle::Initialize(float _life, const Vect4D& start_Pos, const Vect4D& start_Vel, const Vect4D& _scale)
{
	position = start_Pos;
	life = _life;
	velocity = start_Vel;
	scale = _scale;
	coldSide->rotation = 0.0f;
	coldSide->rotation_velocity = 0.5f;

}

void HotParticle::DestroyMe()
{
	ParticleFactory::DestroyParticle(this);
}

void HotParticle::Update(const float time_elapsed)
{
	
	// serious math below - magic secret sauce
	life += time_elapsed;
	position += (velocity * time_elapsed);
	Vect4D z_axis(0.0f, -0.25f, 1.0f);
	Vect4D v(3.0f, 4.0f, 0.0f);
	position.Cross(z_axis, v);
	v.norm(v);
	position += v * (0.05f *  life);


	coldSide->rotation += coldSide->rotation_velocity * time_elapsed *2.01f;
}

void HotParticle::operator = (const HotParticle& rhs)
{

	life = rhs.life;
	position = rhs.position;
	coldSide->rotation = rhs.coldSide->rotation;
	coldSide->rotation_velocity = rhs.coldSide->rotation_velocity;
	scale = rhs.scale;
	velocity = rhs.velocity;
}