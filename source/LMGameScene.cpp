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
#include "LMPlant.h"
#include "LMPlantNode.h"
#include "LMEnergyModel.h"

#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>

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
};

/** The initial position of Lumia */
float LUMIA_POS[] = { 2.5f, 5.0f};

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
/** The number of frame to wait before reinitializing the game */
#define EXIT_COUNT      240

#define ENERGY_RADIUS  3.0f


#pragma mark -
#pragma mark Asset Constants
/** The key for the earth texture in the asset manager */
#define EARTH_TEXTURE   "earth"
/** The key for the win door texture in the asset manager */
#define LUMIA_TEXTURE  "lumia"
/** The name of a plant (for object identification) */
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

    addChild(scene, 0);
    addChild(_worldnode, 1);
    addChild(_debugnode, 2);
    addChild(_winnode,  3);
    addChild(_losenode, 4);
   

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
    for (const std::shared_ptr<LumiaModel> &l : _lumiaList) {
        l->dispose();
    }
    _lumiaList.clear();
    _avatar = nullptr;
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
std::shared_ptr<Texture> image;
std::shared_ptr<scene2::PolygonNode> sprite;
    
#pragma mark : Walls
	// All walls and platforms share the same texture
    image  = _assets->get<Texture>(EARTH_TEXTURE);
	std::string wname = "wall";
//	for (int ii = 0; ii < WALL_COUNT; ii++) {
//		std::shared_ptr<physics2::PolygonObstacle> wallobj;
//
//		Poly2 wall(WALL[ii],WALL_VERTS);
//		// Call this on a polygon to get a solid shape
//		SimpleTriangulator triangulator;
//		triangulator.set(wall);
//		triangulator.calculate();
//		wall.setIndices(triangulator.getTriangulation());
//		wall.setGeometry(Geometry::SOLID);
//
//		wallobj = physics2::PolygonObstacle::alloc(wall);
//		// You cannot add constant "".  Must stringify
//		wallobj->setName(std::string(WALL_NAME)+cugl::strtool::to_string(ii));
//		wallobj->setName(wname);
//
//		// Set the physics attributes
//		wallobj->setBodyType(b2_staticBody);
//		wallobj->setDensity(BASIC_DENSITY);
//		wallobj->setFriction(BASIC_FRICTION);
//		wallobj->setRestitution(BASIC_RESTITUTION);
//		wallobj->setDebugColor(DEBUG_COLOR);
//
//		wall *= _scale;
//		sprite = scene2::PolygonNode::allocWithTexture(image,wall);
//		addObstacle(wallobj,sprite,1);  // All walls share the same texture
//	}

#pragma mark : Platforms
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
#pragma mark : Energy
    int ne = _leveljson->getInt("numEnergy");
    for (int i = 1; i <= ne; i++) {
        std::string es = ("nrg_" + to_string(i));
        std::shared_ptr<cugl::JsonValue> energy = _leveljson->get(es);
        float ex = energy->getFloat("posx");
        float ey = energy->getFloat("posy");
        Vec2 epos = Vec2(ex, ey);
        createEnergy(epos);
    }
#pragma mark : Plant
    int np = _leveljson->getInt("numplants");
    for (int i = 1; i <= np; i++) {
        std::string ps = ("plant_" + to_string(i));
        std::shared_ptr<cugl::JsonValue> plant = _leveljson->get(ps);
        float px = plant->getFloat("posx");
        float py = plant->getFloat("posy");
        float pa = (plant->getFloat("angle"))*M_PI/180;
        createPlant(px, py, i,pa);
    }

