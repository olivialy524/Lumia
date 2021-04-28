//
//  TileModel.cpp
//  Lumia
//
//  Created by Olivia Li on 4/27/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "TileModel.h"
/** The density for most physics objects */
#define BASIC_DENSITY   0.0f
/** Friction of most platforms */

#define BASIC_FRICTION  0.4f
/** The restitution for all physics objects */
#define BASIC_RESTITUTION   0.1f

#define DEBUG_COLOR     Color4::YELLOW

void TileModel::setTextures(const std::shared_ptr<Texture>& image){
    auto platform = getPolygon() * _drawScale;

    // calcuate the drawing overlay scale
    float scalex = platform.getBounds().size.width/image->getWidth();
    float scaley = platform.getBounds().size.height/image->getHeight();
    
    _node = scene2::PolygonNode::allocWithTexture(image);
    _node->setScale(Vec2(scalex, scaley));
    _node->setAngle(getAngle());
    _node->setPosition(getPosition()*_drawScale);
}

bool TileModel::init(Poly2 p) {
    if (PolygonObstacle::init(p)) {
        setBodyType(b2_staticBody);
        setDensity(BASIC_DENSITY);
        setFriction(BASIC_FRICTION);
        setRestitution(BASIC_RESTITUTION);
        setDebugColor(DEBUG_COLOR);

        return true;
    }
    return false;
}

void TileModel::dispose() {
    _node = nullptr;
}

