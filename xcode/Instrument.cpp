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
    if( moveBehavior != NULL){
        delete moveBehavior;
    }
}

void Instrument::move(ci::Vec3f& velocity, ci::Vec3f& position){
    if( moveBehavior != NULL){
        moveBehavior->move(velocity, position);
    }
}

ci::Vec3f Instrument::getPosition(){
    return mPosition;
}

void Instrument::setPosition(ci::Vec3f newPosition){
    mPosition = newPosition;
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

ci::Vec3f Instrument::getSize(){
    return mSize;
}

void Instrument::setSize(ci::Vec3f newSize){
    mSize = newSize;
}

void Instrument::setMoveBehavior(MoveBehavior *mb){
    moveBehavior = mb;
}

double Instrument::getTime(){
    return cinder::app::getElapsedSeconds() - createdAt;
}