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

class Door : public cugl::physics2::PolygonObstacle {
private:
    
    bool _open = false;
     std::shared_ptr<cugl::scene2::SceneNode> _scenenode;
     std::shared_ptr<cugl::scene2::PolygonNode> _node;
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
    
    void changeOpen() {
        _open = not _open;
        if (not _open) {
            getSceneNode()->setColor(cugl::Color4(0,0,0,0));
        }
        else {
            getSceneNode()->setColor(cugl::Color4(0,0,0,0));
    }
    }
     
    bool getOpen() {
        return _open;
    }

     std::shared_ptr<cugl::scene2::PolygonNode> getNode() {
         return _node;
     }

     void setNode(std::shared_ptr<cugl::scene2::PolygonNode> n) {
         _node = n;
     }

     std::shared_ptr<cugl::scene2::SceneNode> getSceneNode() {
         return _scenenode;
     }

     void setSceneNode(std::shared_ptr<cugl::scene2::SceneNode> scene) {
         _scenenode = scene;
     }

 };


#endif /* Door_h */
