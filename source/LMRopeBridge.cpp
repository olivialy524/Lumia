//
//  PFRopeBridge.cpp
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
#include "LMRopeBridge.h"

#include <cugl/cugl.h>
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/b2World.h>

using namespace cugl;

#pragma mark -
#pragma mark Physics Constants
/** The radius of each anchor pin */
#define BRIDGE_PIN_RADIUS   0.1f
/** The density of each plank in the bridge */
#define BASIC_DENSITY       1.0f

#pragma mark -
#pragma mark Contructors
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
bool RopeBridge::init(const cugl::Vec2& start, const cugl::Vec2& end, const cugl::Size& psize, float scale) {
    if (!Obstacle::init(start)) {
        return false;
    }

    setName(std::string(BRIDGE_NAME));
    _drawScale = scale;

	_linksize = psize.width;
	float spacing;

	// Compute the bridge length
	Vec2 dimen = end-start;
	float length = dimen.length();
	Vec2 norm = dimen;
	norm.normalize();

	// If too small, only make one plank.
	int nLinks = (int)(length / _linksize);
	if (nLinks <= 1) {
		nLinks = 1;
		_linksize = length;
		spacing = 0;
	} else {
		spacing = length - nLinks * _linksize;
		spacing /= (nLinks-1);
	}

	// Create the first pin
	std::shared_ptr<physics2::Obstacle> body = physics2::WheelObstacle::alloc(start,BRIDGE_PIN_RADIUS);
	body->setName(BRIDGE_PIN_NAME+strtool::to_string(0));
	body->setDensity(BASIC_DENSITY);
	body->setBodyType(b2_staticBody);
	_bodies.push_back(body);

	// Create the planks
    Size planksize = psize;
	planksize.width = _linksize;
	for (int ii = 0; ii < nLinks; ii++) {
		float t = ii*(_linksize+spacing) + _linksize/2.0f;
		Vec2 pos = norm*t+start;
		std::shared_ptr<physics2::BoxObstacle> plank = physics2::BoxObstacle::alloc(pos, planksize);
		body->setName(PLANK_NAME+strtool::to_string(ii));
		plank->setDensity(BASIC_DENSITY);
		_bodies.push_back(plank);
	}

	// Create the last pin
	body = physics2::WheelObstacle::alloc(end,BRIDGE_PIN_RADIUS);
	body->setName(BRIDGE_PIN_NAME+strtool::to_string(1));
	body->setDensity(BASIC_DENSITY);
	body->setBodyType(b2_staticBody);
	_bodies.push_back(body);

	return true;
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
bool RopeBridge::createJoints(b2World& world) {
	b2Vec2 anchor1(0,0);
	b2Vec2 anchor2(-_linksize/2.0f,0);

	// Definition for a revolute joint
	b2RevoluteJointDef jointDef;
	b2Joint* joint;

	jointDef.bodyA = _bodies[0]->getBody();
	jointDef.bodyB = _bodies[1]->getBody();
	jointDef.localAnchorA = anchor1;
	jointDef.localAnchorB = anchor2;
	jointDef.collideConnected = false;
	joint = world.CreateJoint(&jointDef);
	_joints.push_back(joint);

	// Planks together
	anchor1.x = _linksize / 2;
	for (int ii = 1; ii < _bodies.size()-2; ii++) {
		jointDef.bodyA = _bodies[ii]->getBody();
		jointDef.bodyB = _bodies[ii+1]->getBody();
		jointDef.localAnchorA = anchor1;
		jointDef.localAnchorB = anchor2;
		jointDef.collideConnected = false;
		joint = world.CreateJoint(&jointDef);
		_joints.push_back(joint);
	}

	// Final joint
	anchor2.x = 0;
	int ii = (int)_bodies.size();
	jointDef.bodyA = _bodies[ii-2]->getBody();
	jointDef.bodyB = _bodies[ii-1]->getBody();
	jointDef.localAnchorA = anchor1;
	jointDef.localAnchorB = anchor2;
	jointDef.collideConnected = false;
	joint = world.CreateJoint(&jointDef);
	_joints.push_back(joint);

	return true;
}

/**
 * Disposes all resources and assets of this RopeBridge
 *
 * Any assets owned by this object will be immediately released.  Once
 * disposed, a RopeBridge may not be used until it is initialized again.
 */
void RopeBridge::dispose() {
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
void RopeBridge::setTexture(const std::shared_ptr<cugl::Texture>& texture) {
    _image = texture;
    if (_node == nullptr) {
        return;
    }
    
    // Do not add nodes for the beginning and end bodies
    _node->removeAllChildren();
    for(int ii = 1; ii+1 < _bodies.size(); ii++) {
        std::shared_ptr<scene2::PolygonNode> sprite = scene2::PolygonNode::allocWithTexture(_image);
        sprite->setPosition(_bodies[ii]->getPosition() * _drawScale);
        _node->addChild(sprite);
    }
}


/**
 * Performs any necessary additions to the scene graph node.
 *
 * This method is necessary for custom physics objects that are composed
 * of multiple scene graph nodes.
 */
void RopeBridge::setSceneNode(const std::shared_ptr<cugl::scene2::SceneNode>& node) {
	_node = node;
    if (_image == nullptr) {
        return;
    }

	// Do not add nodes for the beginning and end bodies
	for(int ii = 1; ii+1 < _bodies.size(); ii++) {
		std::shared_ptr<scene2::PolygonNode> sprite = scene2::PolygonNode::allocWithTexture(_image);
		sprite->setPosition(_bodies[ii]->getPosition() * _drawScale);
		_node->addChild(sprite);
	}
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
void RopeBridge::update(float delta) {
	Obstacle::update(delta);
	if (_node != nullptr) {
		std::vector<std::shared_ptr<scene2::SceneNode>> children = _node->getChildren();
		//begin at index 1 since body 0 does not correspond to a node
		//given how we populate the children in setSceneNode
		int i = 1;

		// Update the nodes of the attached bodies
		for (auto it = children.begin(); it != children.end(); ++it) {
			(*it)->setPosition(_bodies[i]->getPosition()*_drawScale);
			(*it)->setAngle(_bodies[i]->getAngle());

			// Propagate the update to the bodies attached to the RopeBridge
			_bodies[i]->update(delta);
			i++;
		}
	}
}
