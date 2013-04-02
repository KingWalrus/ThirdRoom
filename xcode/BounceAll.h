//
//  BounceAll.h
//  ThirdRoom
//
//  Created by Colin Honigman on 4/1/13.
//
//

#ifndef __ThirdRoom__BounceAll__
#define __ThirdRoom__BounceAll__

#include <iostream>
#include "MoveBehavior.h"

class BounceAll : public MoveBehavior{
public:
    
    BounceAll();
    ~BounceAll();
    
    void move(ci::Vec3f& velocity, ci::Vec3f& position);

};

#endif /* defined(__ThirdRoom__BounceAll__) */
