//
//  Bounce.cpp
//  ThirdRoom
//
//  Created by Colin Honigman on 2/21/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Bounce.h"
#include "cinder/gl/gl.h"

Bounce::Bounce(){
    
    mPosition = ci::Vec3f(0, 0, 0);
    mColor = ci::Color(1.0f, 0.0f, 0.0f);
    radius = 2;
    mVelocity = ci::Vec3f(0, 0, 0);
    
}

Bounce::Bounce(ci::Vec3f pos, int size, int idNumber){
    mPosition = pos;
    mColor = ci::Color(1.0f, 0.0f, 0.0f);
    radius = size;
    ID = idNumber;
    mVelocity = ci::Vec3f(0, 0, 0);
}

void Bounce::update(){
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

void Bounce::display(){
    
    ci::gl::drawSphere(mPosition, radius, 12);
    ci::gl::drawSphere(ci::Vec3f(mPosition.x, -40, mPosition.z), 1.0f, 12);
    
}