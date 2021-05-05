//
//  ButtonNode.cpp
//  Lumia
//
//  Created by Olivia Li on 5/4/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "ButtonNode.h"
void ButtonNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    switch (_state){
        case Pressed:{
            setFrame(ANIMATION_SIZE - 1);
            break;
        }
        case GoingUp:{
            _frameCount %= PRESSING_ANIMATION_INTERVAL;
            if (_frameCount == 0){
                int frame = getFrame();
                if (frame == 0){
                    setAnimState(Idle);
                }else{
                    setFrame(frame - 1);
                }
            }
            break;
        }
        case GoingDown:{
            _frameCount %= PRESSING_ANIMATION_INTERVAL;
            if (_frameCount == 0){
                int frame = getFrame();
                if (frame == ANIMATION_SIZE - 1){
                    setAnimState(Pressed);
                }else{
                    setFrame(frame + 1);
                }
            }
            break;
        }
        case Idle:{
            _frameCount %= IDLE_ANIMATION_INTERVAL;
            if (_frameCount == 0){
                int frame = getFrame();
                setFrame(frame == 0 ? 1 : 0);
            }
            break;
        }
    }
    _frameCount ++;
    AnimationNode::draw(batch,transform,tint);
    
}

