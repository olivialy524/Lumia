//
//  LMPlantNode.cpp
//  Lumia
//
//  Created by Vivi Ye on 3/11/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "SpikeNode.h"
#define SHADOW_OFFSET   10.0f


void SpikeNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    AnimationNode::draw(batch,transform,tint);
    
}

