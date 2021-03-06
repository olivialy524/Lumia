//
//  PFSpinner.cpp
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
#include "LMSpinner.h"
#include <cugl/cugl.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/b2World.h>

#pragma mark -
#pragma mark Physics Constants

/** The radius of the central pin */
#define SPIN_PIN_RADIUS 0.1f
/** The density for the spinning barrier */
#define HEAVY_DENSITY  10.0f
/** The density for the central pin */
#define LIGHT_DENSITY   1.0f
/** The dimensions of the spinner */

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
bool Spinner::init(const cugl::Vec2 pos, const cugl::Vec2 size, float scale) {
    if (!Obstacle::init(pos)) {
        return false;
    }

    setName(std::string(SPINNER_NAME));
	_drawScale = scale;
    
	// Create the barrier
	std::shared_ptr<physics2::Obstacle> body = physics2::BoxObstacle::alloc(pos,size);
	body->setName(BARRIER_NAME);
	body->setDensity(HEAVY_DENSITY);
	_bodies.push_back(body);

	// Create the pin
	body = physics2::WheelObstacle::alloc(pos,SPIN_PIN_RADIUS);
	body->setName(SPIN_PIN_NAME);
	body->setDensity(LIGHT_DENSITY);
	body->setBodyType(b2_staticBody);
	_bodies.push_back(body);

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
bool Spinner::createJoints(b2World& world) {
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = _bodies[0]->getBody();
	jointDef.bodyB = _bodies[1]->getBody();
	jointDef.localAnchorA.Set(0,0);
	jointDef.localAnchorB.Set(0,0);
	b2Joint* joint = world.CreateJoint(&jointDef);
	_joints.push_back(joint);

	return true;
}

/**
 * Disposes all resources and assets of this Spinner
 *
 * Any assets owned by this object will be immediately released.  Once
 * disposed, a Spinner may not be used until it is initialized again.
 */
void Spinner::dispose() {
	_node = nullptr;
	_image = nullptr;
	_bodies.clear();
}


#pragma mark -
#pragma mark Scene Graph Internals
/**
 * Sets the plank texture
 *
 * This texture is used to construct the individual scene graph nodes
 * for each plank
 *
 * @param texture   The plank texture
 */
void Spinner::setTexture(const std::shared_ptr<cugl::Texture>& texture) {
    _image = texture;
    if (_node != nullptr) {
        std::shared_ptr<scene2::PolygonNode> sprite = scene2::PolygonNode::allocWithTexture(_image);
        _node->addChild(sprite);
    }
}

/**
 * Performs any necessary additions to the scene graph node.
 *
 * This method is necessary for custom physics objects that are composed
 * of multiple scene graph nodes.
 */
void Spinner::setSceneNode(const std::shared_ptr<cugl::scene2::SceneNode>& node) {
	_node = node;
    if (_image != nullptr) {
        std::shared_ptr<scene2::PolygonNode> sprite = scene2::PolygonNode::allocWithTexture(_image);
        _node->addChild(sprite);
    }
	// Spinner is invisible
}

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
void Spinner::update(float delta) {
	Obstacle::update(delta);
	if (_node != nullptr) {
		std::vector<std::shared_ptr<scene2::SceneNode>> children = _node->getChildren();
		int i = 0;

		// Update the nodes of the attached bodies
		for (auto it = children.begin(); it != children.end(); ++it) {
			(*it)->setPosition(_bodies[i]->getPosition()*_drawScale);
			(*it)->setAngle(_bodies[i]->getAngle());

			// Propagate the update to the bodies attached to the Spinner
			_bodies[i]->update(delta);
			i++;
		}
	}
}
