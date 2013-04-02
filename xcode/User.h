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
    ~User();
    
    void update();
    void display();
    
    enum JointNames{
        head = 0,
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
    
    ci::Vec3f       getJointPosition(int whichJoint);
    void            setJointPosition(int whichJoint, ci::Vec3f newJointPosition);
    int             getUserID();
    
private:
    ci::Vec3f       allJoints[13], allJointsZ[13];
    ci::Vec3f       mSize, mColor;
    bool            active;
    bool            activeJoints[13];
    int             userID;

    
};


#endif /* defined(__ThirdRoom__User__) */
