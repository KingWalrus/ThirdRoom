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
#include "cinder/app/AppBasic.h"

/*! \brief Instrument class
 * The abstract parent of all instruments */
 
class Instrument {
public:
                    Instrument();
                    ~Instrument();
    virtual void    display();
    virtual void    update();
    virtual bool    hitTest();
    ci::Vec3f       getPosition();
    void            setPosition(ci::Vec3f newPosition);
    ci::Vec3f       getVelocity();
    void            setVelocity(ci::Vec3f newVelocity);
    ci::Vec3f       getColor();
    void            setColor(ci::Vec3f newColor);
    
    

private:
    ci::Vec3f       mPos;
    ci::Vec3f       mSize;
    ci::Vec3f       mVelocity;
    ci::Vec3f       mColor;
    bool            bHit;
    bool            bMoving;


};


#endif
