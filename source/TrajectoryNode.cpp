//
//  TrajectoryNode.cpp
//  Lumia
//
//  Created by Vivi Ye on 4/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "TrajectoryNode.h"

void TrajectoryNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
                      const cugl::Mat4& transform, cugl::Color4 tint) {
    float num_points = _points.size() * 1.0f;
    for (int i= 0; i< num_points; i++){

        cugl::Mat4 translation_mat = Mat4::createTranslation(_points[i].x, _points[i].y, 0.0f);
        
        float alpha = 1 - (i/num_points) * (1.0f - end_alpha);
        cugl::Color4 n_tint = cugl::Color4f(1, 1, 1, alpha );
        scene2::PolygonNode::draw(batch, transform * translation_mat, n_tint);
    }
    
//    scene2::PolygonNode::draw(batch, transform , tint);
}

