//
//  Door.cpp
//  Lumia
//
//  Created by Ben Dacek on 4/3/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "SlidingDoor.h"


void SlidingDoor::setTextures(const std::shared_ptr<Texture>& image){
    auto platform = getPolygon() * _drawScale;

    // calcuate the drawing overlay scale
    float scalex = platform.getBounds().size.width/image->getWidth();
    float scaley = platform.getBounds().size.height/image->getHeight();
    _scenenode = scene2::PolygonNode::allocWithTexture(image);
    _scenenode->setScale(Vec2(scalex, scaley));
    _scenenode->setAngle(getAngle());
    _scenenode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
}

void SlidingDoor::dispose() {
    _scenenode = nullptr;
}

void SlidingDoor::Close() {
    if (!getClosing() || getPosition() == getOriginalPos()) {
    return;
    }
    cugl::Vec2 cpos = getPosition();
    cugl::Vec2 dir = -cugl::Vec2(getOriginalPos(),getNewPos());
    dir.normalize();
    setLinearVelocity(dir*2);
    if (cpos.distanceSquared(getOriginalPos()) <= .01) {
        setLinearVelocity(0,0);
        setPosition(getOriginalPos().x,getOriginalPos().y);
        setClosing(false);
    }
}

void SlidingDoor::Open() {
    if (!getOpening() || getPosition() == getNewPos()) {
    return;
    }
    cugl::Vec2 cpos = getPosition();
    cugl::Vec2 dir = cugl::Vec2(getOriginalPos(),getNewPos());
    dir.normalize();
    setLinearVelocity(dir*2);
    if (cpos.distanceSquared(getNewPos()) <= .01) {
        setLinearVelocity(0,0);
        setPosition(getNewPos().x,getNewPos().y);
        setOpening(false);
    }
}

void SlidingDoor::update(float dt) {
    PolygonObstacle::update(dt);
    if (_scenenode != nullptr) {
        _scenenode->setPosition(getPosition()*_drawScale);
        _scenenode->setAngle(getAngle());
    }
}
