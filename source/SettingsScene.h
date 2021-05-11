//
//  UISliderScene.h
//  UI Demo
//
//  This module shows off a slider.  CUGL sliders are omnidirectional, using
//  a bounding rectangle to describe the path.  While they can support fancy
//  graphics, this one is a minimal implementation.
//
//  Author: Walker White
//  Version: 1/20/18
//
#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#include <cugl/cugl.h>

/**
 * A scene for demoing a slider
 */
class SettingsScene : public cugl::Scene2 {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::scene2::Slider> _musicSlider;
    std::shared_ptr<cugl::scene2::Slider> _effectSlider;
    std::shared_ptr<cugl::scene2::Button> _close;

    /** Denotes next scene to switch to after this scene has been deactivated */
    string _nextScene;
    /** Volume to set music level to */
    float _musicVolume;
    /** Volume to set sound effect level to */
    float _effectVolume;
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new scene with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    SettingsScene() : _musicVolume(1.0f), _effectVolume(1.0f) {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~SettingsScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    virtual void dispose() override;
    
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
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    static std::shared_ptr<SettingsScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<SettingsScene> result = std::make_shared<SettingsScene>();
        return (result->init(assets) ? result : nullptr);
    }
    
    /**
     * Sets whether the scene is currently active
     *
     * @param value whether the scene is currently active
     */
    virtual void setActive(bool value) override;

    void setNextScene(string value) { _nextScene = value; }
    /** Returns the string representing the next scene to transition to */
    string getNextScene() { return _nextScene; };

    /** Returns the set music volumd */
    float getMusicVolume() { return _musicVolume; };

    /** Returns the set sound effect volume */
    float getEffectVolume() { return _effectVolume; };
};

#endif /* __SETTINGS_SCENE_H__ */
