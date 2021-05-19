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
#include "CollisionController.h"
#include "EnergyModel.h"
#include "LevelModel.h"
#include "Button.h"
#include "SlidingDoor.h"
#include "GraphNode.h"
#include "TileDataModel.h"
#include "TileModel.h"
//#include "PathFindingController.h"
#include "TrajectoryNode.h"
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
    
    std::shared_ptr<LevelModel> _level;
    
    std::shared_ptr<TileDataModel> _tileManager;
    
    
    // CONTROLLERS
    /** Controller for abstracting out input across multiple platforms */
    std::shared_ptr<InputController> _input;
    
    CollisionController _collisionController;
    
    float _cameraTargetX;
    float _cameraTargetY;
    
    // VIEW
    /** Reference to the physics root of the scene graph */
    std::shared_ptr<cugl::scene2::SceneNode> _worldnode;
    /** Reference to the debug root of the scene graph */
    std::shared_ptr<cugl::scene2::SceneNode> _debugnode;
    
    std::shared_ptr<cugl::scene2::SceneNode> _backbuttonNode;
    
    std::shared_ptr<cugl::scene2::SceneNode> _UIscene;
    std::shared_ptr<cugl::scene2::SceneNode> _pausedUI;
    
    std::shared_ptr<cugl::scene2::Button> _backbutton;
    /** Reference to the lose message label */
    std::shared_ptr<cugl::scene2::Label> _losenode;
    
    std::shared_ptr<cugl::scene2::AnimationNode> _loseAnimation;

    /** The Box2D world */
    std::shared_ptr<cugl::physics2::ObstacleWorld> _world;
    /** The scale between the physics world and the screen (MUST BE UNIFORM) */
    float _scale;

    // Physics objects for the game
    /** References to the magical plants */
    std::list<std::shared_ptr<Plant>> _plantList;
    /** References to the spikes */
    std::list<std::shared_ptr<SpikeModel>> _spikeList;
    /** References to the energy items */
    std::list<std::shared_ptr<EnergyModel>> _energyList;
    /** References to the Lumia bodies */
    std::list<std::shared_ptr<LumiaModel>> _lumiaList;
    
    std::list<std::shared_ptr<Button>> _buttonList;
    
    /** References to the Lumias */
    std::queue<std::shared_ptr<LumiaModel>> _dyingLumiaQueue;
    
    std::list<std::shared_ptr<SlidingDoor>> _doorList;
    /** References to the Lumia bodies */
    std::list<std::shared_ptr<EnemyModel>> _enemyList;
    /** Reference to the player avatar */
    std::shared_ptr<LumiaModel> _avatar;
    
    std::shared_ptr<TrajectoryNode> _trajectoryNode;
    
    std::shared_ptr<scene2::PolygonNode> _avatarIndicatorNode;
    
    std::shared_ptr<cugl::scene2::SceneNode> _scrollNode;
    


    std::list<std::shared_ptr<scene2::PolygonNode>> _tutorialList;

    
    /** Whether or not debug mode is active */
    bool _debug;
    /** Whether we have failed at this world (and need a reset) */
    bool _failed;
    /** Countdown active for winning or losing */
    int _countdown;
    /** Volume level for game music */
    float _musicVolume;
    /** Volume level for sound effects */
    float _effectVolume;
    
    bool _switched;
    
    bool _canSplit;

    int _prevscore;
    
    int _prevstars;
    /** Mark set to handle more sophisticated collision callbacks */
    std::unordered_map<LumiaModel*, std::unordered_set<b2Fixture*>> _sensorFixtureMap;
    /** Mark set to handle more sophisticated collision callbacks */
    std::unordered_map<LumiaModel*, std::unordered_set<b2Fixture*>> _sensorFixtureMap2;
    
    std::unordered_map<Node, NodeState> _graph;

    int _ticks;
    /** Tick of last time a Lumia hit a spike */
    int _lastSpikeCollision;
    
    string _nextScene;
    
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
#pragma mark Game state
    enum GameState {
        Paused,
        Playing
    };
    
    GameState _state = GameState::Playing;

    void setPlaying() { _state = GameState::Playing; }
    
    void setPaused() { _state = GameState::Paused; }

    GameState getState() { return _state; }


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
     * @param level     The level to be loaded into the game world
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, string level);

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
    
    
    float touchstart;
    bool setStart;
    
    
#pragma mark -
#pragma mark State Access
    
    std::vector<std::shared_ptr<cugl::scene2::SceneNode>> _UIelements;
    void render_game(const std::shared_ptr<SpriteBatch>& batch, const std::shared_ptr<SpriteBatch>& UIbatch);

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
	* Returns true if the level is failed.
	*
	* If true, the level will reset after a countdown
	*
	* @return true if the level is failed.
	*/
	bool isFailure() const { return _failed; }

    string getNextScene() { return _nextScene; }

	/**
	* Sets whether the level is failed.
	*
	* If true, the level will reset after a countdown
	*
	* @param value whether the level is failed.
	*/
	void setFailure(bool value);

    /**
    * Sets the volume of game music
    * 
    * @param value the volume of game music
    */
    void setMusicVolume(float value) { _musicVolume = value; };

    /**
    * Sets the volume of sound effects
    *
    * @param value the volume of sound effects
    */
    void setEffectVolume(float value) { _effectVolume = value; };
    
#pragma mark -
#pragma mark Collision Handling
    
    bool didCollideWithLumiaBody(std::shared_ptr<LumiaModel> lumia, physics2::Obstacle* bd, void* fd);
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
    
    void removeLumiaNode(shared_ptr<LumiaModel> lumia);

    void deactivateLumiaPhysics(shared_ptr<LumiaModel> lumia);
    /**
     * Resets the status of the game so that we can play again.
     */
    void reset();

    void checkWin();

    /**
    * Adds a new Lumia to the world.
    */
    std::shared_ptr<LumiaModel> createLumia(int sizeLevel, Vec2 pos, bool isAvatar, Vec2 vel, float angularVel);

    /**
    * Removes the input Lumia from the world.
    *
    * @param  lumia the Lumia to remove
    */
    void removeLumia(std::shared_ptr<LumiaModel> lumia);
    
    void removeEnemy(std::shared_ptr<EnemyModel> enemy);
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
    
    
    void updateGame(float dt);
    
    void updatePaused(float dt, float startX);
    
    int calcScore();
    
    int getStars();
    
    int getPrevScore() {
        return _prevscore;
    }
    
    void setPrevScore(int s) {
        _prevscore = s;
    }
    
    int getPrevStars() {
        return _prevstars;
    }
    
    void setPrevStars(int p) {
        _prevstars = p;
    }
    
    void playSplitSound();
    
    void playDieSound();
    
    void playLightSound();
    
    void playGrowSound();
    /**
     * Calculates trajectory point one timestep into future
     *
     * @param startingPosition the position model is located before launching
     * @param startingVelocity the velocity model will be launched at during aiming
     * @param n timestep
     * @param dt time in seconds since last update frame
     */
    Vec2 getTrajectoryPoint(Vec2& startingPosition, Vec2& startingVelocity, 
                            float n);

  };




#endif /* __GAME_SCENE_H__ */

