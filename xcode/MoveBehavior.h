//
//  MoveBehavior.h
//  ThirdRoom
//
//  Created by Colin Honigman on 4/1/13.
//
//

#ifndef __ThirdRoom__MoveBehavior__
#define __ThirdRoom__MoveBehavior__

#include <iostream>
#include "cinder/Vector.h"

class MoveBehavior {
public:
    MoveBehavior();
    virtual ~MoveBehavior();
    
    virtual void move(ci::Vec3f& velocity, ci::Vec3f& position) = 0;
};

#endif /* defined(__ThirdRoom__MoveBehavior__) */
