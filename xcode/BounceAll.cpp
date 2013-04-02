//
//  BounceAll.cpp
//  ThirdRoom
//
//  Created by Colin Honigman on 4/1/13.
//
//

#include "BounceAll.h"

BounceAll::BounceAll(){
    
}

BounceAll::~BounceAll(){
    
}

void BounceAll::move(ci::Vec3f& velocity, ci::Vec3f& position){
    position += velocity;
}
