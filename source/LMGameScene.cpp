//
//  LMGameScene.cpp
//
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#include "LMGameScene.h"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "LMLumiaModel.h"
#include "LMSpinner.h"
#include "LMRopeBridge.h"
#include "LMBullet.h"
#include "LMPlant.h"

#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <random>

using namespace cugl;

#pragma mark -
#pragma mark Level Geography

/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024
#define SCENE_HEIGHT 576

/** This is the aspect ratio for physics */
#define SCENE_ASPECT 9.0/16.0

/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH   32.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT  18.0f

// Since these appear only once, we do not care about the magic numbers.
// In an actual game, this information would go in a data file.
// IMPORTANT: Note that Box2D units do not equal drawing units
/** The wall vertices */
#define WALL_VERTS 12
#define WALL_COUNT  2

float WALL[WALL_COUNT][WALL_VERTS] = {
	{16.0f, 18.0f, 16.0f, 17.0f,  1.0f, 17.0f,
	1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 18.0f},
	{32.0f, 18.0f, 32.0f,  0.0f, 31.0f,  0.0f,
	31.0f, 17.0f, 16.0f, 17.0f, 16.0f, 18.0f}
};

/** The number of platforms */
#define PLATFORM_VERTS  8
#define PLATFORM_COUNT  10

/** The outlines of all of the platforms */
float PLATFORMS[PLATFORM_COUNT][PLATFORM_VERTS] = {
	{ 1.0f, 3.0f, 6.0f, 3.0f, 6.0f, 2.5f, 1.0f, 2.5f},
	{ 6.0f, 4.0f, 9.0f, 4.0f, 9.0f, 2.5f, 6.0f, 2.5f},
	{23.0f, 4.0f,31.0f, 4.0f,31.0f, 2.5f,23.0f, 2.5f},
	{26.0f, 5.5f,28.0f, 5.5f,28.0f, 5.0f,26.0f, 5.0f},
	{29.0f, 7.0f,31.0f, 7.0f,31.0f, 6.5f,29.0f, 6.5f},
	{24.0f, 8.5f,27.0f, 8.5f,27.0f, 8.0f,24.0f, 8.0f},
	{29.0f,10.0f,31.0f,10.0f,31.0f, 9.5f,29.0f, 9.5f},
	{23.0f,11.5f,27.0f,11.5f,27.0f,11.0f,23.0f,11.0f},
	{19.0f,12.5f,23.0f,12.5f,23.0f,12.0f,19.0f,12.0f},
	{ 1.0f,12.5f, 7.0f,12.5f, 7.0f,12.0f, 1.0f,12.0f}
};

/** The goal door position */
float GOAL_POS[] = { 4.0f,14.0f};
/** The position of the spinning barrier */
float SPIN_POS[] = {13.0f,12.5f};
/** The initial position of the dude */
float DUDE_POS[] = { 2.5f, 5.0f};
/** The position of the rope bridge */
float BRIDGE_POS[] = {9.0f, 3.8f};

#pragma mark -
#pragma mark Physics Constants
/** The new heavier gravity for this world (so it is not so floaty) */
#define DEFAULT_GRAVITY -28.9f
/** The density for most physics objects */
#define BASIC_DENSITY   0.0f
/** The density for a bullet */
#define HEAVY_DENSITY   10.0f
/** Friction of most platforms */
#define BASIC_FRICTION  0.4f
/** The restitution for all physics objects */
#define BASIC_RESTITUTION   0.1f
/** The width of the rope bridge */
#define BRIDGE_WIDTH    14.0f
/** Offset for bullet when firing */
#define BULLET_OFFSET   0.5f
/** The speed of the bullet after firing */
#define BULLET_SPEED   20.0f
/** The number of frame to wait before reinitializing the game */
#define EXIT_COUNT      240


#pragma mark -
#pragma mark Asset Constants
/** The key for the earth texture in the asset manager */
#define EARTH_TEXTURE   "earth"
/** The key for the win door texture in the asset manager */
#define GOAL_TEXTURE    "goal"
/** The key for the win door texture in the asset manager */
#define BULLET_TEXTURE  "bullet"
/** The name of a bullet (for object identification) */
#define BULLET_NAME     "bullet"
#define PLANT_NAME       "plant"
/** The name of a wall (for object identification) */
#define WALL_NAME       "wall"

