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


void EnemyModel::setTextures(const std::shared_ptr<Texture>& texture) {
    
    _sceneNode = scene2::SceneNode::allocWithBounds(Size(texture->getWidth()/5.0f,texture->getHeight()/4.0f));
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
   _node = EnemyNode::alloc(texture, 4, 5, 20);
    auto scale =  getRadius()*2/(texture->getHeight()/4.0f/_drawScale);
   _node->setScale(scale);
   _node->setAnchor(Vec2::ANCHOR_CENTER);
  
   _node->setFrame(0);
   _sceneNode->addChild(_node);
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
 * Create new fixtures for this body, defining the shape
 *
 * This is the primary method to override for custom physics objects
 */
void EnemyModel::createFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    WheelObstacle::createFixtures();
    b2FixtureDef sensorDef;
    sensorDef.density = ENEMY_DENSITY;
    sensorDef.isSensor = true;

    b2CircleShape sensorShape;
    sensorShape.m_radius = _radius * 1.1f;

    sensorDef.shape = &sensorShape;
    _sensorFixture = _body->CreateFixture(&sensorDef);
    _sensorFixture->SetUserData(getSensorName());
}

/**
 * Release the fixtures for this body, reseting the shape
 *
 * This is the primary method to override for custom physics objects.
 */
void EnemyModel::releaseFixtures() {
    if (_body != nullptr) {
        WheelObstacle::releaseFixtures();
        return;
    }
    WheelObstacle::releaseFixtures();
    if (_sensorFixture != nullptr) {
        _body->DestroyFixture(_sensorFixture);
        _sensorFixture = nullptr;
    }
}

/**
 * Disposes all resources and assets of this LumiaModel
 *
 * Any assets owned by this object will be immediately released.  Once
 * disposed, a LumiaModel may not be used until it is initialized again.
 */
void EnemyModel::dispose() {
    _node = nullptr;
    _sensorNode = nullptr;
}

/**
 * Applies the force to the body of this Lumia
 *
 * This method should be called after the force attribute is set.
 */
void EnemyModel::applyForce() {
    if (!isActive()) {
        return;
    }
    
    // If Lumia is on the ground, and Lumia is being launched, apply velocity impulse to body
//    if (isLaunching() && isGrounded()) {
//        b2Vec2 force(getVelocity().x, getVelocity().y);
//        _body->ApplyLinearImpulse(force, _body->GetPosition(), true);
//    }
//    if (!isLaunching() && isGrounded()) {
//        // When Lumia is not being launched (i.e. has landed), want to apply friction to slow X velocity
//        b2Vec2 forceX(-getDamping() * getVX(), 0);
//        _body->ApplyForce(forceX, _body->GetPosition(), true);
//    }
//
//    // put a cap on maximum velocity Lumia can have
//    if (getLinearVelocity().lengthSquared() >= pow(getMaxVelocity(), 2)) {
//        Vec2 vel = getLinearVelocity().normalize().scale(getMaxVelocity());
//        _body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
//    }
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


#pragma mark -
#pragma mark Scene Graph Methods
/**
 * Redraws the outline of the physics fixtures to the debug node
 *
 * The debug node is use to outline the fixtures attached to this object.
 * This is very useful when the fixtures have a very different shape than
 * the texture (e.g. a circular shape attached to a square texture).
 */
void EnemyModel::resetDebug() {
    WheelObstacle::resetDebug();

    PolyFactory factory;
    factory.setSegments(12);
    factory.setGeometry(Geometry::PATH);
    Poly2 poly = factory.makeCircle(Vec2::ZERO,1.15f*getRadius());

    _sensorNode = scene2::WireNode::allocWithTraversal(poly, poly2::Traversal::CLOSED);
    _sensorNode->setColor(DEBUG_COLOR);
    auto size = _debug->getContentSize();
    _sensorNode->setPosition(Vec2(size.width/2.0f, size.height/2.0f));
    _debug->addChild(_sensorNode);
}

void EnemyModel::setDrawScale(float scale){
    _drawScale = scale;
}
