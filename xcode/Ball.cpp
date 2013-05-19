//
//  Ball.cpp
//  ThirdRoom
//
//  Created by Colin Honigman on 3/26/13.
//
//

#include "Ball.h"
#include "User.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

Ball::Ball(bool isHit){
    setHit(isHit);
    lastThrown = 0;
    notes[0] = 48;
    notes[1] = 50;
    notes[2] = 53;
    notes[3] = 46;
    notes[4] = 60;
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
    notes[0] = 48;
    notes[1] = 50;
    notes[2] = 53;
    notes[3] = 46;
    notes[4] = 60;
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

    if(isMoving()){
        
        move(mVelocity, mPosition);
        ci::Vec3f tempVelocity = getVelocity();
        if(getPosition().x > getBoundaries().x+5 || getPosition().x < -getBoundaries().x-5){
            tempVelocity.x = -1*tempVelocity.x;
            setVelocity(tempVelocity);
        }
        if(getPosition().y > getBoundaries().y+5 || getPosition().y < -getBoundaries().y-5){
            tempVelocity.y = -1*tempVelocity.y;
            setVelocity(tempVelocity);
        }
        if(getPosition().z > getBoundaries().z+5 || getPosition().z < -getBoundaries().z-5){
            tempVelocity.z = -1*tempVelocity.z;
            setVelocity(tempVelocity);
        }
    }
    
    setZposition(getPosition());
    // bHitZ = bHit;
    
}

void Ball::wallHit(int side){
    //    switch(side){
    //        case 0:
    //            mVelocity.y *= -1;
    //            break;
    //        case 1:
    //            mVelocity.y *= -1;
    //            break;
    //        case 2:
    //            mVelocity.z *= -1;
    //            break;
    //        case 3:
    //            mVelocity.z *= -1;
    //            break;
    //        case 4:
    //            mVelocity.x *= -1;
    //            break;
    //        case 5:
    //            mVelocity.x *= -1;
    //            break;
    //        default:
    //            break;
    //
    //    }
}


bool Ball::hitTest(User* user){
        if(!user->isActive(user->leftHand) && user->getJointPosition(user->leftHand).distance(getPosition()) < 5 && getLastThrownMinus() > 3){
            user->setActive(user->leftHand);
            setHit(true);
            setMoving(false);
            initializePosition(user->getJointPosition(user->leftHand));
            setColor(ci::ColorA(1.0, 0, 0, 1.0));
            user->setLastCatchLeft(ci::app::getElapsedSeconds());
            
        }

        if( !user->isActive(user->rightHand) &&  user->getJointPosition(user->rightHand).distance(getPosition()) < 5 && getLastThrownMinus() > 3){
            user->setActive(user->rightHand);
            setHit(true);
            setMoving(false);
            initializePosition(user->getJointPosition(user->rightHand));
            user->setLastCatchRight(ci::app::getElapsedSeconds());
            setColor(ci::Vec3f(1.0, 0, 0));
        }
    
    return isHit();
    
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
        return getPosition().x - getZposition().x;
    }
    else if(axis == 1){
        return getPosition().y - getZposition().y;
    }
    else if(axis == 2){
        return getPosition().z - getZposition().z;
    }
    else{
        return 0;
    }
    
}

float Ball::getVecDifference(){
    return getPosition().distance(getZposition());
}

void Ball::setNote(int newNote){
    note = notes[newNote%5];
    //std::cout << note << std::endl;
}
