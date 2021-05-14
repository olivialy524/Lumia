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
#include "BackgroundNode.h"
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <math.h>

using namespace cugl;

#define IN_RANGE(val, rangeMin, rangeMax) (val <= rangeMax && val >= rangeMin ? true : false)

#pragma mark -
#pragma mark Level Geography

/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024
#define SCENE_HEIGHT 576

/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH   50.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT  20.0f

#pragma mark -
#pragma mark Physics Constants
/** The new heavier gravity for this world (so it is not so floaty) */
#define DEFAULT_GRAVITY -13.0f
/** The density for most physics objects */
#define BASIC_DENSITY   0.0f
/** Friction of most platforms */

#define BASIC_FRICTION  0.4f
/** The restitution for all physics objects */
#define BASIC_RESTITUTION   0.1f
/** The number of frame to wait before reinitializing the game */
#define EXIT_COUNT      119
/** The size of an energy item */
#define ENERGY_RADIUS  3.0f


#pragma mark -
#pragma mark Asset Constants
#define AVATAR_INDICATOR "avatar-indicator"
#define SIZE_INDICATOR "size-indicator"
/** The key for the earth texture in the asset manager */
#define EARTH_TEXTURE   "earth"
/** The key for the win door texture in the asset manager */
#define LUMIA_TEXTURE   "lumia"

#define STICKY_TEXTURE  "sticky-wall"

#define ENEMY_TEXTURE   "enemy"
#define ENEMY_CHASE     "enemy-chase"
#define ENEMY_ESCAPE    "enemy-escape"
/** The name of a plant (for object identification) */
#define PLANT_NAME      "plant"

/** The name of a spike (for object identification) */
#define SPIKE_NAME      "spike"

#define LUMIA_NAME      "lumia"

#define BUTTON_NAME     "button"

#define DOOR_NAME       "door"
/** The name of a platform (for object identification) */
#define PLATFORM_NAME   "platform"

#define SPLIT_NAME      "split"

#define DEATH_NAME      "death"

#define ENERGY_NAME     "energy"
/** The font for victory/failure messages */
#define MESSAGE_FONT    "orbitron"
/** The message for winning the game */
#define WIN_MESSAGE     "VICTORY!"
/** The color of the win message */
#define WIN_COLOR       Color4::YELLOW
/** The message for losing the game */
#define LOSE_MESSAGE    "FAILURE!"
/** The color of the lose message */
#define LOSE_COLOR      Color4::RED

#define CAMERA_SPEED 4.0f

#define CAMERA_SHIFT 0.33f

#define WIN_MUSIC "win"

#define CAMERA_UPBOUND 0.85f

#define CAMERA_LOWERBOUND 0.2f

#define GAME_MUSIC "game"

#define LOSE_MUSIC "lose"

#define SPLIT_SOUND "jump"

#define LIGHT_SOUND "pew"

#define DIE_SOUND "die"

#define GROW_SOUND "grow"


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
	_debug(false),
    _canSplit(true),
    _switched(false)
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
bool GameScene::init(const std::shared_ptr<AssetManager>& assets, string level) {
    setName("game");

    _currentLevel = level;
    _level = assets->get<LevelModel>(level);
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
    Size dimen = Application::get()->getDisplaySize();
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    _assets = assets;
    _input = InputController::getInstance();
    _collisionController.init();
    
    std::shared_ptr<Texture> bkgTexture = assets->get<Texture>("background");
    std::shared_ptr<BackgroundNode> bkgNode = BackgroundNode::alloc(bkgTexture);
    bkgNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bkgNode->setPosition(0, 0);
    bkgNode->setScale(dimen.height/bkgTexture->getHeight());
    
   
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
    _scale = dimen.height/rect.size.height;
    _scale *= 1.5f;
//    Vec2 offset((dimen.width-SCENE_WIDTH)/2.0f,(dimen.height-SCENE_HEIGHT)/2.0f);

    _UIscene = assets->get<scene2::SceneNode>("gameUI");
    _UIscene->setContentSize(dimen.width, dimen.height);
    _UIscene->doLayout(); // Repositions the HUD;
    for (auto it : _UIscene->getChildren()) {
        std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(it);
        if (button && button->getName() == "panning"){
            button->addListener([=](const std::string& name, bool down) {
                if (down && _UIscene->isVisible()) {
                    _state = GameState::Paused;
                    _UIscene->setVisible(false);
                    _pausedUI->setVisible(true);
                }
            });
                
        }
        if (button && button->getName() == "pause") {
            button->addListener([=](const std::string& name, bool down) {
                if (down && _UIscene->isVisible()) {
                    _state = GameState::Paused;
                    setActive(false);
                    _nextScene = "pause";
                }
            });
        }
        if (button) {
            button->activate();
        }
    }
        
    _pausedUI = assets->get<scene2::SceneNode>("pausedUI");
    _pausedUI->setContentSize(dimen.width, dimen.height);
    for (auto it : _pausedUI->getChildren()) {
        std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(it);
        if (button->getName() == "exit"){
            button->addListener([=](const std::string& name, bool down) {
                if (down && _pausedUI->isVisible()) {
                    _state = GameState::Playing;
                    _UIscene->setVisible(true);
                    _pausedUI->setVisible(false);
                }
            });
                
        }
        button->activate();
    }
    _pausedUI->setVisible(false);

    _scrollNode = cugl::scene2::PolygonNode::SceneNode::allocWithBounds(_level->getXBound() * _scale, _level->getYBound() * _scale);

    _scrollNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    //_scrollNode->setPosition(0, 0);
    
    // Create the scene graph
    _worldnode = scene2::SceneNode::alloc();
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
//    _worldnode->setPosition(offset);

    _debugnode = scene2::SceneNode::alloc();
    _debugnode->setScale(_scale); // Debug node draws in PHYSICS coordinates
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
//    _debugnode->setPosition(offset);

    _losenode = scene2::Label::alloc(LOSE_MESSAGE, _assets->get<Font>(MESSAGE_FONT));
    _losenode->setAnchor(Vec2::ANCHOR_CENTER);
    _losenode->setPosition(dimen.width/2.0f,dimen.height* 2/3.0f);
    _losenode->setForeground(Color4::WHITE);
    _losenode->setName("losenode");
    _loseAnimation = cugl::scene2::AnimationNode::alloc(assets->get<Texture>("death"), 4, 5, 20);
    _loseAnimation->setAnchor(Vec2::ANCHOR_CENTER);
    _loseAnimation->setPosition(dimen.width/2.0f,dimen.height/3.0f);
    _loseAnimation->setFrame(0);
    _loseAnimation->setName("loseanimation");
    setFailure(false);
    
//    scene->setScale(2.0f);// tentatively scale the backgrouns bigger for camera test
//    addChild(scene, 0);

    _scrollNode->addChild(bkgNode);
    _scrollNode->addChild(_worldnode, 1);
    _scrollNode->addChild(_debugnode, 2);
    _UIscene->addChild(_losenode, 3);
    _UIscene->addChild(_loseAnimation, 4);
    
    addChild(_scrollNode);
    addChild(_UIscene);
    addChild(_pausedUI);
    
    _musicVolume = 1.0f;
    _effectVolume = 1.0f;
    populate();
    _scrollNode->setPosition(-1 * _avatar->getAvatarPos().x + getCamera()->getViewport().size.width * CAMERA_SHIFT, 0);
    _progressLabel = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("gameUI_progress_progresslabel"));
    _progressLabel->setText("0/" + to_string(_plantList.size()));
    _ticks = 0;
    _lastSpikeCollision = NULL;
    setDebug(false);
    
    setActive(true);
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::BLACK);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose() {
    _collisionController.dispose();
    _trajectoryNode->dispose();
    _avatarIndicatorNode->dispose();
    _level->resetLevel();
    _sensorFixtureMap.clear();
    _sensorFixtureMap2.clear();
    _graph.clear();
    if (_UIscene->getChildByName("losenode")) {
        _UIscene->removeChild(_losenode);
    }
    if (_UIscene->getChildByName("loseanimation")) {
        _UIscene->removeChild(_loseAnimation);
    }
    _scrollNode->dispose();
    std::queue<std::shared_ptr<LumiaModel>>().swap(_dyingLumiaQueue);
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

    for (const std::shared_ptr<Door> & d: _doorList) {
        d->dispose();
    }
    _doorList.clear();
            
    for (const std::shared_ptr<Button> & b: _buttonList) {
        b->dispose();
    }
    _buttonList.clear();
            
    for (const std::shared_ptr<EnemyModel> &enemy : _enemyList) {
        enemy->dispose();
    }
    _enemyList.clear();

    for (const std::shared_ptr<scene2::PolygonNode>& t : _tutorialList) {
        t->dispose();
    }
    _tutorialList.clear();

    _world = nullptr;
    _worldnode = nullptr;
    _debugnode = nullptr;
    _losenode = nullptr;
    _progressLabel = nullptr;
    _failed = false;
    _debug = false;
    _state = GameScene::Playing;
    _UIelements.clear();
    Scene2::dispose();
    setActive(false);

    //std::shared_ptr<Sound> source = _assets->get<Sound>(GAME_MUSIC);
    //AudioEngine::get()->getMusicQueue()->play(source, true, _musicVolume);
}

