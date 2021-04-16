//
//  TrajectoryNode.h
//  Lumia
//
//  Created by Vivi Ye on 4/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef TrajectoryNode_h
#define TrajectoryNode_h

#include <string>
#include <cugl/cugl.h>

using namespace cugl;

class TrajectoryNode : public scene2::PolygonNode {
    
    
protected:
   
    std::vector<Vec2> _points;
    float _endAlpha = 0.4f;
    

public:
    ~TrajectoryNode() { dispose(); }
    
    TrajectoryNode(): scene2::PolygonNode(){}
    
    void dispose() override;
    
    void setTextures(const std::shared_ptr<cugl::Texture> &dot_texture){
        setTexture(dot_texture);
    };
    
    
    static std::shared_ptr<TrajectoryNode> alloc(std::shared_ptr<cugl::Texture> t) {
        std::shared_ptr<TrajectoryNode> node = std::make_shared<TrajectoryNode>();
        return (node->initWithTexture(t) ? node : nullptr);
        
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;
    
    void setPoints(std::vector<Vec2> points){
        _points = points;
    }
    
    void addPoint(Vec2 point){
        _points.push_back(point);
    }
    
    void setEndAlpha(float f){
        _endAlpha = f;
    }
    
    void clearPoints(){
        _points.clear();
    }

};
  

#endif /* Tile_h */
