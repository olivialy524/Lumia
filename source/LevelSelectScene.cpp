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
bool LevelSelectScene::init(const std::shared_ptr<AssetManager>& assets) {
    setName("levelselect");
    _input = InputController::getInstance();
    _input->init();
    
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    
    _assets = assets;
    
    
    _scrollNode = cugl::scene2::PolygonNode::SceneNode::allocWithBounds(dimen * 2);
    _scrollNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _scrollNode->setPosition(0.0f, 0.0f);
    
    auto layer = assets->get<scene2::SceneNode>("levelselect");
    _UINode = assets->get<scene2::SceneNode>("levelSelectUI");
    _UINode->setContentSize(dimen);
    _UINode->doLayout(); // This rearranges the children to fit the screen
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    
    std::shared_ptr<Texture> bkgTexture = assets->get<Texture>("levelselectbg");
    std::shared_ptr<BackgroundNode> bkgNode = BackgroundNode::alloc(bkgTexture);
    bkgNode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    bkgNode->setPosition(0, 0);
    bkgNode->setScale(dimen.height/bkgTexture->getHeight());
    _scrollNode->addChild(bkgNode);
    _scrollNode->addChild(layer);
    addChild(_scrollNode);
    addChild(_UINode);
    

    auto levelbuttons = layer->getChildren();
    auto UIbuttons = _UINode->getChildren();

    
    int count = 0;
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

    for (auto it = levelbuttons.begin(); it != levelbuttons.end(); ++it) {
        std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(*it);
        _buttons[button->getName()] = button;
   
        button->addListener([=](const std::string& name, bool down) {
            if (!_input->isDragging()){
            this->_active = down;
            _nextScene = "game";
            _selectedLevel = "json/level" + std::to_string(count+1) + ".json";
            std::cout << _selectedLevel << std::endl;
            }

        });
        
        button->activate();
        count++;
    }
    setActive(_active);
    
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
void LevelSelectScene::setActive(bool value) {
    _active = value;
    if (! value){
        setStart = false;
    }
    for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
        if (value && !it->second->isActive()) {
            it->second->activate();
        } else {
            it->second->deactivate();
        }
    }
}


void LevelSelectScene::update(float timestep){
    _input->update(timestep);
    cugl::Scene2::update(timestep);
    if (_input->isDragging()){
        if (!setStart){
            touchstart = _scrollNode->getPositionX();
            setStart = true;
        }
        _scrollNode->setPositionX(touchstart + _input->getCurrentDrag());
    }else{
        setStart = false;
    }
    
}
