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

class EnergyNode : public cugl::scene2::AnimationNode {
protected:
    int _frameCount;
    
    const int ANIMATION_INTERVAL = 8;
    
    const int ANIMATION_ROWS = 1;
    
    const int ANIMATION_COLS = 9;
        
public:
        
        
    EnergyNode() : _frameCount(0), AnimationNode() {}

    ~EnergyNode() { dispose(); }

    static std::shared_ptr<EnergyNode> alloc(const std::shared_ptr<Texture>& texture,
                                             int rows, int cols, int size) {
        std::shared_ptr<EnergyNode> node = std::make_shared<EnergyNode>();
        return (node->initWithFilmstrip(texture,rows,cols,size) ? node : nullptr);
    }
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

};

#endif /* __LM_ENERGY_NODE_H__ */
