//
//  LMLumiaNode.cpp
//  Lumia
//
//  Created by Olivia Li on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "LumiaNode.h"
#define SHADOW_OFFSET   10.0f


void LumiaNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    
    switch (_state){
        case LumiaAnimState::Idle:{
            break;
        }
        case LumiaAnimState::Splitting:{
            break;
        }
        case LumiaAnimState::Merging:{
            break;
        }
            
    }
    AnimationNode::draw(batch, transform, tint);
    
}
