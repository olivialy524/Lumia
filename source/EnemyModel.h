//
//  EnemyModel.h
//  Lumia
//
//  Created by Olivia Li on 4/5/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef EnemyModel_h
#define EnemyModel_h
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include <cugl/physics2/CUCapsuleObstacle.h>
#include <cugl/scene2/graph/CUWireNode.h>
#include "EnemyNode.h"
#include "LumiaModel.h"

#pragma mark Enemy Model
/**
* Player avatar for the plaform game.
*
* Note that this class uses a capsule shape, not a rectangular shape.  In our
* experience, using a rectangular shape for a character will regularly snag
* on a platform.  The round shapes on the end caps lead to smoother movement.
*/
class EnemyModel : public cugl::physics2::WheelObstacle {
#pragma mark Constants and Enums
protected:
#define SIGNUM(x)  ((x > 0) - (x < 0))
    
/** Debug color for the sensor */
#define DEBUG_COLOR     Color4::YELLOW

    /** The base density of the character */
    static constexpr float ENEMY_DENSITY = 0.10f;
    /** The restitution of the character */
    static constexpr float ENEMY_RESTITUTION = 0.45f;
    /** The factor to multiply by the input */
    static constexpr float ENEMY_FORCE = 20.0f;
    /** The amount to slow the character down */
    static constexpr float ENEMY_DAMPING = 3.0f;
    /** The maximum character speed */
    static constexpr float ENEMY_MAXVELOCITY = 30.0f;
    

public:
    enum EnemyState {
        /** When enemy is wandering around */
        Wander,
        /** When enemy is chasing after Lumia */
        Chasing,
        /** When enemy is fleeing from Lumia */
        Fleeing
    };

    
#pragma mark Attributes
    
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(EnemyModel);

protected:
    /** The current velocity of the enemy */
    cugl::Vec2 _velocity;
    /** Radius of the enemy's body */
    float _radius;
    /** The size level of the enemy */
    int _sizeLevel;
    /** Ground sensor to represent our feet */
    b2Fixture*  _sensorFixture;
    /** Reference to the sensor name (since a constant cannot have a pointer) */
    std::string _sensorName;
    /** The node for debugging the sensor */
    std::shared_ptr<cugl::scene2::WireNode> _sensorNode;

    /** The scene graph node for the enemy. */
    std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    std::shared_ptr<EnemyNode> _node;

    /** The scale between the physics world and the screen (MUST BE UNIFORM) */
    float _drawScale;
    
    /** The current state of this enemy*/
    EnemyState _state;
    
    Vec2 _lastPosition;
    
    LumiaModel* target;
    
    bool _removed;
    
    bool _inCoolDown;

public:
    
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate enemy object.
     *
     * This constructor does not initialize any of the Lumia values beyond
     * the defaults.  To use a LumiaModel, you must call init().
     */
    EnemyModel() : cugl::physics2::WheelObstacle() { }
    
    
    /**
     * Destroys this EnemyModel, releasing all resources.
     */
    virtual ~EnemyModel(void) { dispose(); }
    
    /**
     * Disposes all resources and assets of this EnemyModel
     *
     * Any assets owned by this object will be immediately released.  Once
     * disposed, a EnemyModel may not be used until it is initialized again.
     */
    void dispose();
    
    /**
     * Initializes a new enemy at the origin.
     *
     * The Lumia is a unit square scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init() override { return init(cugl::Vec2::ZERO, 1.0f, 1.0f); }
    
    /**
     * Initializes a new enemy at the given position.
     *
     * The enemy is unit square scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const cugl::Vec2 pos) override { return init(pos, 1.0f, 1.0f); }
    
    /**
     * Initializes a new Lumia at the given position.
     *
     * The enemy has the given size, scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The size of the Lumia in world units
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const cugl::Vec2 pos, float radius) override {
        return init(pos, radius, 1.0f);
    }
    
    /**
     * Initializes a new enemy at the given position.
     *
     * The enemy is sized according to the given drawing scale.
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
    virtual bool init(const cugl::Vec2& pos, float radius, float scale);
    
    /**
     * Sets the textures for this enemy.
     *
     * @param texture      The texture for the enemy filmstrip
     */
    void setTextures(const std::shared_ptr<cugl::Texture>& texture);

    void setDrawScale(float scale);
    
    void setFleeingTint(){
        _node->setColor(Color4f::BLUE);
    }
    
    void resetTint(){
        _node->setColor(Color4f::WHITE);
    }
    
#pragma mark -
#pragma mark Static Constructors
    /**
     * Creates a new enemy at the origin.
     *
     * The enemy is a unit square scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  A newly allocated EnemyModel at the origin
     */
    static std::shared_ptr<EnemyModel> alloc() {
        std::shared_ptr<EnemyModel> result = std::make_shared<EnemyModel>();
        return (result->init() ? result : nullptr);
    }

