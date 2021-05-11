//
//  UISliderScene.cpp
//  UI Demo
//
//  This module shows off a slider.  CUGL sliders are omnidirectional, using
//  a bounding rectangle to describe the path.  While they can support fancy
//  graphics, this one is a minimal implementation.
//
//  Author: Walker White
//  Version: 1/20/18
//
#include "WinScene.h"

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
bool WinScene::init(const std::shared_ptr<AssetManager>& assets) {
    setName("win");

    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }

    
    _assets = assets;
    auto layer = assets->get<scene2::SceneNode>("winscreen");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    addChild(layer);
    
    std::vector<std::shared_ptr<scene2::SceneNode>> pauseSceneChildren = layer->getChildren();
    for (int i = 4; i < pauseSceneChildren.size(); i++) {
        std::shared_ptr<scene2::Button> button = std::dynamic_pointer_cast<scene2::Button>(pauseSceneChildren[i]);
        _buttons[button->getName()] = button;
        if (button->getName() == "settings") {
            button->addListener([=](const std::string& name, bool down) {
                this->_active = down;
                _nextScene = "settings";
            });
        }
        if (button->getName() == "levelselectbutton") {
            button->addListener([=](const std::string& name, bool down) {
                this->_active = down;
                _nextScene = "levelselect";
            });
        }
        if (button->getName() == "continuebutton") {
            button->addListener([=](const std::string& name, bool down) {
                this->_active = down;
                _nextScene = "win-continue";
            });
        }
        if (button->getName() == "replaybutton") {
            button->addListener([=](const std::string& name, bool down) {
                this->_active = down;
                _nextScene = "win-replay";
            });
        }
        button->activate();
    }
    setActive(_active);
    
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    std::shared_ptr<Sound> source = _assets->get<Sound>("ui");
    AudioEngine::get()->getMusicQueue()->play(source, true, 1.0f);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void WinScene::dispose() {
    _buttons.clear();
    _assets = nullptr;
    Scene2::dispose();
}

/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void WinScene::setActive(bool value) {
    _active = value;
    for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
        if (value && !it->second->isActive()) {
            it->second->activate();
        } else {
            it->second->deactivate();
        }
    }
}