#pragma mark -
#pragma mark Level Layout

/**
 * Resets the status of the game so that we can play again.
 *
 * This method disposes of the world and creates a new one.
 */
void GameScene::reset() {
    _scrollNode->setColor(Color4::WHITE);
    _world->clear();
    _worldnode->removeAllChildren();
    _debugnode->removeAllChildren();
    _sensorFixtureMap.clear();
    _sensorFixtureMap2.clear();
    _graph.clear();
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
    for (const std::shared_ptr<Door> & d: _doorList) {
        d->dispose();
    }
    _doorList.clear();
    
    for (const std::shared_ptr<Button> & b: _buttonList) {
        b->dispose();
    }
    _buttonList.clear();
    
    for (const std::shared_ptr<EnemyModel> &enemy : _enemyList) {
        enemy->dispose();
    }
    _enemyList.clear();
    std::queue<std::shared_ptr<LumiaModel>>().swap(_dyingLumiaQueue);
    _collisionController.clearStates();
    _trajectoryNode->dispose();
    _ticks = 0;
    _lastSpikeCollision = NULL;
    _level->resetLevel();
    setFailure(false);
    populate();
    _scrollNode->setPosition(-1 * _avatar->getAvatarPos().x + getCamera()->getViewport().size.width * CAMERA_SHIFT, 0);
    _progressLabel->setText("0/" + to_string(_plantList.size()));
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
//    float xBound = _level->getXBound();
//    float yBound = _level->getYBound();
//    for (int i = 0; i < xBound; i++){
//        for (int j = 0; j < yBound; j++){
//            _graph[{Vec2(i, j)}] = NodeState::Void;
//        }
//    }
    std::shared_ptr<Texture> image;
    std::shared_ptr<scene2::PolygonNode> sprite;
    

#pragma mark : Platforms
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
        image = _assets->get<Texture>("tile3");
        sprite = scene2::PolygonNode::allocWithTexture(image,platform);
        addObstacle(platobj,sprite,1);
        
        // get bounds and world query within the bounds; if there is tile, mark obstacle on the graph, else remain void
    }
    
    std::vector<std::shared_ptr<Tile>> irregular_tiles = _level->getIrregularTile();
   
    for (int i=0; i< irregular_tiles.size(); i++){
        std::shared_ptr<Tile> t = irregular_tiles[i];
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
        std::shared_ptr<TileModel> tileobj = TileModel::alloc(platform);
        tileobj->setAngle(t->getAngle());
        tileobj->setName(std::string(PLATFORM_NAME)+cugl::strtool::to_string(10));
        tileobj->setDrawScale(_scale);;
        tileobj->setPosition(t->getX(), t->getY());
        image = _assets->get<Texture>(t->getFile());
        tileobj->setTextures(image);
        tileobj->setType(t->getType());
        addObstacle(tileobj, tileobj->getSceneNode(), 1);
    }
 
#pragma mark : Energy
    vector<std::shared_ptr<EnergyModel>> energies = _level->getEnergies();
    image = _assets->get<Texture>("energy");
    for (int i = 0; i < energies.size(); i++) {
        auto energy = energies[i];
        energy->setVX(0);
        energy->setDrawScale(_scale);
        energy->setTextures(image);
        addObstacle(energy, energy->getNode(), 0);
        _energyList.push_front(energy);
    }

#pragma mark : Plants
    vector<std::shared_ptr<Plant>> plants = _level->getPlants();
    image = _assets->get<Texture>("lamp");
    for (int i = 0; i < plants.size(); i++) {
        auto plant = plants[i];
        plant->setDrawScale(_scale);
        plant->setTextures(image, plant->getAngle());
        plant->setVX(0);
        addObstacle(plant, plant->getNode(), 0);
        _plantList.push_front(plant);
    }

