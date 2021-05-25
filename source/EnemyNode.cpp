//
//  EnemyNode.cpp
//  Lumia
//
//  Created by Olivia Li on 4/5/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "EnemyNode.h"

void EnemyNode::dispose(){
    _chasingAnimation = nullptr;
    _escapingAnimation = nullptr;
}

bool EnemyNode::setTextures(const std::shared_ptr<cugl::Texture> &chasingAnimation,
                            const std::shared_ptr<cugl::Texture> &escapingAnimation,
                            float radius,
                            float drawScale){
    auto scale =  radius*2/(chasingAnimation->getHeight()/drawScale);
    setScale(scale);
    
    _chasingAnimation = cugl::scene2::AnimationNode::alloc(chasingAnimation, 1, 12, 12);
    _chasingAnimation->setAnchor(Vec2::ZERO);
    _chasingAnimation->setFrame(0);
    _chasingAnimation->setRelativeColor(false);
    addChild(_chasingAnimation);
    
    _escapingAnimation = cugl::scene2::AnimationNode::alloc(escapingAnimation, 1, 12, 12);
    _escapingAnimation->setAnchor(Vec2::ZERO);
    _escapingAnimation->setFrame(0);
    float xscale = chasingAnimation->getWidth() / 12.0f;
    float xscale2 = escapingAnimation->getWidth() / 12.0f;
    _escapingAnimation->setScale( xscale/xscale2);
    _escapingAnimation->setRelativeColor(false);
    addChild(_escapingAnimation);
    return true;
}

void EnemyNode::setAnimState(EnemyAnimState state){
    switch (state){
        case Idle:
        case Escaping:{
            _chasingAnimation->setVisible(false);
            _escapingAnimation->setVisible(true);
            break;
        }
        case Chasing:{
            _chasingAnimation->setVisible(true);
            _escapingAnimation->setVisible(false);
            break;
        }
    }
    _state = state;

}

void EnemyNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    
    switch (_state){
        case Chasing:{
            _frameCount %= ANIMATION_INTERVAL;
            
            if (_frameCount == 0){
                int frame = _chasingAnimation->getFrame() + 1 >= 12 ? 0 : _chasingAnimation->getFrame() + 1 ;
                _chasingAnimation->setFrame(frame);
            }
            break;
        }
        case Idle:
        case Escaping:{
            _frameCount %= ANIMATION_INTERVAL;
            
            if (_frameCount == 0){
                int frame = _escapingAnimation->getFrame() + 1 >= 12 ? 0 : _escapingAnimation->getFrame() + 1 ;
                _escapingAnimation->setFrame(frame);
            }
            break;
        }
    }
    _frameCount++;
    
    SceneNode::draw(batch, transform, tint);
}