#define LUMIA_NAME      "lumia"
/** The name of a platform (for object identification) */
#define PLATFORM_NAME   "platform"
/** The font for victory/failure messages */
#define MESSAGE_FONT    "retro"
/** The message for winning the game */
#define WIN_MESSAGE     "VICTORY!"
/** The color of the win message */
#define WIN_COLOR       Color4::YELLOW
/** The message for losing the game */
#define LOSE_MESSAGE    "FAILURE!"
/** The color of the lose message */
#define LOSE_COLOR      Color4::RED
/** The key the basic game music */
#define GAME_MUSIC      "game"
/** The key the victory game music */
#define WIN_MUSIC       "win"
/** The key the failure game music */
#define LOSE_MUSIC      "lose"
/** The sound effect for firing a bullet */
#define PEW_EFFECT      "pew"
/** The sound effect for a bullet collision */
#define POP_EFFECT      "pop"
/** The sound effect for jumping */
#define JUMP_EFFECT     "jump"
/** The volume for the music */
#define MUSIC_VOLUME    0.7f
/** The volume for sound effects */
#define EFFECT_VOLUME   0.8f
/** The image for the left dpad/joystick */
#define LEFT_IMAGE      "dpad_left"
/** The image for the right dpad/joystick */
#define RIGHT_IMAGE     "dpad_right"

#define BACKGROUND_IMAGE "background"

/** Color to outline the physics nodes */
#define DEBUG_COLOR     Color4::YELLOW
/** Opacity of the physics outlines */
#define DEBUG_OPACITY   192



#pragma mark -
#pragma mark Constructors
/**
 * Creates a new game world with the default values.
 *
 * This constructor does not allocate any objects or start the controller.
 * This allows us to use a controller without a heap pointer.
 */
GameScene::GameScene() : Scene2(),
	_worldnode(nullptr),
	_debugnode(nullptr),
	_world(nullptr),
	_avatar(nullptr),
	_complete(false),
	_debug(false)
{    
}

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
bool GameScene::init(const std::shared_ptr<AssetManager>& assets) {
    _jsonr = cugl::JsonReader::alloc("json/level.json");
    std::shared_ptr<cugl::JsonValue> jv = _jsonr->readJson();
    _leveljson = jv->get("level");
    return init(assets,Rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT),Vec2(0,DEFAULT_GRAVITY));
}

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
bool GameScene::init(const std::shared_ptr<AssetManager>& assets, const Rect& rect) {
    return init(assets,rect,Vec2(0,DEFAULT_GRAVITY));
}

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
bool GameScene::init(const std::shared_ptr<AssetManager>& assets, const Rect& rect, const Vec2& gravity) {
    // Initialize the scene to a locked height (iPhone X is narrow, but wide)
    Size dimen = computeActiveSize();

    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
   
   
    // Start up the input handler
    _assets = assets;
    _input.init(getBounds());
    
    
    std::shared_ptr<scene2::SceneNode> scene = assets->get<scene2::SceneNode>("game");
    scene->setContentSize(dimen);
    scene->doLayout(); // Repositions the HUD;
   
    // Create the world and attach the listeners.
    _world = physics2::ObstacleWorld::alloc(rect,gravity);
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
      beginContact(contact);
    };
    _world->onEndContact = [this](b2Contact* contact) {
      endContact(contact);
    };
  
    // IMPORTANT: SCALING MUST BE UNIFORM
    // This means that we cannot change the aspect ratio of the physics world
    // Shift to center if a bad fit
    _scale = dimen.width == SCENE_WIDTH ? dimen.width/rect.size.width : dimen.height/rect.size.height;
    Vec2 offset((dimen.width-SCENE_WIDTH)/2.0f,(dimen.height-SCENE_HEIGHT)/2.0f);

    // Create the scene graph
    std::shared_ptr<Texture> image;
    _worldnode = scene2::SceneNode::alloc();
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _worldnode->setPosition(offset);

    _debugnode = scene2::SceneNode::alloc();
    _debugnode->setScale(_scale); // Debug node draws in PHYSICS coordinates
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _debugnode->setPosition(offset);

    _winnode = scene2::Label::alloc(WIN_MESSAGE, _assets->get<Font>(MESSAGE_FONT));
    _winnode->setAnchor(Vec2::ANCHOR_CENTER);
    _winnode->setPosition(dimen.width/2.0f,dimen.height/2.0f);
    _winnode->setForeground(WIN_COLOR);
    setComplete(false);

    _losenode = scene2::Label::alloc(LOSE_MESSAGE, _assets->get<Font>(MESSAGE_FONT));
    _losenode->setAnchor(Vec2::ANCHOR_CENTER);
    _losenode->setPosition(dimen.width/2.0f,dimen.height/2.0f);
    _losenode->setForeground(LOSE_COLOR);
    setFailure(false);

    
    _leftnode = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>(LEFT_IMAGE));
    _leftnode->SceneNode::setAnchor(cugl::Vec2::ANCHOR_MIDDLE_RIGHT);
    _leftnode->setScale(0.35f);
    _leftnode->setVisible(false);

    _rightnode = scene2::PolygonNode::allocWithTexture(_assets->get<Texture>(RIGHT_IMAGE));
    _rightnode->SceneNode::setAnchor(cugl::Vec2::ANCHOR_MIDDLE_LEFT);
    _rightnode->setScale(0.35f);
    _rightnode->setVisible(false);

    addChild(scene, 0);
    addChild(_worldnode, 1);
    addChild(_debugnode,2);
    addChild(_winnode,  3);
    addChild(_losenode, 4);
    addChild(_leftnode, 5);
    addChild(_rightnode,6);
   

    populate();
    _active = true;
    _complete = false;
    setDebug(false);
    
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose() {
    if (_active) {
        _input.dispose();
        _world = nullptr;
        _worldnode = nullptr;
        _debugnode = nullptr;
        _winnode = nullptr;
        _losenode = nullptr;
        _leftnode = nullptr;
        _rightnode = nullptr;
        _complete = false;
        _debug = false;
        Scene2::dispose();
    }
}


