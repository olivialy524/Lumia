//
//  LMPlantNode.cpp
//  Lumia
//
//  Created by Vivi Ye on 3/11/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>

#include "PlantNode.h"
#define SHADOW_OFFSET   10.0f


void PlantNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    
//    cugl::Mat4 translation_mat = cugl::Mat4::createTranslation( SHADOW_OFFSET, SHADOW_OFFSET, 0.0f);
//
//    AnimationNode::draw(batch,transform*translation_mat,_stint);
//    AnimationNode::draw(batch,transform,tint);
    PolygonNode::draw(batch,transform,tint);
    
}

