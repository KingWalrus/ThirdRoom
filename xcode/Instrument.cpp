//
//  Instrument.cpp
//  ThirdRoom
//
//  Created by Colin Honigman on 3/26/13.
//
//

#include "Instrument.h"

Instrument::Instrument(){
    
}

Instrument::~Instrument(){
    
}

ci::Vec3f Instrument::getPosition(){
    return mPos;
}

void Instrument::setPosition(ci::Vec3f newPosition){
    mPos = newPosition;
}

ci::Vec3f Instrument::getVelocity(){
    return mVelocity;
}

void Instrument::setVelocity(ci::Vec3f newVelocity){
    mVelocity = newVelocity;
}

ci::Vec3f Instrument::getColor(){
    return mColor;
}

void Instrument::setColor(ci::Vec3f newColor){
    mColor = newColor;
}