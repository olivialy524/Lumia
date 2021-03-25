//
//  LMPlant.cpp
//  Lumia
//
//  Created by Ben Dacek on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "Plant.h"

void Plant::dispose() {
    _node = nullptr;
    _lampNode = nullptr;
    _lampLitNode = nullptr;
}
