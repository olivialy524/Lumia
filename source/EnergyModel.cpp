//
//  LMEnergyModel.cpp
//  Lumia
//
//  Created by Ben Dacek on 3/17/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include "EnergyModel.h"

bool EnergyModel::init(const cugl::Vec2 pos, cugl::Size size) {
    _removed = false;

    if (BoxObstacle::init(pos, size)) {
        return true;
    }
    return false;
}

void EnergyModel::dispose() {
    _node = nullptr;
    _energyNode = nullptr;
}

