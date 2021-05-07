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

class LumiaNode : public cugl::scene2::SceneNode {
#pragma mark Constants and Enums
public:
    enum LumiaAnimState {
        /** When Lumia is still or rolling */
        Idle,
        /** When Lumia is splitting */
        Splitting,
        /** When Lumia is merging */
        Merging,
        /** When Lumia has finished Splitting */
        SplitFinished,
        /** When Lumia is dying */
        Dying,
        /** When Lumia is dead */
        Dead
    };

    
protected:
    LumiaAnimState _state;
    
    int _frameCount;
    
    int _level;
    
    const int IDLE_ANIMATION_INTERVAL = 10;
    
    const int SPLIT_ANIMATION_INTERVAL = 1;
    
    const int DEATH_ANIMATION_INTERVAL = 2;
    
    const int ANIMATION_ROWS = 4;
    
    const int ANIMATION_COLS = 5;
    
    const int ANIMATION_SIZE = 20;
    
    std::shared_ptr<cugl::scene2::AnimationNode> _idleAnimation;
    
    std::shared_ptr<cugl::scene2::AnimationNode> _splittingAnimation;
    
    std::shared_ptr<cugl::scene2::AnimationNode> _deathAnimation;
    
    std::vector<std::shared_ptr<cugl::scene2::PolygonNode>> _indicatorNode;

public:
    
    cugl::Color4 _stint;
    
    LumiaNode() : _state(Idle), _frameCount(0), SceneNode() {}

    ~LumiaNode() { dispose(); }
    
    void dispose();
    /**
     * Init child nodes of Lumia node
     */
    bool setTextures(const std::shared_ptr<cugl::Texture> &idleAnimation,
              const std::shared_ptr<cugl::Texture> &splittingAnimation,
              const std::shared_ptr<cugl::Texture> &deathAnimation,
              const std::shared_ptr<cugl::Texture> &indicator,
              float radius,
              float drawScale);
    
    void setLevel(int level);
    
    void setAnimState(LumiaAnimState state);
    
    LumiaAnimState getAnimState(){
        return _state;
    }
 
    static std::shared_ptr<LumiaNode> alloc(Size size) {
        std::shared_ptr<LumiaNode> node = std::make_shared<LumiaNode>();
        return (node->initWithBounds(size) ? node : nullptr);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

    
};

#endif /* LMLumiaNode_h */
