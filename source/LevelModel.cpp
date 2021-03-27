//
//  LevelModel.cpp
//  Lumia
//
//  Created by Vivi Ye on 3/24/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#define PLANT_NAME       "plant"
#define PLATFORM_NAME    "platform"
#define LUMIA_NAME       "lumia"
#define DEBUG_COLOR     Color4::YELLOW
/** The new heavier gravity for this world (so it is not so floaty) */
#define DEFAULT_GRAVITY -12.0f
/** The density for most physics objects */
#define BASIC_DENSITY   0.0f
/** The density for a bullet */
#define HEAVY_DENSITY   10.0f
/** Friction of most platforms */
#define BASIC_FRICTION  0.4f
/** The restitution for all physics objects */
#define BASIC_RESTITUTION   0.1f
/** The number of frame to wait before reinitializing the game */
#define EXIT_COUNT      240

#define ENERGY_RADIUS  3.0f


#include <stdio.h>
#include "LevelModel.h"

bool LevelModel::preload(const std::shared_ptr<cugl::JsonValue>& json){
    if (json == nullptr) {
        // NOLINTNEXTLINE idk why but clang-tidy is complaining
        CUAssertLog(false, "Failed to load level file");
        return false;
    }
    std::shared_ptr<cugl::JsonValue> _leveljson = json->get("level");
    //plants
    std::shared_ptr<cugl::JsonValue> plants_json = _leveljson->get("plants");
    std::shared_ptr<cugl::JsonValue> tiles_json = _leveljson->get("platforms");
    std::shared_ptr<cugl::JsonValue> lumia_json = _leveljson->get("lumia");
    createPlants(plants_json);
    createTiles(tiles_json);
    createLumia(lumia_json);

    return true;
};

std::vector<std::shared_ptr<Plant>> LevelModel::createPlants(const std::shared_ptr<cugl::JsonValue> &plants){
    
    for (int i=0; i< plants->size(); i++){
        std::shared_ptr<cugl::JsonValue> plant_json = plants->get(i);
        float posx = plant_json ->getFloat("posx");
        float posy = plant_json->getFloat("posy");
        float ang = (plant_json->getFloat("angle"))*M_PI/180;
        cugl::Size size  = Size(0.48f, 0.27f);
        std::shared_ptr<Plant> plant = Plant::alloc(Vec2(posx,posy), size);
        
        //set body parameters
        plant->setBodyType(b2_staticBody);
        plant->setAngle(ang);
        plant->lightDown();
        plant->setFriction(0.0f);
        plant->setRestitution(0.0f);
        plant->setName(PLANT_NAME + to_string(i));
        plant->setDensity(0);
        plant->setBullet(false);
        plant->setGravityScale(0);
        plant->setSensor(true);
        plant->setDebugColor(DEBUG_COLOR);
        plant->setVX(0);
        
        _plants.push_back(plant);
    }
    
    return _plants;
}


std::vector<std::shared_ptr<Tile>> LevelModel::createTiles(const std::shared_ptr<cugl::JsonValue> &platforms){
    for (int i = 0; i < platforms->size(); i++) {
        std::shared_ptr<cugl::JsonValue> platfor = platforms->get(i);
        float x = platfor->getFloat("blx");
        float y = platfor->getFloat("bly");
        float height = platfor->getFloat("height");
        float width = platfor->getFloat("width");
        std::shared_ptr<Tile> t = Tile::alloc(x, y, width, height);
        _tiles.push_back(t);
    }
    
    return _tiles;
}


std::shared_ptr<LumiaModel> LevelModel::createLumia(const std::shared_ptr<cugl::JsonValue> &lumia){
    float lumx = lumia->getFloat("posx");
    float lumy = lumia->getFloat("posy");
    float radius = lumia->getFloat("radius");// change to value from jso
    Vec2 lumiaPos = Vec2(lumx,lumy);
    _lumia = LumiaModel::alloc(lumiaPos,radius);
    _lumia-> setName(LUMIA_NAME);
    _lumia-> setDebugColor(DEBUG_COLOR);
    _lumia-> setSplitting(false);
    _lumia-> setFixedRotation(false);
    
    return  _lumia;
}

