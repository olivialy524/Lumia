//
//  LMPlant.cpp
//  Lumia
//
//  Created by Ben Dacek on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "SpikeModel.h"


void SpikeModel::setTextures(const std::shared_ptr<Texture>& spike, float angle) {
    
    _node = scene2::SceneNode::allocWithBounds(Size(spike->getWidth()/3.0f,spike->getHeight()/2.0f));
    _node->setAnchor(Vec2::ANCHOR_CENTER);
    _node->setAngle(angle);
    _spikeNode = SpikeNode::alloc(spike, 2, 3, 6);
    auto scale =  getWidth()/(spike->getWidth()/3.0f/_drawScale);
    _node->setScale(scale);
    _spikeNode->setAnchor(Vec2::ZERO);
    _spikeNode->setPosition(Vec2(0.0f, spike->getHeight()/2.0f/5.0f));
    _node->addChild(_spikeNode);
}

void SpikeModel::dispose() {
    _node = nullptr;
    _spikeNode = nullptr;
}
/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void SpikeModel::update(float dt) {
    BoxObstacle::update(dt);
    
    if (_node != nullptr) {
        _node->setPosition(getPosition()*_drawScale);
        _node->setAngle(getAngle());
    }
}
