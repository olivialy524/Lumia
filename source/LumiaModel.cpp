//
//  LMDudeModel.cpp
//  This file will be used as reference for building the body of the main character Lumia
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#include "LumiaModel.h"
#include "LumiaNode.h"
#include <cugl/scene2/graph/CUPolygonNode.h>
#include <cugl/scene2/graph/CUTexturedNode.h>
#include <cugl/assets/CUAssetManager.h>


using namespace cugl;

#pragma mark -


void LumiaModel::setTextures(const std::shared_ptr<Texture>& idle, const std::shared_ptr<Texture>& splitting) {
    
    _sceneNode = scene2::SceneNode::allocWithBounds(Size(splitting->getWidth()/5.0f,splitting->getHeight()/4.0f));
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
   _node = LumiaNode::alloc(splitting, 4, 5, 20);
    auto scale =  getRadius()*2/(splitting->getHeight()/4.0f/_drawScale);
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
    _removed = false;
    
    if (WheelObstacle::init(pos,radius)) {
        setDensity(.1 / radius);
        setFriction(0.1f);
        // add bounciness to Lumia
        setRestitution(LUMIA_RESTITUTION);
        setFixedRotation(false);
        
        // Gameplay attributes
        _isGrounded = false;
        _state = Idle;
        _velocity = Vec2::ZERO;
        _isLaunching = false;
        
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
    sensorShape.m_radius = _radius * 1.2f;

    sensorDef.shape = &sensorShape;
    _sensorFixture = _body->CreateFixture(&sensorDef);
    _sensorFixture->SetUserData(getSensorName());
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
    if (!isLaunching() && isGrounded()) {
        // When Lumia is not being launched (i.e. has landed), want to apply friction to slow X velocity
        b2Vec2 forceX(-getDamping() * getVX(), 0);
        _body->ApplyForce(forceX, _body->GetPosition(), true);
    }

    // put a cap on maximum velocity Lumia can have
    if (getLinearVelocity().lengthSquared() >= pow(getMaxVelocity(), 2)) {
        Vec2 vel = getLinearVelocity().normalize().scale(getMaxVelocity());
        _body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
    }
}


/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void LumiaModel::update(float dt) {
    _lastPosition = getPosition();
    WheelObstacle::update(dt);
    
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
void LumiaModel::resetDebug() {
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



void LumiaModel::setDrawScale(float scale){
    _drawScale = scale;
}
