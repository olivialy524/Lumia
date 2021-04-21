//
//  StickyWallNode.h
//  Lumia
//
//  Created by Olivia Li on 4/20/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef StickyWallNode_h
#define StickyWallNode_h

#include <cugl/cugl.h>

using namespace cugl;

class StickyWallNode : public cugl::scene2::AnimationNode {
#pragma mark Constants and Enums
protected:
    
    int _frameCount;

    const int ANIMATION_INTERVAL = 12;
    
    const int FRAME_SIZE = 1;
    
public:
    
    cugl::Color4 _stint;
    
    StickyWallNode() : _frameCount(0), AnimationNode() {}

    ~StickyWallNode() { dispose(); }
    
    static std::shared_ptr<StickyWallNode> alloc(const std::shared_ptr<Texture>& texture,
                                                int rows, int cols, int size) {
        std::shared_ptr<StickyWallNode> node = std::make_shared<StickyWallNode>();
        return (node->initWithFilmstrip(texture,rows,cols,size) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

    
};

#endif /* StickyWallNode_h */
