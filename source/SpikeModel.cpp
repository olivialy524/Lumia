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
    _node = scene2::SceneNode::allocWithBounds(Size(spike->getWidth(), spike->getHeight()));
    _node->setAnchor(Vec2::ANCHOR_CENTER);
    _node->setAngle(angle);
    _spikeNode = SpikeNode::alloc(spike, 1, 1, 1);
    auto scale = getWidth() / (spike->getWidth()/ _drawScale);
    _node->setScale(scale);
    _spikeNode->setAnchor(Vec2::ZERO);
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
