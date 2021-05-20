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
#include "LumiaModel.h"
#include "SlidingDoor.h"
#include "ShrinkingDoor.h"

class Button : public cugl::physics2::BoxObstacle {
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(Button);
protected:
    float _scale;
    std::shared_ptr<SlidingDoor> _SlidingDoor;
    std::shared_ptr<ShrinkingDoor> _ShrinkingDoor;
    std::shared_ptr<LumiaModel> _lumia;
    std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    std::shared_ptr<ButtonNode> _node;
    float _normHeight;
    bool _pushingDown;
    float _drawScale;
    int _cooldown;
    const float NUM_FRAMES = 6.0f;
    bool isSlidingDoor;
    
public:
    Button(): _normHeight(0.6f), _pushingDown(false), _cooldown(0), cugl::physics2::BoxObstacle() { }

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
    std::shared_ptr<SlidingDoor> getSlidingDoor() {
        return _SlidingDoor;
    }
    
    void setSlidingDoor(std::shared_ptr<SlidingDoor> d) {
        _SlidingDoor = d;
    }
    
    std::shared_ptr<ShrinkingDoor> getShrinkingDoor() {
        return _ShrinkingDoor;
    }
    void setShrinkingDoor(std::shared_ptr<ShrinkingDoor> d) {
        _ShrinkingDoor = d;
    }
    std::shared_ptr<LumiaModel> getLumia(){
        return _lumia;
    }
    
    void setLumia(std::shared_ptr<LumiaModel> lumia){
        _lumia = lumia;
    }
    
    bool getPushingDown() {
        return _pushingDown;
    }
    
    void setPushingDown(bool p) {
        _pushingDown = p;
    }
    
    void pushDown();
    
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
    
    void setIsSlidingDoor(bool value){
        isSlidingDoor = value;
    }
    
    bool getIsSlidingDoor(){
        return isSlidingDoor;
    }
};

#endif /* Button_h */
