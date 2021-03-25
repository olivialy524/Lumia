//
//  LMSplitter.cpp
//  Lumia
//
//  Created by Ben Dacek on 3/20/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include "LMSplitter.h"

void Splitter::dispose() {
    _node = nullptr;
    _sceneNode = nullptr;
}