#pragma mark -
#pragma mark Level Layout

/**
 * Resets the status of the game so that we can play again.
 *
 * This method disposes of the world and creates a new one.
 */
void GameScene::reset() {
    _world->clear();
    _worldnode->removeAllChildren();
    _debugnode->removeAllChildren();
    _lumiaSet.clear();
    _avatar = nullptr;
    _goalDoor = nullptr;
    _spinner = nullptr;
    _ropebridge = nullptr;
    for (const std::shared_ptr<Plant> &p : _plants) {
        p->dispose();
    }
    _plants.clear();
      
    setFailure(false);
    setComplete(false);
    populate();
}

/**
 * Lays out the game geography.
 *
 * Pay close attention to how we attach physics objects to a scene graph.
 * The simplest way is to make a subclass, like we do for the Dude.  However,
 * for simple objects you can just use a callback function to lightly couple
 * them.  This is what we do with the crates.
 *
 * This method is really, really long.  In practice, you would replace this
 * with your serialization loader, which would process a level file.
 */
void GameScene::populate() {
#pragma mark : Goal door
    std::shared_ptr<Texture> image;
    std::shared_ptr<scene2::PolygonNode> sprite;
    std::shared_ptr<scene2::WireNode> draw;
    
//	std::shared_ptr<Texture> image = _assets->get<Texture>(GOAL_TEXTURE);
//	std::shared_ptr<scene2::PolygonNode> sprite;
//	std::shared_ptr<scene2::WireNode> draw;
//
//	// Create obstacle
//	Vec2 goalPos = GOAL_POS;
//	Size goalSize(image->getSize().width/_scale,
//	image->getSize().height/_scale);
//	_goalDoor = physics2::BoxObstacle::alloc(goalPos,goalSize);
//
//	// Set the physics attributes
//	_goalDoor->setBodyType(b2_staticBody);
//	_goalDoor->setDensity(0.0f);
//	_goalDoor->setFriction(0.0f);
//	_goalDoor->setRestitution(0.0f);
//	_goalDoor->setSensor(true);
//
//	// Add the scene graph nodes to this object
//	sprite = scene2::PolygonNode::allocWithTexture(image);
//	_goalDoor->setDebugColor(DEBUG_COLOR);
//	addObstacle(_goalDoor, sprite, 0); // Put this at the very back
    
#pragma mark : Walls
	// All walls and platforms share the same texture
    image  = _assets->get<Texture>(EARTH_TEXTURE);
	std::string wname = "wall";
	for (int ii = 0; ii < WALL_COUNT; ii++) {
		std::shared_ptr<physics2::PolygonObstacle> wallobj;

		Poly2 wall(WALL[ii],WALL_VERTS);
		// Call this on a polygon to get a solid shape
		SimpleTriangulator triangulator;
		triangulator.set(wall);
		triangulator.calculate();
		wall.setIndices(triangulator.getTriangulation());
		wall.setGeometry(Geometry::SOLID);

		wallobj = physics2::PolygonObstacle::alloc(wall);
		// You cannot add constant "".  Must stringify
		wallobj->setName(std::string(WALL_NAME)+cugl::strtool::to_string(ii));
		wallobj->setName(wname);

		// Set the physics attributes
		wallobj->setBodyType(b2_staticBody);
		wallobj->setDensity(BASIC_DENSITY);
		wallobj->setFriction(BASIC_FRICTION);
		wallobj->setRestitution(BASIC_RESTITUTION);
		wallobj->setDebugColor(DEBUG_COLOR);

		wall *= _scale;
		sprite = scene2::PolygonNode::allocWithTexture(image,wall);
		addObstacle(wallobj,sprite,1);  // All walls share the same texture
	}

#pragma mark : Platforms
    /**
	for (int ii = 0; ii < PLATFORM_COUNT; ii++) {
		std::shared_ptr<physics2::PolygonObstacle> platobj;
		Poly2 platform(PLATFORMS[ii],8);

		SimpleTriangulator triangulator;
		triangulator.set(platform);
		triangulator.calculate();
		platform.setIndices(triangulator.getTriangulation());
		platform.setGeometry(Geometry::SOLID);

		platobj = physics2::PolygonObstacle::alloc(platform);
		// You cannot add constant "".  Must stringify
		platobj->setName(std::string(PLATFORM_NAME)+cugl::strtool::to_string(ii));

		// Set the physics attributes
		platobj->setBodyType(b2_staticBody);
		platobj->setDensity(BASIC_DENSITY);
		platobj->setFriction(BASIC_FRICTION);
		platobj->setRestitution(BASIC_RESTITUTION);
		platobj->setDebugColor(DEBUG_COLOR);

		platform *= _scale;
		sprite = scene2::PolygonNode::allocWithTexture(image,platform);
		addObstacle(platobj,sprite,1);
	}
    */
    int numplats = _leveljson->getInt("numplatforms");
    for (int i = 1; i <= numplats; i++) {
        std::string platstring = "plat_" + to_string(i);
        std::shared_ptr<cugl::JsonValue> platfor = _leveljson->get(platstring);
        float vox = platfor->getFloat("v1x");
        float voy = platfor->getFloat("v1y");
        float vtwx = platfor->getFloat("v2x");
        float vtwy = platfor->getFloat("v2y");
        float vthx = platfor->getFloat("v3x");
        float vthy = platfor->getFloat("v3y");
        float vfx = platfor->getFloat("v4x");
        float vfy = platfor->getFloat("v4y");
        float platvec[] = {(vox),voy,vtwx,vtwy,vthx,
            vthy,vfx,vfy};
        std::shared_ptr<physics2::PolygonObstacle> platobj;
        Poly2 platform(platvec,8);

        SimpleTriangulator triangulator;
        triangulator.set(platform);
        triangulator.calculate();
        platform.setIndices(triangulator.getTriangulation());
        platform.setGeometry(Geometry::SOLID);

        platobj = physics2::PolygonObstacle::alloc(platform);
        // You cannot add constant "".  Must stringify
        platobj->setName(std::string(PLATFORM_NAME)+cugl::strtool::to_string(i));

        // Set the physics attributes
        platobj->setBodyType(b2_staticBody);
        platobj->setDensity(BASIC_DENSITY);
        platobj->setFriction(BASIC_FRICTION);
        platobj->setRestitution(BASIC_RESTITUTION);
        platobj->setDebugColor(DEBUG_COLOR);

        platform *= _scale;
        sprite = scene2::PolygonNode::allocWithTexture(image,platform);
        addObstacle(platobj,sprite,1);
    }

#pragma mark : Plant
    /**
	Vec2 spinPos = SPIN_POS;
    image = _assets->get<Texture>(SPINNER_TEXTURE);
	_spinner = Spinner::alloc(spinPos,image->getSize()/_scale,_scale);
    _spinner->setTexture(image);
	std::shared_ptr<scene2::SceneNode> node = scene2::SceneNode::alloc();
	_spinner->setSceneNode(node);
	_spinner->setBodyType(b2_staticBody);
	_spinner->setDebugColor(DEBUG_COLOR);
	addObstacle(_spinner, node, 2, false);
    */
    int np = _leveljson->getInt("numplants");
    for (int i = 1; i <= np; i++) {
        std::string ps = ("plant_" + to_string(i));
        std::shared_ptr<cugl::JsonValue> plant = _leveljson->get(ps);
        float px = plant->getFloat("posx");
        float py = plant->getFloat("posy");
        float pa = (plant->getFloat("angle"))*M_PI/180;
        createPlant(px, py, i,pa);
    }
#pragma mark : Rope Bridge
    /**
	Vec2 bridgeStart = BRIDGE_POS;
	Vec2 bridgeEnd   = bridgeStart;
	bridgeEnd.x += BRIDGE_WIDTH;
    image = _assets->get<Texture>(BRIDGE_TEXTURE);
    
	_ropebridge = RopeBridge::alloc(bridgeStart,bridgeEnd,image->getSize()/_scale,_scale);
    _ropebridge->setTexture(image);
	node = scene2::SceneNode::alloc();
	_ropebridge->setSceneNode(node);
	_ropebridge->setBodyType(b2_staticBody);
	_ropebridge->setDebugColor(DEBUG_COLOR);
	addObstacle(_ropebridge, node, 3, false);
     */
#pragma mark : Lumia
	Vec2 lumiaPos = DUDE_POS;
    std::shared_ptr<scene2::SceneNode> node = scene2::SceneNode::alloc();
    image = _assets->get<Texture>(BULLET_TEXTURE);
    float radius = 1.0f;// change to value from json
	_avatar = LumiaModel::alloc(lumiaPos,radius,_scale);
    _avatar-> setTextures(image, DUDE_POS);
    _avatar-> setName(LUMIA_NAME);
	_avatar-> setDebugColor(DEBUG_COLOR);
    _lumiaSet.insert(_avatar.get());
	addObstacle(_avatar,_avatar->getSceneNode(), 4); // Put this at the very front

	std::shared_ptr<Sound> source = _assets->get<Sound>(GAME_MUSIC);
    AudioEngine::get()->getMusicQueue()->play(source, true, MUSIC_VOLUME);
}

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
 * have different z-orders whenever possible.  This will cut down on the amount of drawing done
 *
 * @param obj             The physics object to add
 * @param node            The scene graph node to attach it to
 * @param zOrder          The drawing order
 * @param useObjPosition  Whether to update the node's position to be at the object's position
 */
