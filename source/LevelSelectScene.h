//
//  UIButtonScene.h
//  UI Demo
//
//  This module shows off a basic button.  The button is backed by a NinePatch
//  so it can resize dynamically.  It changes the text on a click.
//
//  Author: Walker White
//  Version: 1/20/18
//
#ifndef __LEVEL_SELECT_SCENE_H__
#define __LEVEL_SELECT_SCENE_H__
#include <cugl/cugl.h>
#include "InputController.h"
#include "BackgroundNode.h"

/**
 * A scene for demoing a simple button
 */
class LevelSelectScene : public cugl::Scene2 {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;

    std::unordered_map<std::string, std::shared_ptr<cugl::scene2::Button>> _buttons;
    /** Denotes next scene to switch to after this scene has been deactivated */
    string _nextScene;
    /** The identifier for the level selected by the player */
    string _selectedLevel;
    
    std::shared_ptr<cugl::scene2::SceneNode> _scrollNode;
    std::shared_ptr<cugl::scene2::SceneNode> _UINode;
    
    float _musicVolume = 1.0;
    
    std::shared_ptr<InputController> _input;
    
    float touchstart;
    bool setStart;
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new scene with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    LevelSelectScene() : _nextScene(""), _selectedLevel("") {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~LevelSelectScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    virtual void dispose() override;
    
    void resetAttributes(){
        _nextScene = "";
        _selectedLevel = "";
    }
    
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
    
    static std::shared_ptr<LevelSelectScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<LevelSelectScene> result = std::make_shared<LevelSelectScene>();
        return (result->init(assets) ? result : nullptr);
    }
    
    /**
     * Sets whether the scene is currently active
     *
     * @param value whether the scene is currently active
     */
    virtual void setActive(bool value) override;

    /** Returns the string representing the next scene to transition to */
    string getNextScene() { return _nextScene; };

    /** Returns the string representing the next scene to transition to */
    string getSelectedLevel() { return _selectedLevel; };
    
    virtual void update(float timestep) override;
    
};

#endif /* __LEVEL_SELECT_SCENE_H__ */
