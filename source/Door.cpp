//
//  Door.cpp
//  Lumia
//
//  Created by Ben Dacek on 4/3/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "Door.h"
void Door::dispose() {
    _node = nullptr;
    _scenenode = nullptr;
}

void Door::Close(float scale) {
    if (!getClosing() || getPosition() == getOriginalPos()) {
    return;
    }
    cugl::Vec2 cpos = getPosition();
    cugl::Vec2 dir = -cugl::Vec2(getOriginalPos(),getNewPos());
    dir.normalize();
    setLinearVelocity(dir*2);
    _node->setPosition(getX()*scale,getY()*scale);
    if (std::abs((cpos.x)-getOriginalPos().x) <= .1 && std::abs((cpos.y)-getOriginalPos().y) <= .1) {
        setLinearVelocity(0,0);
        setPosition(getOriginalPos().x,getOriginalPos().y);
        setClosing(false);
    }
}

void Door::Open(float scale) {
    if (!getOpening() || getPosition() == getNewPos()) {
    return;
    }
    cugl::Vec2 cpos = getPosition();
    cugl::Vec2 dir = cugl::Vec2(getOriginalPos(),getNewPos());
    dir.normalize();
    setLinearVelocity(dir*2);
    _node->setPosition(getX()*scale,getY()*scale);
    if (std::abs((cpos.x)-getNewPos().x) <= .1 && std::abs((cpos.y)-getNewPos().y) <= .1) {
        setLinearVelocity(0,0);
        setPosition(getNewPos().x,getNewPos().y);
        setOpening(false);
    }
}
