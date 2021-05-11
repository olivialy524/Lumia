//
//  LevelModel.cpp
//  Lumia
//
//  Created by Vivi Ye on 3/24/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#define PLANT_NAME       "plant"
#define SPIKE_NAME       "spike"
#define ENERGY_NAME      "energy"
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
//    _lumia = nullptr;
    _enemies.clear();
    _plants.clear();
    _spikes.clear();
    _energies.clear();
    _buttons.clear();
    _doors.clear();
    _irregular_tiles.clear();
    _tiles.clear();
    _stickyWalls.clear();
    _tutorials.clear();
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
    _twoStarScore = _leveljson->getInt("twostars");
    _threeStarScore = _leveljson->getInt("threestars");
    std::shared_ptr<cugl::JsonValue> plants_json = _leveljson->get("plants");
    std::shared_ptr<cugl::JsonValue> spikes_json = _leveljson->get("spikes");
    std::shared_ptr<cugl::JsonValue> energies_json = _leveljson->get("energies");
    std::shared_ptr<cugl::JsonValue> tiles_json = _leveljson->get("platforms");
    std::shared_ptr<cugl::JsonValue> lumia_json = _leveljson->get("lumia");
    std::shared_ptr<cugl::JsonValue> buttondoor_json = _leveljson->get("buttondoors");
    std::shared_ptr<cugl::JsonValue> enemies_json = _leveljson->get("enemies");
    std::shared_ptr<cugl::JsonValue> irregular_tile_json = _leveljson->get("tiles");
    std::shared_ptr<cugl::JsonValue> sticky_walls_json = _leveljson->get("sticky_walls");
    std::shared_ptr<cugl::JsonValue> tutorials_json = _leveljson->get("tutorials");
    createButtonsAndDoors(buttondoor_json);
    createPlants(plants_json);
    createSpikes(spikes_json);
    createEnergies(energies_json);
    createTiles(tiles_json);
    createLumia(lumia_json);
    createIrregular(irregular_tile_json);
    createEnemies(enemies_json);
    createStickyWalls(sticky_walls_json);
    createTutorials(tutorials_json);

    return true;
};