#pragma mark : Lumia
    std::shared_ptr<scene2::SceneNode> node = scene2::SceneNode::alloc();
    image = _assets->get<Texture>(LUMIA_TEXTURE);
    std::shared_ptr<cugl::JsonValue> lum = _leveljson->get("Lumia");
    float lumx = lum->getFloat("posx");
    float lumy = lum->getFloat("posy");
    float radius = lum->getFloat("radius");// change to value from jso
    Vec2 lumiaPos = Vec2(lumx,lumy);
	_avatar = LumiaModel::alloc(lumiaPos,radius,_scale);
    _avatar-> setTextures(image, LUMIA_POS);
    _avatar-> setName(LUMIA_NAME);
	_avatar-> setDebugColor(DEBUG_COLOR);
    _lumiaList.push_back(_avatar);
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
	// Process the toggled key commands
	if (_input.didDebug()) { setDebug(!isDebug()); }
	if (_input.didReset()) { reset(); }
	if (_input.didExit())  {
		CULog("Shutting down");
		Application::get()->quit();
	}
    if (!_failed && !_complete) {
    checkWin();
    }
    
    if(_input.didSwitch()){
        auto it = find(_lumiaList.begin(), _lumiaList.end(), _avatar);
        if (it != _lumiaList.end())
        {
            int index = (int) (it - _lumiaList.begin());
            int switchIndex = index + 1 >= _lumiaList.size() ? 0 : index + 1;
            _avatar = _lumiaList[switchIndex];
        }
    }
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
    
	_avatar->setLaunching(_input.didLaunch());
	_avatar->applyForce();
    _avatar->setSplitting(_input.didSplit());
    _avatar->setMerging(_input.didMerge());

    if (_avatar->isSplitting()){
        float radius = _avatar->getRadius() / 1.4f;
        Vec2 pos = _avatar->getPosition();
        auto temp = _avatar;
        _avatar = createLumia(radius, pos+Vec2(0.5f, 0.0f));
        createLumia(radius, pos-Vec2(0.5f, 0.0f));
        removeLumia(temp);
    } else if(_avatar->isMerging()){
        // find all lumias close enough to _avatar, push them into the direction of lumia. once they contact, merge.
        mergeLumiasNearby();
    }
    
	// Turn the physics engine crank.
	_world->update(dt);

	// Since items may be deleted, garbage collect
	_world->garbageCollect();

	// Record failure if necessary.
	if (!_failed && _avatar->getY() < 0) {
		setFailure(true);
	}
    
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

