//
//  BounceUpDown.h
//  ThirdRoom
//
//  Created by Colin Honigman on 4/1/13.
//
//

#ifndef __ThirdRoom__BounceUpDown__
#define __ThirdRoom__BounceUpDown__

#include <iostream>
#include "MoveBehavior.h"

class BounceUpDown : public MoveBehavior {
public:
    BounceUpDown();
    ~BounceUpDown();
    
    void move(ci::Vec3f& velocity, ci::Vec3f& position);
    
};


#endif /* defined(__ThirdRoom__BounceUpDown__) */
