//
//  Screen.cpp
//  ThirdRoom
//
//  Created by Colin Honigman on 4/27/13.
//
//

#include "Screen.h"
#include "User.h"
#include "cinder/gl/gl.h"

Screen::Screen(bool isHit){
    setHit(isHit);
}

Screen::Screen(ci::Vec3f position){
    setPosition(position);
    setColor(ci::ColorA(1.0, 0.0, 1.0, .7));
    setColor(ci::Vec3f(0.0, 1.0, 1.0));
    
    setSize(ci::Vec3f(1.0, 1.0, 1.0));
    bHit = true;
    mPositionZ = mPosition;
    setMoveBehavior(new BounceUpDown());
    name = "Screen";
    std::cout << "Screen Created" << std::endl;
}

Screen::~Screen(){
    
}

void Screen::update(){
    if(isMoving()){
        move(mVelocity, mPosition);
      //  std::cout << "am i moving?" << std::endl;
    }
}

void Screen::display(){
    setColor(ci::ColorA(mSize.x*.1, 0, 1.0, .7));
    ci::gl::color(mColorA);
   // ci::gl::drawStrokedCube(mPosition, mSize);
   // ci::gl::drawColorCube(mPosition, mSize);
   // ci::gl::drawCube(mPosition,mSize);
    ci::gl::enableAlphaBlending();
    ci::gl::pushMatrices();
    ci::gl::translate(mPosition);
    ci::gl::rotate((mUser->getJointPosition(mUser->rightHand).y - mUser->getJointPosition(mUser->leftHand).y)*90);
    ci::gl::drawTorus(mSize.x/1, 2, 2);
    ci::gl::popMatrices();
}

bool Screen::hitTest(User* user){
    if(bHit){
        mUser = user;
        bMoving = false;
        setSize(ci::Vec3f(user->getJointDifferenceX(user->leftHand, user->rightHand), user->getJointDifferenceY(user->leftHand, user->rightHand), .5));
        user->setActive(user->rightHand);
        user->setActive(user->leftHand);
        setPosition(user->getMidpoint(user->leftHand, user->rightHand));
        if(user->getJointDifferenceZ(user->leftHand, user->rightHand) > 5.0+mSize.x){
           // bHit = false;
            user->setUnactive(user->leftHand);
            user->setUnactive(user->rightHand);
            user->setScreen(false);
            setHit(false);
            std::cout<< "released" << std::endl;
        }
    }

    return bHit;
    
}

bool Screen::collisionTest(Instrument* instrument){
    if(getPosition().distance(instrument->getPosition()) < 4){
        //setVelocity(getVelocity()-instrument->getVelocity());
        instrument->setVelocity(instrument->getVelocity()-getVelocity());
        std::cout << "hit screen!" << std::endl;
        return true;
    }
    else return false;
}

float Screen::getDifference(int axis){
    switch (axis) {
        case 0:
            return mPosition.x - mPositionZ.x;
            break;
        case 1:
            return mPosition.y - mPositionZ.y;
            break;
        case 2:
            return mPosition.z - mPositionZ.z;
            break;
        default:
            return 0;
            break;
    }
}

float Screen::getVecDifference(){
    return mPosition.distance(mPositionZ);
}