//
//  LevelSelectTile.cpp
//  Lumia
//
//  Created by Vivi Ye on 5/6/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include "LevelSelectTile.h"
void LevelSelectTile::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    scene2::PolygonNode::draw(batch, transform, tint);
}
