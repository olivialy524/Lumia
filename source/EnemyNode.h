//
//  EnemyNode.h
//  Lumia
//
//  Created by Olivia Li on 4/5/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef EnemyNode_h
#define EnemyNode_h

#include <cugl/cugl.h>

using namespace cugl;

class EnemyNode : public cugl::scene2::AnimationNode {
#pragma mark Constants and Enums
protected:
    
    int _frameCount;

    int ANIMATION_INTERVAL = 12;
public:
    
    cugl::Color4 _stint;
    
    EnemyNode() : _frameCount(0), AnimationNode() {}

    ~EnemyNode() { dispose(); }
    
    static std::shared_ptr<EnemyNode> alloc(const std::shared_ptr<Texture>& texture,
                                                int rows, int cols, int size) {
        std::shared_ptr<EnemyNode> node = std::make_shared<EnemyNode>();
        return (node->initWithFilmstrip(texture,rows,cols,size) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

    
};


#endif /* EnemyNode_h */
