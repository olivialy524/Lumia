//
//  LMGameScene.cpp
//
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#include "GameScene.h"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "LumiaModel.h"
#include "Plant.h"
#include "PlantNode.h"
#include "EnergyModel.h"
#include "BackgroundNode.h"
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>

using namespace cugl;

#define IN_RANGE(val, rangeMin, rangeMax) (val <= rangeMax && val >= rangeMin ? true : false)

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

#pragma mark -
#pragma mark Gameplay Constants
/** The minimum size a Lumia can be before it is killed */
#define MIN_LUMIA_RADIUS 0.25f
/** The amount of size a Lumia will lose when lighting up a magical plant */
#define PLANT_SIZE_COST 0.25f
/** The amount of size a Lumia will gain when consuming an energy item */
#define ENERGY_SIZE_INC 1.0f
/** The ratio of size Lumia is split into */
#define LUMIA_SPLIT_RATIO 1.4f

#pragma mark -
#pragma mark Physics Constants
/** The new heavier gravity for this world (so it is not so floaty) */
#define DEFAULT_GRAVITY -12.0f
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
/** The size of an energy item */
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

#define SPLIT_NAME      "split"
#define ENERGY_NAME     "energy"
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

#define CAMERA_SPEED 3.0f

#define BACKGROUND_IMAGE "background"

#define LEVEL_NAME "json/techlevel"

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
    _jsonr = cugl::JsonReader::alloc("json/newlevel.json");

    std::shared_ptr<cugl::JsonValue> jv = _jsonr->readJson();
    _leveljson = jv->get("level");
    
    _level = assets->get<LevelModel>("json/newlevel.json");
    _tileManager = assets->get<TileDataModel>("json/tiles.json");
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
   
    _assets = assets;
    _input.init(getBounds());
    
    std::shared_ptr<Texture> bkgTexture = assets->get<Texture>("background");
    std::shared_ptr<BackgroundNode> bkgNode = BackgroundNode::alloc(bkgTexture);
    bkgNode->setPosition(dimen.width/2, dimen.height/2);

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
    
//    scene->setScale(2.0f);// tentatively scale the backgrouns bigger for camera test
//    addChild(scene, 0);
    addChild(bkgNode);
    addChild(_worldnode, 1);
    addChild(_debugnode, 2);
    addChild(_winnode,  3);
    addChild(_losenode, 4);
   

    populate();
    _active = true;
    _complete = false;
    setDebug(false);
    
    getCamera()->setPositionX(_avatar->getAvatarPos().x);
    _cameraTargetX = _avatar->getAvatarPos().x;
    getCamera()->update();
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::BLACK);
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
    _sensorFixtureMap.clear();
    for (const std::shared_ptr<LumiaModel> &l : _lumiaList) {
        l->dispose();
    }
    _lumiaList.clear();
    _avatar = nullptr;

    for (const std::shared_ptr<Plant> &p : _plantList) {
        p->dispose();
    }
    _plantList.clear();
    
    for (const std::shared_ptr<EnergyModel> &e : _energyList) {
        e->dispose();
    }
    _energyList.clear();
    _lumiasToRemove.clear();
    _lumiasToCreate.clear();
    _energiesToRemove.clear();

    _level->resetLevel(LEVEL_NAME);
    setFailure(false);
    setComplete(false);
    populate();
    getCamera()->setPositionX(_avatar->getAvatarPos().x);
    getCamera()->update();
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


