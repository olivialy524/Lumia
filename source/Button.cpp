//
//  Button.cpp
//  Lumia
//
//  Created by Ben Dacek on 4/3/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include "Button.h"
void Button::dispose() {
    _node = nullptr;
    _sceneNode = nullptr;
}

void Button::pushDown(float scale) {
    if (getHeight() <= getNormHeight()/4) {
        return;
    }
    setHeight(getHeight() - .01);
    _node->setContentHeight(_node->getContentHeight() - .01*sqrt(scale));
    setPosition(getPosition().x, getPosition().y-.005);
    _node->setPosition(_node->getPositionX(), _node->getPositionY()-.005*scale);
    if (getHeight() <= getNormHeight()/4) {
        setHeight(getNormHeight()/4);
    }
}
void Button::pushUp(float scale) {
if (getHeight() >= getNormHeight()) {
    return;
}
setHeight(getHeight() + .01);
_node->setContentHeight(_node->getContentHeight() + .01*sqrt(scale));
    setPosition(getPosition().x, getPosition().y+.005);
    _node->setPosition(_node->getPosition().x, _node->getPosition().y+.005*scale);
if (getHeight() >= getNormHeight()) {
    setHeight(getNormHeight());
}
}

