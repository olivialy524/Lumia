//
//  TileDataModel.cpp
//  Lumia
//
//  Created by Vivi Ye on 4/5/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include "TileDataModel.h"

bool TileDataModel::preload(const std::shared_ptr<cugl::JsonValue>& json){
    if (json == nullptr) {
        // NOLINTNEXTLINE idk why but clang-tidy is complaining
        CUAssertLog(false, "Failed to load level file");
        return false;
    }
    std::shared_ptr<cugl::JsonValue> tiles = json->get("tiles");
    for (int i=0; i < tiles->size(); i++){
        std::shared_ptr<cugl::JsonValue> tile_json = tiles->get(i);
        vector<Vec2> tile;
        for (int j = 0; j< tile_json->get("points")->size(); j++){
            vector<float> point = tile_json->get("points")->get(j)->asFloatArray();
            tile.push_back(Vec2(point[0], point[1]));
        }
        _tiles.push_back(tile);
        vector<Vec2> grid;
        for (int j = 0; j< tile_json->get("grid_data")->get("0")->size(); j++){
            vector<float> point = tile_json->get("grid_data")->get("0")->get(j)->asFloatArray();
            grid.push_back(Vec2(point[0], point[1]));
        }
        _griddata0.push_back(grid);
        
        grid.clear();
        for (int j = 0; j< tile_json->get("grid_data")->get("1.57")->size(); j++){
            vector<float> point = tile_json->get("grid_data")->get("1.57")->get(j)->asFloatArray();
            grid.push_back(Vec2(point[0], point[1]));
        }
        _griddata90.push_back(grid);
        
        grid.clear();
        for (int j = 0; j< tile_json->get("grid_data")->get("3.14")->size(); j++){
            vector<float> point = tile_json->get("grid_data")->get("3.14")->get(j)->asFloatArray();
            grid.push_back(Vec2(point[0], point[1]));
        }
        _griddata180.push_back(grid);
        
        grid.clear();
        for (int j = 0; j< tile_json->get("grid_data")->get("4.71")->size(); j++){
            vector<float> point = tile_json->get("grid_data")->get("4.71")->get(j)->asFloatArray();
            grid.push_back(Vec2(point[0], point[1]));
        }
        _griddata270.push_back(grid);
    }
    return true;
};
