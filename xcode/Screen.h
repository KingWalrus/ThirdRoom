//
//  Screen.h
//  ThirdRoom
//
//  Created by Colin Honigman on 4/27/13.
//
//

#ifndef __ThirdRoom__Screen__
#define __ThirdRoom__Screen__

class Instrument;
class User;

#include <iostream>
#include "Instrument.h"

class Screen : public Instrument {
public:
    Screen(bool isHit);
    Screen(ci::Vec3f position);
    ~Screen();
    
    void        display();
    void        update();
    bool        hitTest(User* user);
    float       getDifference(int axis);
    float       getVecDifference();
    bool        collisionTest(Instrument* instrument);
    
    User*       mUser;

};

#endif /* defined(__ThirdRoom__Screen__) */
