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
    setTravelX((dir.x*.1));
    setTravelY((dir.y*.1));
    setPosition(getX()+(dir.x*.1), getY()+(dir.y*.1));
    
    if (getReverse() && std::abs(currpos.x - getOriginalX()) <= .9 && std::abs(currpos.y - getOriginalY()) <= .9 ) {
        reverse();
    }
    else if (!getReverse() && std::abs(currpos.x - getNewX()) <= .9 && std::abs(currpos.y - getNewY()) <= .9 ){
        reverse();
    }
    _node->setPosition(getX()*scale,getY()*scale);
}
void MovingPlatform::dispose() {
    _node = nullptr;
    _scenenode = nullptr;
}
