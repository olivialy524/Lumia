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
    _sceneNode = nullptr;
    _node = nullptr;
}

void Button::setTextures(const std::shared_ptr<Texture>& button) {
    _sceneNode = scene2::SceneNode::allocWithBounds(Size(button->getWidth()/NUM_FRAMES,button->getHeight()));
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
    _node = ButtonNode::alloc(button, 1, 6, 6);
    auto scale =  getWidth()/(button->getWidth()/NUM_FRAMES/_drawScale);
    _sceneNode->setScale(scale);
    _node->setAnchor(Vec2::ZERO);
    _sceneNode->addChild(_node);
}

void Button::pushDown() {
    if (getPushedDown()) {
        return;
    }
    _node->setAnimState(ButtonNode::GoingDown);
    float height = (NUM_FRAMES - _node->getFrame())/NUM_FRAMES * getNormHeight();
    setHeight(height);
}

void Button::pushUp() {
    if (getPushedUp()) {
        return;
    }
    _node->setAnimState(ButtonNode::GoingUp);
    float height = (NUM_FRAMES - _node->getFrame())/NUM_FRAMES * getNormHeight();
    setHeight(height);
}

