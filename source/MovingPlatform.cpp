//
//  MovingPlatform.cpp
//  Lumia
//
//  Created by Ben Dacek on 4/1/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <cugl/cugl.h>
#include "MovingPlatform.h"

void MovingPlatform::move() {
    cugl::Vec2 dest;
    if (getReverse()) {
        dest = cugl::Vec2(getOriginalX(),getOriginalY());
    }
    else {
        dest = cugl::Vec2(getNewX(),getNewY());
    }
    cugl::Vec2 currpos = cugl::Vec2(getX(),getY());
    cugl::Vec2 dir = cugl::Vec2(dest,currpos);
    setX(getX() + dir.x*(1/120));
    setY(getY() + dir.y*(1/120));
}
