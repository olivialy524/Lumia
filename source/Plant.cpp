//
//  LMPlant.cpp
//  Lumia
//
//  Created by Ben Dacek on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "Plant.h"


void Plant::setTextures(const std::shared_ptr<Texture>& plant, float angle) {
    
    _node = scene2::SceneNode::allocWithBounds(Size(plant->getWidth()/10.0f,plant->getHeight()));
    _node->setAnchor(Vec2::ANCHOR_CENTER);
    _node->setAngle(angle);
    _plantNode = PlantNode::alloc(plant, 1, 10, 10);
    auto scale = 2.5f * getWidth()/(plant->getWidth()/10.0f/_drawScale);
    _node->setScale(scale);
    _plantNode->setAnchor(Vec2::ZERO);
    _node->addChild(_plantNode);
}

void Plant::dispose() {
    _node = nullptr;
    _plantNode = nullptr;
}
/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void Plant::update(float dt) {
    BoxObstacle::update(dt);
    
    if (_node != nullptr) {
        _node->setPosition(getPosition()*_drawScale);
        _node->setAngle(getAngle());
    }
}
