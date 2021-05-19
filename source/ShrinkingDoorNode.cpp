//
//  ShrinkingDoorNode.cpp
//  Lumia
//
//  Created by Olivia Li on 5/18/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "ShrinkingDoorNode.h"
void ShrinkingDoorNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    switch (_state){
        case Open:{
            setFrame(ANIMATION_SIZE - 1);
            break;
        }
        case Closing:{
            _frameCount %= ANIMATION_INTERVAL;
            if (_frameCount == 0){
                int frame = getFrame();
                if (frame == 0){
                    setAnimState(Closed);
                }else{
                    setFrame(frame - 1);
                }
            }
            break;
        }
        case Opening:{
            _frameCount %= ANIMATION_INTERVAL;
            if (_frameCount == 0){
                int frame = getFrame();
                if (frame == ANIMATION_SIZE - 1){
                    setAnimState(Open);
                }else{
                    setFrame(frame + 1);
                }
            }
            break;
        }
        case Closed:{
            setFrame(0);
            break;
        }
    }
    _frameCount ++;
    AnimationNode::draw(batch,transform,tint);
    
}


