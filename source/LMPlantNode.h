//
//  LMPlantNode.h
//  Lumia
//
//  Created by Vivi Ye on 3/11/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef LMPlantNode_h
#define LMPlantNode_h

#include <cugl/cugl.h>
#endif /* LMPlantNode_h */
using namespace cugl;

class PlantNode : public cugl::scene2::PolygonNode {
public:
    
    float SHADOW_OFFSET;
    
    
    PlantNode() : PolygonNode() {
    }

    ~PlantNode() { dispose(); }

    static std::shared_ptr<PlantNode> alloc(const std::shared_ptr<Texture>& texture) {
        std::shared_ptr<PlantNode> node = std::make_shared<PlantNode>();
        return (node->initWithTexture(texture) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

};


