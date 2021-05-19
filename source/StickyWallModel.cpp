//
//  StickyWall.cpp
//  Lumia
//
//  Created by Olivia Li on 4/20/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "StickyWallModel.h"

void StickyWallModel::setTextures(const std::shared_ptr<Texture>& texture){
    
    _sceneNode = scene2::SceneNode::allocWithBounds(Size(getWidth()*_drawscale,getHeight()*_drawscale));
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
    _sceneNode->setAngle(getAngle());
    float scale = getWidth() * _drawscale/(texture->getWidth()/5.0f);
    float scale2 = 1.0/scale;
    _node = StickyWallNode::alloc(texture, 1, 5, 5);
    _node->setScale(scale);
    _node->setAnchor(Vec2::ZERO);
    _node->setPosition(0, 0);
    _node->setFrame(0);
    _node->setPolygon(Rect(0, 0,  texture->getWidth()/5.0f, getHeight() * _drawscale * scale2));
    _sceneNode->addChild(_node);
}

bool StickyWallModel::init(const cugl::Vec2 pos, cugl::Poly2 p, float ang) {
    if (PolygonObstacle::init(p)) {
        //set body parameters
        setBodyType(b2_staticBody);
        setAngle(ang);
        setFriction(1.0f);
        setRestitution(0.0f);
        setName("STICKY_WALL");
        setGravityScale(0);
        setPosition(pos);
        setSensor(true);
        
        float nx = 1.0f * cos(ang) - 0.0f * sin(ang);
        float ny = 1.0f * sin(ang) + 0.0f * cos(ang);
        _surfaceNorm = Vec2(nx, ny);
        return true;
    }
    return false;
}

void StickyWallModel::dispose() {
    _node = nullptr;
    _sceneNode = nullptr;
}

