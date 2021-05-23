//
//  Tutorial.h
//  Lumia
//
//  Created by Vivi Ye on 5/23/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//
#include <cugl/cugl.h>
#ifndef Tutorial_h
#define Tutorial_h
//enum TutorialEndCondition {lauch, outOfRange, light, enemy };
//struct Tutorial {
//    Vec2 drawPos;
//    Vec2 sensorPos;
//    float sensorWidth;
//    float sensorHeight;
//    string texture;
//    TutorialEndCondition condition;
//};

class Tutorial{
    
private:
    bool _hasDisplayed = false;
    
public:
    enum TutorialEndCondition {lauch, outOfRange, light, enemy };
    
    Vec2 _drawPos;
    Vec2 _sensorPos;
    float _sensorWidth;
    float _sensorHeight;
    string _texture;
    TutorialEndCondition _condition;
    std::shared_ptr<scene2::PolygonNode> _textureNode;
    
    
    
    Tutorial(){}
    
    ~Tutorial(){}
    
    
    
    Tutorial(Vec2 drawPos, Vec2 sensorPos, float sensorWidth, float sensorHeight, string texture, string endCond){
        _drawPos = drawPos;
        _sensorPos = sensorPos;
        _sensorWidth = sensorWidth;
        _sensorHeight = sensorHeight;
        _texture = texture;
        if (endCond == "launch" ){
            _condition = TutorialEndCondition::lauch;
        }
        else if (endCond == "light"){
            _condition = TutorialEndCondition::light;
        } else{
            _condition = TutorialEndCondition::outOfRange;
        }
    }
    
    
    static std::shared_ptr<Tutorial> alloc(Vec2 drawPos, Vec2 sensorPos, float sensorWidth, float sensorHeight, string texture, string endCond){
        std::shared_ptr<Tutorial> result = std::make_shared<Tutorial>();
        result->_drawPos = drawPos;
        result->_sensorPos = sensorPos;
        result->_sensorWidth = sensorWidth;
        result->_sensorHeight = sensorHeight;
        result->_texture = texture;
        if (endCond == "launch" ){
            result->_condition = TutorialEndCondition::lauch;
        }
        else if (endCond == "light"){
            result->_condition = TutorialEndCondition::light;
        } else{
            result->_condition = TutorialEndCondition::outOfRange;
        }
        return result? result: nullptr;
    }
    
    void setDisplayed(bool displayed){
        _hasDisplayed = displayed;
    }
    
    bool getDisplayed(){
        return _hasDisplayed;
    }
    

    
    
};


#endif /* Tutorial_h */
