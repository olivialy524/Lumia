//
//  BackgroundNode.h
//  Lumia
//
//  Created by Vivi Ye on 3/21/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef BackgroundNode_h
#define BackgroundNode_h


#include <cugl/cugl.h>

using namespace cugl;

class BackgroundNode : public cugl::scene2::PolygonNode {
public:
    
    
    
    
    BackgroundNode() : PolygonNode() {}

    ~BackgroundNode() { dispose(); }

 
 
    static std::shared_ptr<BackgroundNode> alloc(const std::shared_ptr<Texture>& texture) {
        std::shared_ptr<BackgroundNode> node = std::make_shared<BackgroundNode>();
        return (node->initWithTexture(texture) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;
};







#endif /* BackgroundNode_h */