void GameScene::addObstacle(const std::shared_ptr<cugl::physics2::Obstacle>& obj,
                            const std::shared_ptr<cugl::scene2::SceneNode>& node,
                            int zOrder,
                            bool useObjPosition) {
    _world->addObstacle(obj);
    obj->setDebugScene(_debugnode);
    
    // Position the scene graph node (enough for static objects)
  	if (useObjPosition) {
	  	node->setPosition(obj->getPosition()*_scale);
	  }
	  _worldnode->addChild(node, zOrder);
    
    // Dynamic objects need constant updating
    if (obj->getBodyType() == b2_dynamicBody) {
        scene2::SceneNode* weak = node.get(); // No need for smart pointer in callback
        obj->setListener([=](physics2::Obstacle* obs){
            weak->setPosition(obs->getPosition()*_scale);
            weak->setAngle(obs->getAngle());
        });
    }
}


#pragma mark -
#pragma mark Physics Handling

/**
 * Executes the core gameplay loop of this world.
 *
 * This method contains the specific update code for this mini-game. It does
 * not handle collisions, as those are managed by the parent class WorldController.
 * This method is called after input is read, but before collisions are resolved.
 * The very last thing that it should do is apply forces to the appropriate objects.
 *
 * @param  delta    Number of seconds since last animation frame
 */