#pragma mark : Spikes
    vector<std::shared_ptr<SpikeModel>> spikes = _level->getSpikes();
    image = _assets->get<Texture>("spike");
    for (int i = 0; i < spikes.size(); i++) {
        auto spike = spikes[i];
        spike->setDrawScale(_scale);
        spike->setTextures(image, spike->getAngle());
        spike->setVX(0);
        addObstacle(spike, spike->getNode(), 0);
        _spikeList.push_front(spike);
    }
    
#pragma mark : Buttons & Doors
    std::vector<std::shared_ptr<Button>> buttons = _level->getButtons();
    std::vector<std::shared_ptr<Door>> doors = _level->getDoors();
    for (int i = 0; i < buttons.size(); i++) {
        std::shared_ptr<Button> b = buttons[i];
        std::shared_ptr<Door> d = doors[i];
        d->setName("door " + toString(i));
        image = _assets->get<Texture>(DOOR_NAME);
        d->setDrawScale(_scale);
        d->setTextures(image);
        addObstacle(d,d->getSceneNode(),1);
        _doorList.push_front(d);
        b->setName(BUTTON_NAME);
        image = _assets->get<Texture>(BUTTON_NAME);
        b->setDrawScale(_scale);
        b->setTextures(image);
        addObstacle(b,b->getSceneNode(),1);
        _buttonList.push_front(b);
    }
    
#pragma mark : Sticky Walls
    std::vector<std::shared_ptr<StickyWallModel>> stickyWalls = _level->getStickyWalls();
    image = _assets->get<Texture>(STICKY_TEXTURE);
    for (int i = 0; i < stickyWalls.size(); i++) {
        std::shared_ptr<StickyWallModel> s = stickyWalls[i];
        s->setDrawScale(_scale);
        s->setTextures(image);
        s->setDebugColor(DEBUG_COLOR);
        addObstacle(s, s->getSceneNode(), 1);
    }

#pragma mark : Tutorials
    std::vector<LevelModel::Tutorial> tutorials = _level->getTutorials();
    for (int i = 0; i < tutorials.size(); i++) {
        LevelModel::Tutorial t = tutorials[i];
        image = _assets->get<Texture>(t.texture);
        std::shared_ptr<scene2::PolygonNode> tutorialNode = scene2::PolygonNode::allocWithTexture(image);
        Vec2 pos = Vec2(t.posX, t.posY) * _scale;
        tutorialNode->setPosition(pos);
        tutorialNode->setVisible(false);
        _worldnode->addChild(tutorialNode);

        _tutorialList.push_back(tutorialNode);
    }

#pragma mark : Lumia
    image = _assets->get<Texture>(LUMIA_TEXTURE);
    std::shared_ptr<Texture> split = _assets->get<Texture>(SPLIT_NAME);
    std::shared_ptr<Texture> death = _assets->get<Texture>(DEATH_NAME);
    std::shared_ptr<Texture> indicator = _assets->get<Texture>(SIZE_INDICATOR);
    _avatar = _level->getLumia();
    _avatar->setDrawScale(_scale);
    _avatar->setTextures(image, split, death, indicator);
    _avatar->setName(LUMIA_NAME);
	_avatar->setDebugColor(DEBUG_COLOR);
    _lumiaList.push_back(_avatar);
    std::unordered_set<b2Fixture*> fixtures;
    _sensorFixtureMap[_avatar.get()] = fixtures;
    std::unordered_set<b2Fixture*> fixtures2;
    _sensorFixtureMap2[_avatar.get()] = fixtures2;
	addObstacle(_avatar,_avatar->getSceneNode(), 4); // Put this at the very front
    
#pragma mark : Enemies
    vector<std::shared_ptr<EnemyModel>> enemies = _level->getEnemies();
    
    image = _assets->get<Texture>(ENEMY_ESCAPE);
    std::shared_ptr<Texture> chasing = _assets->get<Texture>(ENEMY_CHASE);
    for (int i = 0; i < enemies.size(); i++) {
        std::shared_ptr<EnemyModel> enemy = enemies[i];
        enemy->setDrawScale(_scale);
        enemy->setTextures(chasing, image);
        enemy->setName(ENEMY_TEXTURE);
        enemy->setDebugColor(DEBUG_COLOR);
        addObstacle(enemy, enemy->getSceneNode(), 3);
        _enemyList.push_back(enemy);
    }
    
    
#pragma mark trajectory
    image = _assets->get<Texture>("dot");

    _trajectoryNode = TrajectoryNode::alloc(image);
    _trajectoryNode->setPosition(0.0, 0.0f);
    _worldnode->addChild(_trajectoryNode);

#pragma mark Avatar Indicator
    image = _assets->get<Texture>(AVATAR_INDICATOR);
    _avatarIndicatorNode = scene2::PolygonNode::allocWithTexture(image);
    Vec2 pos = (_avatar->getPosition() + Vec2(0.0f, _avatar->getRadius()+0.3f)) * _scale;
    _avatarIndicatorNode->setPosition(pos);
    _avatarIndicatorNode->setVisible(false);
    Color4f tint = Color4f(1,1,1,0.6f);
    _avatarIndicatorNode->setColor(tint);
    _worldnode->addChild(_avatarIndicatorNode);

    std::shared_ptr<Sound> source = _assets->get<Sound>(GAME_MUSIC);
    AudioEngine::get()->getMusicQueue()->play(source, true, _musicVolume);
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
        node->setPosition(obj->getPosition() * _scale);
    }
    _worldnode->addChild(node, zOrder);

    // Dynamic objects need constant updating
    if (obj->getBodyType() == b2_dynamicBody) {
        scene2::SceneNode* weak = node.get(); // No need for smart pointer in callback
        obj->setListener([=](physics2::Obstacle* obs) {
            if (!obs->isRemoved()) {
                weak->setPosition(obs->getPosition() * _scale);
                weak->setAngle(obs->getAngle());
            }
            });
    }
}


#pragma mark -
#pragma mark Physics Handling


void GameScene::update(float dt) {
    switch (_state) {
        case GameState::Playing:
            updateGame(dt);
            break;
        case GameState::Paused:
            updatePaused(dt, -1 * _avatar->getAvatarPos().x + getCamera()->getViewport().size.width * CAMERA_SHIFT);
        default:
            break;
    }
    
}


/**
 * Executes the core gameplay loop of this world when on a paused state
 *
 */
