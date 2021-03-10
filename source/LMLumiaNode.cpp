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
    
//    CULog(std::to_string(_scale));
    std::cout << _scale << "\n";
    cugl::Mat4 translation_mat = cugl::Mat4::createScale(_scale);
    
    AnimationNode::draw(batch,translation_mat * transform, tint);
    
}