#pragma mark : Platforms
    image  = _assets->get<Texture>(EARTH_TEXTURE);
    std::vector<std::shared_ptr<Tile>> platforms = _level->getTiles();
    for (int i = 0; i < platforms.size(); i++) {
        std::shared_ptr<Tile> tile = platforms[i];
        Rect rectangle = Rect(tile->getX(),tile->getY(),tile->getWidth(),tile->getHeight());
        std::shared_ptr<physics2::PolygonObstacle> platobj;
        Poly2 platform(rectangle,false);
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
        // All walls and platforms share the same texture
        image = _assets->get<Texture>(EARTH_TEXTURE);
        sprite = scene2::PolygonNode::allocWithTexture(image,platform);
        addObstacle(platobj,sprite,1);
    }
    
    std::vector<std::shared_ptr<Tile>> irregular_tiles = _level->getIrregularTile();
   
    for (int i=0; i< irregular_tiles.size(); i++){
        std::shared_ptr<Tile> t = irregular_tiles[i];
        cout << t->getAngle() << endl;
        vector<Vec2> tile_data = _tileManager->getTileData(t->getType()-1);
        Spline2 sp = Spline2(tile_data);
        sp.setClosed(true);
        PolySplineFactory ft(&sp);
        ft.calculate(PolySplineFactory::Criterion::DISTANCE, 0.07f);
        std::shared_ptr<physics2::PolygonObstacle> platobj;
        SimpleTriangulator triangulator;
        Poly2 platform = ft.getPath();
        triangulator.set(platform);
        triangulator.calculate();
        platform.setIndices(triangulator.getTriangulation());
        platform.setGeometry(Geometry::SOLID);
        
        platform += Vec2(t->getX(), t->getY());
        platobj = physics2::PolygonObstacle::alloc(platform);
        platobj->setAngle(t->getAngle());
        platobj->setName(std::string(PLATFORM_NAME)+cugl::strtool::to_string(10));
        
           //  Set the physics attributes
        platobj->setBodyType(b2_staticBody);
        platobj->setDensity(BASIC_DENSITY);
        platobj->setFriction(BASIC_FRICTION);
        platobj->setRestitution(BASIC_RESTITUTION);
        platobj->setDebugColor(DEBUG_COLOR);
        platform *= _scale;
        
        image = _assets->get<Texture>("tile1");
        sprite = scene2::PolygonNode::allocWithTexture(image);
        sprite->setAngle(t->getAngle());
        _world->addObstacle(platobj);
        platobj->setDebugScene(_debugnode);
        sprite->setPosition(platform.getBounds().getMidX(), platform.getBounds().getMidY());
        _worldnode->addChild(sprite, 1);
        
    }
    
//    std::vector<Vec2> points = {
//        Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f),Vec2(0.0f, 3.0f),
//        Vec2(0.0f, 3.0f), Vec2(0.0f, 3.0f), Vec2(0.5f, 3.0f),
//        Vec2(0.5f, 3.0f), Vec2(0.5f, 1.75f), Vec2(1.75f, 0.5f),
//        Vec2(3.0f, 0.5f), Vec2(3.0f, 0.5f), Vec2(3.0f, 0.0f),
//        Vec2(3.0f, 0.0f), Vec2(3.0f, 0.0f), Vec2(0.0f, 0.0f),
//        Vec2(0.0f, 0.0f)
//    };
//
//    Spline2 sp = Spline2(points);
//    sp.setClosed(true);
//    PolySplineFactory ft(&sp);
//    ft.calculate(PolySplineFactory::Criterion::DISTANCE, 0.07f);
//    std::shared_ptr<physics2::PolygonObstacle> platobj;
//    SimpleTriangulator triangulator;
//    Poly2 platform = ft.getPath();
//    triangulator.set(platform);
//    triangulator.calculate();
//    platform.setIndices(triangulator.getTriangulation());
//    platform.setGeometry(Geometry::SOLID);
//
//    platform += Vec2(5.0f, 5.0f);
//    platobj = physics2::PolygonObstacle::alloc(platform);
//    platobj->setAngle(1.57f);
//    platobj->setName(std::string(PLATFORM_NAME)+cugl::strtool::to_string(10));
//
//   //  Set the physics attributes
//    platobj->setBodyType(b2_staticBody);
//    platobj->setDensity(BASIC_DENSITY);
//    platobj->setFriction(BASIC_FRICTION);
//    platobj->setRestitution(BASIC_RESTITUTION);
//    platobj->setDebugColor(DEBUG_COLOR);
////    platobj->setPosition(platform.getBounds().getMidX(), platform.getBounds().getMidY() );
//
//
//    platform *= _scale;
//    image = _assets->get<Texture>("tile1");
//    sprite = scene2::PolygonNode::allocWithTexture(image);
//    sprite->setAngle(1.57f);
//    _world->addObstacle(platobj);
//    platobj->setDebugScene(_debugnode);
//    sprite->setPosition(platform.getBounds().getMidX(), platform.getBounds().getMidY());
//    _worldnode->addChild(sprite, 1);
//
    

    

#pragma mark : Energy
    std::shared_ptr<cugl::JsonValue> energies = _leveljson->get("energies");
    for (int i = 0; i < energies->size(); i++) {
        std::shared_ptr<cugl::JsonValue> energy = energies->get(i);
        float ex = energy->getFloat("posx");
        float ey = energy->getFloat("posy");
        Vec2 epos = Vec2(ex, ey);
        createEnergy(epos);
    }
