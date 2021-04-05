//
//  MovingPlatform.cpp
//  Lumia
//
//  Created by Ben Dacek on 4/1/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <cugl/cugl.h>
#include "MovingPlatform.h"
#include <cugl/physics2/CUPolygonObstacle.h>
void MovingPlatform::move(float scale) {
    
    cugl::Vec2 currpos = cugl::Vec2(getX(),getY());
    cugl::Vec2 dir;
    if (!getReverse()){
        dir = cugl::Vec2(cugl::Vec2(getOriginalX(),getOriginalY()), cugl::Vec2(getNewX(),getNewY()));
    }
    else {
        dir = -cugl::Vec2(cugl::Vec2(getOriginalX(),getOriginalY()), cugl::Vec2(getNewX(),getNewY()));
    }
    dir.normalize();
    //setPosition(getX()+(dir.x*.1), getY()+(dir.y*.1));
    setLinearVelocity(getVelocity()*dir.x,getVelocity()*dir.y);
    setDirX(dir.x);
    setDirY(dir.y);
    _node->setPosition(getX()*scale,getY()*scale);
    if ((getReverse() && std::abs((currpos.x-getWidth()/2) - getOriginalX()) <= .1 && std::abs((currpos.y-(getHeight()/2)) - getOriginalY()) <= .1 ) || (!getReverse() && std::abs((currpos.x-getWidth()/2) - getNewX()) <= .1 && std::abs((currpos.y-(getHeight()/2)) - getNewY()) <= .1 )) {
        
        reverse();
    }
}
void MovingPlatform::dispose() {
    _node = nullptr;
    _scenenode = nullptr;
}
