//
//  Tile.h
//  Lumia
//
//  Created by Vivi Ye on 3/24/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef Tile_h
#define Tile_h

class Tile {
private:
    float _height;
    float _width;
    float _x;
    float _y;
    
public:
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
