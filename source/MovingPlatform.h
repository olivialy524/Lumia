//
//  MovingPlatform.h
//  Lumia
//
//  Created by Ben Dacek on 4/1/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef MovingPlatform_h
#define MovingPlatform_h
#include <cugl/cugl.h>
#include <cugl/physics2/CUPolygonObstacle.h>
class MovingPlatform : cugl::physics2::PolygonObstacle{
private:
    float _ox;
    float _oy;
    float _nx;
    float _ny;
    bool _rev;
public:
    
#pragma mark Constructors
    /**
     * Creates a degenerate Bullet object.
     *
     * This constructor does not initialize any of the Bullet values beyond
     * the defaults.  To use a Bullet, you must call init().
     */
    MovingPlatform() : cugl::physics2::PolygonObstacle() { }
    
    static std::shared_ptr<MovingPlatform> alloc(cugl::Vec2 pos, cugl::Poly2 p, float nx, float ny) {
        std::shared_ptr<MovingPlatform> result = std::make_shared<MovingPlatform>();
        return (result->init(p) ? result : nullptr);
    }
    float getOriginalX() {
        return _ox;
    }
    void setOriginalX(float ox) {
        _ox = ox;
    }
    
    float getOriginalY() {
        return _oy;
    }
    void setOriginalY(float oy) {
        _oy = oy;
    }
    
    float getNewX() {
        return _nx;
    }
    void setNewX(float nx) {
        _nx = nx;
    }
    
    float getNewY() {
        return _ny;
    }
    void setNewY(float ny) {
        _ny = ny;
    }
    
    bool getReverse() {
        return _rev;
    }
    
    void reverse() {
        _rev = not _rev;
    }
    
    void move();
};
#endif /* MovingPlatform_h */