void GameScene::update(float dt) {
	_input.update(dt);
    checkWin();
	// Process the toggled key commands
	if (_input.didDebug()) { setDebug(!isDebug()); }
	if (_input.didReset()) { reset(); }
	if (_input.didExit())  {
		CULog("Shutting down");
		Application::get()->quit();
	}

	// Process the movement
    //if (_input.withJoystick()) {
    //    if (_input.getHorizontal() < 0) {
    //        _leftnode->setVisible(true);
    //        _rightnode->setVisible(false);
    //    } else if (_input.getHorizontal() > 0) {
    //        _leftnode->setVisible(false);
    //        _rightnode->setVisible(true);
    //    } else {
    //        _leftnode->setVisible(false);
    //        _rightnode->setVisible(false);
    //    }
    //    _leftnode->setPosition(_input.getJoystick());
    //    _rightnode->setPosition(_input.getJoystick());
    //} else {
    //    _leftnode->setVisible(false);
    //    _rightnode->setVisible(false);
    //}
    
	_avatar->setVelocity(_input.getLaunch());
	// if Lumia is on ground, player can launch Lumia so we should show the projected trajectory
	if (_avatar->isGrounded()) {
		_avatar->setPlannedVelocity(_input.getLaunch());
		//glColor3f(1, 1, 0);
		//glBegin(GL_LINES);
		//for (int i = 0; i < 180; i++) { // three seconds at 60fps
		//	Vec2 trajectoryPosition = getTrajectoryPoint(_avatar->getPos(), _input.getLaunch(), i, _world);
		//	glVertex2f(trajectoryPosition.x, trajectoryPosition.y);
		//}
		//glEnd();
	}
	_avatar->setJumping( _input.didJump());
	_avatar->setLaunching(_input.didLaunch());
	_avatar->applyForce();
    _avatar->setSplitting(_input.didSplit());
    _avatar->setMerging(_input.didMerge());

	if (_avatar->isJumping() && _avatar->isGrounded()) {
		std::shared_ptr<Sound> source = _assets->get<Sound>(JUMP_EFFECT);
		AudioEngine::get()->play(JUMP_EFFECT,source,false,EFFECT_VOLUME);
	}

    if (_avatar->isSplitting()){
        CULog("gamescene spliting? %d", _avatar->isSplitting());
        _avatar->setSplitForce(Vec2(6.0f, 0.0f));
        _avatar->split();
        createLumia();
    } else if(_avatar->isMerging()){
     // find all lumias close enough to _avatar, push them into the direction of lumia. once they contact, merge.
        mergeLumiasNearby();
        
    }
    
	// Turn the physics engine crank.
	_world->update(dt);

	// Since items may be deleted, garbage collect
	_world->garbageCollect();

	// Add a bullet AFTER physics allows it to hang in front
	// Otherwise, it looks like bullet appears far away
	_avatar->setShooting(_input.didFire());
	if (_avatar->isShooting()) {
		createBullet();
	}

	// Record failure if necessary.
	if (!_failed && _avatar->getY() < 0) {
		setFailure(true);
	}

//    _avatar->setSplitting(false);
    
	// Reset the game if we win or lose.
	if (_countdown > 0) {
		_countdown--;
	} else if (_countdown == 0) {
		reset();
	}
}

