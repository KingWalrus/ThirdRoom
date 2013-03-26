//
//  Bounce.h
//  ThirdRoom
//
//  Created by Colin Honigman on 2/21/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef ThirdRoom_Bounce_h
#define ThirdRoom_Bounce_h
#include <vector>
#include "cinder/app/AppBasic.h"

class Bounce{
public:
    Bounce();
    Bounce(ci::Vec3f pos, int size, int idNumber);
    void display();
    void update();
    ci::Vec3f getPosition(){return mPosition;};
    void setPosition(ci::Vec3f pos){mPosition = pos;};
    int getID(){return ID;};
    void setVelocity(float x, float y, float z){ mVelocity = ci::Vec3f(x, y, z);};
    
private:
    ci::Vec3f mPosition;
    ci::Vec3f mVelocity;
    ci::Color mColor;
    
    int radius;
    int ID;
    
};


#endif
