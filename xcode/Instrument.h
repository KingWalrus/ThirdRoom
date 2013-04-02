//
//  Instrument.h
//  ThirdRoom
//
//  Created by Colin Honigman on 3/26/13.
//
//

#ifndef ThirdRoom_Instrument_h
#define ThirdRoom_Instrument_h

#include <vector>
#include "cinder/Vector.h"
#include "User.h"
#include "MoveBehavior.h"
#include "BounceAll.h"
#include "BounceUpDown.h"

/*! \brief Instrument class
 * The abstract parent of all instruments */
 
class Instrument {
public:
                    Instrument();
                    ~Instrument();
    virtual void    display() = 0;
    virtual void    update() = 0;
    void            move(ci::Vec3f& velocity, ci::Vec3f& position);
    virtual bool    hitTest(User* user) = 0;
    ci::Vec3f       getPosition();
    void            setPosition(ci::Vec3f newPosition);
    ci::Vec3f       getVelocity();
    void            setVelocity(ci::Vec3f newVelocity);
    ci::Vec3f       getColor();
    void            setColor(ci::Vec3f newColor);
    ci::Vec3f       getSize();
    void            setSize(ci::Vec3f newSize);
    void            setMoveBehavior(MoveBehavior* mb);
    
    ci::Vec3f       mPosition;
    ci::Vec3f       mSize;
    ci::Vec3f       mVelocity;
    ci::Vec3f       mColor;
    bool            bHit;
    bool            bMoving;
    
private:
    
    MoveBehavior* moveBehavior;

    


};


#endif
