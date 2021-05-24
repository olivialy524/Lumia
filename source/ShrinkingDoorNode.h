//
//  ShrinkingDoorNode.h
//  Lumia
//
//  Created by Olivia Li on 5/18/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef ShrinkingDoorNode_h
#define ShrinkingDoorNode_h
#include <cugl/cugl.h>
using namespace cugl;

class ShrinkingDoorNode : public cugl::scene2::AnimationNode {
public:
    
    enum ShrinkingDoorAnimState {
        Closed,
        Opening,
        Open,
        Closing
    };

    
protected:
    ShrinkingDoorAnimState _state;
    
    int _frameCount;
    
    const int ANIMATION_INTERVAL = 1;
    
    const int ANIMATION_SIZE = 12;
    
    bool _frameIncreasing;
    
public:
    
    
    ShrinkingDoorNode() : _state(Closed), _frameCount(0), _frameIncreasing(false), AnimationNode() {}

    ~ShrinkingDoorNode() { dispose(); }
    
    void setAnimState(ShrinkingDoorAnimState state){
        _state = state;
    }
    
    ShrinkingDoorAnimState getAnimState(){
        return _state;
    }
 
    static std::shared_ptr<ShrinkingDoorNode> alloc(const std::shared_ptr<Texture>& texture,
                                                int rows, int cols, int size) {
        std::shared_ptr<ShrinkingDoorNode> node = std::make_shared<ShrinkingDoorNode>();
        return (node->initWithFilmstrip(texture,rows,cols,size) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

};



#endif /* ShrinkingDoorNode_h */
