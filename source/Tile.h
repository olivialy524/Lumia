//
//  Tile.h
//  Lumia
//
//  Created by Vivi Ye on 3/24/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include <cugl/scene2/graph/CUWireNode.h>
#ifndef Tile_h
#define Tile_h

class Tile {
private:
    float _height;
    float _width;
    float _x;
    float _y;
    
public:
    
    Tile(float x, float y, float width, float height){
        _x = x;
        _y = y;
        _height = height;
        _width = width;
    }
    
    Tile(){}
    
    static std::shared_ptr<Tile> alloc(float x, float y, float width, float height){
        CULog("hereee");
        std::shared_ptr<Tile> result = std::make_shared<Tile>();
        result->setX(x);
        result->setY(y);
        result->setHeight(height);
        result->setWidth(width);
        return result? result: nullptr;
    }
    
    void setX(float x){
        _x = x;
    }
    float getX(){
        return _x;
    }
    
    void setY(float y){
        _y = y;
    }
    
    float getY(){
        return _y;
    }
    
    void setWidth(float width){
        _width = width;
    }
    
    float getWidth(){
        return _width;
    }
    
    void setHeight(float height){
        _height = height;
    }
    
    float getHeight(){
        return _height;
    }
};

#endif /* Tile_h */
