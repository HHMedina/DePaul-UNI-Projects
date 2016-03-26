#include "controller.h"
#include "Blaster.h"

Controller::Controller(Blaster& controlle){
	blaster = &controlle;

}

Controller::Controller(Controller& other){
	this->blaster = other.blaster;
}

