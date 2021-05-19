//
//  Door.h
//  Lumia
//
//  Created by Ben Dacek on 4/3/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef Door_h
#define Door_h
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
using namespace cugl;

class Door : public cugl::physics2::PolygonObstacle {
private:
    cugl::Vec2 _origpos;
    cugl::Vec2 _newpos;
    bool _opening = false;
    bool _closing = false;
    std::shared_ptr<cugl::scene2::PolygonNode> _scenenode;
    float _drawScale;
 public:

 #pragma mark Constructors
     /**
      * Creates a degenerate Bullet object.
      *
      * This constructor does not initialize any of the Bullet values beyond
      * the defaults.  To use a Bullet, you must call init().
      */
    Door() : cugl::physics2::PolygonObstacle() { }

    void dispose();
    
    static std::shared_ptr<Door> alloc(cugl::Vec2 pos, cugl::Poly2 p) {
        std::shared_ptr<Door> result = std::make_shared<Door>();
        return (result->init(p) ? result : nullptr);
    }
    
    void update(float dt) override;

    void Open();

    void Close();

    std::shared_ptr<cugl::scene2::SceneNode> getSceneNode() {
        return _scenenode;
    }
    
    void setTextures(const std::shared_ptr<cugl::Texture>& door);

    cugl::Vec2 getOriginalPos() {
        return _origpos;
    }
    
    void setOriginalPos(cugl::Vec2 op) {
        _origpos = op;
    }
    
    cugl::Vec2 getNewPos() {
        return _newpos;
    }
    
    void setNewPos(cugl::Vec2 np) {
        _newpos = np;
    }
    
    bool getOpening() {
        return _opening;
    }
    
    void setOpening(bool open) {
        _opening = open;
    }
    
    bool getClosing() {
        return _closing;
    }
    
    void setClosing(bool close) {
        _closing = close;
    }
    
    void setDrawScale(float value){
        _drawScale = value;
    }
 };


#endif /* Door_h */
