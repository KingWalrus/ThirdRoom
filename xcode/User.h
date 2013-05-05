//
//  User.h
//  ThirdRoom
//
//  Created by Colin Honigman on 3/26/13.
//
//

#ifndef __ThirdRoom__User__
#define __ThirdRoom__User__

//class Instrument;
//class Ball;
//class Screen;

#include "Instrument.h"
#include "Ball.h"
#include "Screen.h"
#include <iostream>
#include "cinder/gl/gl.h"
#include "cinder/TriMesh.h"
#include "cinder/app/AppBasic.h"




/*! \brief User Class
 * Stores status, userID, and joint positions */
class User{
public:
    User();
    User(int userId);
    ~User();
    
    void update();
    void display();
    
    enum JointNames{
        head = 0,
        neck,
        leftShoulder,
        rightShoulder,
        leftElbow,
        rightElbow,
        leftHand,
        rightHand,
        torso,
        leftHip,
        rightHip,
        leftKnee,
        rightKnee,
        leftFoot,
        rightFoot
        
    };
    enum axis{
        x = 0,
        y,
        z
    };
    
    ci::Vec3f       getJointPosition(int whichJoint);
    void            setJointPosition(int whichJoint, ci::Vec3f newJointPosition);
    bool            isWaving();
    bool            isWavingLeft();
    bool            isWavingRight();
    bool            isThrowingLeft();
    bool            isThrowingRight();
    void            prepareToClear();
    bool            isClearing();
    bool            isTouchingHands();
    bool            isActive(int whichJoint);
    void            setActive(int whichJoint);
    void            setUnactive(int whichJoint);
    int             getUserID();
    float           getDifference(int whichJoint, int axis);
    float           getPositionDistance(int whichJoint);
    float           getJointDifferenceX(int whichJoint1, int whichJoint2);
    float           getJointDifferenceY(int whichJoint1, int whichJoint2);
    float           getJointDifferenceZ(int whichJoint1, int whichJoint2);
    float           getJointDistance(int whichJoint1, int whichJoint2);
    ci::Vec3f       getPositionDifference(int whichJoint);
    float           getLastThrowLeft();
    float           getLastThrowRight();
    void            setLastCatchLeft(double left);
    void            setLastCatchRight(double right);
    double          getLastCatchLeft();
    double          getLastCatchRight();
    void            setLastThrowLeft(double time);
    void            setLastThrowRight(double time);
    bool            isGrouped();
    void            setGroup(bool );
    ci::ColorA      getColor();
    bool            hasScreen();
    void            setScreen(bool );
    ci::Vec3f       getMidpoint(int whichJoint1, int whichJoint2);
    Instrument*     isGesturing();
    
private:
    ci::Vec3f       allJoints[15], allJointsZ[15];
    ci::Vec3f       mSize, mColor;
    bool            active;
    bool            activeJoints[15];
    int             userID;
    bool            wave;
    ci::TriMesh     mesh;
    double          time;
    bool            preparingToClear;
    double          lastLeftThrow, lastRightThrow, catchLeft, catchRight;
    bool            grouped;
    bool            screen;
    int             waveCountLeft;
    int             waveCountRight;
    
    
    
};


#endif /* defined(__ThirdRoom__User__) */
