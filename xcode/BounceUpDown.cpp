//
//  BounceUpDown.cpp
//  ThirdRoom
//
//  Created by Colin Honigman on 4/1/13.
//
//

#include "BounceUpDown.h"

BounceUpDown::BounceUpDown(){
    
}

BounceUpDown::~BounceUpDown(){
    
}

void BounceUpDown::move(ci::Vec3f &velocity, ci::Vec3f &position){

        position.y += velocity.y;


}