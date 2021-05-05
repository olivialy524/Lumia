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
}


void Button::setTextures(const std::shared_ptr<Texture>& button) {
    
    _sceneNode = scene2::SceneNode::allocWithBounds(Size(button->getWidth()/6.0f,button->getHeight()));
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
    _node = ButtonNode::alloc(button, 1, 6, 6);
    auto scale =  getWidth()/(button->getWidth()/6.0f/_drawScale);
    _sceneNode->setScale(scale);
    _node->setAnchor(Vec2::ZERO);
    _sceneNode->addChild(_node);
}

void Button::pushDown() {
    if (getPushedDown()) {
        return;
    }
//    _node->setContentHeight(_node->getContentHeight() - .01*sqrt(scale));
    _node->setAnimState(ButtonNode::GoingDown);
    float height = (6 - _node->getFrame())/6.0f * getNormHeight();
    setHeight(height);
//    setPosition(getPosition().x, getPosition().y-.015);
//    _node->setPosition(_node->getPositionX(), _node->getPositionY()-.005*scale);
//    if (getHeight() <= getNormHeight()/6.0f) {
//        setHeight(getNormHeight()/6.0f);
//    }
}
void Button::pushUp() {
    if (getPushedUp()) {
        return;
    }
//    setHeight(getHeight() + .03);
//    _node->setContentHeight(_node->getContentHeight() + .01*sqrt(scale));
    _node->setAnimState(ButtonNode::GoingUp);
    float height = (6 - _node->getFrame())/6.0f * getNormHeight();
    setHeight(height);
//    setPosition(getPosition().x, getPosition().y+.015);
//    _node->setPosition(_node->getPosition().x, _node->getPosition().y+.005*scale);
//    if (getHeight() >= getNormHeight()) {
//        setHeight(getNormHeight());
//    }
}

/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void Button::update(float dt) {
    BoxObstacle::update(dt);
    if (_node!=nullptr) {
//        _sceneNode->setPosition(getPosition()*_drawScale);
//        _sceneNode->setAngle(getAngle());
//        float height = (6 - _node->getFrame())/6.0f * getNormHeight();
//        setHeight(height);
    }
}

