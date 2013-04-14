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
    std::cout << "Ball Created" << std::endl;
    createdAt = cinder::app::getElapsedSeconds();
}


Ball::~Ball(){
    if(mUser != NULL){
        delete mUser;
    }
    
}

void Ball::display(){
    
    ci::gl::drawSphere(mPosition, mSize.x, 18);
    
}

void Ball::update(){
    if(bHit){
        bMoving = false;
//        if(mPosition.distance(mPositionZ) > 2 && getTime() > 2){
//            
//            bMoving = true;
//            setVelocity(ci::Vec3f(getDifference(0), getDifference(1), getDifference(2)));
//            bHit = false;
//            //std::cout << "throw " << mVelocity << std::endl;
//        }
        
    }
//    if(!bHit && bHitZ && getTime() > 2){
//        bMoving = true;
//        
//        setVelocity(ci::Vec3f(getDifference(0), getDifference(1), getDifference(2)));
//        std::cout << "throw" << mVelocity << mPosition << mPositionZ << std::endl;
//    }
    if(bMoving){
        //std::cout << "moving " << mVelocity << mPosition << std::endl;
        //mPosition += mVelocity;
        move(mVelocity, mPosition);
        if(mPosition.x > 45 || mPosition.x < -45){
            mVelocity.x = -1*mVelocity.x;
        }
        if(mPosition.y > 40 || mPosition.y < -40){
            mVelocity.y = -1*mVelocity.y;
        }
        if(mPosition.z > 50 || mPosition.z < -50){
            mVelocity.z = -1*mVelocity.z;
        }
    }

    
    mPositionZ = mPosition;
    bHitZ = bHit;
    mControllerPositionZ = mControllerPosition;
    
}


bool Ball::hitTest(User* user){
    
    if(user->getJointPosition(user->leftHand).distance(mPosition) > -3 && user->getJointPosition(user->leftHand).distance(mPosition) < 3){
        bHit = true;
        setPosition(user->getJointPosition(user->leftHand));
        user->setActive(user->leftHand);
        mControllerPosition = user->getJointPosition(user->leftHand);
        if(user->isThrowingLeft()){
            setVelocity(ci::Vec3f(user->getDifference(user->leftHand, 0), user->getDifference(user->leftHand,1), user->getDifference(user->leftHand,2)));
            std::cout << mVelocity << std::endl;
            bHit = false;
            bMoving = true;
        }
        return bHit;
        
    }
    else{
        user->setUnactive(user->leftHand);
        bHit = false;
    }
    if(user->getJointPosition(user->rightHand).distance(mPosition) > -3 && user->getJointPosition(user->rightHand).distance(mPosition) < 3){
        bHit = true;
        setPosition(user->getJointPosition(user->rightHand));
        user->setActive(user->rightHand);
        mControllerPosition = user->getJointPosition(user->rightHand);
        if(user->isThrowingRight()){
            setVelocity(ci::Vec3f(user->getDifference(user->rightHand, 0), user->getDifference(user->rightHand,1), user->getDifference(user->rightHand,2)));
            bHit = false;
            bMoving = true;
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
