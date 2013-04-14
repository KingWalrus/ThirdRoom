//
//  Gravity.h
//  ThirdRoom
//
//  Created by Colin Honigman on 4/9/13.
//
//

#ifndef __ThirdRoom__Gravity__
#define __ThirdRoom__Gravity__

#include <iostream>
#include "Ball.h"

class Gravity : public Ball {
public:
    Gravity(Instrument* i);
    ~Gravity();

    void update();
private:
    Instrument* instrument;
   

};


#endif /* defined(__ThirdRoom__Gravity__) */