#pragma mark : Plant
    vector<std::shared_ptr<Plant>> plants = _level->getPlants();
    for (int i = 0; i < plants.size(); i++) {
        std::shared_ptr<Texture> image = _assets->get<Texture>("lamp");
        std::shared_ptr<scene2::SceneNode> _sceneNode = scene2::SceneNode::allocWithBounds(image->getSize());
        plants[i]->setNode(_sceneNode);
        _sceneNode->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<PlantNode> sprite = PlantNode::alloc(image);
        sprite->setAnchor(Vec2::ANCHOR_CENTER);
        sprite->setAngle(plants[i]->getAngle());
        _sceneNode->addChild(sprite);
        plants[i]->setVX(0);
        addObstacle(plants[i], _sceneNode, 0);
        _plantList.push_front(plants[i]);
    }

    
#pragma mark : Lumia
    std::shared_ptr<scene2::SceneNode> node = scene2::SceneNode::alloc();
    image = _assets->get<Texture>(LUMIA_TEXTURE);
    _avatar = _level->getLumia();
    _avatar-> setDrawScale(_scale);
    _avatar-> setTextures(image);
    _avatar-> setName(LUMIA_NAME);
	_avatar-> setDebugColor(DEBUG_COLOR);
    _avatar-> setFixedRotation(false);
    _lumiaList.push_back(_avatar);
    
    std::unordered_set<b2Fixture*> fixtures;
    _sensorFixtureMap[_avatar.get()] = fixtures;
	addObstacle(_avatar,_avatar->getSceneNode(), 4); // Put this at the very front
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
    

    if (_lumiasToRemove.size() > 0) {
        for (const std::shared_ptr<LumiaModel>& lumia : _lumiasToRemove) {
            removeLumia(lumia);
        }
        _lumiasToRemove.clear();
    }

    if (_lumiasToCreate.size() > 0) {
        for (const LumiaBody& lumia : _lumiasToCreate) {
            createLumia(lumia.radius, lumia.position, lumia.isAvatar);
        }
        _lumiasToCreate.clear();
    }

    if (_energiesToRemove.size() > 0) {
        for (const std::shared_ptr<EnergyModel>& energy : _energiesToRemove) {
            removeEnergy(energy);
        }
        _energiesToRemove.clear();
    }

    if(_input.didSwitch()){
        cugl::Vec2 tapLocation = _input.getSwitch(); // screen coordinates

        for (auto & lumia : _lumiaList) {
            cugl::Vec2 lumiaPosition = lumia->getPosition() * _scale; // world coordinates
            cugl::Vec3 tapLocationWorld = getCamera()->screenToWorldCoords(tapLocation);
            CULog("lumia: (%f, %f) tap: (%f, %f)", lumiaPosition.x, lumiaPosition.y, tapLocationWorld.x, tapLocation.y);

            float radius = lumia->getRadius() * _scale; // world coordinates
            if (IN_RANGE(tapLocationWorld.x, lumiaPosition.x - radius, lumiaPosition.x + radius) &&
                IN_RANGE(tapLocationWorld.y, lumiaPosition.y - radius, lumiaPosition.y + radius)) {
                _avatar = lumia;
            }
        }
    }

	_avatar->setVelocity(_input.getLaunch());

	// if Lumia is on ground, player can launch Lumia so we should show the projected
    // trajectory if player is dragging
	//if (_avatar->isGrounded() && _input.isDragging()) {
	//	glColor3f(1, 1, 1);
	//	glBegin(GL_LINES);
	//	for (int i = 0; i < 180; i++) { // three seconds at 60fps
	//		Vec2 trajectoryPosition = getTrajectoryPoint(_avatar->getPosition(), _input.getPlannedLaunch(), i, _world, dt);
	//		glVertex2f(trajectoryPosition.x, trajectoryPosition.y);
	//	}
	//	glEnd();
	//}

    //glEnable(GL_POINT_SMOOTH);
    //glPointSize(5);
    _cameraTargetX = _avatar->getAvatarPos().x;
