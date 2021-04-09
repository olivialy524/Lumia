//
//  LevelModel.cpp
//  Lumia
//
//  Created by Vivi Ye on 3/24/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#define PLANT_NAME       "plant"
#define ENERGY_NAME       "energy"
#define PLATFORM_NAME    "platform"
#define LUMIA_NAME       "lumia"
#define ENEMY_NAME       "enemy"
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
#include "LumiaModel.h"



void LevelModel::dispose(){
    _lumia->dispose();
    _lumia = nullptr;
    for (const std::shared_ptr<EnemyModel> &e : _enemies) {
        e->dispose();
    }
    _enemies.clear();
    
    for (const std::shared_ptr<Plant> &p : _plants) {
        p->dispose();
    }
    _plants.clear();

    for (const std::shared_ptr<EnergyModel>& en : _energies) {
        en->dispose();
    }
    _energies.clear();

    _irregular_tiles.clear();
    _tiles.clear();
}

bool LevelModel::preload(const std::shared_ptr<cugl::JsonValue>& json){
    if (json == nullptr) {
        // NOLINTNEXTLINE idk why but clang-tidy is complaining
        CUAssertLog(false, "Failed to load level file");
        return false;
    }
    _levelJson = json;
    std::shared_ptr<cugl::JsonValue> _leveljson = json->get("level");
    _xBound = _leveljson->getFloat("xBound");
    _yBound = _leveljson->getFloat("yBound");
    std::shared_ptr<cugl::JsonValue> plants_json = _leveljson->get("plants");
    std::shared_ptr<cugl::JsonValue> energies_json = _leveljson->get("energies");
    std::shared_ptr<cugl::JsonValue> tiles_json = _leveljson->get("platforms");
    std::shared_ptr<cugl::JsonValue> lumia_json = _leveljson->get("lumia");
    std::shared_ptr<cugl::JsonValue> enemies_json = _leveljson->get("enemies");
    std::shared_ptr<cugl::JsonValue> irregular_tile_json = _leveljson->get("tiles");
    createPlants(plants_json);
    createEnergies(energies_json);
    createTiles(tiles_json);
    createLumia(lumia_json);
    createIrregular(irregular_tile_json);
    createEnemies(enemies_json);

    return true;
};

std::vector<std::shared_ptr<Plant>> LevelModel::createPlants(const std::shared_ptr<cugl::JsonValue> &plants){
    
    for (int i=0; i< plants->size(); i++){
        std::shared_ptr<cugl::JsonValue> plant_json = plants->get(i);
        float posx = plant_json ->getFloat("posx");
        float posy = plant_json->getFloat("posy");
        float ang = (plant_json->getFloat("angle"))*M_PI/180.0f;
        cugl::Size size  = Size(0.5f, 0.5f);
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

std::vector<std::shared_ptr<EnergyModel>> LevelModel::createEnergies(const std::shared_ptr<cugl::JsonValue>& energies) {

    for (int i = 0; i < energies->size(); i++) {
        std::shared_ptr<cugl::JsonValue> energy_json = energies->get(i);
        float posx = energy_json->getFloat("posx");
        float posy = energy_json->getFloat("posy");
        cugl::Size size = Size(0.65f, 0.65f);
        std::shared_ptr<EnergyModel> energy = EnergyModel::alloc(Vec2(posx, posy), size);

        //set body parameters
        energy->setBodyType(b2_staticBody);
        energy->setFriction(0.0f);
        energy->setRestitution(0.0f);
        energy->setName(ENERGY_NAME);
        energy->setDensity(0);
        energy->setBullet(false);
        energy->setGravityScale(0);
        energy->setSensor(true);
        energy->setDebugColor(DEBUG_COLOR);
        energy->setVX(0);

        _energies.push_back(energy);
    }

    return _energies;
}

std::vector<std::shared_ptr<EnemyModel>> LevelModel::createEnemies(const std::shared_ptr<cugl::JsonValue> &enemies){
    
    for (int i=0; i< enemies->size(); i++){
        std::shared_ptr<cugl::JsonValue> enemy_json = enemies->get(i);
        float posx = enemy_json ->getFloat("posx");
        float posy = enemy_json->getFloat("posy");
        float radius = enemy_json->getFloat("radius");
        Vec2 pos = Vec2(posx, posy);
        auto enemy = EnemyModel::alloc(pos,radius);
        enemy-> setName(ENEMY_NAME);
        enemy-> setDebugColor(DEBUG_COLOR);
        _enemies.push_back(enemy);
    }
    
    return _enemies;
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

std::vector<std::shared_ptr<Tile>> LevelModel::createIrregular(const std::shared_ptr<cugl::JsonValue> &platforms){
    for (int i = 0; i < platforms->size(); i++) {
        std::shared_ptr<cugl::JsonValue> platfor = platforms->get(i);
        float x = platfor->getFloat("posx");
        float y = platfor->getFloat("posy");
        int type = platfor->getInt("type");
        float angle = platfor->getFloat("angle");
        string file_name = platfor->getString("texture");
        std::shared_ptr<Tile> t = Tile::alloc(x, y, angle, type);
        t->setFile(file_name);
        _irregular_tiles.push_back(t);
    }

    return _irregular_tiles;
}


std::shared_ptr<LumiaModel> LevelModel::createLumia(const std::shared_ptr<cugl::JsonValue> &lumia){
    float lumx = lumia->getFloat("posx");
    float lumy = lumia->getFloat("posy");
    int sizeLevel = lumia->getInt("sizelevel");
    Vec2 lumiaPos = Vec2(lumx,lumy);
    _lumia = LumiaModel::alloc(lumiaPos, LumiaModel::sizeLevels[sizeLevel].radius);
    _lumia->setName(LUMIA_NAME);
    _lumia->setDebugColor(DEBUG_COLOR);
    _lumia->setFixedRotation(false);
    _lumia->setDensity(LumiaModel::sizeLevels[sizeLevel].density);
    _lumia->setSizeLevel(sizeLevel);
    
    return  _lumia;
}
