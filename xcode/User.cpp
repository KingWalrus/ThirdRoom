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

User::User(int userId){
    userID = userId;
    active = true;
    mColor = ci::Vec3f(255, 0, 0);
}

User::~User(){
    
}

void User::update(){

    for(int i = 0; i < 13; i++){
        allJointsZ[i] = allJoints[i];
        
    }
    
}

void User::display(){
    
    ci::gl::color(mColor.x, mColor.y, mColor.z, 255);
    ci::gl::drawVector(allJoints[0], allJoints[2]);
    //ci::gl::drawVector(allJoints[1], allJoints[2]);
    ci::gl::drawVector(allJoints[0], allJoints[3]);
    ci::gl::drawVector(allJoints[2], allJoints[4]);
    ci::gl::drawVector(allJoints[3], allJoints[5]);
    ci::gl::drawVector(allJoints[4], allJoints[6]);
    ci::gl::drawVector(allJoints[0], allJoints[6]);
    ci::gl::drawVector(allJoints[0], allJoints[7]);
    ci::gl::drawVector(allJoints[5], allJoints[7]);
    ci::gl::drawVector(allJoints[3], allJoints[8]);
    ci::gl::drawVector(allJoints[2], allJoints[9]);
    ci::gl::drawVector(allJoints[8], allJoints[10]);
    ci::gl::drawVector(allJoints[9], allJoints[11]);
    ci::gl::drawVector(allJoints[10], allJoints[12]);
    ci::gl::drawVector(allJoints[11], allJoints[13]);
    
    
    
}

ci::Vec3f User::getJointPosition(int whichJoint){
    return allJoints[whichJoint];
}

void User::setJointPosition(int whichJoint, ci::Vec3f newJointPosition){
    allJoints[whichJoint] = newJointPosition;
}

int User::getUserID(){
    return userID;
}

//******************************************************************************
//                  Gestures
//******************************************************************************
bool User::isWaving(){
    
    if(allJoints[rightHand].y > allJoints[rightElbow].y){
        
        if(getDifference(rightHand, 0) < -1 && getVecDifference(rightHand) > 2){
            return true;
        }
        
    }
    else if(allJoints[leftHand].y > allJoints[leftElbow].y){
        
        if(getDifference(leftHand, 0) < -1 && getVecDifference(leftHand) > 2){
            return true;
        }
    }
    else{
        
        return false;
    }
    
    return false;
}

bool User::isWavingLeft(){
    if(allJoints[leftHand].y > allJoints[leftElbow].y){
        
        if(getDifference(leftHand, 0) < -2 && getVecDifference(leftHand) > 2){
            std::cout << "Waving left" << std::endl;
            return true;
        }
        else return false;
    }
    else{
        return false;
    }
    
   
}

bool User::isWavingRight(){
    if(allJoints[rightHand].y > allJoints[rightElbow].y){
        
        if(getDifference(rightHand, 0) < -2 && getVecDifference(rightHand) > 2){
            std::cout << "Waving right" << std::endl;
            return true;
        }
        else return false;
        
    }
    else{
        return false;
    }
}

bool User::isThrowingLeft(){
    //if(activeJoints[leftHand]){
        if(getVecDifference(leftHand) > 1.5){
            //std::cout << "throwing left" << std::endl;
            return true;
        }
        else return false;
    //}
   // else return false;
}

bool User::isThrowingRight(){
   // if(activeJoints[rightHand]){
        //std::cout << "active Right" << std::endl;
   // std::cout << getVecDifference(rightHand) << std::endl;
        if(getVecDifference(rightHand) > 1.5){
           // std::cout << "throwing right" << std::endl;
            return true;
        }
        else return false;
    //}
    //else return false;
}

//******************************************************************************
//******************************************************************************

float User::getDifference(int whichJoint, int axis){
    if(axis == 0){
        return allJoints[whichJoint].x - allJointsZ[whichJoint].x;
    }
    else if(axis == 1){
        return allJoints[whichJoint].y - allJointsZ[whichJoint].y;
    }
    else if(axis == 2){
        return allJoints[whichJoint].z - allJointsZ[whichJoint].z;
    }
    else{
        return 0;
    }
    
}

float User::getVecDifference(int whichJoint){
    return allJoints[whichJoint].distance(allJointsZ[whichJoint]);
}

bool User::isActive(int whichJoint){
    return activeJoints[whichJoint];
}

void User::setActive(int whichJoint){
    activeJoints[whichJoint] = true;
}

void User::setUnactive(int whichJoint){
    activeJoints[whichJoint] = false;
}
