//
//  InstrumentDecorator.h
//  ThirdRoom
//
//  Created by Colin Honigman on 4/9/13.
//
//

#ifndef __ThirdRoom__InstrumentDecorator__
#define __ThirdRoom__InstrumentDecorator__

#include <iostream>
#include "Ball.h"

class InstrumentDecorator : public Instrument{
public:
    InstrumentDecorator();
    ~InstrumentDecorator();
    
    virtual void        display() = 0;
    virtual void        update() = 0;
    virtual bool        hitTest(User* user) = 0;
};

#endif /* defined(__ThirdRoom__InstrumentDecorator__) */
