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
#include "SettingsScene.h"

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
bool SettingsScene::init(const std::shared_ptr<AssetManager>& assets) {
    setName("settings");

    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }

    
    _assets = assets;
    auto layer = assets->get<scene2::SceneNode>("settings");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    addChild(layer);
    
    _musicSlider = std::dynamic_pointer_cast<scene2::Slider>(assets->get<scene2::SceneNode>("settings_musicslider"));
    _effectSlider = std::dynamic_pointer_cast<scene2::Slider>(assets->get<scene2::SceneNode>("settings_effectslider"));
    
    _musicSlider->addListener([=](const std::string& name, float value) {
        _musicVolume = value;
    });

    _effectSlider->addListener([=](const std::string& name, float value) {
        _effectVolume = value;
    });


    _close = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("settings_close"));
    _close->addListener([=](const std::string& name, bool down) {
        this->_active = down;
        _nextScene = "levelselect";
    });
    
    setActive(_active);
    
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    
    std::shared_ptr<Sound> source = _assets->get<Sound>("ui");
    AudioEngine::get()->getMusicQueue()->play(source, true, _musicVolume);
    
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void SettingsScene::dispose() {
    _musicSlider = nullptr;
    _effectSlider = nullptr;
    _assets = nullptr;
    Scene2::dispose();
}

/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void SettingsScene::setActive(bool value) {
    _active = value;
    if (value && !_musicSlider->isActive()) {
        _musicSlider->activate();
    } else if (!value && _musicSlider->isActive()) {
        _musicSlider->deactivate();
    }

    if (value && !_effectSlider->isActive()) {
        _effectSlider->activate();
    } else if (!value && _effectSlider->isActive()) {
        _effectSlider->deactivate();
    }

    if (value && !_close->isActive()) {
        _close->activate();
    } else if (!value && _close->isActive()) {
        _close->deactivate();
    }
}
