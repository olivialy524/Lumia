//
//  LMDudeModel.h

//  This file will be used as reference for building the body of the main character Lumia
//
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#ifndef __LM_DUDE_MODEL_H__
#define __LM_DUDE_MODEL_H__
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include <cugl/physics2/CUCapsuleObstacle.h>
#include <cugl/scene2/graph/CUWireNode.h>
#include "LMLumiaNode.h"

#pragma mark -
#pragma mark Drawing Constants
/** The texture for the character avatar */
#define DUDE_TEXTURE    "dude"
/** Identifier to allow us to track the sensor in ContactListener */
#define SENSOR_NAME     "dudesensor"


#pragma mark -
#pragma mark Physics Constants
/** The factor to multiply by the input */
#define DUDE_FORCE      20.0f
/** The amount to slow the character down */
#define DUDE_DAMPING    3.0f
/** The maximum character speed */
#define DUDE_MAXSPEED   5.0f


#pragma mark -
#pragma mark Dude Model
/**
* Player avatar for the plaform game.
*
* Note that this class uses a capsule shape, not a rectangular shape.  In our
* experience, using a rectangular shape for a character will regularly snag
* on a platform.  The round shapes on the end caps lead to smoother movement.
*/
class LumiaModel : public cugl::physics2::WheelObstacle {
private:
	/** This macro disables the copy constructor (not allowed on physics objects) */
	CU_DISALLOW_COPY_AND_ASSIGN(LumiaModel);

protected:
	/** The current velocity of Lumia */
	cugl::Vec2 _velocity;
    /** The planned velocity of Lumia */
    cugl::Vec2 _plannedVelocity;
	/** Which direction is the character facing */
	bool _faceRight;
	/** How long until we can jump again */
	int  _jumpCooldown;
	/** Whether we are actively jumping */
	bool _isJumping;
    /** Whether we are actively launching */
    bool _isLaunching;
	/** How long until we can shoot again */
	int  _shootCooldown;
	/** Whether our feet are on the ground */
	bool _isGrounded;
	/** Whether we are actively shooting */
	bool _isShooting;
    
    bool _isSplitting;
    
    bool _isMerging;
    
    float _radius;
	/** Ground sensor to represent our feet */
	b2Fixture*  _sensorFixture;
	/** Reference to the sensor name (since a constant cannot have a pointer) */
	std::string _sensorName;
	/** The node for debugging the sensor */
	std::shared_ptr<cugl::scene2::WireNode> _sensorNode;

	/** The scene graph node for the Dude. */
	std::shared_ptr<cugl::scene2::SceneNode> _sceneNode;
    std::shared_ptr<LumiaNode> _node;

	/** The scale between the physics world and the screen (MUST BE UNIFORM) */
	float _drawScale;
    
    Vec2 _splitForce;

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
     * Creates a degenerate Dude object.
     *
     * This constructor does not initialize any of the dude values beyond
     * the defaults.  To use a DudeModel, you must call init().
     */
    LumiaModel() : cugl::physics2::WheelObstacle() { }
    
    
    /**
     * Destroys this DudeModel, releasing all resources.
     */
    virtual ~LumiaModel(void) { dispose(); }
    
    /**
     * Disposes all resources and assets of this DudeModel
     *
     * Any assets owned by this object will be immediately released.  Once
     * disposed, a DudeModel may not be used until it is initialized again.
     */
    void dispose();
    