    /**
     * Creates a new enemy at the given position.
     *
     * The enemy is a unit square scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     *
     * @return  A newly allocated EnemyModel at the given position
     */
    static std::shared_ptr<EnemyModel> alloc(const cugl::Vec2& pos) {
        std::shared_ptr<EnemyModel> result = std::make_shared<EnemyModel>();
        return (result->init(pos) ? result : nullptr);
    }

    /**
     * Creates a new enemy at the given position.
     *
     * The enemy has the given size, scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param radius  The size of the enemy in world units
     *
     * @return  A newly allocated EnemyModel at the given position with the given scale
     */
    static std::shared_ptr<EnemyModel> alloc(const cugl::Vec2& pos, float radius) {
        std::shared_ptr<EnemyModel> result = std::make_shared<EnemyModel>();
        return (result->init(pos, radius) ? result : nullptr);
    }

    /**
     * Creates a new enemy at the given position.
     *
     * The enemy is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param radius  The size of the enemy in world units
     * @param scale The drawing scale (world to screen)
     *
     * @return  A newly allocated EnemyModel at the given position with the given scale
     */
    static std::shared_ptr<EnemyModel> alloc(const cugl::Vec2& pos, float radius, float scale) {
        std::shared_ptr<EnemyModel> result = std::make_shared<EnemyModel>();
        return (result->init(pos, radius, scale) ? result : nullptr);
    }
    

#pragma mark -
#pragma mark Animation
    /**
     * Returns the scene graph node representing this EnemyModel.
     *
     * By storing a reference to the scene graph node, the model can update
     * the node to be in sync with the physics info. It does this via the
     * {@link Obstacle#update(float)} method.
     *
     * @return the scene graph node representing this EnemyModel.
     */
    const std::shared_ptr<cugl::scene2::SceneNode>& getSceneNode() { return _sceneNode; }
    
    
    const std::shared_ptr<cugl::scene2::SceneNode>& getNode() const { return _node; }

    /**
     * Sets the scene graph node representing this EnemyModel.
     *
     * Note that this method also handles creating the nodes for the body parts
     * of this EnemyModel. Since the obstacles are decoupled from the scene graph,
     * initialization (which creates the obstacles) occurs prior to the call to
     * this method. Therefore, to be drawn to the screen, the nodes of the attached
     * bodies must be added here.
     *
     * The bubbler also uses the world node when adding bubbles to the scene, so
     * the input node must be added to the world BEFORE this method is called.
     *
     * By storing a reference to the scene graph node, the model can update
     * the node to be in sync with the physics info. It does this via the
     * {@link Obstacle#update(float)} method.
     *
     * @param node  The scene graph node representing this LumiaModel, which has been added to the world node already.
     */
    void setSceneNode(const std::shared_ptr<cugl::scene2::SceneNode>& node) {
        _sceneNode = node;
        _sceneNode->setPosition(getPosition() * _drawScale);
    }

    
#pragma mark -
#pragma mark Attribute Properties
    /**
     * Returns current velocity of enemy.
     *
     * @return velocity of enemy.
     */
    cugl::Vec2 getVelocity() const { return _velocity; }
    
    cugl::Vec2 getLastPosition() const { return _lastPosition; }

    int getSizeLevel() { return _sizeLevel; }

    void setSizeLevel(int value) { _sizeLevel = value; }
    
    void setInCoolDown(bool value){
        _inCoolDown = value;
    }
    
    bool getInCoolDown(){
        return _inCoolDown;
    }
    
    void setState(EnemyState state){
        _state = state;
    }
    
    EnemyState getState(){
        return _state;
    }
    
    /**
     * Sets velocity of enemy.
     *
     * @param value velocity of enemy.
     */
    void setVelocity(cugl::Vec2 value) { _velocity = value; }

    /**
     * Returns how hard the brakes are applied to get an enemy to stop moving
     *
     * @return how hard the brakes are applied to get an enemy to stop moving
     */
    float getDamping() const { return ENEMY_DAMPING; }
    
    /**
     * Returns the upper limit on enemys velocity.
     *
     * @return the upper limit on enemy's velocity.
     */
    float getMaxVelocity() const { return ENEMY_MAXVELOCITY; }
    
    /**
     * Returns the name of the ground sensor
     *
     * This is used by ContactListener
     *
     * @return the name of the ground sensor
     */
    std::string* getSensorName() { return &_sensorName; }

    void setRemoved(bool value){
        _removed =value;
    }
    bool getRemoved(){
        return _removed;
    }
    
    void update(float dt) override;
};


#endif /* EnemyModel_h */
