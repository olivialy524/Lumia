//
//  StickyWall.h
//  Lumia
//
//  Created by Olivia Li on 4/20/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef StickyWallModel_h
#define StickyWallModel_h
#include "StickyWallNode.h"

class StickyWallModel : public cugl::physics2::PolygonObstacle {
protected:
    /** The scene graph node for the sticky wall. */
    std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    std::shared_ptr<StickyWallNode> _node;
    float _drawscale;
    
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(StickyWallModel);
 public:

 #pragma mark Constructors
     /**
      * Creates a degenerate sticky wall object.
      *
      */
    StickyWallModel() : cugl::physics2::PolygonObstacle() { }
    
    /**
     * Destroys this StickyWallModel, releasing all resources.
     */
    virtual ~StickyWallModel(void) { dispose(); }

    void dispose();
    
    bool init(const cugl::Vec2 pos, cugl::Poly2 p, float ang);

    static std::shared_ptr<StickyWallModel> alloc(cugl::Vec2 pos, cugl::Poly2 p, float ang) {
        std::shared_ptr<StickyWallModel> result = std::make_shared<StickyWallModel>();
        return (result->init(pos, p, ang) ? result : nullptr);
    }
    
    std::shared_ptr<cugl::scene2::SceneNode> getSceneNode() {
        return _sceneNode;
    }

    void setTextures(const std::shared_ptr<Texture>& texture);
    
    void setDrawScale(float scale) {
        _drawscale = scale;
    }
    
 };

#endif /* StickyWallModel_h */
