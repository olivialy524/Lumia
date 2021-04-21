//
//  StickyWallNode.cpp
//  Lumia
//
//  Created by Olivia Li on 4/20/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include "StickyWallNode.h"


void StickyWallNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
   
    AnimationNode::draw(batch,transform,Color4f::WHITE);
}
