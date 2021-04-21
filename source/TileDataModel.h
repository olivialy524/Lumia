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
    
    vector<Vec2> getTileGridData(int type, float angle){
        auto points =  _griddata0[type];
        
        vector<Vec2> res;
        
        for (int i=0; i < points.size(); i++){
            float x = points[i].x;
            float y = points[i].y;
            float res_x = cos(angle) * x - sin(angle) * y;
            float res_y = sin(angle) * x + cos(angle) * y;
            res_x = roundf(res_x * 100) / 100;
            res_y = roundf(res_y * 100) / 100;
            res.push_back(Vec2(res_x, res_y));
        }
        
        return res;
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

