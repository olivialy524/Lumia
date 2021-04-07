//
//  LevelModel.h
//  Lumia
//
//  Created by Vivi Ye on 3/24/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef LevelModel_h
#define LevelModel_h
#include "LumiaModel.h"
#include "EnemyModel.h"
#include "Plant.h"
#include "Tile.h"

class LevelModel : public cugl::Asset {
private:
    std::vector<std::shared_ptr<Plant>> _plants;
    std::vector<std::shared_ptr<Tile>> _tiles;
    std::vector<std::shared_ptr<EnemyModel>> _enemies;
    std::vector<std::shared_ptr<Tile>> _iregular_tiles;
    std::shared_ptr<LumiaModel> _lumia;
    
    float _xBound;
    
    float _yBound;
    
    std::vector<std::shared_ptr<Plant>> createPlants(const std::shared_ptr<cugl::JsonValue>& plants);
    
    std::vector<std::shared_ptr<Tile>> createTiles(const std::shared_ptr<cugl::JsonValue>& tiles);
    
    std::vector<shared_ptr<EnemyModel>> createEnemies(const std::shared_ptr<cugl::JsonValue>& enemies);
    
    std::vector<std::shared_ptr<Tile>> createIrregular(const std::shared_ptr<cugl::JsonValue>& tiles);
    
    std::shared_ptr<LumiaModel> createLumia(const std::shared_ptr<cugl::JsonValue>& lumia);
    
    std::shared_ptr<cugl::JsonValue> _levelJson;
    
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
    static std::shared_ptr<LevelModel> alloc() {
        std::shared_ptr<LevelModel> result = std::make_shared<LevelModel>();
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
    static std::shared_ptr<LevelModel> alloc(const std::string& file) {
        std::shared_ptr<LevelModel> result = std::make_shared<LevelModel>();
        return (result->init(file) ? result : nullptr);
    }
    
    
#pragma mark Level Attributes
    
    float getXBound(){
        return _xBound;
    }
    
    float getYBound(){
        return _yBound;
    }
    
    std::shared_ptr<LumiaModel> getLumia(){
        return _lumia;
    }
        
    std::vector<std::shared_ptr<Tile>> getTiles(){
        return _tiles;
    }
    
    std::vector<std::shared_ptr<Plant>> getPlants(){
        return _plants;
    }
    
    
    std::vector<std::shared_ptr<EnemyModel>> getEnemies(){
        return _enemies;
    }
        
    std::vector<std::shared_ptr<Tile>> getIrregularTile(){
        return _iregular_tiles;
    }
    
    void resetLevel(const std::string& file){
        dispose();
        preload(_levelJson);
    }
    
    bool preload(const std::string& file) override {
        std::shared_ptr<cugl::JsonReader> reader = cugl::JsonReader::allocWithAsset(file);
        return preload(reader->readJson());
    }
    
    bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;
    
    
#pragma mark Constructors
    LevelModel() {};
    
    /**
     * Destroys this LumiaModel, releasing all resources.
     */
    virtual ~LevelModel(void) { dispose(); }
    
    void dispose();
    
    
};








#endif /* LevelModel_h */
