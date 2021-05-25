//
//  EnemyModel.cpp
//  Lumia
//
//  Created by Olivia Li on 4/5/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "EnemyNode.h"
#include "EnemyModel.h"
#include <cugl/scene2/graph/CUPolygonNode.h>
#include <cugl/scene2/graph/CUTexturedNode.h>
#include <cugl/assets/CUAssetManager.h>


using namespace cugl;

#pragma mark -


void EnemyModel::setTextures(const std::shared_ptr<Texture>& chasing, const std::shared_ptr<Texture>& escaping) {
    
    _sceneNode = EnemyNode::alloc(Size(chasing->getWidth()/ 12.0f,chasing->getHeight()));
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
    _sceneNode->setTextures(chasing, escaping, getRadius(), _drawScale);
}

#pragma mark Constructors

/**
 * Initializes a new Lumia at the given position.
 *
 * The Lumia is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param pos   Initial position in world coordinates
 * @param size  The size of the Lumia in world units
 * @param scale The drawing scale (world to screen)
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool EnemyModel::init(const cugl::Vec2& pos, float radius, float scale) {
    _drawScale = scale;
    
    if (WheelObstacle::init(pos,radius)) {
        // Gameplay attributes
        _velocity = Vec2::ZERO;
        _radius = radius;
        _removed = false;
        _inCoolDown = false;
        _sizeLevel = 2;

        setDensity(LumiaModel::sizeLevels[_sizeLevel].density);
        setFriction(0.1f);
        // add bounciness to enemy
        setRestitution(ENEMY_RESTITUTION);
        setFixedRotation(false);
        setGravityScale(0.1f);

        return true;
    }
    return false;
}


#pragma mark -
#pragma mark Physics Methods

/**
 * Disposes all resources and assets of this LumiaModel
 *
 * Any assets owned by this object will be immediately released.  Once
 * disposed, a LumiaModel may not be used until it is initialized again.
 */
void EnemyModel::dispose() {
    _sensorNode = nullptr;
}


/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void EnemyModel::update(float dt) {
    _lastPosition = getPosition() * _drawScale;
    WheelObstacle::update(dt);
    
    setLinearVelocity(_velocity);
    if (_sceneNode != nullptr && !isRemoved()) {
        _sceneNode->setPosition(getPosition()*_drawScale);
        _sceneNode->setAngle(getAngle());
    }
}

void EnemyModel::setDrawScale(float scale){
    _drawScale = scale;
}
