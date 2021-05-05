//
//  ButtonNode.h
//  Lumia
//
//  Created by Olivia Li on 5/4/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef ButtonNode_h
#define ButtonNode_h

#include <cugl/cugl.h>
using namespace cugl;

class ButtonNode : public cugl::scene2::AnimationNode {
public:
    
    enum ButtonAnimState {
        Idle,
        GoingDown,
        Pressed,
        GoingUp
    };

    
protected:
    ButtonAnimState _state;
    
    int _frameCount;
    
    const int PRESSING_ANIMATION_INTERVAL = 8;
    
    const int IDLE_ANIMATION_INTERVAL = 25;
    
    const int ANIMATION_SIZE = 6;
    
    bool _frameIncreasing;
    
public:
    
    
    ButtonNode() : _state(Idle), _frameCount(0), _frameIncreasing(false), AnimationNode() {}

    ~ButtonNode() { dispose(); }
    
    void setAnimState(ButtonAnimState state){
        _state = state;
    }
    
    ButtonAnimState getAnimState(){
        return _state;
    }
 
    static std::shared_ptr<ButtonNode> alloc(const std::shared_ptr<Texture>& texture,
                                                int rows, int cols, int size) {
        std::shared_ptr<ButtonNode> node = std::make_shared<ButtonNode>();
        return (node->initWithFilmstrip(texture,rows,cols,size) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

};


#endif /* ButtonNode_h */
