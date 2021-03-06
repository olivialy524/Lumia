//
//  PFRopeBridge.h
//  PlatformerDemo
//
//  This module provides the connected ropebridge from 3152. This time it is fully
//  assembled for you.  Note that this module handles its own scene graph management.
//  As a ComplexObstacle owns all of its child obstacles, it is natural for it to
//  own the corresponding scene graph.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt well
//  to data driven design.  This demo has a lot of simplifications to make it a bit
//  easier to see how everything fits together.  However, the model classes and how
//  they are initialized will need to be changed if you add dynamic level loading.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White and Anthony Perello
//  Version:  2/9/17
//
#ifndef __LM_ROPE_BRIDGE_H__
#define __LM_ROPE_BRIDGE_H__
#include <cugl/cugl.h>
#include <cugl/physics2/CUComplexObstacle.h>


using namespace cugl;

#pragma mark -
#pragma mark Bridge Constants
/** The key for the texture for the bridge planks */
#define BRIDGE_TEXTURE "bridge"
/** The debug name for the entire obstacle */
#define BRIDGE_NAME     "bridge"
/** The debug name for each plank */
#define PLANK_NAME      "barrier"
/** The debug name for each anchor pin */
#define BRIDGE_PIN_NAME "pin"


#pragma mark -
#pragma mark Rope Bridge
/**
* A bridge with planks connected by revolute joints.
*
* Note that this module handles its own scene graph management.  As a ComplexObstacle
* owns all of its child obstacles, it is natural for it to own the corresponding
* scene graph.
*/
class RopeBridge : public cugl::physics2::ComplexObstacle {
protected:
	/** The width of a single plank in the bridge */
	float _linksize;
	/** A parent scene graph node for the bridge. */
	std::shared_ptr<cugl::scene2::SceneNode> _node;
	/** The scale between the physics world and the screen (MUST BE UNIFORM) */
	float _drawScale;
    /** The image that is used for drawing links in the bridge */
	std::shared_ptr<Texture> _image;

    
public:
#pragma mark Constructors
    /**
     * Creates a degenerate rope bridge.
     *
     * This constructor does not initialize any of the RopeBridge values beyond
     * the defaults.  To use a rope bridge, you must call init().
     */
    RopeBridge() : ComplexObstacle() { }
    
    /**
     * Destroys this RopeBridge, releasing all resources.
     */
    virtual ~RopeBridge(void) { dispose(); }
    
    /**
     * Disposes all resources and assets of this RopeBridge
     *
     * Any assets owned by this object will be immediately released.  Once
     * disposed, a RopeBridge may not be used until it is initialized again.
     */
    void dispose();
    
    /**
     * Initializes a new RopeBridge at the origin.
     *
     * The RopeBridge is scaled so that 1 pixel = 1 Box2d unit. The rope bridge
     * has a single plank and is one unit long.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init() override { return init(cugl::Vec2::ZERO, cugl::Vec2::ONE); }
    
    /**
     * Initializes a new RopeBridge with the given start and end positions
     *
     * The RopeBridge is scaled so that 1 pixel = 1 Box2d unit.  The rope bridge
     * has a single plank.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param start The starting position of the bridge, in world coordinates
     * @param end   The ending position of the bridge, in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const cugl::Vec2& start, const cugl::Vec2& end) {
        return init(start, end, end-start, 1.0f);
    }
    
    /**
     * Initializes a new RopeBridge with the given start and end positions
     *
     * The RopeBridge is scaled so that 1 pixel = 1 Box2d unit.  The number of
     * planks in the bridge is determined by psize.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param start The starting position of the bridge, in world coordinates
     * @param end   The ending position of the bridge, in world coordinates
     * @param psize The size of a single plank, in world units
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const cugl::Vec2& start, const cugl::Vec2& end, const cugl::Size& psize) {
        return init(start, end, psize, 1.0f);
    }
    
    /**
     * Initializes a new RopeBridge with the given start and end positions, and drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param start The starting position of the bridge, in world coordinates
     * @param end   The ending position of the bridge, in world coordinates
     * @param psize The size of a single plank, in world units
     * @param scale The drawing scale (world to screen)
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const cugl::Vec2& start, const cugl::Vec2& end, const cugl::Size& psize, float scale);

    
#pragma mark -
#pragma mark Static Constructors
	/**
     * Creates a new RopeBridge at the origin.
     *
     * The RopeBridge is scaled so that 1 pixel = 1 Box2d unit. The rope bridge
     * has a single plank and is one unit long.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  A newly allocated RopeBridge at the origin
     */
	static std::shared_ptr<RopeBridge> alloc() {
		std::shared_ptr<RopeBridge> result = std::make_shared<RopeBridge>();
		return (result->init() ? result : nullptr);
	}

