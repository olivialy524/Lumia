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


void LumiaModel::setTextures(const std::shared_ptr<Texture>& idle, const std::shared_ptr<Texture>& splitting,  const std::shared_ptr<Texture>& indicator) {
    
    _sceneNode = LumiaNode::alloc(Size(splitting->getWidth()/5.0f,splitting->getHeight()/4.0f));
    _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
    _sceneNode->setLevel(_sizeLevel);
    _sceneNode->setTextures(idle, splitting, indicator, getRadius(), _drawScale);
   
    
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
struct LumiaModel::LumiaSize size0 = { 0.51f, 0.68f };
struct LumiaModel::LumiaSize size1 = { 0.69f, 0.44f };
struct LumiaModel::LumiaSize size2 = { 0.83f, 0.39f };
struct LumiaModel::LumiaSize size3 = { 1.0f, 0.30f };
struct LumiaModel::LumiaSize size4 = { 1.2f, 0.21f };
struct LumiaModel::LumiaSize size5 = { 1.44f, 0.19f };
std::vector<LumiaModel::LumiaSize> LumiaModel::sizeLevels = { size0, size1, size2, size3, size4, size5 };

bool LumiaModel::init(const cugl::Vec2& pos, float radius, float scale) {
    _drawScale = scale;
    _removed = false;
    
    if (WheelObstacle::init(pos,radius)) {
        // Gameplay attributes
        _isGrounded = false;
        _isRolling = false;
        _state = Idle;
        _sizeLevel = 2;
        _velocity = Vec2::ZERO;
        _isLaunching = false;
        _removed = false;
        _isOnStickyWall = false;
        _isOnButton = false;
        _radius = radius;

        setDensity(LumiaModel::sizeLevels[_sizeLevel].density);
        setFriction(0.2f);
        setAngularDamping(0.2f);
        // add bounciness to Lumia
        setRestitution(LUMIA_RESTITUTION);
        setFixedRotation(false);
        
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
    sensorDef.density = LumiaModel::sizeLevels[_sizeLevel].density * 0.45f;
    sensorDef.isSensor = true;

    b2CircleShape sensorShape;
    sensorShape.m_radius = _radius * 1.8f;

    sensorDef.shape = &sensorShape;
    _sensorFixture = _body->CreateFixture(&sensorDef);
    _sensorFixture->SetUserData(getLaunchSensorName());
    
    b2FixtureDef sensorDef2;
    sensorDef2.density = 0;
    sensorDef2.isSensor = true;

    b2CircleShape sensorShape2;
    sensorShape2.m_radius = _radius * 1.1f;

    sensorDef2.shape = &sensorShape2;
    _sensorFixture2 = _body->CreateFixture(&sensorDef2);
    _sensorFixture2->SetUserData(getFrictionSensorName());
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
    _sceneNode = nullptr;
    _sensorNode = nullptr;
    _sensorNode2 = nullptr;
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
    
    if ((isOnStickyWall() || isOnButton()) && !isLaunching()){
        setLinearVelocity(Vec2::ZERO);
        setAngularVelocity(0.0f);
        b2Vec2 force(_stickDirection.x, _stickDirection.y);
        _body->ApplyLinearImpulse(force, _body->GetPosition(), true);
    } else if ((isOnStickyWall() || isOnButton()) && isLaunching()){
        unStick();
    }
    
    // If Lumia is on the ground, and Lumia is being launched, apply velocity impulse to body
    if (isLaunching() && isGrounded()) {
        b2Vec2 force(getVelocity().x, getVelocity().y);
        _body->ApplyLinearImpulse(force, _body->GetPosition(), true);
    }
    if (!isLaunching() && isRolling()) {
        // When Lumia is not being launched (i.e. has landed), want to apply friction to slow X velocity
        b2Vec2 forceX(-getDamping() * getVX(), 0);
        _body->ApplyForce(forceX, _body->GetPosition(), true);
    }

    // put a cap on maximum velocity Lumia can have
    if (getLinearVelocity().lengthSquared() >= pow(getMaxVelocity(), 2)) {
        Vec2 vel = getLinearVelocity().normalize().scale(getMaxVelocity());
        setLinearVelocity(vel);
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
    Poly2 poly = factory.makeCircle(Vec2::ZERO,1.8f*getRadius());

    _sensorNode = scene2::WireNode::allocWithTraversal(poly, poly2::Traversal::CLOSED);
    _sensorNode->setColor(Color4f::RED);
    auto size = _debug->getContentSize();
    _sensorNode->setPosition(Vec2(size.width/2.0f, size.height/2.0f));
    _debug->addChild(_sensorNode);
    
    Poly2 poly2 = factory.makeCircle(Vec2::ZERO,1.1f*getRadius());
    _sensorNode2 = scene2::WireNode::allocWithTraversal(poly, poly2::Traversal::CLOSED);
    _sensorNode2->setColor(Color4f::RED);
    size = _debug->getContentSize();
    _sensorNode2->setPosition(Vec2(size.width/2.0f, size.height/2.0f));
    _debug->addChild(_sensorNode2);
}



void LumiaModel::setDrawScale(float scale){
    _drawScale = scale;
}
