//
//  Ball.h
//  ThirdRoom
//
//  Created by Colin Honigman on 3/26/13.
//
//

#ifndef __ThirdRoom__Ball__
#define __ThirdRoom__Ball__

//class Instrument;
class User;

#include <iostream>
#include "Instrument.h"
//#include "User.h"

/*! \brief The Ball Class
 * The first kind of instrument that is sphere shaped and can be directly
 * interacted with by the user */

class Ball : public Instrument{
public:
    Ball(bool isHit);
    Ball(ci::Vec3f position, ci::Vec3f roomDimensions);
    ~Ball();
    
    void        display();
    void        update();
    bool        hitTest(User* user);
    float       getDifference(int axis);
    float       getVecDifference();
    void        wallHit(int side);
    bool        ballHit(Instrument* ball);
    bool        collisionTest(Instrument* instrument);
    void        setNote(int newNote);
private:
    User*       mUser;
    long        lastThrown;
    int         notes[5];

    
};

#endif /* defined(__ThirdRoom__Ball__) */
