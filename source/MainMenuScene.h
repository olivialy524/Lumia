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
#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__
#include <cugl/cugl.h>

/**
 * A scene for demoing a simple button
 */
class MainMenuScene : public cugl::Scene2 {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;

    std::shared_ptr<cugl::scene2::Button> _button;

    /** Denotes next scene to switch to after this scene has been deactivated */
    string _nextScene;
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new scene with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    MainMenuScene() : _nextScene("") {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~MainMenuScene() { dispose(); }
    
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
    
    static std::shared_ptr<MainMenuScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<MainMenuScene> result = std::make_shared<MainMenuScene>();
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

};

#endif /* __MAIN_MENU_SCENE_H__ */