	/**
     * Creates a new RopeBridge with the given start and end positions
     *
     * The RopeBridge is scaled so that 1 pixel = 1 Box2d unit. The rope bridge
     * has a single plank.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  start   The starting position of the bridge, in world coordinates
     * @param  end     The ending position of the bridge, in world coordinates
     *
     * @return  A newly allocated RopeBridge with the given start and end positions
     */
	static std::shared_ptr<RopeBridge> alloc(const cugl::Vec2& start, const cugl::Vec2& end) {
		std::shared_ptr<RopeBridge> result = std::make_shared<RopeBridge>();
		return (result->init(start, end) ? result : nullptr);
	}

	/**
     * Creates a new RopeBridge with the given start and end positions
     *
     * The RopeBridge is scaled so that 1 pixel = 1 Box2d unit.  The number of
     * planks in the bridge is determined by psize.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param start The starting position of the bridge, in world coordinates
     * @param end   The ending position of the bridge, in world coordinates
     * @param psize The size of a single plank, in world units
     *
     * @return  A newly allocated RopeBridge with the given start and end positions and drawing scale
     */
	static std::shared_ptr<RopeBridge> alloc(const cugl::Vec2& start, const cugl::Vec2& end, const cugl::Size& psize) {
		std::shared_ptr<RopeBridge> result = std::make_shared<RopeBridge>();
		return (result->init(start, end, psize) ? result : nullptr);
	}

    /**
     * Creates a new RopeBridge with the given start and end positions, and drawing scale.
     *
     * The RopeBridge is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param start The starting position of the bridge, in world coordinates
     * @param end   The ending position of the bridge, in world coordinates
     * @param psize The size of a single plank, in world units
     * @param scale The drawing scale (world to screen)
     *
     * @return  A newly allocated RopeBridge with the given start and end positions and drawing scale
     */
    static std::shared_ptr<RopeBridge> alloc(const cugl::Vec2& start, const cugl::Vec2& end, const cugl::Size& psize, float scale) {
		std::shared_ptr<RopeBridge> result = std::make_shared<RopeBridge>();
		return (result->init(start, end, psize, scale) ? result : nullptr);
	}
    
    
#pragma mark -
#pragma mark Physics Methods
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
	bool createJoints(b2World& world) override;

    
#pragma mark -
#pragma mark Animation
    /**
     * Returns the plank texture
     * 
     * This texture is used to construct the individual scene graph nodes
     * for each plank
     *
     * @return the plank texture
     */
    const std::shared_ptr<cugl::Texture>& getTexture() const { return _image; }
    
    /**
     * Sets the plank texture
     *
     * This texture is used to construct the individual scene graph nodes
     * for each plank
     *
     * @param texture   The plank texture
     */
    void setTexture(const std::shared_ptr<cugl::Texture>& texture);
    
	/**
     * Returns the scene graph node representing this RopeBridge.
     *
     * By storing a reference to the scene graph node, the model can update
     * the node to be in sync with the physics info. It does this via the
     * {@link Obstacle#update(float)} method.
     *
     * @return the scene graph node representing this RopeBridge.
     */
	const std::shared_ptr<cugl::scene2::SceneNode>& getNode() const { return _node; }

	/**
     * Sets the scene graph node representing this RopeBridge.
     *
     * Note that this method also handles creating the nodes for the body parts
     * of this RopeBridge. Since the obstacles are decoupled from the scene graph,
     * initialization (which creates the obstacles) occurs prior to the call to
     * this method. Therefore, to be drawn to the screen, the nodes of the attached
     * bodies must be added here.
     *
     * By storing a reference to the scene graph node, the model can update
     * the node to be in sync with the physics info. It does this via the
     * {@link Obstacle#update(float)} method.
     *
     * @param node  The scene graph node representing this RopeBridge, which has been added to the world node already.
     */
	void setSceneNode(const std::shared_ptr<cugl::scene2::SceneNode>& node);

    
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
	void update(float delta) override;

};

#endif /* __PF_ROPE_BRIDGE_H__ */
