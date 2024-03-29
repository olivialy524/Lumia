//
//  LMPlantNode.h
//  Lumia
//
//  Created by Vivi Ye on 3/11/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef SpikeNode_h
#define SpikeNode_h

#include <cugl/cugl.h>
using namespace cugl;

class SpikeNode : public cugl::scene2::AnimationNode {

protected:
    
    int _frameCount;
    
    const int ANIMATION_INTERVAL = 2;
public:
    
    
    SpikeNode() : _frameCount(0), AnimationNode() {}

    ~SpikeNode() { dispose(); }
 
    static std::shared_ptr<SpikeNode> alloc(const std::shared_ptr<Texture>& texture,
                                                int rows, int cols, int size) {
        std::shared_ptr<SpikeNode> node = std::make_shared<SpikeNode>();
        return (node->initWithFilmstrip(texture,rows,cols,size) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

};

#endif /* __SPIKE_NODE_H__ */
