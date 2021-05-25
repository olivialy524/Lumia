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

class EnemyNode : public cugl::scene2::SceneNode {
#pragma mark Constants and Enums
public:
    enum EnemyAnimState {
        /** When enemy is still */
        Idle,
        /** When enemy is chasing lumia */
        Chasing,
        /** When enemy is escaping from lumia */
        Escaping
    };

    
protected:
    EnemyAnimState _state;
    
    int _frameCount;
    
    const int ANIMATION_INTERVAL = 8;
    
    std::shared_ptr<cugl::scene2::AnimationNode> _idleAnimation;
    
    std::shared_ptr<cugl::scene2::AnimationNode> _chasingAnimation;
    
    std::shared_ptr<cugl::scene2::AnimationNode> _escapingAnimation;

public:
    
    EnemyNode() : _state(Idle), _frameCount(0), SceneNode() {}

    ~EnemyNode() { dispose(); }
    
    void dispose();
    /**
     * Init child nodes of Enemy node
     */
    bool setTextures(const std::shared_ptr<cugl::Texture> &chasingAnimation,
              const std::shared_ptr<cugl::Texture> &escapingAnimation,
              float radius,
              float drawScale);
    
    void setAnimState(EnemyAnimState state);
    
    EnemyAnimState getAnimState(){
        return _state;
    }
 
    static std::shared_ptr<EnemyNode> alloc(Size size) {
        std::shared_ptr<EnemyNode> node = std::make_shared<EnemyNode>();
        return (node->initWithBounds(size) ? node : nullptr);
    }
    
    void setRelative(bool r){
        _chasingAnimation->setRelativeColor(r);
        _escapingAnimation->setRelativeColor(r);
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;

    
};


#endif /* EnemyNode_h */
