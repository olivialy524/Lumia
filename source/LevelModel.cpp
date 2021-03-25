//
//  LevelModel.cpp
//  Lumia
//
//  Created by Vivi Ye on 3/24/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

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
    std::vector<std::shared_ptr<Plant>> plants = createPlants(plants_json);

    return true;
};

std::vector<std::shared_ptr<Plant>> LevelModel::createPlants(const std::shared_ptr<cugl::JsonValue> &plants){
    
    for (int i=0; i< plants->size(); i++){
        std::string si = to_string(i);
        std::string ps = ("plant_" + si);
        std::shared_ptr<cugl::JsonValue> plant = plants->get(i);
        float posx = plant->getFloat("posx");
        float posy = plant->getFloat("posy");
        float pa = (plant->getFloat("angle"))*M_PI/180;
        cugl::Size size = cugl::Size(0.28f, 0.42f);
//        std::shared_ptr<Plant> p = Plant::alloc(Vec2(posx,posy), size);
//        p->setBodyType(b2_staticBody);
//        p->setAngle(ang);
//        p->lightDown();
//        p->setFriction(0.0f);
//        p->setRestitution(0.0f);
//        p->setName(PLANT_NAME + to_string(nplant));
//        p->setDensity(0);
//        p->setBullet(false);
//        p->setGravityScale(0);
//        p->setSensor(true);
//        p->setDebugColor(DEBUG_COLOR);
//        p->setDrawScale(_scale);
        
    }
    
    return std::vector<std::shared_ptr<Plant>>();
    
}
