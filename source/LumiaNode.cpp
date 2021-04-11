//
//  LMLumiaNode.cpp
//  Lumia
//
//  Created by Olivia Li on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "LumiaNode.h"

bool LumiaNode::init(const std::shared_ptr<cugl::Texture> &idleAnimation,
                     const std::shared_ptr<cugl::Texture> &splittingAnimation){
    _idleAnimation = AnimationNode::alloc(idleAnimation, ANIMATION_ROWS, ANIMATION_COLS, ANIMATION_SIZE);
    _idleAnimation
    return true;
}

void LumiaNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    
    switch (_state){
        case LumiaAnimState::Idle:{
            _frameCount %= IDLE_ANIMATION_INTERVAL;
            
            break;
        }
        case LumiaAnimState::Splitting:{
            _frameCount %= SPLIT_ANIMATION_INTERVAL;
            if (_frameCount == 0){
                int frame = AnimationNode::getFrame() + 1;
                AnimationNode::setFrame(frame);
                if (frame == 19){
                    setAnimState(LumiaAnimState::SplitFinished);
                }
            }
            break;
        }
        default:{
            break;
        }
    }
    _frameCount++;
    
    AnimationNode::draw(batch, transform, tint);
    
}
