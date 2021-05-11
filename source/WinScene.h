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
#ifndef __WIN_SCENE_H__
#define __WIN_SCENE_H__

#include <cugl/cugl.h>
#include "BackgroundNode.h"

/**
 * A scene for demoing a slider
 */
class WinScene : public cugl::Scene2 {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::unordered_map<std::string, std::shared_ptr<cugl::scene2::Button>> _buttons;

    /** Denotes next scene to switch to after this scene has been deactivated */
    string _nextScene;
    
    std::shared_ptr<cugl::scene2::SceneNode> _wrapperNode;
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new scene with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    WinScene() : _nextScene("") {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~WinScene() { dispose(); }
    
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
    
    static std::shared_ptr<WinScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<WinScene> result = std::make_shared<WinScene>();
        return (result->init(assets) ? result : nullptr);
    }
    
    /**
     * Sets whether the scene is currently active
     *
     * @param value whether the scene is currently active
     */
    virtual void setActive(bool value) override;

    /** Returns the string representing the next scene to transition to */
    string getNextScene() { return _nextScene; }

    void setLevelNumber(const std::shared_ptr<cugl::AssetManager>& assets, string value) {
        std::shared_ptr<cugl::scene2::Label> levelLabel = std::dynamic_pointer_cast<cugl::scene2::Label>(assets->get<cugl::scene2::SceneNode>("winscreen_levelbanner_levellabel"));
        levelLabel->setText(value);
    }

    void setWinLabel(const std::shared_ptr<cugl::AssetManager>& assets, string value) {
        std::shared_ptr<cugl::scene2::Label> winLabel = std::dynamic_pointer_cast<cugl::scene2::Label>(assets->get<cugl::scene2::SceneNode>("winscreen_winlabel"));
        winLabel->setText(value);
    }
};

#endif /* __WIN_SCENE_H__ */
