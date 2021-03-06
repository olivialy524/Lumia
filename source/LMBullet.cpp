//
//  PFBullet.cpp
//  PlatformDemo
//
//  This class is a simple extension of WheelObstacle in order to simplify the
//  process of adding and removing Bullets from the game world, as well as for
//  drawing the sprite of the bullet.
//
//  Author:  Walker White and Anthony Perello
//  Version: 2/9/21
//
#include "LMBullet.h"
#include <cugl/scene2/graph/CUPolygonNode.h>
#include <cugl/scene2/graph/CUTexturedNode.h>
#include <cugl/assets/CUAssetManager.h>

#pragma mark -
#pragma mark Initializers

using namespace cugl;


/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void Bullet::update(float dt) {
	WheelObstacle::update(dt);

	if (_node != nullptr) {
		_node->setPosition(getPosition()*_drawScale);
		_node->setAngle(getAngle());
	}
}

/**
 * Disposes all resources and assets of this Bullet
 *
 * Any assets owned by this object will be immediately released.  Once
 * disposed, a Bullet may not be used until it is initialized again.
 */
void Bullet::dispose() {
	_node = nullptr;
}




