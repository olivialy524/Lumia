//
//  LMGameScene.h
//
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include <cugl/cugl.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <unordered_set>
#include <vector>
#include "InputController.h"
#include "LumiaModel.h"
#include "Plant.h"
#include "EnergyModel.h"
#include "LevelModel.h"

/**
 * This class is the primary gameplay constroller for the demo.
 *
 * A world has its own objects, assets, and input controller.  Thus this is
 * really a mini-GameEngine in its own right.  As in 3152, we separate it out
 * so that we can have a separate mode for the loading screen.
 */
class GameScene : public cugl::Scene2 {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::JsonReader> _jsonr;
    
    std::shared_ptr<cugl::JsonValue> _leveljson;
    
    std::shared_ptr<LevelModel> _level;
    // CONTROLLERS
    /** Controller for abstracting out input across multiple platforms */
    InputController _input;
    
    float _cameraTargetX;
    
    // VIEW
    /** Reference to the physics root of the scene graph */
    std::shared_ptr<cugl::scene2::SceneNode> _worldnode;
    /** Reference to the debug root of the scene graph */
    std::shared_ptr<cugl::scene2::SceneNode> _debugnode;
    /** Reference to the win message label */
    std::shared_ptr<cugl::scene2::Label> _winnode;
    /** Reference to the lose message label */
    std::shared_ptr<cugl::scene2::Label> _losenode;

    /** The Box2D world */
    std::shared_ptr<cugl::physics2::ObstacleWorld> _world;
    /** The scale between the physics world and the screen (MUST BE UNIFORM) */
    float _scale;

    // Physics objects for the game
    /** References to the magical plants */
    std::list<std::shared_ptr<Plant>> _plantList;
    /** References to the energy items */
    std::list<std::shared_ptr<EnergyModel>> _energyList;
    /** References to the Lumia bodies */
    std::list<std::shared_ptr<LumiaModel>> _lumiaList;
    /** Reference to the player avatar */
    std::shared_ptr<LumiaModel> _avatar;

    /** Information representing a Lumia to create */
    struct LumiaBody {
        /** The position to spawn the Lumia body at */
        cugl::Vec2 position;
        /** The radius of the Lumia body */
        float radius;
        /** Whether or not the Lumia body should be controlled by the player */
        bool isAvatar;
    };

    /** List of Lumia bodies to remove in next update step */
    std::list<std::shared_ptr<LumiaModel>> _lumiasToRemove;
    /** List of Lumia bodies to create in next update step */
    std::list<LumiaBody> _lumiasToCreate;
    /** List of energy item sto remove in next update step */
    std::list<std::shared_ptr<EnergyModel>> _energiesToRemove;

    /** Whether we have completed this "game" */
    bool _complete;
    /** Whether or not debug mode is active */
    bool _debug;
    /** Whether we have failed at this world (and need a reset) */
    bool _failed;
    /** Countdown active for winning or losing */
    int _countdown;
      
    /** Mark set to handle more sophisticated collision callbacks */
    std::unordered_map<LumiaModel*, std::unordered_set<b2Fixture*>> _sensorFixtureMap;

#pragma mark Internal Object Management
    /**
     * Lays out the game geography.
     *
     * Pay close attention to how we attach physics objects to a scene graph.
     * The simplest way is to make a subclass, like we do for the dude.  However,
     * for simple objects you can just use a callback function to lightly couple
     * them.  This is what we do with the crates.
     *
     * This method is really, really long.  In practice, you would replace this
     * with your serialization loader, which would process a level file.
     */
    void populate();
    
    /**
     * Adds the physics object to the physics world and loosely couples it to the scene graph
     *
     * There are two ways to link a physics object to a scene graph node on the
     * screen.  One way is to make a subclass of a physics object, like we did
     * with dude.  The other is to use callback functions to loosely couple
     * the two.  This function is an example of the latter.
     *
     * In addition, scene graph nodes have a z-order.  This is the order they are
     * drawn in the scene graph node.  Objects with the different textures should
     * have different z-orders whenever possible.  This will cut down on the 
     * amount of drawing done
     *
     * @param obj    The physics object to add
     * @param node   The scene graph node to attach it to
     * @param zOrder The drawing order
     * @param useObjPosition  Whether to update the node's position to be at the object's position
     */
    void addObstacle(const std::shared_ptr<cugl::physics2::Obstacle>& obj,
                     const std::shared_ptr<cugl::scene2::SceneNode>& node,
                     int zOrder, bool useObjPosition=true);

