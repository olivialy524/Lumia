//
//  LMPlant.h
//  Lumia
//
//  Created by Ben Dacek on 3/9/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef LMPlant_h
#define LMPlant_h
#include <cugl/cugl.h>
#include <cugl/physics2/CUBoxObstacle.h>
#include <cugl/scene2/graph/CUWireNode.h>

#pragma mark -
#pragma mark Plant Model
class Plant : public cugl::physics2::WheelObstacle {
protected:
    /** The scene graph node for the Bullet. */
    std::shared_ptr<cugl::scene2::SceneNode> _node;
    /** The scale between the physics world and the screen (MUST BE UNIFORM) */
    float _drawScale;
    bool _isLit;

public:
#pragma mark Constructors
    /**
     * Creates a degenerate Bullet object.
     *
     * This constructor does not initialize any of the Bullet values beyond
     * the defaults.  To use a Bullet, you must call init().
     */
    Plant() : WheelObstacle() { }
    
    /**
     * Destroys this Bullet, releasing all resources.
     */
    virtual ~Plant(void) { dispose(); }
    
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
    static std::shared_ptr<Plant> alloc(const cugl::Vec2& pos, float radius) {
        std::shared_ptr<Plant> result = std::make_shared<Plant>();
        return (result->init(pos, radius) ? result : nullptr);
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
    const std::shared_ptr<cugl::scene2::SceneNode>& getSceneNode() const { return _node; }
    
    /**
     * Sets the scene graph node representing this Bullet.
     *
     * @param node  The scene graph node representing this Bullet, which has
     *              been added to the world node already.
     */
    void setSceneNode(const std::shared_ptr<cugl::scene2::SceneNode>& node) {
        _node = node;
    }
    
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

#pragma mark -
#pragma mark Physics Methods

    bool getIsLit() {
        return _isLit;
    }
    
    void lightUp() {
        _isLit = true;
    }
};
#endif /* LMPlant_h */
