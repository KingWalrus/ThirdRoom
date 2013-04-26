//
//  Ball.cpp
//  ThirdRoom
//
//  Created by Colin Honigman on 3/26/13.
//
//

#include "Ball.h"
#include "cinder/gl/gl.h"


Ball::Ball(){
    //setColor(       ci::Vec3f(1.0, 1.0, 1.0)    );
    //setSize(        ci::Vec3f(2.0, 2.0, 2.0)    );
    //setPosition(    ci::Vec3f(0.0, -30.0, 0.0)  );
}

Ball::Ball(ci::Vec3f position){
    setPosition(position);
    setColor(ci::Vec3f(1.0, 1.0, 1.0));
    setSize(ci::Vec3f(2.0, 2.0, 2.0));
    bHit = true;
    mPositionZ = position;
    setMoveBehavior(new BounceAll());
    //std::cout << "Ball Created" << std::endl;
    createdAt = cinder::app::getElapsedSeconds();
}


Ball::~Ball(){
    if(mUser != NULL){
        delete mUser;
    }
    
}

void Ball::display(){
    ci::gl::color(mColor.x, mColor.y, mColor.z);
    ci::gl::drawSphere(mPosition, mSize.x, 18);
    
}

void Ball::update(){
    if(bHit){
        bMoving = false;
    }
    if(bMoving){

        move(mVelocity, mPosition);
        if(mPosition.x > 50 || mPosition.x < -50){
            mVelocity.x = -1*mVelocity.x;
        }
        if(mPosition.y > 40 || mPosition.y < -40){
            mVelocity.y = -1*mVelocity.y;
        }
        if(mPosition.z > 75 || mPosition.z < -75){
            mVelocity.z = -1*mVelocity.z;
        }
    }

    
    mPositionZ = mPosition;
    bHitZ = bHit;
    mControllerPositionZ = mControllerPosition;
    
}


bool Ball::hitTest(User* user){
    
    if(user->getJointPosition(user->leftHand).distance(mPosition) > -4 && user->getJointPosition(user->leftHand).distance(mPosition) < 4 && !user->isActive(user->leftHand)){
        bHit = true;
        setPosition(user->getJointPosition(user->leftHand));
        user->setActive(user->leftHand);
        mControllerPosition = user->getJointPosition(user->leftHand);
        setColor(ci::Vec3f(1.0, 0, 0));
        if(user->isThrowingLeft() && ci::app::getElapsedSeconds()-createdAt > 3){
            //std::cout << "user throwing left" << std::endl;
            setVelocity(ci::Vec3f(user->getDifference(user->leftHand, 0), user->getDifference(user->leftHand,1), user->getDifference(user->leftHand,2)));
            //std::cout << mVelocity << std::endl;
            bHit = false;
            bMoving = true;
            setColor(ci::Vec3f(1.0, 1.0, 1.0));
        }
        
        return bHit;
        
    }
    else{
        user->setUnactive(user->leftHand);
        bHit = false;
    }
    
    if(user->getJointPosition(user->rightHand).distance(mPosition) > -4 && user->getJointPosition(user->rightHand).distance(mPosition) < 4 && !user->isActive(user->rightHand)){
        bHit = true;
        setPosition(user->getJointPosition(user->rightHand));
        user->setActive(user->rightHand);
        mControllerPosition = user->getJointPosition(user->rightHand);
        setColor(ci::Vec3f(1.0, 0, 0));
        if(user->isThrowingRight() && ci::app::getElapsedSeconds()-createdAt > 3){
            //std::cout << "user throwing right" << std::endl;
            setVelocity(ci::Vec3f(user->getDifference(user->rightHand, 0), user->getDifference(user->rightHand,1), user->getDifference(user->rightHand,2)));
            bHit = false;
            bMoving = true;
            setColor(ci::Vec3f(1.0, 1.0, 1.0));
        }
        return bHit;
        
    }
    else{
        user->setUnactive(user->rightHand);
        bHit = false;
        
    }
    
    return bHit;
    
}

float Ball::getDifference(int axis){
    if(axis == 0){
        return mControllerPosition.x - mPositionZ.x;
    }
    else if(axis == 1){
        return mControllerPosition.y - mPositionZ.y;
    }
    else if(axis == 2){
        return mControllerPosition.z - mPositionZ.z;
    }
    else{
        return 0;
    }
    
}

float Ball::getVecDifference(){
    return mPosition.distance(mPositionZ);
}