void GameScene::updatePaused(float dt, float startX) {
    _input->update(dt);
    if (_input->isDragging()){
        if (!setStart){
            touchstart = _scrollNode->getPositionX();
            setStart = true;
        }
        _scrollNode->setPositionX(touchstart + _input->getCurrentDrag());
    }else{
        setStart = false;
    }
    
    if(!_input->isDragging() && _input->didSwitch()){
        cugl::Vec2 tapLocation = _input->getSwitch(); // screen coordinates

        for (const std::shared_ptr<LumiaModel>& lumia : _lumiaList) {
            cugl::Vec2 lumiaPosition = lumia->getPosition() * _scale; // world coordinates
            cugl::Vec3 tapLocationWorld = getCamera()->screenToWorldCoords(tapLocation) - _scrollNode->getPosition();
            float radius = lumia->getRadius() * _scale; // world coordinates
            if (IN_RANGE(tapLocationWorld.x, (lumiaPosition.x - radius) - 8, (lumiaPosition.x + radius) + 8) &&
                IN_RANGE(tapLocationWorld.y, (lumiaPosition.y - radius) - 8, (lumiaPosition.y + radius) + 8)) {
                _avatar = lumia;
                _state = GameState::Playing;
                _UIscene->setVisible(true);
                _pausedUI->setVisible(false);
            }
        }

    }
    
}


/**
 * Executes the core gameplay loop of this world.
 *
 * @param  delta    Number of seconds since last animation frame
 */