/**
* Sets whether the level is completed.
*
* If true, the level will advance after a countdown
*
* @param value whether the level is completed.
*/
void GameScene::setComplete(bool value) {
    bool change = _complete != value;
	_complete = value;
	if (value && change) {
		std::shared_ptr<Sound> source = _assets->get<Sound>(WIN_MUSIC);
		AudioEngine::get()->getMusicQueue()->play(source, false, MUSIC_VOLUME);
		_winnode->setVisible(true);
		_countdown = EXIT_COUNT;
	} else if (!value) {
		_winnode->setVisible(false);
		_countdown = -1;
	}
}

/**
 * Sets whether the level is failed.
 *
 * If true, the level will reset after a countdown
 *
 * @param value whether the level is failed.
 */
void GameScene::setFailure(bool value) {
	_failed = value;
	if (value) {
		std::shared_ptr<Sound> source = _assets->get<Sound>(LOSE_MUSIC);
        AudioEngine::get()->getMusicQueue()->play(source, false, MUSIC_VOLUME);
		_losenode->setVisible(true);
		_countdown = EXIT_COUNT;
	} else {
		_losenode->setVisible(false);
		_countdown = -1;
	}
}


/**
 * Add a new bullet to the world and send it in the right direction.
 */
void GameScene::createBullet() {
	float offset = BULLET_OFFSET;
	Vec2 pos = _avatar->getPosition();
	pos.x += (_avatar->isFacingRight() ? offset : -offset);

	std::shared_ptr<Texture> image = _assets->get<Texture>(BULLET_TEXTURE);
	float radius = 0.5f*image->getSize().width/_scale;

	std::shared_ptr<Bullet> bullet = Bullet::alloc(pos, radius);
	bullet->setName(BULLET_NAME);
	bullet->setDensity(HEAVY_DENSITY);
	bullet->setBullet(true);
	bullet->setGravityScale(0);
	bullet->setDebugColor(DEBUG_COLOR);
	bullet->setDrawScale(_scale);

	std::shared_ptr<scene2::PolygonNode> sprite = scene2::PolygonNode::allocWithTexture(image);
	bullet->setSceneNode(sprite);

	// Compute position and velocity
	float speed  = (_avatar->isFacingRight() ? BULLET_SPEED : -BULLET_SPEED);
	bullet->setVX(speed);
	addObstacle(bullet, sprite, 5);

	std::shared_ptr<Sound> source = _assets->get<Sound>(PEW_EFFECT);
	AudioEngine::get()->play(PEW_EFFECT,source, false, EFFECT_VOLUME, true);
    
}

