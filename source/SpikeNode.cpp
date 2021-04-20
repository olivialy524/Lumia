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
    //switch (_state){
    //    case Dark:{
    //        setFrame(0);
    //        break;
    //    }
    //    case LightingUp:{
    //        _frameCount %= LIGHT_UP_ANIMATION_INTERVAL;
    //        if (_frameCount == 0){
    //            int frame = getFrame() + 1;
    //            setFrame(frame);
    //            if (frame == LIT_ANIMATION_END){
    //                setAnimState(PlantAnimState::Lit);
    //            }
    //        }
    //        break;
    //    }
    //    case Lit:{
    //        _frameCount %= LIT_ANIMATION_INTERVAL;
    //        if (_frameCount == 0){
    //            int frame = getFrame() + (_frameIncreasing ? 1 : -1);
    //            setFrame(frame);
    //            if (frame == LIT_ANIMATION_END || frame == LIT_ANIMATION_START){
    //                _frameIncreasing = !_frameIncreasing;
    //            }
    //        }
    //        break;
    //    }
    //}
    //_frameCount ++;
    //AnimationNode::draw(batch,transform,tint);
    
}

