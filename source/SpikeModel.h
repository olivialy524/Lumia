//
//  LMPlant.h
//  Lumia
//
//  Created by Ben Dacek on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef SpikeModel_h
#define SpikeModel_h
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include <cugl/scene2/graph/CUWireNode.h>
#include "SpikeNode.h"

#pragma mark -
#pragma mark Spike Model
class SpikeModel : public cugl::physics2::BoxObstacle {
protected:
    std::shared_ptr<SpikeNode> _spikeNode;
    std::shared_ptr<cugl::scene2::SceneNode> _node;
    /** The scale between the physics world and the screen (MUST BE UNIFORM) */
    float _drawScale;

public:
#pragma mark Constructors
    /**
     * Creates a degenerate Bullet object.
     *
     * This constructor does not initialize any of the Bullet values beyond
     * the defaults.  To use a Bullet, you must call init().
     */
    SpikeModel() : BoxObstacle() { }
    
    /**
     * Destroys this Bullet, releasing all resources.
     */
    virtual ~SpikeModel(void) { dispose(); }
    
    /**
     * Disposes all resources and assets of this Bullet
     *
     * Any assets owned by this object will be immediately released.  Once
     * disposed, a Bullet may not be used until it is initialized again.
     */
    void dispose();
    
#pragma mark -
#pragma mark Static Constructors
    /**
     * Creates a new Bullet at the given position.
     *
     * The bullet is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  radius   The radius of the Bullet obstacle.
     *
     * @return  A newly allocated Bullet at the given position, with the given radius
     */
    static std::shared_ptr<SpikeModel> alloc(const cugl::Vec2& pos, cugl::Size size) {
        std::shared_ptr<SpikeModel> result = std::make_shared<SpikeModel>();
        return (result->init(pos, size) ? result : nullptr);
    }

    static std::shared_ptr<SpikeModel> alloc(const cugl::Vec2& pos) {
        std::shared_ptr<SpikeModel> result = std::make_shared<SpikeModel>();
        return (result->init(pos) ? result : nullptr);
    }

#pragma mark -
#pragma mark Animation
    /**
     * Returns the scene graph node representing this Bullet.
     *
     * By storing a reference to the scene graph node, the model can update
     * the node to be in sync with the physics info. It does this via the
     * {@link Obstacle#update(float)} method.
     *
     * @return the scene graph node representing this Bullet.
     */
    const std::shared_ptr<cugl::scene2::SceneNode>& getNode() const { return _node; }
    
    void setSpikeNode(const std::shared_ptr<SpikeNode>& node) {
        _spikeNode = node;
        _node->addChild(_spikeNode);
    }
    
    void setNode(const std::shared_ptr<cugl::scene2::SceneNode>& node) {
        _node = node;
    }
    
    /**
     * Sets the textures for this lumia.
     *
     * @param lumia      The texture for the lumia filmstrip
     */
    void setTextures(const std::shared_ptr<cugl::Texture>& spike, float angle);

    
    /**
     * Sets the ratio of the Bullet sprite to the physics body
     *
     * The Bullet needs this value to convert correctly between the physics
     * coordinates and the drawing screen coordinates.  Otherwise it will
     * interpret one Box2D unit as one pixel.
     *
     * All physics scaling must be uniform.  Rotation does weird things when
     * attempting to scale physics by a non-uniform factor.
     *
     * @param scale The ratio of the Bullet sprite to the physics body
     */
    void setDrawScale(float scale) {
        _drawScale = scale;
    }
    /**
     * Updates the object's physics state (NOT GAME LOGIC).
     *
     * We use this method to reset cooldowns.
     *
     * @param delta Number of seconds since last animation frame
     */
    void update(float dt) override;
};

#endif /* __SPIKE_MODEL_H__ */
