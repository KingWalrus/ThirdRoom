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
    setColor(       ci::Vec3f(1.0, 1.0, 1.0)    );
    setSize(        ci::Vec3f(2.0, 2.0, 2.0)    );
    setPosition(    ci::Vec3f(0.0, -30.0, 0.0)  );
}

Ball::~Ball(){
    if(mUser != NULL){
        delete mUser;
    }
    
}

void Ball::update(){
    if(bMoving){
        mPosition += mVelocity;
        if(mPosition.x > 45 || mPosition.x < -45){
            mVelocity.x = -1*mVelocity.x;
        }
        if(mPosition.y > 30 || mPosition.y < -40){
            mVelocity.y = -1*mVelocity.y;
        }
        if(mPosition.z > 60 || mPosition.z < -60){
            mVelocity.z = -1*mVelocity.z;
        }
    }
}

void Ball::display(){
    
    ci::gl::drawSphere(mPosition, mSize.x, 18);
    
}

bool Ball::hitTest(User* user){
    
    if(user->getJointPosition(user->leftHand).distance(mPosition) > -1 && user->getJointPosition(user->leftHand).distance(mPosition) < 1){
        bHit = true;
    }
    
}