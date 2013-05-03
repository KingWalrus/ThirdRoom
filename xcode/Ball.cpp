//
//  Ball.cpp
//  ThirdRoom
//
//  Created by Colin Honigman on 3/26/13.
//
//

#include "Ball.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

Ball::Ball(bool isHit){
    bHit = isHit;
}

Ball::Ball(ci::Vec3f position, ci::Vec3f roomDimensions){
    setPosition(position);
    setColor(ci::ColorA(1.0, 1.0, 1.0, 1.0));
    setSize(ci::Vec3f(2.0, 2.0, 2.0));
    //bHit = true;
    //mPositionZ = position;
    setMoveBehavior(new BounceAll());
    //std::cout << "Ball Created" << std::endl;
    //createdAt = cinder::app::getElapsedSeconds();

    //note = cinder::randInt(40, 100);
   // roomSize = cinder::Vec3f(roomDimensions.x/2, roomDimensions.y/2, roomDimensions.z/2);
    setName("Ball");
    lastThrown = 0;
}


Ball::~Ball(){
    if(mUser != NULL){
        delete mUser;
    }
}

void Ball::display(){
    ci::gl::color(getColor());
    ci::gl::drawSphere(getPosition(), getSize().x, 18);
    
}

void Ball::update(){
    if(isHit()){
        setMoving(false);
//        if(getPosition().distance(getZposition()) > 3){
//            std::cout << "throwing" << std::endl;
//            setVelocity(getPositionDifference());
//            bHit = false;
//            bMoving = true;
//            setColor(ci::Vec3f(1.0, 1.0, 1.0));
//            lastThrown = ci::app::getElapsedSeconds();
//            
//        }
//            if(mUser->isThrowingRight() && ci::app::getElapsedSeconds()-createdAt > 3){
//                std::cout << "throwing right" << std::endl;
//                //std::cout << "user throwing right" << std::endl;
//                setVelocity(ci::Vec3f(mUser->getDifference(mUser->rightHand, 0), mUser->getDifference(mUser->rightHand,1), mUser->getDifference(mUser->rightHand,2)));
//                bHit = false;
//                bMoving = true;
//                setColor(ci::Vec3f(1.0, 1.0, 1.0));
//            }
    }
    if(isMoving()){

        move(getVelocity(), getPosition());
        ci::Vec3f tempVelocity = getVelocity();
        if(getPosition().x > getBoundaries().x || getPosition().x < -getBoundaries().x){            
            tempVelocity.x = -1*tempVelocity.x;
            setVelocity(tempVelocity);
        }
        if(getPosition().y > getBoundaries().y-5 || getPosition().y < -getBoundaries().y-5){
            mVelocity.y = -1*mVelocity.y;
        }
        if(getPosition().z > getBoundaries().z || getPosition().z < -getBoundaries().z-5){
            mVelocity.z = -1*mVelocity.z;
        }
    }
    
    
    mPositionZ = mPosition;
    bHitZ = bHit;
    mControllerPositionZ = mControllerPosition;
    
}

void Ball::wallHit(int side){
    switch(side){
        case 0:
            mVelocity.y *= -1;
            break;
        case 1:
            mVelocity.y *= -1;
            break;
        case 2:
            mVelocity.z *= -1;
            break;
        case 3:
            mVelocity.z *= -1;
            break;
        case 4:
            mVelocity.x *= -1;
            break;
        case 5:
            mVelocity.x *= -1;
            break;
        default:
            break;

    }
}

bool Ball::hitTest(User* user){
    double currentTime = ci::app::getElapsedSeconds();
    
    if( user->getJointPosition(user->leftHand).distance(mPosition) > -8 && user->getJointPosition(user->leftHand).distance(mPosition) < 8 && !user->isActive(user->leftHand) && currentTime-lastThrown > 5 && currentTime-(user->getLastThrowLeft()) >3 ){
        bHit = true;
        mUser = user;
        setPosition(user->getJointPosition(user->leftHand));
        user->setActive(user->leftHand);
        mControllerPosition = user->getJointPosition(user->leftHand);
        setColor(ci::Vec3f(1.0, 0, 0));
        if(user->isThrowingLeft() && currentTime - createdAt > 3){
            std::cout << "user throwing left" << std::endl;
            setVelocity(user->getPositionDifference(user->leftHand));
            //std::cout << mVelocity << std::endl;
            bHit = false;
            bMoving = true;
            setColor(ci::randVec3f());
            lastThrown = currentTime;
            move(mVelocity, mPosition);
            user->setUnactive(user->leftHand);
            user->setLastThrowLeft(currentTime);
            mUser = NULL;
        }
        
    }
    else{
        user->setUnactive(user->leftHand);
        mUser = NULL;
        bHit = false;
    }
    
    
    if( user->getJointPosition(user->rightHand).distance(mPosition) >-8 && user->getJointPosition(user->rightHand).distance(mPosition) < 8 && !user->isActive(user->rightHand) && currentTime-lastThrown > 5 && currentTime-(user->getLastThrowRight() > 3)){
        //std::cout << user->getJointPosition(user->rightHand).distance(mPosition) << std::endl;
        bHit = true;
        mUser = user;
        setPosition(user->getJointPosition(user->rightHand));
        user->setActive(user->rightHand);
        mControllerPosition = user->getJointPosition(user->rightHand);
        
        setColor(ci::Vec3f(1.0, 0, 0));
        if(user->isThrowingRight() && currentTime-createdAt > 3){
            std::cout << "user throwing right" << std::endl;
            setVelocity(user->getPositionDifference(user->rightHand));
            bHit = false;
            bMoving = true;
            setColor(ci::randVec3f());
            lastThrown = currentTime;
            user->setUnactive(user->rightHand);
            user->setLastThrowRight(currentTime);
            mUser = NULL;
        }
        
    }
    else{
        user->setUnactive(user->rightHand);
        bHit = false;
        mUser = NULL;
        
    }
    
    
    return bHit;
    
}

bool Ball::ballHit(Instrument* ball){
    if(getPosition().distance(ball->getPosition()) < 4){
        setVelocity(getVelocity()-ball->getVelocity());
        ball->setVelocity(ball->getVelocity()-getVelocity());
       // std::cout << "hit balls!" << std::endl;
        return true;
    }
    else return false;
}

bool Ball::collisionTest(Instrument* instrument){
    if(getPosition().distance(instrument->getPosition()) < 4){
        ci::Vec3f holdVelocity = getVelocity();
        setVelocity(instrument->getVelocity());
        instrument->setVelocity(holdVelocity);
        setColor(ci::randVec3f());
        instrument->setColor(ci::randVec3f());
        //std::cout << "hit balls!" << std::endl;
        return true;
    }
    else return false;
}

float Ball::getDifference(int axis){
    if(axis == 0){
        return mPosition.x - mPositionZ.x;
    }
    else if(axis == 1){
        return mPosition.y - mPositionZ.y;
    }
    else if(axis == 2){
        return mPosition.z - mPositionZ.z;
    }
    else{
        return 0;
    }
    
}

float Ball::getVecDifference(){
    return mPosition.distance(mPositionZ);
}
