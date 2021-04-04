//
//  Door.cpp
//  Lumia
//
//  Created by Ben Dacek on 4/3/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "Door.h"

void Door::dispose() {
    _node = nullptr;
    _scenenode = nullptr;
}
