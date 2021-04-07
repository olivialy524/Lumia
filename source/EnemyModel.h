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
    static constexpr float LUMIA_DENSITY = 0.10f;
    /** The restitution of the character */
    static constexpr float LUMIA_RESTITUTION = 0.45f;
    /** The factor to multiply by the input */
    static constexpr float LUMIA_FORCE = 20.0f;
    /** The amount to slow the character down */
    static constexpr float LUMIA_DAMPING = 3.0f;
    /** The maximum character speed */
    static constexpr float LUMIA_MAXVELOCITY = 30.0f;
    

public:
    enum EnemyState {
        /** When Lumia is still or rolling */
        Wander,
        /** When Lumia is splitting */
        Chasing,
        /** When Lumia is merging */
        Fleeing
    };

    
#pragma mark Attributes
    
private:
    /** This macro disables the copy constructor (not allowed on physics objects) */
    CU_DISALLOW_COPY_AND_ASSIGN(EnemyModel);

protected:
    /** The current velocity of Lumia */
    cugl::Vec2 _velocity;
    /** Radius of Lumia's body */
    float _radius;
    /** Ground sensor to represent our feet */
    b2Fixture*  _sensorFixture;
    /** Reference to the sensor name (since a constant cannot have a pointer) */
    std::string _sensorName;
    /** The node for debugging the sensor */
    std::shared_ptr<cugl::scene2::WireNode> _sensorNode;

    /** The scene graph node for Lumia. */
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

    /**
    * Redraws the outline of the physics fixtures to the debug node
    *
    * The debug node is use to outline the fixtures attached to this object.
    * This is very useful when the fixtures have a very different shape than
    * the texture (e.g. a circular shape attached to a square texture).
    */
    virtual void resetDebug() override;

public:
    
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate Lumia object.
     *
     * This constructor does not initialize any of the Lumia values beyond
     * the defaults.  To use a LumiaModel, you must call init().
     */
    EnemyModel() : cugl::physics2::WheelObstacle() { }
    
    
    /**
     * Destroys this LumiaModel, releasing all resources.
     */
    virtual ~EnemyModel(void) { dispose(); }
    
    void setVelocity();
    /**
     * Disposes all resources and assets of this LumiaModel
     *
     * Any assets owned by this object will be immediately released.  Once
     * disposed, a LumiaModel may not be used until it is initialized again.
     */
    void dispose();
    
    /**
     * Initializes a new Lumia at the origin.
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
     * Initializes a new Lumia at the given position.
     *
     * The Lumia is unit square scaled so that 1 pixel = 1 Box2d unit
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
     * The Lumia has the given size, scaled so that 1 pixel = 1 Box2d unit
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
    virtual bool init(const cugl::Vec2& pos, float radius, float scale);
    
    /**
     * Sets the textures for this lumia.
     *
     * @param lumia      The texture for the lumia filmstrip
     */
    void setTextures(const std::shared_ptr<cugl::Texture>& texture);

    void setDrawScale(float scale);
    
#pragma mark -
#pragma mark Static Constructors
    /**
     * Creates a new Lumia at the origin.
     *
     * The Lumia is a unit square scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  A newly allocated LumiaModel at the origin
     */
    static std::shared_ptr<EnemyModel> alloc() {
        std::shared_ptr<EnemyModel> result = std::make_shared<EnemyModel>();
        return (result->init() ? result : nullptr);
    }

    /**
     * Creates a new Lumia at the given position.
     *
     * The Lumia is a unit square scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     *
     * @return  A newly allocated LumiaModel at the given position
     */
    static std::shared_ptr<EnemyModel> alloc(const cugl::Vec2& pos) {
        std::shared_ptr<EnemyModel> result = std::make_shared<EnemyModel>();
        return (result->init(pos) ? result : nullptr);
    }

    /**
     * Creates a new Lumia at the given position.
     *
     * The Lumia has the given size, scaled so that 1 pixel = 1 Box2d unit
     *
      * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The size of the Lumia in world units
     *
     * @return  A newly allocated LumiaModel at the given position with the given scale
     */
    static std::shared_ptr<EnemyModel> alloc(const cugl::Vec2& pos, float radius) {
        std::shared_ptr<EnemyModel> result = std::make_shared<EnemyModel>();
        return (result->init(pos, radius) ? result : nullptr);
    }

    /**
     * Creates a new Lumia at the given position.
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
     * @return  A newly allocated LumiaModel at the given position with the given scale
     */
    static std::shared_ptr<EnemyModel> alloc(const cugl::Vec2& pos, float radius, float scale) {
        std::shared_ptr<EnemyModel> result = std::make_shared<EnemyModel>();
        return (result->init(pos, radius, scale) ? result : nullptr);
    }
    

#pragma mark -
#pragma mark Animation
    /**
     * Returns the scene graph node representing this LumiaModel.
     *
     * By storing a reference to the scene graph node, the model can update
     * the node to be in sync with the physics info. It does this via the
     * {@link Obstacle#update(float)} method.
     *
     * @return the scene graph node representing this LumiaModel.
     */
    const std::shared_ptr<cugl::scene2::SceneNode>& getSceneNode() { return _sceneNode; }
    
    
    const std::shared_ptr<cugl::scene2::SceneNode>& getNode() const { return _node; }

    /**
     * Sets the scene graph node representing this LumiaModel.
     *
     * Note that this method also handles creating the nodes for the body parts
     * of this LumiaModel. Since the obstacles are decoupled from the scene graph,
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
     * Returns current velocity of Lumia.
     *
     * This is the result of drag-and-release input.
     *
     * @return velocity of Lumia.
     */
    cugl::Vec2 getVelocity() const { return _velocity; }
    
    cugl::Vec2 getLastPosition() const {return _lastPosition;}
    
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
     * Sets velocity of Lumia.
     *
     * This is the result of drag-and-release input.
     *
     * @param value velocity of Lumia.
     */
    void setVelocity(cugl::Vec2 value) { _velocity = value; }
    /**
     * Returns ow hard the brakes are applied to get a Lumia to stop moving
     *
     * @return ow hard the brakes are applied to get a Lumia to stop moving
     */
    float getDamping() const { return LUMIA_DAMPING; }
    
    /**
     * Returns the upper limit on Lumia's velocity.
     *
     * @return the upper limit on Lumia'v velocity.
     */
    float getMaxVelocity() const { return LUMIA_MAXVELOCITY; }
    
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
#pragma mark -
#pragma mark Physics Methods
    /**
     * Creates the physics Body(s) for this object, adding them to the world.
     *
     * This method overrides the base method to keep your ship from spinning.
     *
     * @param world Box2D world to store body
     *
     * @return true if object allocation succeeded
     */
    void createFixtures() override;
    
    /**
     * Release the fixtures for this body, reseting the shape
     *
     * This is the primary method to override for custom physics objects.
     */
    void releaseFixtures() override;
    
    /**
     * Updates the object's physics state (NOT GAME LOGIC).
     *
     * We use this method to reset cooldowns.
     *
     * @param delta Number of seconds since last animation frame
     */
    void update(float dt) override;
    
    /**
     * Applies the force to the body of this Lumia
     *
     * This method should be called after the force attribute is set.
     */
    void applyForce();
};


#endif /* EnemyModel_h */
