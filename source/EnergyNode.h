//
//  LMPlantNode.h
//  Lumia
//
//  Created by Vivi Ye on 3/11/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef EnergyNode_h
#define EnergyNode_h

#include <cugl/cugl.h>
using namespace cugl;

class EnergyNode : public cugl::scene2::PolygonNode {
public:
    
    float SHADOW_OFFSET;
    
    
    EnergyNode() : PolygonNode() {
    }

    ~EnergyNode() { dispose(); }

    static std::shared_ptr<EnergyNode> alloc(const std::shared_ptr<Texture>& texture) {
        std::shared_ptr<EnergyNode> node = std::make_shared<EnergyNode>();
        return (node->initWithTexture(texture) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

};

#endif /* __LM_ENERGY_NODE_H__ */