void GameScene::createPlant(float posx, float posy, int nplant, float ang) {

    std::shared_ptr<Texture> image = _assets->get<Texture>("lamp");
    
    cugl::Size size  = 0.3*image->getSize()/(_scale);

    std::shared_ptr<Plant> p = Plant::alloc(Vec2(posx,posy), size);
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

    std::shared_ptr<PlantNode> sprite = PlantNode::alloc(image);
    sprite->setAngle(ang);
    p->setSceneNode(sprite);

    p->setVX(0);
    addObstacle(p, sprite, 0);
    _plants.push_front(p);
}
void GameScene::createEnergy(Vec2 pos) {
    std::shared_ptr<Texture> image = _assets->get<Texture>(EARTH_TEXTURE);
    cugl::Size size = Size(2,2);
    std::shared_ptr<EnergyModel> nrg = EnergyModel::alloc(pos, size);
    nrg->setGravityScale(0);
    nrg->setBodyType(b2_staticBody);
    nrg->setSensor(true);
    nrg->setName("nrg_");
    cugl::Rect rectangle = Rect(pos, size);
    cugl::Poly2 poly = Poly2(rectangle);
    std::shared_ptr<cugl::scene2::PolygonNode> pn = cugl::scene2::PolygonNode::alloc(rectangle);
    std::shared_ptr<scene2::PolygonNode> sprite = scene2::PolygonNode::allocWithTexture(image,poly);
    sprite->setScale(_scale);
    nrg->setNode(sprite);
    addObstacle(nrg,sprite,0);
    _energies.push_front(nrg);
    
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
 * Add a new LUmia to the world.
 */
std::shared_ptr<LumiaModel> GameScene::createLumia(float radius, Vec2 pos) {
    std::shared_ptr<Texture> image = _assets->get<Texture>(LUMIA_TEXTURE);
    std::shared_ptr<LumiaModel> lumia = LumiaModel::alloc(pos, radius, _scale);
    lumia-> setTextures(image, pos);
    lumia->setDebugColor(DEBUG_COLOR);
    lumia-> setName(LUMIA_NAME);
    addObstacle(lumia, lumia->getSceneNode(), 5);
    
    _lumiaList.push_back(lumia);
    return lumia;
 
}

void GameScene::mergeLumiasNearby(){
    for (const std::shared_ptr<LumiaModel> &lumia : _lumiaList) {
        if (lumia==_avatar){
            continue;
        }
        Vec2 avatarPos = _avatar->getPosition();
        Vec2 lumiaPos = lumia->getPosition();
        float dist = avatarPos.distance(lumiaPos);
        if (dist <= lumia->getRadius() + _avatar->getRadius()){
            float radius = (_avatar->getRadius() + lumia->getRadius()) / 1.35f;
            Vec2 pos = _avatar->getPosition();
            auto temp = _avatar;
            _avatar = createLumia(radius, pos+Vec2(0.5f, 0.0f));
            removeLumia(temp);
            removeLumia(lumia);
            break;
        } else if (dist < 10.0f){
            //set lumia velocity to move toward avatar
            Vec2 distance = avatarPos-lumiaPos;
            lumia->setLinearVelocity(distance.normalize().scale(5.0f));
        }
    }
}

void GameScene::removeLumia(shared_ptr<LumiaModel> lumia) {
    // do not attempt to remove a Lumia that has already been removed
    if (lumia->isRemoved()) {
        return;
    }
    _worldnode->removeChild(lumia->getSceneNode());
    
    std::vector<shared_ptr<LumiaModel>>::iterator position = std::find(_lumiaList.begin(), _lumiaList.end(), lumia);
    if (position != _lumiaList.end())
        _lumiaList.erase(position);
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

//    if (bd1->getName() == LUMIA_NAME){
//        auto lumia = std::make_shared<LumiaModel>((LumiaModel*) bd1);
//        if (lumia->getSensorName() == fd1 && lumia.get() != bd2){
//            lumia->setGrounded(true);
//            // Could have more than one ground
//            _sensorFixtures.emplace(fix2);
//        }
//    }
//    else if (bd2->getName() == LUMIA_NAME){
//        auto lumia = std::make_shared<LumiaModel>((LumiaModel*) bd2);
//        if (lumia->getSensorName() == fd2 && lumia.get() != bd1){
//            lumia->setGrounded(true);
//            // Could have more than one ground
//            _sensorFixtures.emplace(fix1);
//        }
//    }
	// See if we have landed on the ground.
    for (const std::shared_ptr<LumiaModel> &lumia : _lumiaList){
        
        if (bd1->getName().substr(0,5) == PLANT_NAME && bd2 == lumia.get()) {
            if (!((Plant*)bd1)->getIsLit()) {
                ((Plant*)bd1)->lightUp();
            }
        }else if (bd2->getName().substr(0,5) == PLANT_NAME && bd1 == lumia.get()) {
            if (!((Plant*)bd2)->getIsLit()) {
                ((Plant*)bd2)->lightUp();
            }
        }
        if (bd1->getName() == "nrg_" && bd2 == lumia.get()) {
            if (!(bd1->isRemoved())) {
            ((LumiaModel*)bd2)->merge(1);
            _worldnode->removeChild(((EnergyModel*)bd1)->getNode());
            ((EnergyModel*)bd1)->dispose();
            ((EnergyModel*)bd1)->markRemoved(true);
            }
        }
        else if (bd2->getName() == "nrg_" && bd1 == lumia.get()) {
            if (!(bd2->isRemoved())) {
            ((LumiaModel*)bd1)->merge(1);
            _worldnode->removeChild(((EnergyModel*)bd2)->getNode());
            ((EnergyModel*)bd2)->dispose();
            ((EnergyModel*)bd2)->markRemoved(true);
            }
        }
        
	    if ((lumia->getSensorName() == fd2 && lumia.get() != bd1) ||
		    (lumia->getSensorName() == fd1 && lumia.get() != bd2)) {
		    lumia->setGrounded(true);
		    // Could have more than one ground
		    _sensorFixtures.emplace(lumia.get() == bd1 ? fix2 : fix1);
	    }
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

    
    for (const std::shared_ptr<LumiaModel> &lumia : _lumiaList){
        if ((lumia->getSensorName() == fd2 && lumia.get() != bd1) ||
            (lumia->getSensorName() == fd1 && lumia.get() != bd2)) {
            _sensorFixtures.erase(lumia.get() == bd1 ? fix2 : fix1);
            if (_sensorFixtures.empty()) {
                lumia->setGrounded(false);
            }
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
