//
//  LevelSelectTile.h
//  Lumia
//
//  Created by Vivi Ye on 5/6/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef LevelSelectTile_h
#define LevelSelectTile_h
#include <string>
#include <cugl/cugl.h>

using namespace cugl;

class LevelSelectTile : public scene2::PolygonNode {
    
    
protected:
   
    

public:
    ~LevelSelectTile() { dispose(); }
    
    LevelSelectTile(): scene2::PolygonNode(){}
    
    static std::shared_ptr<LevelSelectTile> alloc(std::shared_ptr<cugl::Texture> t) {
        std::shared_ptr<LevelSelectTile> node = std::make_shared<LevelSelectTile>();
        return (node->initWithTexture(t) ? node : nullptr);
        
    }

    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,
              const cugl::Mat4& transform, cugl::Color4 tint) override;
    
};
  


#endif /* LevelSelectTile_h */
