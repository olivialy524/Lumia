//
//  PFSpinner.h
//  PlatformerDemo
//
//  This class provides a spinning rectangle on a fixed pin, as ComplexObstacles
//  always make joint management easier.
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
#ifndef __LM_SPINNER_H__
#define __LM_SPINNER_H__
#include <cugl/cugl.h>
#include <cugl/physics2/CUComplexObstacle.h>

using namespace cugl;

#pragma mark -
#pragma mark Spinner Constants
/** The key for the spinner texture */
#define SPINNER_TEXTURE "spinner"
/** The debug name for the entire obstacle */
#define SPINNER_NAME    "spinner"
/** The debug name for the spinning barrier */
#define BARRIER_NAME    "barrier"
/** The debug name for the central pin */
#define SPIN_PIN_NAME   "pin"


#pragma mark -
#pragma mark Spinner Obstacle
/**
* Spinning platform for the plaform game.
*
* We did not really need a separate class for this, as it has no update.  However, 
* ComplexObstacles always make joint management easier.
*/
class Spinner : public cugl::physics2::ComplexObstacle {
private:
	/** The scene graph node for the Spinner. */
	std::shared_ptr<cugl::scene2::SceneNode> _node;
	/** The scale between the physics world and the screen (MUST BE UNIFORM) */
	float _drawScale;
    /** The image used to draw the Spinner */
	std::shared_ptr<Texture> _image;

public:
#pragma mark Constructors
    /**
     * Creates a degenerate spinner object.
     *
     * This constructor does not initialize any of the spinner values beyond
     * the defaults.  To use a Spinner, you must call init().
     */
    Spinner() : ComplexObstacle() { }
    
    /**
     * Destroys this Spinner, releasing all resources.
     */
    virtual ~Spinner(void) { dispose(); }
    
    /**
     * Disposes all resources and assets of this Spinner
     *
     * Any assets owned by this object will be immediately released.  Once
     * disposed, a Spinner may not be used until it is initialized again.
     */
    void dispose();
    
    /**
     * Initializes a new spinner at the origin.
     *
     * The spinner is a unit square, scaled so that 1 pixel = 1 Box2d unit.
     * The spinner is centered on its position.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init() override { return init(cugl::Vec2::ZERO, cugl::Size(1,1), 1.0f); }
    
    /**
     * Initializes a new spinner at the given position.
     *
     * The spinner is a unit square, scaled so that 1 pixel = 1 Box2d unit.
     * The spinner is centered on its position.
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
    virtual bool init(const cugl::Vec2 pos) override { return init(pos, cugl::Size(1,1), 1.0f); }
    
    /**
     * Initializes a new spinner at the given position.
     *
     * The spinner has the given size, scaled so that 1 pixel = 1 Box2d unit.
     * The spinner is centered on its position.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The spinner size in world units
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const cugl::Vec2 pos, const cugl::Vec2 size) { return init(pos, size, 1.0f); }
    
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
    virtual bool init(const cugl::Vec2 pos, const cugl::Vec2 size, float scale);

    
#pragma mark -
#pragma mark Static Constructors
	/**
	 * Creates a new spinner at the origin.
	 *
     * The spinner is a unit square, scaled so that 1 pixel = 1 Box2d unit.
     * The spinner is centered on its position.
	 *
	 * The scene graph is completely decoupled from the physics system.
	 * The node does not have to be the same size as the physics body. We
	 * only guarantee that the scene graph node is positioned correctly
	 * according to the drawing scale.
	 *
	 * @return  A newly allocated Spinner at the origin
	 */
	static std::shared_ptr<Spinner> alloc() {
		std::shared_ptr<Spinner> result = std::make_shared<Spinner>();
		return (result->init() ? result : nullptr);
	}

    /**
     * Creates a new spinner at the given position.
     *
     * The spinner is a unit square, scaled so that 1 pixel = 1 Box2d unit.
     * The spinner is centered on its position.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     *
     * @return  A newly allocated spinner at the given position
     */
    static std::shared_ptr<Spinner> alloc(const cugl::Vec2& pos) {
        std::shared_ptr<Spinner> result = std::make_shared<Spinner>();
        return (result->init(pos) ? result : nullptr);
    }
    
    /**
     * Creates a new spinner at the given position.
     *
     * The spinner has the given size, scaled so that 1 pixel = 1 Box2d unit.
     * The spinner is centered on its position.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The spinner size in world units
     *
     * @return  A newly allocated spinner at the given position, with the given size.
     */
    static std::shared_ptr<Spinner> alloc(const cugl::Vec2& pos, const cugl::Size& size) {
        std::shared_ptr<Spinner> result = std::make_shared<Spinner>();
        return (result->init(pos, size) ? result : nullptr);
    }
    
    /**
     * Creates a new spinner at the given position.
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
     * @return  A newly allocated spinner at the given position, with the given drawing scale.
     */
    static std::shared_ptr<Spinner> alloc(const cugl::Vec2& pos, const cugl::Size& size, float scale) {
        std::shared_ptr<Spinner> result = std::make_shared<Spinner>();
        return (result->init(pos, size, scale) ? result : nullptr);
    }


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
	 * Returns the scene graph node representing this Spinner.
	 *
	 * By storing a reference to the scene graph node, the model can update
	 * the node to be in sync with the physics info. It does this via the
	 * {@link Obstacle#update(float)} method.
	 *
	 * @return the scene graph node representing this Spinner.
	 */
	const std::shared_ptr<cugl::scene2::SceneNode>& getSceneNode() const { return _node; }

    /**
     * Sets the scene graph node representing this Spinner.
     *
     * Note that this method also handles creating the nodes for the body parts
     * of this Spinner. Since the obstacles are decoupled from the scene graph,
     * initialization (which creates the obstacles) occurs prior to the call to
     * this method. Therefore, to be drawn to the screen, the nodes of the attached
     * bodies must be added here.
     *
     * By storing a reference to the scene graph node, the model can update
     * the node to be in sync with the physics info. It does this via the
     * {@link Obstacle#update(float)} method.
     *
     * @param node  The scene graph node representing this Spinner, which has 
     *              been added to the world node already.
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

#endif /* __PF_SPINNER_H__ */
