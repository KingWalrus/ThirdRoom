//
//  User.cpp
//  ThirdRoom
//
//  Created by Colin Honigman on 3/26/13.
//
//

#include "User.h"

User::User(){
    active = true;
    
}

User::~User(){
    
}

void User::update(){
    
    for(int i = 0; i < 13; i++){
        allJointsZ[i] = allJoints[i];
    }
}

void User::display(){
    
    ci::gl::color(mColor.x, mColor.y, mColor.z);
    ci::gl::drawVector(allJoints[0], allJoints[1]);
    ci::gl::drawVector(allJoints[0], allJoints[2]);
    ci::gl::drawVector(allJoints[0], allJoints[3]);
    ci::gl::drawVector(allJoints[0], allJoints[4]);
    ci::gl::drawVector(allJoints[0], allJoints[5]);
    ci::gl::drawVector(allJoints[0], allJoints[6]);
    ci::gl::drawVector(allJoints[0], allJoints[7]);
    ci::gl::drawVector(allJoints[0], allJoints[8]);
    ci::gl::drawVector(allJoints[0], allJoints[9]);
    ci::gl::drawVector(allJoints[0], allJoints[10]);
    ci::gl::drawVector(allJoints[0], allJoints[11]);
    ci::gl::drawVector(allJoints[0], allJoints[12]);
    
    
    
}

ci::Vec3f User::getJointPosition(int whichJoint){
    return allJoints[whichJoint];
}

void User::setJointPosition(int whichJoint, ci::Vec3f newJointPosition){
    allJoints[whichJoint] = newJointPosition;
}