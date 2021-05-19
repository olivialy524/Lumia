//
//  ShrinkingDoor.cpp
//  Lumia
//
//  Created by Olivia Li on 5/18/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#include <stdio.h>
#include "ShrinkingDoor.h"
#define NUM_FRAMES   12

void ShrinkingDoor::setTextures(const std::shared_ptr<Texture>& door){
//    _sceneNode = scene2::SceneNode::allocWithBounds(Size(door->getWidth()/NUM_FRAMES,door->getHeight()));
//    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
    _sceneNode = ShrinkingDoorNode::alloc(door, 1, 6, 6);
    auto scale =  _door->getWidth()/(door->getWidth()/NUM_FRAMES/_drawScale);
    _sceneNode->setScale(scale);
    _sceneNode->setAnchor(Vec2::ZERO);
    _sceneNode->setAngle(getAngle());
//    _sceneNode->addChild(_node);
}

void ShrinkingDoor::dispose() {
    _sceneNode = nullptr;
    _bodies.clear();
}

void ShrinkingDoor::Close() {
    if (getClosed() || !getClosing()) {
        return;
    }
    _sceneNode->setAnimState(ShrinkingDoorNode::Closing);
    float height = (NUM_FRAMES - _sceneNode->getFrame())/NUM_FRAMES * getNormHeight();
    _door->setHeight(height);
}

void ShrinkingDoor::Open() {
    if (getOpened() || !getOpening()) {
        return;
    }
    _sceneNode->setAnimState(ShrinkingDoorNode::Opening);
    float height = (NUM_FRAMES - _sceneNode->getFrame())/NUM_FRAMES * getNormHeight();
    _door->setHeight(height);
}

void ShrinkingDoor::update(float dt) {
//    PolygonObstacle::update(dt);
    if (_sceneNode != nullptr) {
        _sceneNode->setPosition(getPosition()*_drawScale);
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
    if (!Obstacle::init(pos)) {
        return false;
    }
    _opening = false;
    _closing = false;
    _normHeight = size.y;
    
    // Create the bottom
    std::shared_ptr<physics2::Obstacle> body = physics2::BoxObstacle::alloc(pos,Vec2(0.8f,0.5f));
    body->setName("bottom");
    body->setBodyType(b2_staticBody);
    _bodies.push_back(body);

    // Create the door
    _door = physics2::BoxObstacle::alloc(pos,size);
    _door->setName("door");
    _door->setBodyType(b2_staticBody);
    _bodies.push_back(body);

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
//bool ShrinkingDoor::createJoints(b2World& world) {
//    b2RevoluteJointDef jointDef;
//    jointDef.bodyA = _bodies[0]->getBody();
//    jointDef.bodyB = _bodies[1]->getBody();
//    jointDef.localAnchorA.Set(0,0);
//    jointDef.localAnchorB.Set(0,0);
//    b2Joint* joint = world.CreateJoint(&jointDef);
//    _joints.push_back(joint);
//
//    return true;
//}


#pragma mark -
#pragma mark Physics
/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * This method is called AFTER the collision resolution state. Therefore, it
 * should not be used to process actions or any other gameplay information.
 * Its primary purpose is to adjust changes to the fixture, which have to
 * take place after collision.
 *
 * In other words, this is the method that updates the scene graph.  If you
 * forget to call it, it will not draw your changes.
 *
 * @param delta Timing values from parent loop
 */
//void ShrinkingDoor::update(float delta) {
//    Obstacle::update(delta);
//    if (_node != nullptr) {
//        std::vector<std::shared_ptr<scene2::SceneNode>> children = _node->getChildren();
//        int i = 0;
//
//        // Update the nodes of the attached bodies
//        for (auto it = children.begin(); it != children.end(); ++it) {
//            (*it)->setPosition(_bodies[i]->getPosition()*_drawScale);
//            (*it)->setAngle(_bodies[i]->getAngle());
//
//            // Propagate the update to the bodies attached to the Spinner
//            _bodies[i]->update(delta);
//            i++;
//        }
//    }
//}
