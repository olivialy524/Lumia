//
//  LMLumiaNode.h
//  Lumia
//
//  Created by Olivia Li on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef LumiaNode_h
#define LumiaNode_h

#include <cugl/cugl.h>

using namespace cugl;

class LumiaNode : public cugl::scene2::AnimationNode {
#pragma mark Constants and Enums
public:
    enum LumiaAnimState {
        /** When Lumia is still or rolling */
        Idle,
        /** When Lumia is splitting */
        Splitting,
        /** When Lumia is merging */
        Merging,
        
        SplitFinished
    };

    
protected:
    LumiaAnimState _state;
    
    int _frameCount;

    int ANIMATION_INTERVAL = 1;
public:
    
    cugl::Color4 _stint;
    
    LumiaNode() : _state(Idle), _frameCount(0), AnimationNode() {}

    ~LumiaNode() { dispose(); }
    
    void setAnimState(LumiaAnimState state){
        _state = state;
    }
    
    LumiaAnimState getAnimState(){
        return _state;
    }
 
    static std::shared_ptr<LumiaNode> alloc(const std::shared_ptr<Texture>& texture,
                                                int rows, int cols, int size) {
        std::shared_ptr<LumiaNode> node = std::make_shared<LumiaNode>();
        return (node->initWithFilmstrip(texture,rows,cols,size) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

    
};

#endif /* LMLumiaNode_h */
