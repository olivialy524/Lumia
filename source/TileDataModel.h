//
//  LevelModel.h
//  Lumia
//
//  Created by Vivi Ye on 3/24/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef TileDataModel_h
#define TileDataModel_h
#include <string>
#include <iostream>
#include "Tile.h"
#include <cugl/cugl.h>
#include <math.h>
using namespace cugl;


class TileDataModel : public cugl::Asset {
private:
    vector<vector<Vec2>> _tiles;
//    vector<vector<Vec2>> _griddata;
    vector<vector<Vec2>> _griddata0;
    vector<vector<Vec2>> _griddata90;
    vector<vector<Vec2>> _griddata180;
    vector<vector<Vec2>> _griddata270;
  
public:
    
#pragma mark Static Constructors
    /**
     * Creates a new game level with no source file.
     *
     * The source file can be set at any time via the setFile() method. This method
     * does NOT load the asset.  You must call the load() method to do that.
     *
     * @return  an autoreleased level file
     */
    static std::shared_ptr<TileDataModel> alloc() {
        std::shared_ptr<TileDataModel> result = std::make_shared<TileDataModel>();
        return (result->init("") ? result : nullptr);
    }

    /**
     * Creates a new game level with the given source file.
     *
     * This method does NOT load the level. You must call the load() method to do that.
     * This method returns false if file does not exist.
     *
     * @return  an autoreleased level file
     */
    static std::shared_ptr<TileDataModel> alloc(const std::string& file) {
        std::shared_ptr<TileDataModel> result = std::make_shared<TileDataModel>();
        return (result->init(file) ? result : nullptr);
    }
    
#pragma mark Attributes
    
    vector<Vec2> getTileData(int type){
        return _tiles[type];
    }
    
    vector<Vec2> getTileGridData(int type){
        return _griddata0[type];
    }
    
    vector<Vec2> getTileGridData(int type, float plt_angle){
        auto angle = fmod (plt_angle + 6.28f, 6.28f);
        angle = roundf(angle * 100) / 100;
        
        if (angle == 0.0f){
            return _griddata0[type];
            
        }else if (angle == 1.57f){
            return _griddata90[type];
             
        }else if (angle == 3.14){
            return _griddata180[type];
            
        }else if (angle == 4.17){
            return _griddata270[type];
            
        }else{
            return _griddata0[type];
        }
    }
    
    bool preload(const std::string& file) override {
        std::shared_ptr<cugl::JsonReader> reader = cugl::JsonReader::allocWithAsset(file);
        return preload(reader->readJson());
    }
    
    bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    
    
#pragma mark constructors
    
    TileDataModel() {};
    
    virtual ~TileDataModel() = default;
    
    
};








#endif /* LevelModel_h */

