//
//  LMLumiaNode.cpp
//  Lumia
//
//  Created by Olivia Li on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "LumiaNode.h"

void LumiaNode::dispose(){
    _idleAnimation = nullptr;
    _splittingAnimation = nullptr;
}

bool LumiaNode::setTextures(const std::shared_ptr<cugl::Texture> &idleAnimation,
                     const std::shared_ptr<cugl::Texture> &splittingAnimation,
                     const std::shared_ptr<cugl::Texture> &deathAnimation,
                     const std::shared_ptr<cugl::Texture> &indicator,
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

    float angle = 2*3.14f/(_level+1);
    Vec2 center = Vec2(splittingAnimation->getWidth()/5.0f/2.0f,splittingAnimation->getHeight()/4.0f/2.0f);
    float r = idleAnimation->getHeight()/4.0f/2.0f * 0.9f;
    
    for (int i=0; i<= _level; i++){
        std::shared_ptr<cugl::scene2::PolygonNode> ind = cugl::scene2::PolygonNode::allocWithTexture(indicator);
        ind->setAnchor(Vec2::ANCHOR_CENTER);
        float ang = angle * i;
        ind->setPosition(center.x + r*cos(1.57 - ang), center.y + r*sin(1.57 - ang));
        
        _indicatorNode.push_back(ind);
    }
    
    for (auto node : _indicatorNode){
        node->setScale(0.4f);
        addChild(node);
    }
    
    _splittingAnimation = cugl::scene2::AnimationNode::alloc(splittingAnimation, ANIMATION_ROWS, ANIMATION_COLS, ANIMATION_SIZE);
    _splittingAnimation->setAnchor(Vec2::ANCHOR_CENTER);
    _splittingAnimation->setFrame(0);
    addChild(_splittingAnimation);
    
    _deathAnimation = cugl::scene2::AnimationNode::alloc(deathAnimation, ANIMATION_ROWS, ANIMATION_COLS, ANIMATION_SIZE);
    _deathAnimation->setAnchor(Vec2::ZERO);
    _deathAnimation->setPosition(offset/2);
    float xscale = idleAnimation->getWidth();
    float xscale2 = deathAnimation->getWidth();
    _deathAnimation->setScale(xscale/xscale2);
    _deathAnimation->setFrame(0);
    addChild(_deathAnimation);
    
    setAnimState(Idle);
    return true;
}

void LumiaNode::setLevel(int level){
    _level = level;
}

void LumiaNode::setAnimState(LumiaAnimState state){
    switch (state){
        case Splitting:
        case SplitFinished:{
            _splittingAnimation->setVisible(true);
            _idleAnimation->setVisible(false);
            _deathAnimation->setVisible(false);
            for (auto node : _indicatorNode){
                node->setVisible(false);
            }
            break;
        }
        case Dead:
        case Dying:{
            _splittingAnimation->setVisible(false);
            _idleAnimation->setVisible(false);
            _deathAnimation->setVisible(true);
            for (auto node : _indicatorNode){
                node->setVisible(false);
            }
            break;
        }
        default:{
            _splittingAnimation->setVisible(false);
            _idleAnimation->setVisible(true);
            _deathAnimation->setVisible(false);
            for (auto node : _indicatorNode){
                node->setVisible(true);
            }
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
        case Dying:{
            _frameCount %= DEATH_ANIMATION_INTERVAL;
            
            if (_frameCount == 0){
                int frame = _deathAnimation->getFrame() + 1;
                _deathAnimation->setFrame(frame);
                if (frame == ANIMATION_SIZE - 1){
                    setAnimState(LumiaAnimState::Dead);
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
