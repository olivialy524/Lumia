//
//  LMPlantNode.cpp
//  Lumia
//
//  Created by Vivi Ye on 3/11/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "EnergyNode.h"

void EnergyNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    
    _frameCount %= ANIMATION_INTERVAL;
    if (_frameCount == 0){
        int frame = getFrame() + 1 >= ANIMATION_COLS ? 0 : getFrame() + 1 ;
        setFrame(frame);
    }
    _frameCount++;
    AnimationNode::draw(batch,transform,tint);
    
}

