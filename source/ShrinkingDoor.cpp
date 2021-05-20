//
//  ShrinkingDoor.cpp
//  Lumia
//
//  Created by Olivia Li on 5/18/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "ShrinkingDoor.h"
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/b2World.h>
#define NUM_FRAMES   12

void ShrinkingDoor::setTextures(const std::shared_ptr<Texture>& door){
//    _sceneNode = scene2::SceneNode::allocWithBounds(Size(door->getWidth()/NUM_FRAMES,door->getHeight()));
//    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
    _sceneNode = ShrinkingDoorNode::alloc(door, NUM_FRAMES, 1, NUM_FRAMES);
    auto scale =  (_door->getWidth() + 0.4f)/(door->getWidth()/_drawScale);
    _sceneNode->setScale(scale);
    _sceneNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
//    lloc(pos,Vec2(0.5f,0.8f));
    Vec2 offset = Vec2(-0.25f * _drawScale, -0.4f * _drawScale);
    _sceneNode->setPosition(getPosition()*_drawScale + offset);
    _sceneNode->setAngle(getAngle());
}

void ShrinkingDoor::dispose() {
    _sceneNode = nullptr;
    _bodies.clear();
}

void ShrinkingDoor::Close() {
    if (getClosed() || !getClosing()) {
        _door->setSensor(false);
        return;
    }
    _sceneNode->setAnimState(ShrinkingDoorNode::Closing);
}

void ShrinkingDoor::Open() {
    if (getOpened() || !getOpening()) {
        _door->setSensor(true);
        return;
    }
    _sceneNode->setAnimState(ShrinkingDoorNode::Opening);
}

void ShrinkingDoor::update(float dt) {
    Obstacle::update(dt);
    if (_sceneNode != nullptr) {
        Vec2 offset = Vec2(-0.25f * _drawScale, -0.4f * _drawScale);
        _sceneNode->setPosition(getPosition()*_drawScale + offset);
        _sceneNode->setAngle(getAngle());
    }
}

#pragma mark -
#pragma mark Initializers
/**
 * Initializes a new spinner at the given position.
 *
 * The spinner is sized according to the given drawing scale. The spinner
 * is centered on its position.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param pos   Initial position in world coordinates
 * @param size  The spinner size in world units
 * @param scale The drawing scale (world to screen)
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool ShrinkingDoor::init(const cugl::Vec2 pos, const cugl::Vec2 size, float ang) {
    if (!ComplexObstacle::init(pos)) {
        return false;
    }
    _opening = false;
    _closing = false;
    _normHeight = size.x;
    
    // Create the bottom
    std::shared_ptr<physics2::Obstacle> body = physics2::BoxObstacle::alloc(pos,Vec2(0.5f,0.8f));
    body->setName("bottom");
    body->setBodyType(b2_staticBody);
    _bodies.push_back(body);

    // Create the door
    _door = physics2::BoxObstacle::alloc(pos + Vec2(1.5f, 0.0f),size);
    _door->setName("door");
    _door->setBodyType(b2_staticBody);
    _bodies.push_back(_door);
    setBodyType(b2_staticBody);
    setAngle(ang);
    return true;
}

/**
 * Creates the joints for this object.
 *
 * This method is executed as part of activePhysics. This is the primary method to
 * override for custom physics objects.
 *
 * @param world Box2D world to store joints
 *
 * @return true if object allocation succeeded
 */
bool ShrinkingDoor::createJoints(b2World& world) {
    b2RevoluteJointDef jointDef;
    jointDef.bodyA = _bodies[0]->getBody();
    jointDef.bodyB = _bodies[1]->getBody();
    jointDef.localAnchorA.Set(0,0);
    jointDef.localAnchorB.Set(0,0);
    b2Joint* joint = world.CreateJoint(&jointDef);
    _joints.push_back(joint);

    return true;
}
