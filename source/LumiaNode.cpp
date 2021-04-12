//
//  LMLumiaNode.cpp
//  Lumia
//
//  Created by Olivia Li on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "LumiaNode.h"

void LumiaNode::dispose(){
    _idleAnimation = nullptr;
    _splittingAnimation = nullptr;
}

bool LumiaNode::setTextures(const std::shared_ptr<cugl::Texture> &idleAnimation,
                     const std::shared_ptr<cugl::Texture> &splittingAnimation,
                     float radius,
                     float drawScale){
    auto scale = radius*2/(idleAnimation->getHeight()/4.0f/drawScale);
    _idleAnimation = cugl::scene2::AnimationNode::alloc(idleAnimation, ANIMATION_ROWS, ANIMATION_COLS, ANIMATION_SIZE);
    setScale(scale);
    _idleAnimation->setAnchor(Vec2::ZERO);
    Vec2 offset = Vec2(splittingAnimation->getWidth()/5.0f - idleAnimation->getWidth()/5.0f ,splittingAnimation->getHeight()/4.0f - idleAnimation->getHeight()/4.0f);
    _idleAnimation->setPosition(offset/2);
    _idleAnimation->setFrame(0);
    addChild(_idleAnimation);
    
    _splittingAnimation = cugl::scene2::AnimationNode::alloc(splittingAnimation, ANIMATION_ROWS, ANIMATION_COLS, ANIMATION_SIZE);
    _splittingAnimation->setAnchor(Vec2::ANCHOR_CENTER);
    _splittingAnimation->setFrame(0);
    addChild(_splittingAnimation);
    
    setAnimState(Idle);
    return true;
}

void LumiaNode::setAnimState(LumiaAnimState state){
    switch (state){
        case Splitting:
        case SplitFinished:{
            _splittingAnimation->setVisible(true);
            _idleAnimation->setVisible(false);
            break;
        }
        default:{
            _splittingAnimation->setVisible(false);
            _idleAnimation->setVisible(true);
            break;
        }
    }
    _state = state;

}

void LumiaNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    
    switch (_state){
        case Idle:{
            _frameCount %= IDLE_ANIMATION_INTERVAL;
            if (_frameCount == 0){
                int frame = _idleAnimation->getFrame() + 1 >= ANIMATION_SIZE ? 0 : _idleAnimation->getFrame() + 1 ;
                _idleAnimation->setFrame(frame);
            }
            
            break;
        }
        case Splitting:{
            _frameCount %= SPLIT_ANIMATION_INTERVAL;
            
            if (_frameCount == 0){
                int frame = _splittingAnimation->getFrame() + 1;
                _splittingAnimation->setFrame(frame);
                if (frame == ANIMATION_SIZE - 1){
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
    
    SceneNode::draw(batch, transform, tint);
    
}
