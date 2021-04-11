//
//  Button.cpp
//  Lumia
//
//  Created by Ben Dacek on 4/3/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "Button.h"
void Button::dispose() {
    _node = nullptr;
    _sceneNode = nullptr;
}
