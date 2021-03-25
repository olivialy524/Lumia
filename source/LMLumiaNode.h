//
//  LMLumiaNode.h
//  Lumia
//
//  Created by Olivia Li on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef LMLumiaNode_h
#define LMLumiaNode_h

#include <cugl/cugl.h>

using namespace cugl;

class LumiaNode : public cugl::scene2::SceneNode {
//class LumiaNode : public cugl::scene2::AnimationNode {
public:
    
    std::shared_ptr<cugl::scene2::AnimationNode> splittingNode;
    
    cugl::Color4 _stint;
    
    
    LumiaNode() : AnimationNode() {}

    ~LumiaNode() { dispose(); }

 
 
    static std::shared_ptr<LumiaNode> alloc(const std::shared_ptr<Texture>& texture,
                                                int rows, int cols, int size) {
        std::shared_ptr<LumiaNode> node = std::make_shared<LumiaNode>();
        return (node->initWithFilmstrip(texture,rows,cols,size) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

    
};

#endif /* LMLumiaNode_h */
