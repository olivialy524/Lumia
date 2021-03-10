//
//  LMLumiaNode.cpp
//  Lumia
//
//  Created by Olivia Li on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "LMLumiaNode.h"
#define SHADOW_OFFSET   10.0f


void LumiaNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    
//    CULog("here");
    cugl::Mat4 translation_mat = transform.getInverse() * cugl::Mat4::createScale(2.0f) * transform;
    
    AnimationNode::draw(batch,transform * translation_mat, tint);
    
}
