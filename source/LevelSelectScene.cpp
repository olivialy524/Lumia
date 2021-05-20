//
//  UIButtonScene.cpp
//  UI Demo
//
//  This module shows off a basic button.  The button is backed by a NinePatch
//  so it can resize dynamically.  It changes the text on a click.
//
//  Author: Walker White
//  Version: 1/20/18
//
#include "LevelSelectScene.h"

using namespace cugl;

/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool LevelSelectScene::init(const std::shared_ptr<AssetManager>& assets, std::shared_ptr<cugl::JsonValue> saveFile) {
    setName("levelselect");

    _input = InputController::getInstance();
    _input->init();
    
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
//    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    
    
    _assets = assets;
    
    

    
    auto layer = assets->get<scene2::SceneNode>("levelselect");
    _UINode = assets->get<scene2::SceneNode>("levelSelectUI");

    
    _UINode->setContentSize(dimen);
    _UINode->doLayout(); // This rearranges the children to fit the screen
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    
    std::shared_ptr<Texture> bkgTexture = assets->get<Texture>("background");
    std::shared_ptr<BackgroundNode> bkgNode = BackgroundNode::alloc(bkgTexture);
    bkgNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bkgNode->setPosition(0,0.0f);
    
    _scrollNode = cugl::scene2::PolygonNode::SceneNode::allocWithBounds(bkgTexture->getWidth(), bkgTexture->getHeight());
    _scrollNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    
    _scrollNode->addChild(bkgNode);
    
    std::shared_ptr<Texture> tile3 = assets->get<Texture>("tile3");
    std::shared_ptr<Texture> tile4 = assets->get<Texture>("tile4");
    
    float grpw = 1840.0f;
    for (int i = 0; i< 3; i++){
    addTileGroup(i * grpw + 50.0f, tile3, tile4);
    }
//    _scrollNode->addChild(tileNode);
    _scrollNode->addChild(layer);
    _scrollNode->setScale(dimen.height/bkgTexture->getHeight());
    _scrollNode->setPosition(0.0f, 0.0f);
    
    
    
    addChild(_scrollNode);
    addChild(_UINode);
    
    

    auto levelbuttons = layer->getChildren();
    auto UIbuttons = _UINode->getChildren();

    
  
    for (int i = 1; i <UIbuttons.size(); i++) {
        std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(UIbuttons[i]);
        if (button->getName() == "settings") {
            button->addListener([=](const std::string& name, bool down) {
                this->_active = down;
                _nextScene = "settings";
            });
        }
        button->activate();
    }
    
    int count = 1;
    for (auto it = levelbuttons.begin(); it != levelbuttons.end(); ++it) {
        std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(*it);
        _buttons[button->getName()] = button;
        
        if (count <= 3) {
            button->addListener([=](const std::string& name, bool down) {
                if (!_input->isDragging()){
                    this->_active = down;
                    _nextScene = "game";
                    _selectedLevel = "json/tutorial" + std::to_string(count) + ".json";
                }
            });
       } else {
           button->addListener([=](const std::string& name, bool down) {
               if (!_input->isDragging()){
                   this->_active = down;
                   _nextScene = "game";
                   _selectedLevel = "json/level" + std::to_string(count - 3) + ".json";
               }
           });
       }
        button->setPosition(buttonPositions[count-1]);
        button->setScale(1.8f);
        button->activate();
        count++;
    }
    setActive(_active, saveFile);
    
    lockLevels(assets, saveFile);

    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LevelSelectScene::dispose() {
    _buttons.clear();
    _assets = nullptr;
    Scene2::dispose();
}

/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void LevelSelectScene::setActive(bool value, std::shared_ptr<cugl::JsonValue> saveFile) {
    _active = value;
    if (!value){
        setStart = false;
    }

    if (!value) {
        // deactivate all buttons if setActive(false)
        for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
            it->second->deactivate();
        }
    } else {
        // only activate unlocked levels if setActive(true)
        auto layer = _assets->get<scene2::SceneNode>("levelselect");
        auto levelbuttons = layer->getChildren();

        std::shared_ptr<cugl::JsonValue> levelSaves = saveFile->get("level_saves");
        // order of levelbuttons in assets.json must be in same order as in save.json
        for (int i = 0; i < levelbuttons.size(); i++) {
            std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(levelbuttons[i]);
            std::shared_ptr<cugl::JsonValue> level = levelSaves->get(i);

            if (level->getBool("completed")) {
                std::dynamic_pointer_cast<scene2::TexturedNode>(button->getChildByName("up"))->setTexture(_assets->get<Texture>("level_complete"));
                std::dynamic_pointer_cast<scene2::Label>(button->getChildByName("up")->getChildByName("label"))->setForeground(Color4::WHITE);
            } else {
                std::dynamic_pointer_cast<scene2::TexturedNode>(button->getChildByName("up"))->setTexture(_assets->get<Texture>("level_incomplete"));
                std::dynamic_pointer_cast<scene2::Label>(button->getChildByName("up")->getChildByName("label"))->setForeground(Color4::BLACK);
            }

            if (level->getBool("unlocked")) {
                CULog((level->getString("name") + " unlocked").c_str());
                if (!button->isActive()) {
                    button->activate();
                }
                button->setColor(Color4::WHITE);
            } else {
                CULog((level->getString("name") + " locked").c_str());
                if (button->isActive()) {
                    button->deactivate();
                }
                button->setColor(Color4::RED);
            }
        }
    }
    /*for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
        if (value && !it->second->isActive()) {
            it->second->activate();
        } else {
            it->second->deactivate();
        }
    }*/
}


