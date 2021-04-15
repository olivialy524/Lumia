//
//  LMEnergyModel.cpp
//  Lumia
//
//  Created by Ben Dacek on 3/17/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include "EnergyModel.h"

void EnergyModel::setTextures(const std::shared_ptr<Texture>& texture){
    
    _node = scene2::SceneNode::allocWithBounds(Size(texture->getWidth()/9.0f,texture->getHeight()));
    _node->setAnchor(Vec2::ANCHOR_CENTER);
    _energyNode = EnergyNode::alloc(texture, 1, 9, 9);
    auto scale =  getWidth()/(texture->getWidth()/9.0f/_drawScale);
    _node->setScale(scale);
    _energyNode->setAnchor(Vec2::ZERO);
    _node->addChild(_energyNode);
}

bool EnergyModel::init(const cugl::Vec2 pos, cugl::Size size) {
    _removed = false;

    if (BoxObstacle::init(pos, size)) {
        return true;
    }
    return false;
}

void EnergyModel::dispose() {
    _node = nullptr;
    _energyNode = nullptr;
}

