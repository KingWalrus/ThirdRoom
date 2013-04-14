//
//  User.h
//  ThirdRoom
//
//  Created by Colin Honigman on 3/26/13.
//
//

#ifndef __ThirdRoom__User__
#define __ThirdRoom__User__

#include <iostream>
#include "cinder/gl/gl.h"

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
    bool            isActive(int whichJoint);
    void            setActive(int whichJoint);
    void            setUnactive(int whichJoint);
    int             getUserID();
    float           getDifference(int whichJoint, int axis);
    float           getVecDifference(int whichJoint);
    
private:
    ci::Vec3f       allJoints[14], allJointsZ[14];
    ci::Vec3f       mSize, mColor;
    bool            active;
    bool            activeJoints[14];
    int             userID;
    bool            wave;


    
};


#endif /* defined(__ThirdRoom__User__) */