std::vector<std::shared_ptr<Plant>> LevelModel::createPlants(const std::shared_ptr<cugl::JsonValue> &plants){
    
    for (int i=0; i< plants->size(); i++){
        std::shared_ptr<cugl::JsonValue> plant_json = plants->get(i);
        float posx = plant_json ->getFloat("posx");
        float posy = plant_json->getFloat("posy");
        float ang = (plant_json->getFloat("angle"));
        cugl::Size size  = Size(1.1f, 0.75f);
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

std::vector<std::shared_ptr<SpikeModel>> LevelModel::createSpikes(const std::shared_ptr<cugl::JsonValue>& spikes) {

    for (int i = 0; i < spikes->size(); i++) {
        std::shared_ptr<cugl::JsonValue> spike_json = spikes->get(i);
        float posx = spike_json->getFloat("posx");
        float posy = spike_json->getFloat("posy");
        float ang = (spike_json->getFloat("angle")) * M_PI / 180.0f;
        cugl::Size size = Size(1.0f, 0.5f);
        std::shared_ptr<SpikeModel> spike = SpikeModel::alloc(Vec2(posx, posy), size);

        //set body parameters
        spike->setBodyType(b2_staticBody);
        spike->setAngle(ang);
        spike->setFriction(0.0f);
        spike->setRestitution(0.0f);
        spike->setName(SPIKE_NAME);
        spike->setDensity(0);
        spike->setBullet(false);
        spike->setGravityScale(0);
        spike->setDebugColor(DEBUG_COLOR);
        spike->setVX(0);

        _spikes.push_back(spike);
    }
    return _spikes;
}

std::vector<std::shared_ptr<StickyWallModel>> LevelModel::createStickyWalls(const std::shared_ptr<cugl::JsonValue> &stickyWalls){
    
    for (int i=0; i< stickyWalls->size(); i++){
        std::shared_ptr<cugl::JsonValue> stickyWalls_json = stickyWalls->get(i);
        float posx = stickyWalls_json ->getFloat("posx");
        float posy = stickyWalls_json->getFloat("posy");
        float ang = (stickyWalls_json->getFloat("angle"));
        float height = stickyWalls_json->getFloat("height");
        float width = stickyWalls_json->getFloat("width");
        
        Rect rectangle = Rect(posx,posy,width,height);
        Poly2 platform(rectangle,false);
        SimpleTriangulator triangulator;
        triangulator.set(platform);
        triangulator.calculate();
        platform.setIndices(triangulator.getTriangulation());
        platform.setGeometry(Geometry::SOLID);
        std::shared_ptr<StickyWallModel> stickyWall = StickyWallModel::alloc(Vec2(posx,posy), platform, ang);
        
        _stickyWalls.push_back(stickyWall);
    }
    return _stickyWalls;
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
        int sizeLevel = enemy_json->getInt("sizelevel");
        Vec2 pos = Vec2(posx, posy);
        auto enemy = EnemyModel::alloc(pos, LumiaModel::sizeLevels[sizeLevel].radius);
        enemy->setName(ENEMY_NAME);
        enemy->setDebugColor(DEBUG_COLOR);
        enemy->setSizeLevel(sizeLevel);
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

std::vector<LevelModel::Tutorial> LevelModel::createTutorials(const std::shared_ptr<cugl::JsonValue>& tutorials) {
    for (int i = 0; i < tutorials->size(); i++) {
        std::shared_ptr<cugl::JsonValue> tutorial = tutorials->get(i);
        float posX = tutorial->getFloat("posx");
        float posY = tutorial->getFloat("posy");
        string textureKey = tutorial->getString("texture");
        _tutorials.push_back(Tutorial{ posX, posY, textureKey });
    }

    return _tutorials;
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

std::vector<std::shared_ptr<Button>> LevelModel::createButtonsAndDoors(const std::shared_ptr<cugl::JsonValue>& buttonsAndDoors) {
    for (int i = 0; i < buttonsAndDoors->size(); i++) {
        std::shared_ptr<cugl::JsonValue> buttondoor = buttonsAndDoors->get(i);
        std::shared_ptr<cugl::JsonValue> button = buttondoor->get("button");
        std::shared_ptr<cugl::JsonValue> door = buttondoor->get("door");
        float ox = door->getFloat("oblx");
        float oy = door->getFloat("obly");
        float nx = door->getFloat("nblx");
        float ny = door->getFloat("nbly");
        float angle = door->getFloat("angle");
        Rect rectangle = Rect(ox,oy,3.0f,0.5f);
        std::shared_ptr<Door> d;
        Poly2 platform(rectangle,false);
        SimpleTriangulator triangulator;
        triangulator.set(platform);
        triangulator.calculate();
        platform.setIndices(triangulator.getTriangulation());
        platform.setGeometry(Geometry::SOLID);
        cugl::Vec2 orpos = cugl::Vec2(ox,oy);
        d = Door::alloc(orpos, platform);
        d->setAngle(angle);
        d->setOriginalPos(orpos);
        d->setNewPos(cugl::Vec2(nx,ny));
        d->setDensity(10000);
        d->setGravityScale(0);
        d->setRestitution(BASIC_RESTITUTION);
        d->setAnchor(Vec2(0,0));
        d->setDebugColor(DEBUG_COLOR);
        _doors.push_back(d);
        float bx = button->getFloat("posx");
        float by = button->getFloat("posy");
        std::shared_ptr<Button> b;
        b = Button::alloc(Vec2(bx,by), Size(1,0.6f));
        b->setDensity(BASIC_DENSITY);
        b->setBodyType(b2_staticBody);
        b->setRestitution(BASIC_RESTITUTION);
        b->setDebugColor(DEBUG_COLOR);
        b->setDoor(d);
        _buttons.push_back(b);
    }
    return _buttons;
}

