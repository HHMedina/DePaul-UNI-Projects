#ifndef LISTENINGSTATE_H
#define LISTENINGSTATE_H
#include "SoundConsciousListeningState.h"
#include "SoundSystem.h"
#include "DebugVisualizer.h"
#include "P2Math.h"

class ListeningState: public SoundConsciousListeningState{
	friend class SoundConscious;
	friend class DefaultSoundConscious;
	friend class SoundSystem;

protected:
	ListeningState(){}
	ListeningState(const ListeningState&){}
	const ListeningState& operator = (const ListeningState&){}
	virtual ~ListeningState(){instance=NULL;}
	
	static ListeningState& GetInstance(){
		if(!instance){
			instance = new ListeningState();
		}
		return *instance;
	}


	static ListeningState* instance;//this is a singleton

	static void DeleteMe(){
		delete GetInstance().instance;
	}

	virtual void Update(Vect& pos,Vect& dir) override{

		//Note: The sound positions are updated at SetSoundPosition() in SoundSystem.cpp
		// when going from Azul to irrklang space i use negated the position paramere's z value

		Matrix dirAzul = Matrix(ROT_XYZ,dir[x],dir[y],dir[z]);
		Matrix dirMat;

		//Used for conversion between left and right hand coordinate system
		Matrix mToggle_YZ;
		mToggle_YZ.set(ROW_0,Vect(1,0,0,0));
		mToggle_YZ.set(ROW_1,Vect(0,1,0,0));
		mToggle_YZ.set(ROW_2,Vect(0,0,-1,0));
		mToggle_YZ.set(ROW_3,Vect(0,0,0,1));
		
		Vect azulUp = dirAzul.get(ROW_1);
		azulUp[w] =0;
		azulUp.norm();
		azulUp[w] =0;
		Vect tempForwardAzul = dirAzul.get(ROW_2);
		tempForwardAzul[w] =0;
		tempForwardAzul.norm();
		tempForwardAzul[w] =0;
			
		
		//convertAzul (right hand) to irrklang(left hand)
		Matrix irrkMat = dirAzul * mToggle_YZ;
		Vect irrkUp = azulUp*mToggle_YZ;
		irrkUp[w] =0;
		irrkUp.norm();
		irrkUp[w] =0;
		Vect irrkDir = irrkMat.get(ROW_2);
		irrkDir[w] =0;
		irrkDir.norm();
		irrkDir[w] =0;
		//DebugVisualizer::ShowPoint(pos+(irrkDir *10),Vect(0,1,0),5);//irrklang forward
		//DebugVisualizer::ShowPoint(pos+(tempForwardAzul *10),Vect(1,1,0),5); //azul forward
		//DebugVisualizer::ShowPoint(pos+(azulUp *10),Vect(0,0,1),5); //azul Up
		//DebugVisualizer::ShowPoint(pos+(irrkUp *10),Vect(1,0,1),5); //irrk Up

		SoundSystem::GetSoundEngine()->setListenerPosition(irrklang::vec3df(pos[x],pos[y],-pos[z]), irrklang::vec3df(irrkDir[x],irrkDir[y],irrkDir[z]),irrklang::vec3df(0,0,0),irrklang::vec3df(irrkUp[x],irrkUp[y],irrkUp[z])); //negatives needed to transform to irrklang space

	}
};
#endif