//    getCamera()->setPositionX(_avatar->getAvatarPos().x);
    float currentPosX = getCamera()->getPosition().x;
    float diff = _cameraTargetX - currentPosX;
    if (std::abs(diff) <= 3){
        getCamera()->setPositionX(_cameraTargetX);
    }else{
        float new_pos = currentPosX + CAMERA_SPEED * sgn(diff);
        getCamera()->setPositionX(new_pos);
    }
    getCamera()->update();
    
	_avatar->setLaunching(_input.didLaunch());
	_avatar->applyForce();
    if(_input.didMerge()){
        _avatar->setState(LumiaModel::LumiaState::Merging);
    }else if (_input.didSplit()){
        _avatar->setState(LumiaModel::LumiaState::Splitting);
    }else{
        _avatar->setState(LumiaModel::LumiaState::Idle);
    }
    
    switch (_avatar->getState()){
        case LumiaModel::LumiaState::Splitting:{
            float radius = _avatar->getRadius() / LUMIA_SPLIT_RATIO;
            if (radius > MIN_LUMIA_RADIUS) {
                Vec2 pos = _avatar->getPosition();
                Vec2 offset = Vec2(0.5f + radius, 0.0f);

                // TODO: has issues with potentially spawning Lumia body inside or on the otherside of a wall
                // http://www.iforce2d.net/b2dtut/world-querying
                std::shared_ptr<LumiaModel> temp = _avatar;
                createLumia(radius, pos + offset, true);
                createLumia(radius, pos - offset, false);
                removeLumia(temp);
            }
            break;
        }
        case LumiaModel::LumiaState::Merging:{
            mergeLumiasNearby();
            break;
        }
        case LumiaModel::LumiaState::Idle:{
            break;
        }
    }
            
	// Turn the physics engine crank.
	_world->update(dt);

	// Since items may be deleted, garbage collect
	_world->garbageCollect();

	// Record failure if necessary.
	if (!_failed && _avatar->getY() < 0 || _lumiaList.size() == 0) {
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
//		std::shared_ptr<Sound> source = _assets->get<Sound>(WIN_MUSIC);
//		AudioEngine::get()->getMusicQueue()->play(source, false, MUSIC_VOLUME);
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
//		std::shared_ptr<Sound> source = _assets->get<Sound>(LOSE_MUSIC);
//      AudioEngine::get()->getMusicQueue()->play(source, false, MUSIC_VOLUME);
		_losenode->setVisible(true);
		_countdown = EXIT_COUNT;
	} else {
		_losenode->setVisible(false);
		_countdown = -1;
	}
}


void GameScene::createEnergy(Vec2 pos) {
    std::shared_ptr<Texture> image = _assets->get<Texture>(EARTH_TEXTURE);
    cugl::Size size = Size(2, 2);
    std::shared_ptr<EnergyModel> nrg = EnergyModel::alloc(pos, size);

    nrg->setGravityScale(0);
    nrg->setBodyType(b2_staticBody);
    nrg->setSensor(true);
    nrg->setName(ENERGY_NAME);

    cugl::Rect rectangle = Rect(pos, size);
    cugl::Poly2 poly = Poly2(rectangle);

    std::shared_ptr<cugl::scene2::PolygonNode> pn = cugl::scene2::PolygonNode::alloc(rectangle);
    std::shared_ptr<scene2::PolygonNode> sprite = scene2::PolygonNode::allocWithTexture(image, poly);
    sprite->setScale(_scale);
    nrg->setNode(sprite);

    addObstacle(nrg, sprite, 0);
    _energyList.push_back(nrg);
}

void GameScene::checkWin() {
    for (auto const& i : _plantList) {
        if (!(i->getIsLit())) {
            return;
        }
    }
    setComplete(true);
}

/**
 * Add a new Lumia to the world.
 */
std::shared_ptr<LumiaModel> GameScene::createLumia(float radius, Vec2 pos, bool isAvatar) {
    std::shared_ptr<Texture> image = _assets->get<Texture>(LUMIA_TEXTURE);
    std::shared_ptr<LumiaModel> lumia = LumiaModel::alloc(pos, radius, _scale);
    lumia-> setTextures(image);
    lumia-> setDebugColor(DEBUG_COLOR);
    lumia-> setName(LUMIA_NAME);
    lumia-> setFixedRotation(false);
    lumia-> setDensity(0.1f / radius);
    addObstacle(lumia, lumia->getSceneNode(), 5);
    
    _lumiaList.push_back(lumia);
    std::unordered_set<b2Fixture*> fixtures;
    _sensorFixtureMap[lumia.get()] = fixtures;

    if (isAvatar) {
        _avatar = lumia;
    }

    return lumia;
}

