#include "Sensor.h"

Sensor::Sensor(){
    state = NONE;
}

Sensor::~Sensor(){

}

TYPE Sensor::getState(){
    return state;
}

void Sensor::setState(TYPE state){
    this->state = state;
}