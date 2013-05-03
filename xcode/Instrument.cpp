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

Instrument::Instrument(bool isHit){
    bHit = isHit;
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
    else{
        std::cout << "move behavior is null" << std::endl;
    }
}

ci::Vec3f Instrument::getPosition(){
    return mPosition;
}

void Instrument::initializePosition(ci::Vec3f firstPosition){
    mPosition = firstPosition;
    mPositionZ = mPosition;
}

void Instrument::setPosition(ci::Vec3f newPosition){
    mPosition = newPosition;
}

ci::Vec3f Instrument::getZposition(){
    return mPositionZ;
}

void Instrument::setZposition(ci::Vec3f lastPosition){
    mPositionZ = lastPosition;
}

ci::Vec3f Instrument::getVelocity(){
    return mVelocity;
}
ci::Vec3f* Instrument::getVelocityPtr(){
    return &mVelocity;
}

void Instrument::setVelocity(ci::Vec3f newVelocity){
    mVelocity = newVelocity;
    //std::cout << mVelocity << std::endl;
}

ci::ColorA Instrument::getColor(){
    return ci::ColorA(mColor.x, mColor.y, mColor.z, 1.0);
}

void Instrument::setColor(ci::Vec3f newColor){
    mColor = newColor;
}

void Instrument::setColor(ci::ColorA newColor){
    mColorA = newColor;
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

void Instrument::setNote(int newNote){
    note = newNote;
}

int Instrument::getNote(){
    return note;
}

std::string Instrument::getName(){
    return name;
}

void Instrument::setName(std::string n){
    name = n;
}

ci::Vec3f Instrument::getVectorDifference(ci::Vec3f v1, ci::Vec3f v2){
    ci::Vec3f temp = ci::Vec3f(v2.x-v1.x, v2.y-v1.y, v2.z-v1.z);
    return temp;
}

ci::Vec3f Instrument::getPositionDifference(){
    ci::Vec3f temp = ci::Vec3f(mPosition.x-mPositionZ.x, mPosition.y-mPositionZ.y, mPosition.z-mPositionZ.z);
    return temp;
}

int Instrument::getUserID(){
    
    return userID;
}

void Instrument::setUserID(int _userID){
    userID = _userID;
    
}

void Instrument::setCreatedAt(double creationTime){
    createdAt = creationTime;
}

void Instrument::setBoundaries(ci::Vec3f boundaries){
    roomSize = boundaries;
}

ci::Vec3f Instrument::getBoundaries(){
    return roomSize;
}

bool Instrument::isHit(){
    return bHit;
}

bool Instrument::isMoving(){
    return bMoving;
}

void Instrument::setHit(bool hit){
    bHit = hit;
}

void Instrument::setMoving(bool moving){
    bMoving = moving;
}