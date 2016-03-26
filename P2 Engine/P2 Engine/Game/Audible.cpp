#include "Audible.h"
#include "No3DSound.h"

Audible::Audible(){
	my3DSound = &No3DSound::GetInstance();
}