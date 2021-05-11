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
#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include <cugl/cugl.h>
#include "BackgroundNode.h"
/**
 * A scene for demoing a slider
 */
class PauseScene : public cugl::Scene2 {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::unordered_map<std::string, std::shared_ptr<cugl::scene2::Button>> _buttons;

    /** Denotes next scene to switch to after this scene has been deactivated */
    string _nextScene;

    int _totalEnemies;
    int _defeatedEnemies;

    int _totalPlants;
    int _litPlants;
    
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
    PauseScene() : _nextScene("") {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~PauseScene() { dispose(); }
    
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
    
    static std::shared_ptr<PauseScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<PauseScene> result = std::make_shared<PauseScene>();
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
        std::shared_ptr<cugl::scene2::Label> levelLabel = std::dynamic_pointer_cast<cugl::scene2::Label>(assets->get<cugl::scene2::SceneNode>("pausescreen_levelicon_label"));
        levelLabel->setText(value);
    }

    int getTotalEnemies() { return _totalEnemies; }
    void setTotalEnemies(int value) { _totalEnemies = value; }
    int getDefeatedEnemies() { return _defeatedEnemies; }
    void setDefeatedEnemies(int value) { _defeatedEnemies = value; }

    int getTotalPlants() { return _totalPlants; }
    void setTotalPlants(int value) { _totalPlants = value; }
    int getLitPlants() { return _litPlants; }
    void setLitPlants(int value) { _litPlants = value; }
};

#endif /* __PAUSE_SCENE_H__ */
