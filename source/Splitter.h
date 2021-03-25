//
//  LMSplitter.h
//  Lumia
//
//  Created by Ben Dacek on 3/20/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#ifndef Splitter_h
#define Splitter_h

class Splitter : public cugl::physics2::BoxObstacle {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(Splitter);
protected:
    float _scale;
    std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    std::shared_ptr<cugl::scene2::PolygonNode> _node;
    float _cooldown = 0.0f;
public:
    Splitter() : cugl::physics2::BoxObstacle() { }

    virtual ~Splitter(void) { dispose(); }

    void dispose();

    static std::shared_ptr<Splitter> alloc() {
        std::shared_ptr<Splitter> result = std::make_shared<Splitter>();
        return (result->init() ? result : nullptr);
    }

    static std::shared_ptr<Splitter> alloc(const cugl::Vec2& pos) {
        std::shared_ptr<Splitter> result = std::make_shared<Splitter>();
        return (result->init(pos) ? result : nullptr);
    }

    static std::shared_ptr<Splitter> alloc(const cugl::Vec2& pos, cugl::Size size) {
        std::shared_ptr<Splitter> result = std::make_shared<Splitter>();
        return (result->init(pos, size) ? result : nullptr);
    }
    
    std::shared_ptr<cugl::scene2::PolygonNode> getNode() {
        return _node;
    }
    void setNode(std::shared_ptr<cugl::scene2::PolygonNode> n) {
        _node = n;
    }
    float getCooldown() {
        return _cooldown;
    }
    
    void setCooldown(float f) {
        _cooldown = f;
    }
};

#endif /* Splitter_h */