void GameScene::createPlant(float posx, float posy, int nplant, float ang) {

    std::shared_ptr<Texture> image = _assets->get<Texture>(BULLET_TEXTURE);
    float radius = 0.5*image->getSize().width/(_scale);

    std::shared_ptr<Plant> p = Plant::alloc(Vec2(posx,posy), radius);
    p->setBodyType(b2_staticBody);
    p->setAngle(ang);
    p->lightDown();
    p->setFriction(0.0f);
    p->setRestitution(0.0f);
    p->setName(PLANT_NAME + to_string(nplant));
    p->setDensity(0);
    p->setBullet(false);
    p->setGravityScale(0);
    p->setSensor(true);
    p->setDebugColor(DEBUG_COLOR);
    p->setDrawScale(_scale);

    std::shared_ptr<scene2::PolygonNode> sprite = scene2::PolygonNode::allocWithTexture(image);
    sprite->setAngle(ang);
    p->setSceneNode(sprite);

    p->setVX(0);
    addObstacle(p, sprite, 0);
    _plants.push_front(p);
}

void GameScene::checkWin() {
    for (auto const& i : _plants) {
        if (!(i->getIsLit())) {
            return;
        }
    }
    setComplete(true);
}

/**
 * Add a new bullet to the world and send it in the right direction.
 */
void GameScene::createLumia() {
    Vec2 pos = _avatar->getPosition();
    pos.x -= 0.5f;
    std::shared_ptr<Texture> image = _assets->get<Texture>(BULLET_TEXTURE);
    float radius = _avatar->getRadius();
    
    std::shared_ptr<LumiaModel> lumia = LumiaModel::alloc(pos, radius, _scale);
    lumia-> setTextures(image, pos);
    lumia->setDebugColor(DEBUG_COLOR);
    lumia-> setName(LUMIA_NAME);
    lumia->setSplitForce(Vec2(-6.0f, 0.0f));
    addObstacle(lumia, lumia->getSceneNode(), 5); // Put this at the very front
    
    _lumiaSet.insert(lumia.get());
 
}
/**
 * Removes a new bullet from the world.
 *
 * @param  bullet   the bullet to remove
 */
void GameScene::removeBullet(Bullet* bullet) {
  // do not attempt to remove a bullet that has already been removed
	if (bullet->isRemoved()) {
		return;
	}
	_worldnode->removeChild(bullet->getSceneNode());
	bullet->setDebugScene(nullptr);
	bullet->markRemoved(true);

	std::shared_ptr<Sound> source = _assets->get<Sound>(POP_EFFECT);
	AudioEngine::get()->play(POP_EFFECT,source,false,EFFECT_VOLUME, true);
}

void GameScene::mergeLumiasNearby(){
    for (LumiaModel* lumia : _lumiaSet){
        if (lumia==_avatar.get()){
            continue;
        }
        Vec2 avatarPos = _avatar->getPosition();
        Vec2 lumiaPos = lumia->getPosition();
        float dist = avatarPos.distance(lumiaPos);
        if (dist <= lumia->getRadius() + _avatar->getRadius()){
            _avatar->merge(lumia->getRadius());
            removeLumia(lumia);
            break;
        } else if (dist < 10.0f){
            //set lumia velocity to move toward avatar
            Vec2 distance = avatarPos-lumiaPos;
            lumia->setLinearVelocity(distance.normalize().scale(5.0f));
        }
        
    }
}

void GameScene::removeLumia(LumiaModel* lumia) {
  // do not attempt to remove a bullet that has already been removed
    if (lumia->isRemoved()) {
        return;
    }
    _worldnode->removeChild(lumia->getSceneNode());
    _lumiaSet.erase(lumia);
    lumia->setDebugScene(nullptr);
    lumia->markRemoved(true);
}