void LevelSelectScene::update(float timestep){
    _input->update(timestep);
    cugl::Scene2::update(timestep);
    if (_input->isDragging()){
        if (!setStart){
            touchstart = _scrollNode->getPositionX();
            setStart = true;
        }
        float target = touchstart + _input->getCurrentDrag();
        if (target > 0){
            target = 0;
        }
        float limit = -1 * _scrollNode->getWidth() -  Application::get()->getDisplaySize().width/2;
        if (target < limit){
            target = limit ;
        }
        _scrollNode->setPositionX(target);
        
    }else{
        setStart = false;
    }
    
}

void LevelSelectScene::lockLevels(const std::shared_ptr<AssetManager>& assets, std::shared_ptr<cugl::JsonValue> saveFile) {
    auto layer = assets->get<scene2::SceneNode>("levelselect");
    auto levelbuttons = layer->getChildren();

    std::shared_ptr<cugl::JsonValue> levelSaves = saveFile->get("level_saves");
    // order of levelbuttons in assets.json must be in same order as in save.json
    for (int i = 0; i < levelbuttons.size(); i++) {
        std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(levelbuttons[i]);
        std::shared_ptr<cugl::JsonValue> level = levelSaves->get(i);

        if (level->getBool("completed")) {
            std::dynamic_pointer_cast<scene2::TexturedNode>(button->getChildByName("up"))->setTexture(assets->get<Texture>("level_complete"));
            std::dynamic_pointer_cast<scene2::Label>(button->getChildByName("up")->getChildByName("label"))->setForeground(Color4::WHITE);
        } else {
            std::dynamic_pointer_cast<scene2::TexturedNode>(button->getChildByName("up"))->setTexture(assets->get<Texture>("level_incomplete"));
            std::dynamic_pointer_cast<scene2::Label>(button->getChildByName("up")->getChildByName("label"))->setForeground(Color4::BLACK);
        }

        if (level->getBool("unlocked")) {
            CULog((level->getString("name") + " unlocked").c_str());
            if (!button->isActive()) {
                button->activate();
            }
            button->setColor(Color4::WHITE);
        } else {
            CULog((level->getString("name") + " locked").c_str());
            if (button->isActive()) {
                button->deactivate();
            }
            button->setColor(Color4::RED);
        }
    }
}

void LevelSelectScene::addTileGroup(float offset, std::shared_ptr<Texture> tile3, std::shared_ptr<Texture> tile4 ){
    float scale = 40.0f/tile3->getHeight();
    float tilew = tile4->getWidth();
    float tileh = tile3->getHeight();
    float ypos = 600.0f;
    
    std::shared_ptr<LevelSelectTile> tileNode = LevelSelectTile::alloc(tile3);
    tileNode->setAnchor(Vec2::ANCHOR_CENTER);
    tileNode->setPosition(offset + 120.0f, ypos);
    tileNode->setScale(scale);
    _scrollNode->addChild(tileNode);
    std::shared_ptr<LevelSelectTile> tileNode2 = LevelSelectTile::alloc(tile4);
    tileNode2->setAnchor(Vec2::ANCHOR_CENTER);
    tileNode2->setPosition(offset + 360.0f, ypos - 120.0f + 20.0f);
    tileNode2->setScale(scale);
    tileNode2->setAngle(1.57f);
    _scrollNode->addChild(tileNode2);
    
    std::shared_ptr<LevelSelectTile> tileNode3 = LevelSelectTile::alloc(tile4);
    tileNode3->setAnchor(Vec2::ANCHOR_CENTER);
    tileNode3->setPosition(offset + 600.0f - 40.0f , ypos - 350.0f + 15.0f );
    tileNode3->setScale(scale);
    tileNode3->setAngle(4.71f);
    _scrollNode->addChild(tileNode3);
    
    std::shared_ptr<LevelSelectTile> tileNode4 = LevelSelectTile::alloc(tile3);
    tileNode4->setAnchor(Vec2::ANCHOR_CENTER);
    tileNode4->setPosition(offset + 800.0f , ypos - tilew*4/2 * scale + tileh * scale );
    tileNode4->setScale(scale);
    tileNode4->setAngle(0);
    _scrollNode->addChild(tileNode4);
    
    std::shared_ptr<LevelSelectTile> tileNode5 = LevelSelectTile::alloc(tile3);
    tileNode5->setAnchor(Vec2::ANCHOR_CENTER);
    tileNode5->setPosition(offset + 1040.0f , ypos - tilew*4/2 * scale + tileh * scale );
    tileNode5->setScale(scale);
    tileNode5->setAngle(0);
    _scrollNode->addChild(tileNode5);
    
    std::shared_ptr<LevelSelectTile> tileNode6 = LevelSelectTile::alloc(tile4);
    tileNode6->setAnchor(Vec2::ANCHOR_CENTER);
    tileNode6->setPosition(offset + 1280.0f , ypos - tilew*3/2 * scale + 0.5 * tileh * scale );
    tileNode6->setScale(scale);
    tileNode6->setAngle(0);
    _scrollNode->addChild(tileNode6);
    
    std::shared_ptr<LevelSelectTile> tileNode7 = LevelSelectTile::alloc(tile4);
    tileNode7->setAnchor(Vec2::ANCHOR_CENTER);
    tileNode7->setPosition(offset + 1480.0f , ypos - tilew*1/2 * scale + 0.5 * tileh * scale );
    tileNode7->setScale(scale);
    tileNode7->setAngle(3.14f);
    _scrollNode->addChild(tileNode7);
    
    
    std::shared_ptr<LevelSelectTile> tileNode8 = LevelSelectTile::alloc(tile3);
    tileNode8->setAnchor(Vec2::ANCHOR_CENTER);
    tileNode8->setPosition(offset + 1720.0f , ypos );
    tileNode8->setScale(scale);
    tileNode8->setAngle(3.14f);
    _scrollNode->addChild(tileNode8);
    
}
