//
//  EnemyNode.cpp
//  Lumia
//
//  Created by Olivia Li on 4/5/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "EnemyNode.h"


void EnemyNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    if (_frameCount == 0){
        int frame = AnimationNode::getFrame() + 1 > 19 ? 0 : AnimationNode::getFrame() + 1;
        AnimationNode::setFrame(frame);
    }
    _frameCount = (_frameCount + 1) % ANIMATION_INTERVAL;
    
    AnimationNode::draw(batch, transform, tint);
    
}
