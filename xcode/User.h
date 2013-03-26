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
#include "cinder/app/AppBasic.h"

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
        
    } joints;
    
    ci::Vec3f       getJointPosition(int whichJoint);
    int             getUserID();
    
private:
    ci::Vec3f       allJoints[13], allJointsZ[13];
    ci::Vec3f       mSize, mColor;
    bool            active;
    int             userID;

    
};


#endif /* defined(__ThirdRoom__User__) */
