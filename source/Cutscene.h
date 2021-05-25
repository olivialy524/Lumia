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
#ifndef __CUT_SCENE_H__
#define __CUT_SCENE_H__
#include <cugl/cugl.h>
#include "InputController.h"

#define LINE1 "Far, far away in the distant reaches of space,"
#define LINE2 "a ball of light flies through the darkness."
#define LINE3 "This is Lumia,"
#define LINE4 "and it has at long last found an abandoned planet to call home."
#define LINE5 "However, little does Lumia know..."
#define LINE6 "Beneath the lifeless ruins and remains,"
#define LINE7 "are long forgotten dangers of the past..."
#define BREAK "................................................................................................"
#define LINE8 "Lumia's Journey Begins..."
#define LINE9 "(tap for tutorial)"

/**
 * A scene for demoing a simple button
 */
class Cutscene : public cugl::Scene2 {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;

    std::shared_ptr<InputController> _input;

    std::shared_ptr<cugl::scene2::AnimationNode> _prologueAnimation1;
    std::shared_ptr<cugl::scene2::AnimationNode> _prologueAnimation2;

    int _tapped;

    string _sceneType;
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
    Cutscene() : _nextScene("") {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~Cutscene() { dispose(); }
    
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
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, string sceneType);
    
    static std::shared_ptr<Cutscene> alloc(const std::shared_ptr<cugl::AssetManager>& assets, string sceneType) {
        std::shared_ptr<Cutscene> result = std::make_shared<Cutscene>();
        return (result->init(assets, sceneType) ? result : nullptr);
    }
    
    /**
     * Sets whether the scene is currently active
     *
     * @param value whether the scene is currently active
     */
    virtual void setActive(bool value) override;

    /** Returns the string representing the next scene to transition to */
    string getNextScene() { return _nextScene; };

    virtual void update(float timestep) override;
};

#endif /* __CUT_SCENE_H__ */
