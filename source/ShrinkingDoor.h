//
//  ShrinkingDoor.h
//  Lumia
//
//  Created by Olivia Li on 5/18/21.
//  Copyright © 2021 Cornell Game Design Initiative. All rights reserved.
//

#ifndef ShrinkingDoor_h
#define ShrinkingDoor_h
#include <cugl/cugl.h>
#include <cugl/physics2/CUComplexObstacle.h>
#include "ShrinkingDoorNode.h"
using namespace cugl;

class ShrinkingDoor : public cugl::physics2::ComplexObstacle {
private:
    bool _opening;
    bool _closing;
    std::shared_ptr<ShrinkingDoorNode> _sceneNode;
    std::shared_ptr<physics2::BoxObstacle> _door;
    float _drawScale;
    float _normHeight;
    Vec2 _rotatedOffset;
    
public:
#pragma mark Constructors
    /**
     * Creates a degenerate spinner object.
     *
     * This constructor does not initialize any of the spinner values beyond
     * the defaults.  To use a Spinner, you must call init().
     */
    ShrinkingDoor() : ComplexObstacle() { }
    
    /**
     * Destroys this Spinner, releasing all resources.
     */
    virtual ~ShrinkingDoor(void) { dispose(); }
    
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
    virtual bool init() override { return init(cugl::Vec2::ZERO, cugl::Size(1,1),0.0f); }
    
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
    virtual bool init(const cugl::Vec2 pos) override { return init(pos, cugl::Size(1,1),0.0f); }
    
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
    virtual bool init(const cugl::Vec2 pos, const cugl::Vec2 size, float angle);

    
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
    static std::shared_ptr<ShrinkingDoor> alloc() {
        std::shared_ptr<ShrinkingDoor> result = std::make_shared<ShrinkingDoor>();
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
    static std::shared_ptr<ShrinkingDoor> alloc(const cugl::Vec2& pos) {
        std::shared_ptr<ShrinkingDoor> result = std::make_shared<ShrinkingDoor>();
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
    static std::shared_ptr<ShrinkingDoor> alloc(const cugl::Vec2& pos, const cugl::Size& size, float angle) {
        std::shared_ptr<ShrinkingDoor> result = std::make_shared<ShrinkingDoor>();
        return (result->init(pos, size, angle) ? result : nullptr);
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
     * Returns the scene graph node representing this Spinner.
     *
     * By storing a reference to the scene graph node, the model can update
     * the node to be in sync with the physics info. It does this via the
     * {@link Obstacle#update(float)} method.
     *
     * @return the scene graph node representing this Spinner.
     */
    const std::shared_ptr<ShrinkingDoorNode>& getSceneNode() const { return _sceneNode; }
    
    void setTextures(const std::shared_ptr<cugl::Texture>& door);

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

    void Open();

    void Close();

#pragma mark -
#pragma mark Attributes
    bool getOpening() {
        return _opening;
    }
    
    void setOpening(bool open) {
        _opening = open;
    }
    
    bool getClosing() {
        return _closing;
    }
    
    void setClosing(bool close) {
        _closing = close;
    }
    
    void setDrawScale(float value){
        _drawScale = value;
    }
    
    bool getOpened(){
        if (_sceneNode != nullptr){
            return _sceneNode->getAnimState() == ShrinkingDoorNode::ShrinkingDoorAnimState::Open;
        }
        return false;
    }
    
    bool getClosed(){
        if (_sceneNode != nullptr){
            return _sceneNode->getAnimState() == ShrinkingDoorNode::ShrinkingDoorAnimState::Closed;
        }
        return false;
        
    }
    
    float getNormHeight(){
        return _normHeight;
    }
 };



#endif /* ShrinkingDoor_h */