void GameScene::removeLumia(shared_ptr<LumiaModel> lumia) {
    // do not attempt to remove a Lumia that has already been removed
    if (lumia->isRemoved()) {
        return;
    }
    _sensorFixtureMap.erase(lumia.get());
    _worldnode->removeChild(lumia->getSceneNode());

    std::list<shared_ptr<LumiaModel>>::iterator position = std::find(_lumiaList.begin(), _lumiaList.end(), lumia);
    if (position != _lumiaList.end())
        _lumiaList.erase(position);

    lumia->dispose();
    lumia->setDebugScene(nullptr);
    lumia->markRemoved(true);
}

void GameScene::removeEnergy(shared_ptr<EnergyModel> energy) {
    // do not attempt to remove an energy item that has already been removed
    if (energy->isRemoved()) {
        return;
    }
    _worldnode->removeChild(energy->getNode());

    std::list<shared_ptr<EnergyModel>>::iterator position = std::find(_energyList.begin(), _energyList.end(), energy);
    if (position != _energyList.end())
        _energyList.erase(position);

    energy->dispose();
    energy->setDebugScene(nullptr);
    energy->markRemoved(true);
}

void GameScene::mergeLumiasNearby(){
    Vec2 avatarPos = _avatar->getPosition();

    for (const std::shared_ptr<LumiaModel> &lumia : _lumiaList) {
        if (lumia == _avatar){
            continue;
        }
        
        Vec2 lumiaPos = lumia->getPosition();
        float dist = avatarPos.distanceSquared(lumiaPos);

        if (dist < 100.0f){
            //set lumia velocity to move toward avatar
            Vec2 distance = avatarPos-lumiaPos;
            lumia->setLinearVelocity(distance.normalize().scale(5.0f));
        }
    }
}

/**
* Calculates trajectory point one timestep into future
*
* @param startingPosition the position model is located before launching
* @param startingVelocity the velocity model will be launched at during aiming
* @param n timestep
*/
Vec2 GameScene::getTrajectoryPoint(Vec2& startingPosition, Vec2& startingVelocity,
						float n, std::shared_ptr<cugl::physics2::ObstacleWorld> _world, float dt) {
	//velocity and gravity are given per second but we want time step values here
	// float t = 1 / 60.0f; // seconds per time step (at 60fps)
    float t = dt;
	Vec2 stepVelocity = t * startingVelocity; // m/s
	Vec2 stepGravity = t * t * _world->getGravity(); // m/s/s
    return startingPosition + n * stepVelocity + 0.5f * (n * n + n) * stepGravity;
}


#pragma mark -
#pragma mark Collision Handling

void GameScene::processPlantLumiaCollision(float newRadius, const std::shared_ptr<LumiaModel> lumia) {
    // Lumia body remains if above min size, otherwise kill Lumia body
    if (newRadius >= MIN_LUMIA_RADIUS) {
        Vec2 newPosition = Vec2(lumia->getPosition().x, lumia->getPosition().y - PLANT_SIZE_COST);
        struct LumiaBody lumiaNew = { newPosition, newRadius, lumia == _avatar };

        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
        _lumiasToCreate.push_back(lumiaNew);
    } else {
        // if avatar is killed, player is given control of nearest Lumia body
        if (lumia == _avatar) {
            float minDistance = FLT_MAX;
            std::shared_ptr<LumiaModel> closestLumia = NULL;
            for (const std::shared_ptr<LumiaModel>& lumiaOther : _lumiaList) {
                if (lumiaOther == lumia) {
                    continue;
                }

                float distance = lumia->getPosition().distanceSquared(lumiaOther->getPosition());
                if (distance < minDistance) {
                    minDistance = distance;
                    closestLumia = lumiaOther;
                }
            }

            if (closestLumia != NULL) {
                _avatar = closestLumia;
            }
        }
        _lumiasToRemove.push_back(lumia);
        lumia->setRemoved(true);
    }
}

void GameScene::processEnergyLumiaCollision(const std::shared_ptr<EnergyModel> energy, const std::shared_ptr<LumiaModel> lumia) {
    _energiesToRemove.push_back(energy);
    energy->setRemoved(true);

    float newRadius = lumia->getRadius() + ENERGY_SIZE_INC;
    Vec2 newPosition = Vec2(lumia->getPosition().x, lumia->getPosition().y + ENERGY_SIZE_INC);
    struct LumiaBody lumiaNew = { newPosition, newRadius, lumia == _avatar };

    _lumiasToRemove.push_back(lumia);
    lumia->setRemoved(true);
    _lumiasToCreate.push_back(lumiaNew);
}

