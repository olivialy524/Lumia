//
//  BackgroundNode.cpp
//  Lumia
//
//  Created by Vivi Ye on 3/21/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>


#include "BackgroundNode.h"
#define SHADOW_OFFSET   10.0f


void BackgroundNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    cugl::Mat4 translation_mat_left = cugl::Mat4::createTranslation( -1.0f * getTexture()->getWidth() * getScaleY(), 0.0f, 0.0f);
    cugl::Mat4 translation_mat_right = cugl::Mat4::createTranslation( 1.0f * getTexture()->getWidth()* getScaleY(), 0.0f, 0.0f);
    cugl::Mat4 translation_mat_right2 = cugl::Mat4::createTranslation( 2.0f * getTexture()->getWidth()* getScaleY(), 0.0f, 0.0f);
    PolygonNode::draw(batch,transform,tint);
    PolygonNode::draw(batch,transform * translation_mat_left,tint);
    PolygonNode::draw(batch,transform * translation_mat_right,tint);
    PolygonNode::draw(batch,transform * translation_mat_right2,tint);
    
}