/**
* Calculates trajectory point one timestep into future
*
* @param startingPosition the position model is located before launching
* @param startingVelocity the velocity model will be launched at during aiming
* @param n timestep
*/
Vec2 GameScene::getTrajectoryPoint(b2Vec2& startingPosition, Vec2& startingVelocity,
						float n, std::shared_ptr<cugl::physics2::ObstacleWorld> _world) {
	//velocity and gravity are given per second but we want time step values here
	float t = 1 / 60.0f; // seconds per time step (at 60fps)
	Vec2 stepVelocity = t * startingVelocity; // m/s
	Vec2 stepGravity = t * t * _world->getGravity(); // m/s/s
	return Vec2(startingPosition.x + n * stepVelocity.x, startingPosition.y + n * stepVelocity.y) + 0.5f * (n * n + n) * stepGravity;
}


#pragma mark -
#pragma mark Collision Handling
/**
 * Processes the start of a collision
 *
 * This method is called when we first get a collision between two objects.  We use
 * this method to test if it is the "right" kind of collision.  In particular, we
 * use it to test if we make it to the win door.
 *
 * @param  contact  The two bodies that collided
 */
void GameScene::beginContact(b2Contact* contact) {
	b2Fixture* fix1 = contact->GetFixtureA();
	b2Fixture* fix2 = contact->GetFixtureB();

	b2Body* body1 = fix1->GetBody();
	b2Body* body2 = fix2->GetBody();

	void* fd1 = fix1->GetUserData();
	void* fd2 = fix2->GetUserData();

	physics2::Obstacle* bd1 = (physics2::Obstacle*)body1->GetUserData();
    physics2::Obstacle* bd2 = (physics2::Obstacle*)body2->GetUserData();

	if (bd1->getName() == BULLET_NAME && bd2 != _avatar.get()) {
		removeBullet((Bullet*)bd1);
	} else if (bd2->getName() == BULLET_NAME && bd1 != _avatar.get()) {
		removeBullet((Bullet*)bd2);
	}
    if (bd1->getName().substr(0,5) == PLANT_NAME && bd2 == _avatar.get()) {
        ((Plant*)bd1)->lightUp();
    }
    else if (bd2->getName().substr(0.5) == PLANT_NAME && bd1 == _avatar.get()) {
        ((Plant*)bd2)->lightUp();
    }
 
	// See if we have landed on the ground.
	if ((_avatar->getSensorName() == fd2 && _avatar.get() != bd1) ||
		(_avatar->getSensorName() == fd1 && _avatar.get() != bd2)) {
		_avatar->setGrounded(true);
		// Could have more than one ground
		_sensorFixtures.emplace(_avatar.get() == bd1 ? fix2 : fix1);
	}

	// If we hit the "win" door, we are done
	if((bd1 == _avatar.get()   && bd2 == _goalDoor.get()) ||
		(bd1 == _goalDoor.get() && bd2 == _avatar.get())) {
		setComplete(true);
	}
}

/**
 * Callback method for the start of a collision
 *
 * This method is called when two objects cease to touch.  The main use of this method
 * is to determine when the characer is NOT on the ground.  This is how we prevent
 * double jumping.
 */
void GameScene::endContact(b2Contact* contact) {
	b2Fixture* fix1 = contact->GetFixtureA();
	b2Fixture* fix2 = contact->GetFixtureB();

	b2Body* body1 = fix1->GetBody();
	b2Body* body2 = fix2->GetBody();

	void* fd1 = fix1->GetUserData();
	void* fd2 = fix2->GetUserData();

	void* bd1 = body1->GetUserData();
	void* bd2 = body2->GetUserData();


	if ((_avatar->getSensorName() == fd2 && _avatar.get() != bd1) ||
		(_avatar->getSensorName() == fd1 && _avatar.get() != bd2)) {
		_sensorFixtures.erase(_avatar.get() == bd1 ? fix2 : fix1);
		if (_sensorFixtures.empty()) {
			_avatar->setGrounded(false);
		}
	}
}

/**
 * Returns the active screen size of this scene.
 *
 * This method is for graceful handling of different aspect
 * ratios
 */
Size GameScene::computeActiveSize() const {
    Size dimen = Application::get()->getDisplaySize();
    float ratio1 = dimen.width/dimen.height;
    float ratio2 = ((float)SCENE_WIDTH)/((float)SCENE_HEIGHT);
    if (ratio1 < ratio2) {
        dimen *= SCENE_WIDTH/dimen.width;
    } else {
        dimen *= SCENE_HEIGHT/dimen.height;
    }
    return dimen;
}
