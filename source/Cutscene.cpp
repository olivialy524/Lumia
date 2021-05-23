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
#include "Cutscene.h"

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
bool Cutscene::init(const std::shared_ptr<AssetManager>& assets, string sceneType) {
    setName("cutscene");

    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    _assets = assets;
    auto layer = assets->get<scene2::SceneNode>("cutscene");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    addChild(layer);
    
    _buttonNext = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("cutscene_next"));
    _buttonPrev = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("cutscene_previous"));
    _sceneIndex = 1;

    if (sceneType == "prologue") {
        std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_prologue1"))->setVisible(true);
        std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_prologue2"))->setVisible(false);
        std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_epilogue1"))->setVisible(false);
        std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_epilogue2"))->setVisible(false);
        std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("cutscene_previous"))->setVisible(false);

        _buttonNext->addListener([=](const std::string& name, bool down) {
            if (!down) {
                CULog("%i", _sceneIndex);
                if (_sceneIndex == 2) {
                    this->_active = down;
                    _nextScene = "levelselect";
                } else if (_sceneIndex == 1) {
                    _sceneIndex += 1;
                    std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_prologue1"))->setVisible(false);
                    std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_prologue2"))->setVisible(true);
                    std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("cutscene_previous"))->setVisible(true);
                }
            }
        });

        _buttonPrev->addListener([=](const std::string& name, bool down) {
            if (!down) {
                if (_sceneIndex == 2) {
                    _sceneIndex -= 1;
                    std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_prologue1"))->setVisible(true);
                    std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_prologue2"))->setVisible(false);
                    std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("cutscene_previous"))->setVisible(false);
                }
            }
        });
    } else if (sceneType == "epilogue") {
        std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_prologue1"))->setVisible(false);
        std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_prologue2"))->setVisible(false);
        std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_epilogue1"))->setVisible(true);
        std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_epilogue2"))->setVisible(false);
        std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("cutscene_previous"))->setVisible(false);

        _buttonNext->addListener([=](const std::string& name, bool down) {
            if (!down) {
                if (_sceneIndex == 2) {
                    this->_active = down;
                    _nextScene = "win";
                } else if (_sceneIndex == 1) {
                    _sceneIndex += 1;
                    std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_epilogue1"))->setVisible(false);
                    std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_epilogue2"))->setVisible(true);
                    std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("cutscene_previous"))->setVisible(true);
                }
            }
        });

        _buttonPrev->addListener([=](const std::string& name, bool down) {
            if (!down) {
                if (_sceneIndex == 2) {
                    _sceneIndex -= 1;
                    std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_epilogue1"))->setVisible(true);
                    std::dynamic_pointer_cast<scene2::TexturedNode>(assets->get<scene2::SceneNode>("cutscene_epilogue2"))->setVisible(false);
                    std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("cutscene_previous"))->setVisible(false);
                }
            }
        });
    }
    

    if (_active) {
        _buttonNext->activate();
        _buttonPrev->activate();
    }
    
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void Cutscene::dispose() {
    _buttonNext = nullptr;
    _buttonPrev = nullptr;
    _assets = nullptr;
    Scene2::dispose();
}

/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void Cutscene::setActive(bool value) {
    _active = value;
    if (value) {
        _buttonNext->activate();
        _buttonPrev->activate();
    } else {
        _buttonNext->deactivate();
        _buttonPrev->deactivate();
    }
}

