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

void Button::pushDown() {
    if (getHeight() <= getNormHeight()/4) {
        return;
    }
    setHeight(getHeight() - .01);
    setPosition(getPosition().x, getPosition().y-.005);
    if (getHeight() <= getNormHeight()/4) {
        setHeight(getNormHeight()/4);
    }
}
void Button::pushUp() {
if (getHeight() >= getNormHeight()) {
    return;
}
setHeight(getHeight() + .01);
    setPosition(getPosition().x, getPosition().y+.005);
if (getHeight() >= getNormHeight()) {
    setHeight(getNormHeight());
}
}

