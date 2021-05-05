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
#include "ButtonNode.h"
#include "Door.h"

class Button : public cugl::physics2::BoxObstacle {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(Button);
protected:
    float _scale;
    std::shared_ptr<Door> _door;
    std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    std::shared_ptr<ButtonNode> _node;
    bool _inCoolDown;
    float _normHeight;
    bool _pushingDown;
    bool _pushingUp;
    float _drawScale;
    int _cooldown;
public:
    Button(): _normHeight(0.6f),_pushingDown(false), _pushingUp(false),_inCoolDown(false), _cooldown(0), cugl::physics2::BoxObstacle() { }

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
    
    void setTextures(const std::shared_ptr<cugl::Texture>& button);

    std::shared_ptr<cugl::scene2::SceneNode> getSceneNode() {
        return _sceneNode;
    }
    
    float getNormHeight() {
        return _normHeight;
    }
    
    void setNormHeight(float h) {
        _normHeight = h;
    }
    std::shared_ptr<Door> getDoor() {
        return _door;
    }
    
    void setDoor(std::shared_ptr<Door> d) {
        _door = d;
    }
    bool getPushingDown() {
        return _pushingDown;
    }
    
    void setPushingDown(bool p) {
        _pushingDown = p;
    }
    
    bool getPushingUp() {
        return _pushingUp;
    }
    
    void setPushingUp(bool p) {
        _pushingUp = p;
    }
    
    void pushDown();
    
    void setInCoolDown(bool value){
        _inCoolDown = value;
    }
    
    bool getInCoolDown(){
        return _inCoolDown;
    }
    int getCD() {
        return _cooldown;
    }

    void incCD() {
        _cooldown++;
    }

    void resetCD() {
        _cooldown = 0;
    }
    
    void pushUp();
    
    void setDrawScale(float value){
        _drawScale = value;
    }
    
    bool getPushedDown(){
        if (_node != nullptr){
            return _node->getAnimState() == ButtonNode::ButtonAnimState::Pressed;
        }
        return false;
    }
    
    bool getPushedUp(){
        if (_node != nullptr){
            return _node->getAnimState() == ButtonNode::ButtonAnimState::Idle;
        }
        return false;
        
    }
    
    /**
     * Updates the object's physics state (NOT GAME LOGIC).
     *
     * We use this method to reset cooldowns.
     *
     * @param delta Number of seconds since last animation frame
     */
    void update(float dt) override;
};

#endif /* Button_h */