    /**
     * Initializes a new dude at the origin.
     *
     * The dude is a unit square scaled so that 1 pixel = 1 Box2d unit
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
     * Initializes a new dude at the given position.
     *
     * The dude is unit square scaled so that 1 pixel = 1 Box2d unit
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
     * Initializes a new dude at the given position.
     *
     * The dude has the given size, scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The size of the dude in world units
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const cugl::Vec2 pos, float radius) override {
        return init(pos, radius, 1.0f);
    }
    
    /**
     * Initializes a new dude at the given position.
     *
     * The dude is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The size of the dude in world units
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
    void setTextures(const std::shared_ptr<cugl::Texture>& lumia, Vec2 initPos);

    
#pragma mark -
#pragma mark Static Constructors
	/**
	 * Creates a new dude at the origin.
	 *
	 * The dude is a unit square scaled so that 1 pixel = 1 Box2d unit
	 *
	 * The scene graph is completely decoupled from the physics system.
	 * The node does not have to be the same size as the physics body. We
	 * only guarantee that the scene graph node is positioned correctly
	 * according to the drawing scale.
	 *
	 * @return  A newly allocated DudeModel at the origin
	 */
	static std::shared_ptr<LumiaModel> alloc() {
		std::shared_ptr<LumiaModel> result = std::make_shared<LumiaModel>();
		return (result->init() ? result : nullptr);
	}

	/**
	 * Creates a new dude at the given position.
	 *
	 * The dude is a unit square scaled so that 1 pixel = 1 Box2d unit
	 *
	 * The scene graph is completely decoupled from the physics system.
	 * The node does not have to be the same size as the physics body. We
	 * only guarantee that the scene graph node is positioned correctly
	 * according to the drawing scale.
	 *
     * @param pos   Initial position in world coordinates
	 *
	 * @return  A newly allocated DudeModel at the given position
	 */
	static std::shared_ptr<LumiaModel> alloc(const cugl::Vec2& pos) {
		std::shared_ptr<LumiaModel> result = std::make_shared<LumiaModel>();
		return (result->init(pos) ? result : nullptr);
	}

    /**
	 * Creates a new dude at the given position.
	 *
     * The dude has the given size, scaled so that 1 pixel = 1 Box2d unit
	 *
 	 * The scene graph is completely decoupled from the physics system.
	 * The node does not have to be the same size as the physics body. We
	 * only guarantee that the scene graph node is positioned correctly
	 * according to the drawing scale.
	 *
	 * @param pos   Initial position in world coordinates
     * @param size  The size of the dude in world units
	 *
	 * @return  A newly allocated DudeModel at the given position with the given scale
	 */
	static std::shared_ptr<LumiaModel> alloc(const cugl::Vec2& pos, float radius) {
		std::shared_ptr<LumiaModel> result = std::make_shared<LumiaModel>();
		return (result->init(pos, radius) ? result : nullptr);
	}

	/**
	 * Creates a new dude at the given position.
	 *
	 * The dude is sized according to the given drawing scale.
	 *
	 * The scene graph is completely decoupled from the physics system.
	 * The node does not have to be the same size as the physics body. We
	 * only guarantee that the scene graph node is positioned correctly
	 * according to the drawing scale.
	 *
	 * @param pos   Initial position in world coordinates
     * @param size  The size of the dude in world units
	 * @param scale The drawing scale (world to screen)
	 *
	 * @return  A newly allocated DudeModel at the given position with the given scale
	 */
	static std::shared_ptr<LumiaModel> alloc(const cugl::Vec2& pos, float radius, float scale) {
		std::shared_ptr<LumiaModel> result = std::make_shared<LumiaModel>();
		return (result->init(pos, radius, scale) ? result : nullptr);
	}
    

#pragma mark -
#pragma mark Animation
    /**
     * Returns the scene graph node representing this DudeModel.
     *
     * By storing a reference to the scene graph node, the model can update
     * the node to be in sync with the physics info. It does this via the
     * {@link Obstacle#update(float)} method.
     *
     * @return the scene graph node representing this DudeModel.
     */
	const std::shared_ptr<cugl::scene2::SceneNode>& getSceneNode() const { return _sceneNode; }
    
    
    const std::shared_ptr<cugl::scene2::SceneNode>& getNode() const { return _node; }

    /**
     * Sets the scene graph node representing this DudeModel.
     *
     * Note that this method also handles creating the nodes for the body parts
     * of this DudeModel. Since the obstacles are decoupled from the scene graph,
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
     * @param node  The scene graph node representing this DudeModel, which has been added to the world node already.
     */
	void setSceneNode(const std::shared_ptr<cugl::scene2::SceneNode>& node) {
        _sceneNode = node;
        _sceneNode->setPosition(getPosition() * _drawScale);
    }
    
