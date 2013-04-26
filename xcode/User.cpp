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
    for(int i = 0; i < 15; i++){
        activeJoints[i] = false;
    }
    time = ci::app::getElapsedSeconds();
    preparingToClear = false;
}

User::~User(){
    
}

void User::update(){
    
    prepareToClear();
    for(int i = 0; i < 14; i++){
        allJointsZ[i] = allJoints[i];
        
    }
    
}

void User::display(){
    /*
    ci::gl::color(mColor.x, mColor.y, mColor.z, 255);
    ci::gl::drawVector(allJoints[head],             allJoints[leftShoulder]);
    ci::gl::drawVector(allJoints[head],             allJoints[rightShoulder]);
    ci::gl::drawVector(allJoints[leftShoulder],     allJoints[leftElbow]);
    ci::gl::drawVector(allJoints[rightShoulder],    allJoints[rightElbow]);
    ci::gl::drawVector(allJoints[leftElbow],        allJoints[leftHand]);
    ci::gl::drawVector(allJoints[rightElbow],       allJoints[rightHand]);
    ci::gl::drawVector(allJoints[head],             allJoints[leftHand]);
    ci::gl::drawVector(allJoints[head],             allJoints[rightHand]);
    ci::gl::drawVector(allJoints[leftShoulder],     allJoints[leftHip]);
    ci::gl::drawVector(allJoints[rightShoulder],    allJoints[rightHip]);
    ci::gl::drawVector(allJoints[leftHip],          allJoints[leftKnee]);
    ci::gl::drawVector(allJoints[rightHip],         allJoints[rightKnee]);
    ci::gl::drawVector(allJoints[leftKnee],         allJoints[leftFoot]);
    ci::gl::drawVector(allJoints[rightKnee],        allJoints[rightFoot]);
    */
    double tempTime = ci::app::getElapsedSeconds();
    double tempCosTime = tempTime;
    tempTime = (1 + sin(tempTime-time))/2;
    tempCosTime = (1 + cos(tempCosTime-time))/2;
    mesh.clear();
    mesh.appendVertex(getJointPosition(head));
    mesh.appendColorRGB(ci::Color(tempTime, 0, 0));
    mesh.appendVertex(getJointPosition(leftShoulder));
    mesh.appendColorRGB(ci::Color(0, tempCosTime, 0));
    mesh.appendVertex(getJointPosition(leftElbow));
    mesh.appendColorRGB(ci::Color(0,0, tempTime));
    mesh.appendVertex(getJointPosition(leftHand));
    mesh.appendColorRGB(ci::Color(tempCosTime, 0, 0));
    mesh.appendVertex(getJointPosition(leftHip));
    mesh.appendColorRGB(ci::Color(0, tempTime, 0));
    mesh.appendVertex(getJointPosition(leftKnee));
    mesh.appendColorRGB(ci::Color(0, 0, tempCosTime));
    mesh.appendVertex(getJointPosition(leftFoot));
    mesh.appendColorRGB(ci::Color(tempTime, 0, 0));
    mesh.appendVertex(getJointPosition(rightFoot));
    mesh.appendColorRGB(ci::Color(0, tempCosTime, 0));
    mesh.appendVertex(getJointPosition(rightKnee));
    mesh.appendColorRGB(ci::Color(0, 0, tempTime));
    mesh.appendVertex(getJointPosition(rightHip));
    mesh.appendColorRGB(ci::Color(tempCosTime, 0, 0));
    mesh.appendVertex(getJointPosition(rightHand));
    mesh.appendColorRGB(ci::Color(0, tempTime, 0));
    mesh.appendVertex(getJointPosition(rightElbow));
    mesh.appendColorRGB(ci::Color(0, 0, tempCosTime));
    mesh.appendVertex(getJointPosition(rightShoulder));
    mesh.appendColorRGB(ci::Color(tempTime, 0, 0));
    
    int vIdx0 = mesh.getNumVertices()-13;
    int vIdx1 = mesh.getNumVertices()-12;
    int vIdx2 = mesh.getNumVertices()-11;
    int vIdx3 = mesh.getNumVertices()-10;
    int vIdx4 = mesh.getNumVertices()-9;
    int vIdx5 = mesh.getNumVertices()-8;
    int vIdx6 = mesh.getNumVertices()-7;
    int vIdx7 = mesh.getNumVertices()-6;
    int vIdx8 = mesh.getNumVertices()-5;
    int vIdx9 = mesh.getNumVertices()-4;
    int vIdx10 = mesh.getNumVertices()-3;
    int vIdx11 = mesh.getNumVertices()-2;
    int vIdx12 = mesh.getNumVertices()-1;
    
    mesh.appendTriangle(vIdx0, vIdx1, vIdx2);
    mesh.appendTriangle(vIdx1, vIdx2, vIdx3);
    mesh.appendTriangle(vIdx2, vIdx3, vIdx4);
    mesh.appendTriangle(vIdx3, vIdx4, vIdx5);
    mesh.appendTriangle(vIdx4, vIdx5, vIdx6);
    mesh.appendTriangle(vIdx5, vIdx6, vIdx7);
    mesh.appendTriangle(vIdx6, vIdx7, vIdx8);
    mesh.appendTriangle(vIdx7, vIdx8, vIdx9);
    mesh.appendTriangle(vIdx8, vIdx9, vIdx10);
    mesh.appendTriangle(vIdx9, vIdx10, vIdx11);
    mesh.appendTriangle(vIdx10, vIdx11, vIdx12);
    mesh.appendTriangle(vIdx11, vIdx12, vIdx0);
    
    ci::gl::draw(mesh);

    
    
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
        
        if(getDifference(rightHand, 0) < -2 && getVecDifference(rightHand) > 2){
            return true;
        }
        
    }
    else if(allJoints[leftHand].y > allJoints[leftElbow].y){
        
        if(getDifference(leftHand, 0) < -2 && getVecDifference(leftHand) > 2){
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
            //std::cout << "Waving left" << std::endl;
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
           // std::cout << "Waving right" << std::endl;
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
        if(getVecDifference(leftHand) > 1.0){
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
    //std::cout << getVecDifference(rightHand) << std::endl;
        if(getVecDifference(rightHand) > 1.0){
           // std::cout << "throwing right" << std::endl;
            return true;
        }
        else return false;
    //}
    //else return false;
}

void User::prepareToClear(){
    if(allJoints[rightHand].x < allJoints[leftShoulder].x && allJoints[rightHand].y > allJoints[leftElbow].y){
        preparingToClear = true;
    }
    else preparingToClear = false;
}

bool User::isClearing(){
    if(allJoints[rightHand].x < allJoints[leftShoulder].x && allJoints[rightHand].y > allJoints[leftElbow].y){
        //std::cout << getDifference(rightHand, 0) << std::endl;
        if((getDifference(rightHand, 0) < -2 || getDifference(rightHand, 0) > 2) && getVecDifference(rightHand) > 2){
            std::cout << "CLEARING!!!" << std::endl;
            return true;
        }
        else return false;
    }
    else return false;
    
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

ci::ColorA User::getColor(){
    return ci::ColorA(mColor.x, mColor.y, mColor.z, 1.0);
}