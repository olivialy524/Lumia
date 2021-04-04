//
//  Button.h
//  Lumia
//
//  Created by Ben Dacek on 4/3/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef Button_h
#define Button_h
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include "Door.h"

class Button : public cugl::physics2::BoxObstacle {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(Button);
protected:
    float _scale;
    std::shared_ptr<Door> _door;
    std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    std::shared_ptr<cugl::scene2::PolygonNode> _node;
    int _cooldown;
public:
    Button() : cugl::physics2::BoxObstacle() { }

    virtual ~Button(void) { dispose(); }

    void dispose();

    static std::shared_ptr<Button> alloc() {
        std::shared_ptr<Button> result = std::make_shared<Button>();
        return (result->init() ? result : nullptr);
    }

    static std::shared_ptr<Button> alloc(const cugl::Vec2& pos) {
        std::shared_ptr<Button> result = std::make_shared<Button>();
        return (result->init(pos) ? result : nullptr);
    }

    static std::shared_ptr<Button> alloc(const cugl::Vec2& pos, cugl::Size size) {
        std::shared_ptr<Button> result = std::make_shared<Button>();
        return (result->init(pos, size) ? result : nullptr);
    }
    
    std::shared_ptr<cugl::scene2::PolygonNode> getNode() {
        return _node;
    }
    void setNode(std::shared_ptr<cugl::scene2::PolygonNode> n) {
        _node = n;
    }
    
    std::shared_ptr<Door> getDoor() {
        return _door;
    }
    
    void setDoor(std::shared_ptr<Door> d) {
        _door = d;
    }
    
};

#endif /* Button_h */