    void split();
    
    void merge(float addRadius);

    
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

    /**
     * Returns current position of Lumia (used for trajectory prediction).
     *
     * @return position of Lumia.
     */
    b2Vec2 getPos() const { return _body->GetPosition(); }
    
    /**
     * Sets velocity of Lumia.
     *
     * This is the result of drag-and-release input.
     *
     * @param value velocity of Lumia.
     */
    void setVelocity(cugl::Vec2 value) { _velocity = value; }

    /**
     * Sets planned velocity of Lumia.
     *
     * This is what the player is currently aiming Lumia towards before the launch.
     *
     * @param value planned velocity of Lumia.
     */
    void setPlannedVelocity(cugl::Vec2 value) { _plannedVelocity = value; }
    
    /**
     * Returns true if the dude is actively firing.
     *
     * @return true if the dude is actively firing.
     */
    bool isShooting() const { return _isShooting && _shootCooldown <= 0; }
    
    /**
     * Sets whether the dude is actively firing.
     *
     * @param value whether the dude is actively firing.
     */
    void setShooting(bool value) { _isShooting = value; }
    
    /**
     * Returns true if the dude is actively jumping.
     *
     * @return true if the dude is actively jumping.
     */
    bool isJumping() const { return _isJumping && _jumpCooldown <= 0; }

    /**
     * Returns true if the dude is actively launching.
     *
     * @return true if the dude is actively launching.
     */
    bool isLaunching() const { return _isLaunching; }
    
    void setSplitForce(Vec2 sForce){ _splitForce = sForce;}
    /**
     * Sets whether the dude is actively jumping.
     *
     * @param value whether the dude is actively jumping.
     */
    void setJumping(bool value) { _isJumping = value; }

    /**
     * Sets whether the dude is actively launching.
     *
     * @param value whether the dude is actively launching.
     */
    void setLaunching(bool value) { _isLaunching = value; }
    
    /**
     * Sets whether the dude is actively jumping.
     *
     * @param value whether the dude is actively jumping.
     */
    void setSplitting(bool value) { _isSplitting = value; }
    
    bool isSplitting() const {return _isSplitting;}
    /**
     * Sets whether the dude is actively jumping.
     *
     * @param value whether the dude is actively jumping.
     */
    void setMerging(bool value) { _isMerging = value; }
    
    bool isMerging() const {return _isMerging;}
    /**
     * Returns true if the dude is on the ground.
     *
     * @return true if the dude is on the ground.
     */
    bool isGrounded() const { return _isGrounded; }
    
    /**
     * Sets whether the dude is on the ground.
     *
     * @param value whether the dude is on the ground.
     */
    void setGrounded(bool value) { _isGrounded = value; }
    
    /**
     * Returns how much force to apply to get the dude moving
     *
     * Multiply this by the input to get the movement value.
     *
     * @return how much force to apply to get the dude moving
     */
    float getForce() const { return DUDE_FORCE; }
    
    /**
     * Returns ow hard the brakes are applied to get a dude to stop moving
     *
     * @return ow hard the brakes are applied to get a dude to stop moving
     */
    float getDamping() const { return DUDE_DAMPING; }
    
    /**
     * Returns the upper limit on dude left-right movement.
     *
     * This does NOT apply to vertical movement.
     *
     * @return the upper limit on dude left-right movement.
     */
    float getMaxSpeed() const { return DUDE_MAXSPEED; }
    
    /**
     * Returns the name of the ground sensor
     *
     * This is used by ContactListener
     *
     * @return the name of the ground sensor
     */
    std::string* getSensorName() { return &_sensorName; }
    
    /**
     * Returns true if this character is facing right
     *
     * @return true if this character is facing right
     */
    bool isFacingRight() const { return _faceRight; }

    
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
     * Applies the force to the body of this dude
     *
     * This method should be called after the force attribute is set.
     */
    void applyForce();
};

#endif /* __PF_DUDE_MODEL_H__ */
