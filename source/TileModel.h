//
//  TileModel.h
//  Lumia
//
//  Created by Olivia Li on 4/27/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef TileModel_h
#define TileModel_h
#include <cugl/cugl.h>
using namespace cugl;

class TileModel : public cugl::physics2::PolygonObstacle {
protected:
    /** The scene graph node for the sticky wall. */
    std::shared_ptr<cugl::scene2::PolygonNode> _node;
    float _drawScale;
    int _type;
    
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(TileModel);
 public:

 #pragma mark Constructors
     /**
      * Creates a degenerate sticky wall object.
      *
      */
    TileModel() : cugl::physics2::PolygonObstacle() { }
    
    /**
     * Destroys this StickyWallModel, releasing all resources.
     */
    virtual ~TileModel(void) { dispose(); }

    void dispose();

    bool init(Poly2 p);
    
    static std::shared_ptr<TileModel> alloc(Poly2 p) {
        std::shared_ptr<TileModel> result = std::make_shared<TileModel>();
        return (result->init(p) ? result : nullptr);
    }
    
    std::shared_ptr<cugl::scene2::SceneNode> getSceneNode() {
        return _node;
    }

    void setTextures(const std::shared_ptr<Texture>& texture);
    
    void setDrawScale(float scale) {
        _drawScale = scale;
    }
    
    void setType(int type){
        _type = type;
    }
    
    int getType() const{
        return _type;
    }
    
 };
#endif /* TileModel_h */