void GameScene::updateGame(float dt) {
    if (_switched){
        _input->clearAvatarStates();
    }
    if (_ticks % 8 == 0){
        _switched = false;
    }
    _input->update(dt);

	// Process the toggled key commands
	if (_input->didDebug()) { setDebug(!isDebug()); }
	if (_input->didReset()) { reset(); }
	if (_input->didExit())  {
		CULog("Shutting down");
		Application::get()->quit();
	}
    

    for (const std::shared_ptr<LumiaModel>& lumia : _collisionController.getLumiasToRemove()) {
        if (lumia->isDying()){
            deactivateLumiaPhysics(lumia);
            _dyingLumiaQueue.push(lumia);
        }else{
            removeLumia(lumia);
        }
    }

    for (const std::shared_ptr<EnemyModel>& enemy : _collisionController.getEnemiesToRemove()) {
        removeEnemy(enemy);
    }

    for (const std::shared_ptr<LumiaModel>& lumia : _collisionController.getLumiasToStick()) {
        lumia->setOnStickyWall(true);
    }

    for (const std::shared_ptr<LumiaModel>& lumia : _collisionController.getLumiasToUnstick()) {
        lumia->unStick();
    }

    for (const CollisionController::LumiaBody& lumia : _collisionController.getLumiasToCreate()) {
        createLumia(lumia.sizeLevel, lumia.position, lumia.isAvatar, lumia.vel, lumia.angularVel);
    }

    for (const std::shared_ptr<EnergyModel>& energy : _collisionController.getEnergiesToRemove()) {
        playGrowSound();
        removeEnergy(energy);
    }

    for (const std::shared_ptr<scene2::PolygonNode>& tutorial : _tutorialList) {
        Vec2 tutorialPos = tutorial->getPosition();
        Vec2 avatarPos = _avatar->getPosition() *_scale;

        if (IN_RANGE(avatarPos.x, tutorialPos.x - 150, tutorialPos.x + 150) &&
            IN_RANGE(avatarPos.y, tutorialPos.y - 150, tutorialPos.y + 150)) {
            tutorial->setVisible(true);
        } else {
            tutorial->setVisible(false);
        }
    }
    _collisionController.clearStates();
    
    for (auto & door : _doorList) {
        if (door->getOpening()) {
            door->setBodyType(b2_dynamicBody);
            door->Open();
        }
        else if (door->getClosing()) {
            door->setBodyType(b2_dynamicBody);
            door->Close();
        }
        else {
            door->setBodyType(b2_staticBody);
        }
    }
    
    for (auto & button : _buttonList) {
        button->incCD();
        if (button->getPushingDown()) {
            button->pushDown();
            if (button->getCD() >= 30) {
                button->resetCD();
            }
            auto lumia = button->getLumia();
            if (lumia->isOnButton()){
                lumia->setStickDirection(button->getPosition()-lumia->getPosition());
            }
        }
        else if (button->getCD() >= 5) {
            button->pushUp();
            button->resetCD();
        }
    }

    // check if Lumia bodies fell out of the level, and remove as needed
    for (const std::shared_ptr<LumiaModel>& lumia : _lumiaList) {
        if (lumia->getY() < 0) {
            if (lumia == _avatar) {
                std::shared_ptr<LumiaModel> temp = _avatar;
                switchToNearestLumia(_avatar);
                _collisionController.addLumiaToRemove(temp);
                temp->setRemoved(true);
            } else {
                _collisionController.addLumiaToRemove(lumia);
                playDieSound();
                lumia->setRemoved(true);
            }
        }
    }

    if(_input->didSwitch()){
        cugl::Vec2 tapLocation = _input->getSwitch(); // screen coordinates

        for (const std::shared_ptr<LumiaModel>& lumia : _lumiaList) {
            cugl::Vec2 lumiaPosition = lumia->getPosition() * _scale; // world coordinates
            cugl::Vec3 tapLocationWorld = getCamera()->screenToWorldCoords(tapLocation) - _scrollNode->getPosition();
            float radius = lumia->getRadius() * _scale; // world coordinates
            if (IN_RANGE(tapLocationWorld.x, (lumiaPosition.x - radius) - 8, (lumiaPosition.x + radius) + 8) &&
                IN_RANGE(tapLocationWorld.y, (lumiaPosition.y - radius) - 8, (lumiaPosition.y + radius) + 8)) {
                _avatar = lumia;
            }
        }
    }
    if (_lumiaList.size() > 1){
        Vec2 pos = (_avatar->getPosition() + Vec2(0.0f, _avatar->getRadius()+0.8f)) * _scale;
        _avatarIndicatorNode->setVisible(true);
        _avatarIndicatorNode->setPosition(pos);
    }else{
        _avatarIndicatorNode->setVisible(false);
    }

	// if Lumia is on ground, player can launch Lumia so we should show the projected
    // trajectory if player is dragging
    if (! (_avatar->isGrounded() && _input->isDragging()) || _ticks % 3 == 0){
        _trajectoryNode->clearPoints();
    }
    
	if (!_avatar->isRemoved()&&_avatar->isGrounded() && _input->isDragging() && _ticks % 3 == 0) {
        Vec2 startPos = _avatar->getPosition();
        float m = _avatar->getMass();
        Vec2 plannedImpulse = _input->getPlannedLaunch();
        Vec2 initialVelocity = plannedImpulse / m;
		for (int i = 1; i < 40; i+=5) {
			Vec2 trajectoryPosition = getTrajectoryPoint(startPos, initialVelocity, i);
            _trajectoryNode->addPoint(trajectoryPosition * _scale);
		}
        float endAlpha = (0.9f*plannedImpulse.lengthSquared()) / pow(_input->getMaximumLaunchVelocity(), 2);
        _trajectoryNode->setEndAlpha(endAlpha);
	}
      

    
    _scrollNode->setPosition(-1 * _avatar->getAvatarPos().x + getCamera()->getViewport().size.width * CAMERA_SHIFT, 0);
    _avatar->setVelocity(_input->getLaunch());
	_avatar->setLaunching(_input->didLaunch());
	_avatar->applyForce();
    if(!_avatar->isRemoved()){
        if(_input->didMerge()){
            _avatar->setState(LumiaModel::LumiaState::Merging);
        }else if (_input->didSplit() && _avatar->getSizeLevel()!=0){
            playSplitSound();
            _avatar->setState(LumiaModel::LumiaState::Splitting);
        }else{
            _avatar->setState(LumiaModel::LumiaState::Idle);
        }
    }
    
    _canSplit = true;
    switch (_avatar->getState()){
        case LumiaModel::LumiaState::Splitting:{
            int currentSizeLevel = _avatar->getSizeLevel();
            Vec2 pos = _avatar->getPosition();
            float radius = LumiaModel::sizeLevels[currentSizeLevel].radius;
            Vec2 offset = Vec2(0.5f + radius, 0.0f);
            if (_avatar->isDoneSplitting() && _world->inBounds(_avatar.get())) {
                Vec2 currentVel = _avatar->getLinearVelocity();
                float currentAngularVel = _avatar->getAngularVelocity();

                Vec2 splitVel1 = Vec2::ZERO;
                Vec2 splitVel2 = Vec2::ZERO;

                if (IN_RANGE(currentVel.x, -1, 1) && currentVel.y > 0) {
                    // Lumia velocity is North
                    splitVel1 = Vec2(currentVel.x - 1.0f, currentVel.y);
                    splitVel2 = Vec2(currentVel.x + 1.0f, currentVel.y);
                } else if (currentVel.x > 1 && currentVel.y > 1) {
                    // Lumia velocity is North East
                    splitVel1 = Vec2(currentVel.x, currentVel.y + 1.0f);
                    splitVel2 = Vec2(currentVel.x, currentVel.y - 1.0f);
                } else if (currentVel.x > 0 && IN_RANGE(currentVel.y, -1, 1)) {
                    // Lumia velocity is East
                    splitVel1 = Vec2(currentVel.x, currentVel.y + 1.0f);
                    splitVel2 = Vec2(currentVel.x, currentVel.y - 1.0f);
                } else if (currentVel.x > 0 && currentVel.y < -1) {
                    // Lumia velocity is South East
                    splitVel1 = Vec2(currentVel.x, currentVel.y + 1.0f);
                    splitVel2 = Vec2(currentVel.x, currentVel.y - 1.0f);
                } else if (IN_RANGE(currentVel.x, -1, 1) && currentVel.y < 0) {
                    // Lumia velocity is South
                    splitVel1 = Vec2(currentVel.x - 1.0f, currentVel.y);
                    splitVel2 = Vec2(currentVel.x + 1.0f, currentVel.y);
                } else if (currentVel.x < -1 && currentVel.y < -1) {
                    // Lumia velocity is South West
                    splitVel1 = Vec2(currentVel.x, currentVel.y + 1.0f);
                    splitVel2 = Vec2(currentVel.x, currentVel.y - 1.0f);
                } else if (currentVel.x < 0 && IN_RANGE(currentVel.y, -1, 1)) {
                    // Lumia velocity is West
                    splitVel1 = Vec2(currentVel.x, currentVel.y + 1.0f);
                    splitVel2 = Vec2(currentVel.x, currentVel.y - 1.0f);
                } else if (currentVel.x < -1 && currentVel.y < 1) {
                    // Lumia velocity is North West
                    splitVel1 = Vec2(currentVel.x, currentVel.y + 1.0f);
                    splitVel2 = Vec2(currentVel.x, currentVel.y - 1.0f);
                }
                removeLumiaNode(_avatar);
                if ((currentSizeLevel + 1) % 2 == 0) {
                    int newSize = ((currentSizeLevel + 1) / 2) - 1;

                    createLumia(newSize,
                        pos + offset,
                        currentVel.x >= 0,
                        splitVel1,
                        currentVel.x >= 0 ? currentAngularVel : -currentAngularVel
                    );
                    createLumia(newSize,
                        pos - offset,
                        currentVel.x < 0,
                        splitVel2,
                        currentVel.x < 0 ? currentAngularVel : -currentAngularVel
                    );
                } else {
                    int newSize = ((currentSizeLevel + 1) / 2) - 1;
                    int newSize2 = newSize + 1;

                    createLumia(newSize,
                        pos + offset,
                        currentVel.x >= 0,
                        splitVel1,
                        currentVel.x >= 0 ? currentAngularVel : -currentAngularVel
                    );
                    createLumia(newSize2,
                        pos - offset,
                        currentVel.x < 0,
                        splitVel2,
                        currentVel.x < 0 ? currentAngularVel : -currentAngularVel
                    );
                }
            } else if (!_avatar->isRemoved() && _world->inBounds(_avatar.get())) {
                std::function< bool(b2Fixture *fixture)> cb = [this](b2Fixture *fixture){
                    b2Body* body = fixture->GetBody();
                    physics2::Obstacle* bd = (physics2::Obstacle*)body->GetUserData();
                    if (bd->getName().substr(0,8) == PLATFORM_NAME) {
                        if (((TileModel*)bd)->getType() == 3){
                            _canSplit = false;
                            return true;
                        }
                    }
                    return false;
                };
                Vec2 leftPos = Vec2(pos.x-offset.x, pos.y-0.1f);
                Rect aabb = Rect(leftPos.x,leftPos.y,offset.x*1.5f,radius);// left bottom x, y, w, h
                _world->queryAABB(cb, aabb);
                if (!_canSplit){
                    _avatar->setState(LumiaModel::LumiaState::Idle);
                    break;
                }
                if (_avatar->getSizeLevel() > 0) {
                    deactivateLumiaPhysics(_avatar);
                }
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
    int size = _dyingLumiaQueue.size();
    for (int i = 0; i < size; i++){
        shared_ptr<LumiaModel> lumia = _dyingLumiaQueue.front();
        _dyingLumiaQueue.pop();
        if (lumia->isDead()){
            if (lumia == _avatar){
                switchToNearestLumia(_avatar);
            }
            playDieSound();
            removeLumiaNode(lumia);
        }else{
            _dyingLumiaQueue.push(lumia);
        }
    }
    if (_ticks % 100 == 0){
        for (auto & enemy : _enemyList){
            std::shared_ptr<LumiaModel> closestLumia;
            Vec2 enemyPos = enemy->getPosition();
            float dist = numeric_limits<float>::infinity();
            for (auto & lumia : _lumiaList){
                Vec2 lumiaPos = lumia->getPosition();
                if (enemyPos.distanceSquared(lumiaPos) < dist){
                    dist = enemyPos.distanceSquared(lumiaPos);
                    closestLumia = lumia;
                }
            }

            if (dist < 50.0f) {
                //set enemy velocity to move away or towards closest Lumia
                Vec2 distance = closestLumia->getPosition() - enemyPos;
                if (closestLumia->getSizeLevel() > enemy->getSizeLevel()) {
                    enemy->setVelocity(-distance.normalize() * 1.5f);
                    enemy->setEscaping();
                } else {
                    enemy->setVelocity(distance.normalize() * 1.5f);
                    enemy->setChasing();
                }
            } else {
                enemy->setVelocity(Vec2::ZERO);
                enemy->setIdle();
            }
            enemy->setInCoolDown(false);
        }
    }

    _ticks++;
//    for (auto & lumia : _lumiaList){
//        Vec2 lastPos = lumia->getLastPosition();
//        _graph[{Vec2(floor(lastPos.x), floor(lastPos.y))}] = NodeState::Void;
//        Vec2 curPos = lumia->getPosition();
//        _graph[{Vec2(floor(curPos.x), floor(curPos.y))}] = NodeState::Lumia;
//    }
//
//    for (auto & enemy : _enemyList){
//        Vec2 lastPos = enemy->getLastPosition();
//        _graph[{Vec2(floor(lastPos.x), floor(lastPos.y))}] = NodeState::Void;
//        Vec2 curPos = enemy->getPosition();
//        _graph[{Vec2(floor(curPos.x), floor(curPos.y))}] = NodeState::Enemy;
//    }
	// Turn the physics engine crank.
	_world->update(dt);

	// Since items may be deleted, garbage collect
	_world->garbageCollect();

	// Record failure if necessary.
	if (!_failed && _lumiaList.size() == 0) {
		setFailure(true);
	}
    
    if (!_failed) {
        checkWin();
    }

	// Reset the game if we win or lose.
	if (_countdown > 0) {
        if (_failed){
            if (_countdown % 6 == 0){
                int frame = _loseAnimation->getFrame() + 1 < 20 ? _loseAnimation->getFrame() + 1 : 19;
                _loseAnimation->setFrame(frame);
            }
        }
        _countdown--;
	} else if (_countdown == 0) {
        _loseAnimation->setFrame(0);
		reset();
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
        AudioEngine::get()->getMusicQueue()->play(source, false, _musicVolume);
		_losenode->setVisible(true);
        _loseAnimation->setVisible(true);
        _scrollNode->setColor(Color4f(0.35f, 0.35f, 0.35f, 1.0f));
		_countdown = EXIT_COUNT;
	} else {
		_losenode->setVisible(false);
        _loseAnimation->setVisible(false);
		_countdown = -1;
	}
}

void GameScene::checkWin() {
    for (auto const& i : _plantList) {
        if (!(i->getIsLit())) {
            return;
        }
    }

    int remainingSize = 0;
    for (auto const& l : _lumiaList) {
        remainingSize += l->getSizeLevel() + 1;
    }
    _remainingSize = remainingSize - 1; // win is counted before the last Lumia touching a plant can be reduced in size
    if (_remainingSize >= _level->getThreeStarScore()) {
        _stars = 3;
    } else if (_remainingSize >= _level->getTwoStarScore()) {
        _stars = 2;
    } else if (_remainingSize >= 0) {
        _stars = 1;
    } else {
        _stars = 0;
    }
    _state = GameState::Paused;
    setActive(false);
    _nextScene = "win";
}

void GameScene::playSplitSound() {
    std::shared_ptr<Sound> source = _assets->get<Sound>(SPLIT_SOUND);
    AudioEngine::get()->play(SPLIT_SOUND, source, false, _effectVolume, true);
}

void GameScene::playLightSound() {
    std::shared_ptr<Sound> source = _assets->get<Sound>(LIGHT_SOUND);
    AudioEngine::get()->play(LIGHT_SOUND, source, false, _effectVolume, true);
}

void GameScene::playDieSound() {
    std::shared_ptr<Sound> source = _assets->get<Sound>(DIE_SOUND);
    AudioEngine::get()->play(DIE_SOUND, source, false, _effectVolume, true);
}

void GameScene::playGrowSound() {
    std::shared_ptr<Sound> source = _assets->get<Sound>(GROW_SOUND);
    AudioEngine::get()->play(GROW_SOUND, source, false, _effectVolume, true);
}
/**
 * Add a new Lumia to the world.
 */
std::shared_ptr<LumiaModel> GameScene::createLumia(int sizeLevel, Vec2 pos, bool isAvatar, Vec2 vel, float angularVel) {
    std::shared_ptr<Texture> image = _assets->get<Texture>(LUMIA_TEXTURE);
    std::shared_ptr<Texture> splitting = _assets->get<Texture>(SPLIT_NAME);
    std::shared_ptr<Texture> death = _assets->get<Texture>(DEATH_NAME);
    std::shared_ptr<Texture> indicator = _assets->get<Texture>(SIZE_INDICATOR);
    std::shared_ptr<LumiaModel> lumia = LumiaModel::alloc(pos, LumiaModel::sizeLevels[sizeLevel].radius, _scale);
    lumia->setDebugColor(DEBUG_COLOR);
    lumia->setName(LUMIA_NAME);
    lumia->setFixedRotation(false);
    lumia->setDensity(LumiaModel::sizeLevels[sizeLevel].density);
    lumia->setLinearVelocity(vel);
    lumia->setAngularVelocity(angularVel);
    lumia->setSizeLevel(sizeLevel);
    lumia->setTextures(image, splitting, death, indicator);

    addObstacle(lumia, lumia->getSceneNode(), 5);
    
    _lumiaList.push_back(lumia);
    std::unordered_set<b2Fixture*> fixtures;
    _sensorFixtureMap[lumia.get()] = fixtures;
    std::unordered_set<b2Fixture*> fixtures2;
    _sensorFixtureMap2[lumia.get()] = fixtures2;

    if (isAvatar) {
        _avatar = lumia;
    }

    return lumia;
}

void GameScene::deactivateLumiaPhysics(shared_ptr<LumiaModel> lumia) {
    // do not attempt to remove a Lumia that has already been removed
    if (_avatar->isRemoved()) {
        return;
    }
    _sensorFixtureMap.erase(lumia.get());
    _sensorFixtureMap2.erase(lumia.get());
    lumia->markRemoved(true);
}

void GameScene::removeLumiaNode(shared_ptr<LumiaModel> lumia) {
    std::list<shared_ptr<LumiaModel>>::iterator position = std::find(_lumiaList.begin(), _lumiaList.end(), lumia);
    if (position != _lumiaList.end())
        _lumiaList.erase(position);
    
    _worldnode->removeChild(lumia->getSceneNode());
    lumia->dispose();
    lumia->setDebugScene(nullptr);
}

void GameScene::removeLumia(shared_ptr<LumiaModel> lumia) {
    // do not attempt to remove a Lumia that has already been removed
    if (lumia->isRemoved()) {
        return;
    }
    _sensorFixtureMap.erase(lumia.get());
    _sensorFixtureMap2.erase(lumia.get());
    _worldnode->removeChild(lumia->getSceneNode());

    std::list<shared_ptr<LumiaModel>>::iterator position = std::find(_lumiaList.begin(), _lumiaList.end(), lumia);
    if (position != _lumiaList.end())
        _lumiaList.erase(position);

    lumia->dispose();
    lumia->setDebugScene(nullptr);
    lumia->markRemoved(true);
}

void GameScene::removeEnemy(shared_ptr<EnemyModel> enemy) {
    // do not attempt to remove a Lumia that has already been removed
    if (enemy->isRemoved()) {
        return;
    }
    playGrowSound();
    _worldnode->removeChild(enemy->getSceneNode());

    std::list<shared_ptr<EnemyModel>>::iterator position = std::find(_enemyList.begin(), _enemyList.end(), enemy);
    if (position != _enemyList.end())
        _enemyList.erase(position);

    enemy->dispose();
    enemy->setDebugScene(nullptr);
    enemy->markRemoved(true);
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

void GameScene::mergeLumiasNearby() {
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

void GameScene::switchToNearestLumia(const std::shared_ptr<LumiaModel> lumia) {
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
    _switched = true;
    if (closestLumia != NULL) {
        _avatar = closestLumia;
    }
}

/**
* Calculates trajectory point one timestep into future
*
* @param startingPosition the position model is located before launching
* @param startingVelocity the velocity model will be launched at during aiming
* @param n timestep
*/
Vec2 GameScene::getTrajectoryPoint(Vec2& startingPosition, Vec2& startingVelocity, float n) {
	//velocity and gravity are given per second but we want time step values here
    float t = 1.0f / 60.0f;
	Vec2 stepVelocity = t * startingVelocity; // m/s
	Vec2 stepGravity = t * t * _world->getGravity(); // m/s/s
    Vec2 estmPos = startingPosition + n * stepVelocity + 0.5f * (n * n + n) * stepGravity;
    return estmPos;
}


#pragma mark -
#pragma mark Collision Handling

bool GameScene::didCollideWithLumiaBody(std::shared_ptr<LumiaModel> lumia, physics2::Obstacle* bd, void* fd){
    return bd == lumia.get() && lumia->getLaunchSensorName() != fd && lumia->getFrictionSensorName() != fd;
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
        if ((bd1 != lumia.get() && bd2 != lumia.get()) || lumia->getRemoved()){
            continue;
        }

        // handle collision between magical plant and Lumia
        if (bd1->getName().substr(0,5) == PLANT_NAME && didCollideWithLumiaBody(lumia, bd2, fd2)) {
            // plant must not already be lit
            if (!((Plant*)bd1)->getIsLit()) {
                ((Plant*)bd1)->lightUp();
                playLightSound();
                _collisionController.processPlantLumiaCollision(lumia->getSmallerSizeLevel(), lumia, lumia == _avatar);

                int numPlantsLit = 0;
                for (const std::shared_ptr<Plant>& p : _plantList) {
                    if (p->getIsLit()) {
                        numPlantsLit++;
                    }
                }
                _progressLabel->setText(to_string(numPlantsLit) + "/" + to_string(_plantList.size()));
            }
        } else if (bd2->getName().substr(0, 5) == PLANT_NAME && didCollideWithLumiaBody(lumia, bd1, fd1)) {
            if (!((Plant*)bd2)->getIsLit()) {
                ((Plant*)bd2)->lightUp();
                playLightSound();
                _collisionController.processPlantLumiaCollision(lumia->getSmallerSizeLevel(), lumia, lumia == _avatar);

                int numPlantsLit = 0;
                for (const std::shared_ptr<Plant>& p : _plantList) {
                    if (p->getIsLit()) {
                        numPlantsLit++;
                    }
                }
                _progressLabel->setText(to_string(numPlantsLit) + "/" + to_string(_plantList.size()));
            }
        // handle collision between spike and Lumia
        } else if((bd1->getName().substr(0, 5) == SPIKE_NAME && didCollideWithLumiaBody(lumia, bd2, fd2)) ||
            (bd2->getName().substr(0, 5) == SPIKE_NAME && didCollideWithLumiaBody(lumia, bd1, fd1))){
            if (_lastSpikeCollision == NULL) {
                _lastSpikeCollision = _ticks;
                _collisionController.processSpikeLumiaCollision(lumia->getSmallerSizeLevel(), lumia, lumia == _avatar);
            } else if (_ticks - _lastSpikeCollision > 30) {
                _lastSpikeCollision = _ticks;
                _collisionController.processSpikeLumiaCollision(lumia->getSmallerSizeLevel(), lumia, lumia == _avatar);
            }
        }
        // handle collision between enemy and Lumia
        else if (bd1->getName() == ENEMY_TEXTURE && didCollideWithLumiaBody(lumia, bd2, fd2)) {
            for (const std::shared_ptr<EnemyModel>& enemy : _enemyList) {
                if (enemy.get() == bd1 && !enemy->getRemoved() && !enemy->getInCoolDown()) {
                    _collisionController.processEnemyLumiaCollision(enemy, lumia, lumia == _avatar);
                    break;
                }
            }
        } else if (bd2->getName() == ENEMY_TEXTURE && didCollideWithLumiaBody(lumia, bd1, fd1)) {
            for (const std::shared_ptr<EnemyModel>& enemy : _enemyList) {
                if (enemy.get() == bd2 && !enemy->getRemoved() && !enemy->getInCoolDown()) {
                    _collisionController.processEnemyLumiaCollision(enemy, lumia, lumia == _avatar);
                    break;
                }
            }
        }
        // handle collision between energy item and Lumia
        else if (bd1->getName() == ENERGY_NAME && didCollideWithLumiaBody(lumia, bd2, fd2)) {
            for (const std::shared_ptr<EnergyModel>& energy : _energyList) {
                if (energy.get() == bd1 && !energy->getRemoved()) {
                    _collisionController.processEnergyLumiaCollision(energy, lumia, lumia == _avatar);
                    break;
                }
            }
        } else if (bd2->getName() == ENERGY_NAME && didCollideWithLumiaBody(lumia, bd1, fd1)) {
            for (const std::shared_ptr<EnergyModel>& energy : _energyList) {
                if (energy.get() == bd2 && !energy->getRemoved()) {
                    _collisionController.processEnergyLumiaCollision(energy, lumia, lumia == _avatar);
                    break;
                }
            }
        }
        else if (bd1->getName() == BUTTON_NAME && lumia->getFrictionSensorName()==fd2) {
            for (const std::shared_ptr<Button>& button : _buttonList) {
                if (button.get() == bd1) {
                    _collisionController.processButtonLumiaCollision(lumia, button);
                    break;
                }
            }
        }
        else if (bd2->getName() == BUTTON_NAME && lumia->getFrictionSensorName()==fd1) {
            for (const std::shared_ptr<Button>& button : _buttonList) {
                if (button.get() == bd2) {
                    _collisionController.processButtonLumiaCollision(lumia, button);
                    break;
                }
            }
        }
        // handle collision between two Lumias
        else if (bd1->getName() == LUMIA_NAME && didCollideWithLumiaBody(lumia, bd2, fd2)) {
            for (const std::shared_ptr<LumiaModel>& lumia2 : _lumiaList) {
                if (lumia2.get() == bd1 && !lumia2->getRemoved() && _avatar->getState() == LumiaModel::LumiaState::Merging) {
                    _collisionController.processLumiaLumiaCollision(lumia, lumia2, lumia == _avatar || lumia2 == _avatar);
                    break;
                }
            }
            break;
        } else if (bd2->getName() == LUMIA_NAME && didCollideWithLumiaBody(lumia, bd1, fd1)) {
            for (const std::shared_ptr<LumiaModel>& lumia2 : _lumiaList) {
                if (lumia2.get() == bd2 && !lumia2->getRemoved() && _avatar->getState() == LumiaModel::LumiaState::Merging) {
                    _collisionController.processLumiaLumiaCollision(lumia, lumia2, lumia == _avatar || lumia2 == _avatar);
                    break;
                }
            }
        }
        else if (bd2->getName()=="STICKY_WALL" && didCollideWithLumiaBody(lumia, bd1, fd1)){
            _collisionController.processStickyWallLumiaCollision(lumia, (StickyWallModel*)bd2);
            
        }
        else if (bd1->getName()=="STICKY_WALL" && didCollideWithLumiaBody(lumia, bd2, fd2)){
            _collisionController.processStickyWallLumiaCollision(lumia, (StickyWallModel*)bd1);
            
        }
        // detect if lumia can launch
        if (((lumia->getLaunchSensorName() == fd2 && lumia.get() != bd1) ||
            (lumia->getLaunchSensorName() == fd1 && lumia.get() != bd2))) {
            lumia->setGrounded(true);
            // Could have more than one ground
            std::unordered_set<b2Fixture*> & sensorFixtures = _sensorFixtureMap[lumia.get()];
            sensorFixtures.emplace(lumia.get() == bd1 ? fix2 : fix1);
        }
        // detect if use friction on lumia
        else if (((lumia->getFrictionSensorName() == fd2 && lumia.get() != bd1) ||
            (lumia->getFrictionSensorName() == fd1 && lumia.get() != bd2))) {
            lumia->setRolling(true);
            // Could have more than one ground
            std::unordered_set<b2Fixture*> & sensorFixtures = _sensorFixtureMap2[lumia.get()];
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

    physics2::Obstacle* bd1 = (physics2::Obstacle*)body1->GetUserData();
    physics2::Obstacle* bd2 = (physics2::Obstacle*)body2->GetUserData();

    
    for (const std::shared_ptr<LumiaModel> &lumia : _lumiaList){
        if ((lumia->getLaunchSensorName() == fd2 && lumia.get() != bd1) ||
            (lumia->getLaunchSensorName() == fd1 && lumia.get() != bd2)) {
            std::unordered_set<b2Fixture*> & sensorFixtures = _sensorFixtureMap[lumia.get()];
            sensorFixtures.erase(lumia.get() == bd1 ? fix2 : fix1);
            if (sensorFixtures.empty()) {
                lumia->setGrounded(false);
            }
        }
        if ((lumia->getFrictionSensorName() == fd2 && lumia.get() != bd1) ||
            (lumia->getFrictionSensorName() == fd1 && lumia.get() != bd2)) {
            std::unordered_set<b2Fixture*> & sensorFixtures = _sensorFixtureMap2[lumia.get()];
            sensorFixtures.erase(lumia.get() == bd1 ? fix2 : fix1);
            if (sensorFixtures.empty()) {
                lumia->setRolling(false);
            }
        }
        if (bd1->getName() == BUTTON_NAME && lumia->getFrictionSensorName()==fd2) {
            for (const std::shared_ptr<Button>& button : _buttonList) {
                if (button.get() == bd1) {
                    _collisionController.processButtonLumiaEnding(lumia, button);
                    break;
                }
            }
        }
        else if (bd2->getName() == BUTTON_NAME && lumia->getFrictionSensorName()==fd1) {
            for (const std::shared_ptr<Button>& button : _buttonList) {
                if (button.get() == bd2) {
                    _collisionController.processButtonLumiaEnding(lumia, button);
                    break;
                }
            }
        }
        else if ((bd2->getName()=="STICKY_WALL" && didCollideWithLumiaBody(lumia, bd1, fd1)) ||
                 (bd1->getName()=="STICKY_WALL" && didCollideWithLumiaBody(lumia, bd2, fd2))){
            _collisionController.processStickyWallLumiaEnding(lumia);
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
//    float ratio1 = dimen.width/dimen.height;
//    float ratio2 = ((float)SCENE_WIDTH)/((float)SCENE_HEIGHT);
//    if (ratio1 < ratio2) {
//        dimen *= SCENE_WIDTH/dimen.width;
//    } else {
//        dimen *= SCENE_HEIGHT/dimen.height;
//    }
    return dimen;
}



void GameScene::render_game(const std::shared_ptr<SpriteBatch>& batch, const std::shared_ptr<SpriteBatch>& UIbatch){
    Scene2::render(batch);
    
    

}
