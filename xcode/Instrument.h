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
//#include <string>
#include "cinder/Vector.h"
#include "User.h"
#include "MoveBehavior.h"
#include "BounceAll.h"
#include "BounceUpDown.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

/*! \brief Instrument class
 * The abstract parent of all instruments */
 
class Instrument {
public:
                    Instrument(bool isHit);
                    Instrument();
    virtual         ~Instrument();
    virtual void    display() = 0;
    virtual void    update() = 0;
    void            move(ci::Vec3f& velocity, ci::Vec3f& position);
    virtual bool    hitTest(User* user) = 0;
    virtual bool    collisionTest(Instrument* instrument) = 0;
    
    void            initializePosition(ci::Vec3f firstPosition); //sets mPosition and mPositionZ
    ci::Vec3f       getPosition();
    ci::Vec3f       getZposition();
    void            setPosition(ci::Vec3f newPosition);
    void            setZposition(ci::Vec3f lastPosition);
    
    ci::Vec3f       getVelocity();
    ci::Vec3f*      getVelocityPtr();
    void            setVelocity(ci::Vec3f newVelocity);
    
    ci::ColorA      getColor();
    void            setColor(ci::Vec3f newColor);
    void            setColor(ci::ColorA newColor);
    
    ci::Vec3f       getSize();
    void            setSize(ci::Vec3f newSize);
   
    void            setCreatedAt(double creationTime);
    double          getTime();
    void            setLastThrown(double thrownTime);;
    
    void            setNote(int newNote);
    int             getNote();
    
    std::string     getName();
    void            setName(std::string n);
    
    ci::Vec3f       getVectorDifference(ci::Vec3f v1, ci::Vec3f v2);
    ci::Vec3f       getPositionDifference();
    
    int             getUserID();
    void            setUserID(int _userID);
    
    void            setMoveBehavior(MoveBehavior* mb);
    void            setBoundaries(ci::Vec3f boundaries);
    ci::Vec3f       getBoundaries();

    void            setHit(bool hit);
    bool            isHit();
    void            setMoving(bool moving);
    bool            isMoving();
    
private:
    
    MoveBehavior* moveBehavior;
    ci::Vec3f   roomSize;
    
    ci::Vec3f       mPosition;
    ci::Vec3f       mPositionZ;
    ci::Vec3f       mSize;
    ci::Vec3f       mVelocity;
    ci::Vec3f       mColor;
    ci::Vec3f       mControllerPosition;
    ci::Vec3f       mControllerPositionZ;
    ci::ColorA      mColorA;
    bool            bHit;
    bool            bHitZ;
    bool            bMoving;
    double          createdAt;
    int             note;
    std::string     name;
    int             userID;
    double          lastThrown;



};


#endif
