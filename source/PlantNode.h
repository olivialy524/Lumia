//
//  LMPlantNode.h
//  Lumia
//
//  Created by Vivi Ye on 3/11/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef PlantNode_h
#define PlantNode_h

#include <cugl/cugl.h>
using namespace cugl;

class PlantNode : public cugl::scene2::AnimationNode {
public:
    
    enum PlantAnimState {
        Dark,
        LightingUp,
        Lit
    };

    
protected:
    PlantAnimState _state;
    
    int _frameCount;
    
    const int LIGHT_UP_ANIMATION_INTERVAL = 2;
    
    const int LIT_ANIMATION_INTERVAL = 18;
    
    const int LIT_ANIMATION_START = 3;
    
    const int LIT_ANIMATION_END = 5;
    
    bool _frameIncreasing;
    
public:
    
    
    PlantNode() : _state(Dark), _frameCount(0), _frameIncreasing(false), AnimationNode() {}

    ~PlantNode() { dispose(); }
    
    void setAnimState(PlantAnimState state){
        _state = state;
    }
    
    PlantAnimState getAnimState(){
        return _state;
    }
 
    static std::shared_ptr<PlantNode> alloc(const std::shared_ptr<Texture>& texture,
                                                int rows, int cols, int size) {
        std::shared_ptr<PlantNode> node = std::make_shared<PlantNode>();
        return (node->initWithFilmstrip(texture,rows,cols,size) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

};

#endif /* __LM_PLANT_NODE_H__ */
