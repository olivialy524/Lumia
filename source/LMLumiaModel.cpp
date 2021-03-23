//
//  LMDudeModel.cpp
//  This file will be used as reference for building the body of the main character Lumia
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#include "LMLumiaModel.h"
#include "LMLumiaNode.h"
#include <cugl/scene2/graph/CUPolygonNode.h>
#include <cugl/scene2/graph/CUTexturedNode.h>
#include <cugl/assets/CUAssetManager.h>

#define SIGNUM(x)  ((x > 0) - (x < 0))

#pragma mark -
#pragma mark Physics Constants

/** Number of rows in the ship image filmstrip */
#define LUMIA_ROWS       1
/** Number of columns in this ship image filmstrip */
#define LUMIA_COLS       1
/** Number of elements in this ship image filmstrip */
#define LUMIA_FRAMES     1
/** The amount to shrink the sensor fixture (horizontally) relative to the image */
#define LUMIA_SSHRINK  0.6f
/** Height of the sensor attached to the player's feet */
#define SENSOR_HEIGHT   0.1f
/** The density of the character */
#define LUMIA_DENSITY    0.13f
/** The restitution of the character */
#define LUMIA_RESTITUTION 0.5f


/** Debug color for the sensor */
#define DEBUG_COLOR     Color4::RED


using namespace cugl;

#pragma mark -


void LumiaModel::setTextures(const std::shared_ptr<Texture>& lumia, Vec2 initPos) {
    _sceneNode = scene2::SceneNode::allocWithBounds(lumia->getSize());
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
   _node = LumiaNode::alloc(lumia, 1, 1, 1);
    auto scale =  getRadius()*2/(lumia->getWidth()/_drawScale);
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
bool LumiaModel::init(const cugl::Vec2& pos, float radius, float scale) {
    _drawScale = scale;
    
    if (WheelObstacle::init(pos,radius)) {
        setDensity(LUMIA_DENSITY);
        setFriction(0.05f);
        // add bounciness to Lumia
        setRestitution(LUMIA_RESTITUTION);
        setFixedRotation(false);
        
        // Gameplay attributes
        _isGrounded = false;
        
        _radius = radius;
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
void LumiaModel::createFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    WheelObstacle::createFixtures();
    b2FixtureDef sensorDef;
    sensorDef.density = LUMIA_DENSITY;
    sensorDef.isSensor = true;

    b2CircleShape sensorShape;
    sensorShape.m_radius = _radius + 0.1f;

    sensorDef.shape = &sensorShape;
    _sensorFixture = _body->CreateFixture(&sensorDef);
    _sensorFixture->SetUserData(getSensorName());
}


void LumiaModel::split(){
//    CULog("mass pre split %f", _body->GetMass());
    _radius = _radius / 1.4f;
    WheelObstacle::setRadius(_radius);
    resetMass();
    
//    CULog("mass post split %f", _body->GetMass());
    _node->setScale(_node->getScale()/1.4f);
}

void LumiaModel::merge(float addRadius){
//    CULog("mass pre merge %f", _body->GetMass());
    float newRadius = 0.4f * addRadius + _radius;
    float scale = newRadius / _radius;
    _radius = newRadius;
    WheelObstacle::setRadius(_radius);
    resetMass();
    
    _node->setScale(_node->getScale()*scale);
    _node->setPosition(Vec2(-getRadius()*_drawScale, -getRadius()*_drawScale));
//    CULog("mass post merge %f", _body->GetMass());
}
/**
 * Release the fixtures for this body, reseting the shape
 *
 * This is the primary method to override for custom physics objects.
 */
void LumiaModel::releaseFixtures() {
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
void LumiaModel::dispose() {
    _node = nullptr;
    _sensorNode = nullptr;
}

/**
 * Applies the force to the body of this Lumia
 *
 * This method should be called after the force attribute is set.
 */
void LumiaModel::applyForce() {
    if (!isActive()) {
        return;
    }
    
    // If Lumia is on the ground, and Lumia is being launched, apply velocity impulse to body
    if (isLaunching() && isGrounded()) {
        b2Vec2 force(getVelocity().x, getVelocity().y);
        _body->ApplyLinearImpulse(force, _body->GetPosition(), true);
    }
    if (isSplitting()){
        b2Vec2 force(_splitForce.x, _splitForce.y);
        _body->ApplyLinearImpulse(force,_body->GetPosition(),true);
    }
    
    if (!isLaunching() && isGrounded()) {
        // When Lumia is not being launched (i.e. has landed), want to apply friction to slow X velocity
        b2Vec2 forceX(-getDamping() * getVX(), 0);
        _body->ApplyForce(forceX, _body->GetPosition(), true);
    }

    // Don't want to be moving. Damp out player motion
    //if (getVelocity().x != 0.0f) {
    //    if (isGrounded()) {
    //        // Instant friction on the ground
    //        b2Vec2 vel = _body->GetLinearVelocity();
    //        vel.x = 0; // If you set y, you will stop a jump in place
    //        _body->SetLinearVelocity(vel);
    //    } else {
    //        // Damping factor in the air
    //        b2Vec2 force(-getDamping()*getVX(),0);
    //        _body->ApplyForce(force,_body->GetPosition(),true);
    //    }
    //}
    //
    //// Velocity too high, clamp it
    //if (fabs(getVX()) >= getMaxSpeed()) {
    //    setVX(SIGNUM(getVX())*getMaxSpeed());
    //} else {
    //    b2Vec2 force(getVelocity().x,0);
    //    _body->ApplyForce(force,_body->GetPosition(),true);
    //}
    //
    //// Jump!
    //if (isJumping() && isGrounded()) {
    //    b2Vec2 force(0, Lumia_JUMP);
    //    _body->ApplyLinearImpulse(force,_body->GetPosition(),true);
    //}
}


/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void LumiaModel::update(float dt) {
    WheelObstacle::update(dt);
    
    if (_sceneNode != nullptr) {
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
void LumiaModel::resetDebug() {
    WheelObstacle::resetDebug();
    float w = LUMIA_SSHRINK*getRadius();
    float h = SENSOR_HEIGHT;
    Poly2 poly(Rect(-w/2.0f,-h/2.0f,w,h));

    _sensorNode = scene2::WireNode::allocWithTraversal(poly, poly2::Traversal::INTERIOR);
    _sensorNode->setColor(DEBUG_COLOR);
    _sensorNode->setPosition(Vec2(_debug->getContentSize().width/2.0f, 0.0f));
    _debug->addChild(_sensorNode);
}