void GameScene::processLumiaLumiaCollision(const std::shared_ptr<LumiaModel> lumia, const std::shared_ptr<LumiaModel> lumia2) {
    float newRadius = (lumia->getRadius() + lumia2->getRadius()) / LUMIA_SPLIT_RATIO;
    Vec2 newPosition = Vec2(lumia->getPosition().x, lumia->getPosition().y + (newRadius - lumia->getRadius()));
    struct LumiaBody lumiaNew = { newPosition, newRadius, lumia == _avatar || lumia2 == _avatar };

    _lumiasToRemove.push_back(lumia);
    lumia->setRemoved(true);
    _lumiasToRemove.push_back(lumia2);
    lumia2->setRemoved(true);

    _lumiasToCreate.push_back(lumiaNew);
}


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

	// See if we have landed on the ground.
    for (const std::shared_ptr<LumiaModel> &lumia : _lumiaList) {
        if (bd1 != lumia.get() && bd2 != lumia.get()){
            continue;
        }
        // handle collision between magical plant and Lumia
        if (bd1->getName().substr(0,5) == PLANT_NAME && bd2 == lumia.get()) {
            float newRadius = lumia->getRadius() - PLANT_SIZE_COST;

            // Lumia needs enough size to light up a plant and plant must not already be lit
            if (!((Plant*)bd1)->getIsLit() && newRadius > 0.0f) {
                ((Plant*)bd1)->lightUp();
                processPlantLumiaCollision(newRadius, lumia);
            }
        } else if (bd2->getName().substr(0, 5) == PLANT_NAME && bd1 == lumia.get()) {
            float newRadius = lumia->getRadius() - PLANT_SIZE_COST;

            if (!((Plant*)bd2)->getIsLit() && newRadius > 0.0f) {
                ((Plant*)bd2)->lightUp();
                processPlantLumiaCollision(newRadius, lumia);
            }
        }
        // handle collision between energy item and Lumia
        if (bd1->getName() == ENERGY_NAME && bd2 == lumia.get()) {
            for (const std::shared_ptr<EnergyModel>& energy : _energyList) {
                if (energy.get() == bd1 && !energy->getRemoved()) {
                    processEnergyLumiaCollision(energy, lumia);
                    break;
                }
            }
        } else if (bd2->getName() == ENERGY_NAME && bd1 == lumia.get()) {
            for (const std::shared_ptr<EnergyModel>& energy : _energyList) {
                if (energy.get() == bd2 && !energy->getRemoved()) {
                    processEnergyLumiaCollision(energy, lumia);
                    break;
                }
            }
        }
        
        // handle collision between two Lumias
        if (bd1->getName() == LUMIA_NAME && bd2 == lumia.get()) {
            for (const std::shared_ptr<LumiaModel>& lumia2 : _lumiaList) {
                if (lumia2.get() == bd1 && !lumia2->getRemoved()) {
                    processLumiaLumiaCollision(lumia, lumia2);
                    break;
                }
            }
            break;
        } else if (bd2->getName() == LUMIA_NAME && bd1 == lumia.get()) {
            for (const std::shared_ptr<LumiaModel>& lumia2 : _lumiaList) {
                if (lumia2.get() == bd2 && !lumia2->getRemoved()) {
                    processLumiaLumiaCollision(lumia, lumia2);
                    break;
                }
            }
        }

        // handle detection of Lumia and ground
	    if (((lumia->getSensorName() == fd2 && lumia.get() != bd1) ||
		    (lumia->getSensorName() == fd1 && lumia.get() != bd2))) {
		    lumia->setGrounded(true);
		    // Could have more than one ground
            std::unordered_set<b2Fixture*> & sensorFixtures = _sensorFixtureMap[lumia.get()];
		    sensorFixtures.emplace(lumia.get() == bd1 ? fix2 : fix1);
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
            std::unordered_set<b2Fixture*> & sensorFixtures = _sensorFixtureMap[lumia.get()];
            sensorFixtures.erase(lumia.get() == bd1 ? fix2 : fix1);
            if (sensorFixtures.empty()) {
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