    /**
     * Returns the active screen size of this scene.
     *
     * This method is for graceful handling of different aspect
     * ratios
     */
    cugl::Size computeActiveSize() const;
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new game world with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    GameScene();
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~GameScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();
    
    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  This initializer uses the default scale.
     *
     * @param assets    The (loaded) assets for this game mode
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  The bounds are in terms of the Box2d
     * world, not the screen.
     *
     * @param assets    The (loaded) assets for this game mode
     * @param rect      The game bounds in Box2d coordinates
     *
     * @return  true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, const cugl::Rect& rect);
    
    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  The bounds are in terms of the Box2d
     * world, not the screen.
     *
     * @param assets    The (loaded) assets for this game mode
     * @param rect      The game bounds in Box2d coordinates
     * @param gravity   The gravitational force on this Box2d world
     *
     * @return  true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, const cugl::Rect& rect, const cugl::Vec2& gravity);
    
    
#pragma mark -
#pragma mark State Access
    /**
     * Returns true if debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @return true if debug mode is active.
     */
    bool isDebug( ) const { return _debug; }
    
    /**
     * Sets whether debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @param value whether debug mode is active.
     */
    void setDebug(bool value) { _debug = value; _debugnode->setVisible(value); }
    
    /**
     * Returns true if the level is completed.
     *
     * If true, the level will advance after a countdown
     *
     * @return true if the level is completed.
     */
    bool isComplete( ) const { return _complete; }
    
    /**
     * Sets whether the level is completed.
     *
     * If true, the level will advance after a countdown
     *
     * @param value whether the level is completed.
     */
	void setComplete(bool value);

	/**
	* Returns true if the level is failed.
	*
	* If true, the level will reset after a countdown
	*
	* @return true if the level is failed.
	*/
	bool isFailure() const { return _failed; }

	/**
	* Sets whether the level is failed.
	*
	* If true, the level will reset after a countdown
	*
	* @param value whether the level is failed.
	*/
	void setFailure(bool value);
    
#pragma mark -
#pragma mark Collision Handling

    /** Processes a collision between Lumia and a magical plant */
    void processPlantLumiaCollision(float newRadius, const std::shared_ptr<LumiaModel> lumia);

    /** Processes a collision between Lumia and an energy item */
    void processEnergyLumiaCollision(const std::shared_ptr<EnergyModel> energy, const std::shared_ptr<LumiaModel> lumia);

    /** Processes a collision between Lumia and another Lumia */
    void processLumiaLumiaCollision(const std::shared_ptr<LumiaModel> lumia, const std::shared_ptr<LumiaModel> lumia2);

	/**
	* Processes the start of a collision
	*
	* This method is called when we first get a collision between two objects.  We use
	* this method to test if it is the "right" kind of collision.  In particular, we
	* use it to test if we make it to the win door.  We also us it to eliminate bullets.
	*
	* @param  contact  The two bodies that collided
	*/
	void beginContact(b2Contact* contact);

	/**
	* Processes the end of a collision
	*
	* This method is called when we no longer have a collision between two objects.  
	* We use this method allow the character to jump again.
	*
	* @param  contact  The two bodies that collided
	*/
	void endContact(b2Contact* contact);

#pragma mark -
#pragma mark Gameplay Handling
    /**
     * The method called to update the game mode.
     *
     * This method contains any gameplay code that is not an OpenGL call.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep);

    /**
     * Resets the status of the game so that we can play again.
     */
    void reset();

    void createEnergy(Vec2 pos);
    
    void checkWin();

    /**
    * Adds a new Lumia to the world.
    */
    std::shared_ptr<LumiaModel> createLumia(float radius, Vec2 pos, bool isAvatar);

    /**
    * Removes the input Lumia from the world.
    *
    * @param  lumia the Lumia to remove
    */
    void removeLumia(std::shared_ptr<LumiaModel> lumia);

    /**
    * Removes the input energy item from the world.
    *
    * @param  energy the energy item to remove
    */
    void removeEnergy(std::shared_ptr<EnergyModel> energy);
    
    /** Gives nearby Lumia velocity towards player avatar so they merge on contact */
    void mergeLumiasNearby();

    /** Set player avatar to the nearest Lumia body that is not the parameter lumia */
    void switchToNearestLumia(const std::shared_ptr<LumiaModel> lumia);

    /**
     * Calculates trajectory point one timestep into future
     *
     * @param startingPosition the position model is located before launching
     * @param startingVelocity the velocity model will be launched at during aiming
     * @param n timestep
     * @param dt time in seconds since last update frame
     */
    Vec2 getTrajectoryPoint(Vec2& startingPosition, Vec2& startingVelocity, 
                            float n, std::shared_ptr<cugl::physics2::ObstacleWorld> _world, float dt);

  };

#endif /* __LM_GAME_SCENE_H__ */
