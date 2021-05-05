//
//  LMApp.h
//  This file will be used as reference for building the root class of the game Lumia
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#ifndef __LUMIA_APP_H__
#define __LUMIA_APP_H__
#include <cugl/cugl.h>
#include "GameScene.h"
#include "LoadingScene.h"
#include "LevelSelectScene.h"
#include "MainMenuScene.h"
#include "SettingsScene.h"
#include "PauseScene.h"
#include "InputController.h"

/**
 * This class represents the application root for the platform demo.
 */
class LumiaApp : public cugl::Application {
protected:
    enum CurrentScene{
        Loading,
        Main,
        LevelSelect,
        Settings,
        Game,
        Pause
    };
    
    /** The global sprite batch for drawing (only want one of these) */
    std::shared_ptr<cugl::SpriteBatch> _batch;
    
    std::shared_ptr<cugl::SpriteBatch> _UIbatch;
    /** The global asset manager */
    std::shared_ptr<cugl::AssetManager> _assets;

    // Player modes
    /** The primary controller for the game world */
    GameScene _gameplay;
    /** The controller for the loading screen */
    LoadingScene _loading;
    /** The controller for the level select screen */
    LevelSelectScene _levelSelect;
    /** The controller for the main menu screen */
    MainMenuScene _mainMenu;
    /** The controller for the settings screen */
    SettingsScene _settings;
    /** The controller for the settings screen */
    PauseScene _pause;
    /** The current scene of the game*/
    CurrentScene _scene;
    
public:
#pragma mark Constructors
    /**
     * Creates, but does not initialized a new application.
     *
     * This constructor is called by main.cpp.  You will notice that, like
     * most of the classes in CUGL, we do not do any initialization in the
     * constructor.  That is the purpose of the init() method.  Separation
     * of initialization from the constructor allows main.cpp to perform
     * advanced configuration of the application before it starts.
     */
    LumiaApp() : cugl::Application(), _scene(Loading) {}
    
    /**
     * Disposes of this application, releasing all resources.
     *
     * This destructor is called by main.cpp when the application quits.
     * It simply calls the dispose() method in Application.  There is nothing
     * special to do here.
     */
    ~LumiaApp() { }
    
    
#pragma mark Application State

    /**
     * The method called after OpenGL is initialized, but before running the application.
     *
     * This is the method in which all user-defined program intialization should
     * take place.  You should not create a new init() method.
     *
     * When overriding this method, you should call the parent method as the
     * very last line.  This ensures that the state will transition to FOREGROUND,
     * causing the application to run.
     */
    virtual void onStartup() override;
    
    /**
     * The method called when the application is ready to quit.
     *
     * This is the method to dispose of all resources allocated by this
     * application.  As a rule of thumb, everything created in onStartup()
     * should be deleted here.
     *
     * When overriding this method, you should call the parent method as the
     * very last line.  This ensures that the state will transition to NONE,
     * causing the application to be deleted.
     */
    virtual void onShutdown() override;
    
    /**
     * The method called when the application is suspended and put in the background.
     *
     * When this method is called, you should store any state that you do not
     * want to be lost.  There is no guarantee that an application will return
     * from the background; it may be terminated instead.
     *
     * If you are using audio, it is critical that you pause it on suspension.
     * Otherwise, the audio thread may persist while the application is in
     * the background.
     */
    virtual void onSuspend() override;
    
    /**
     * The method called when the application resumes and put in the foreground.
     *
     * If you saved any state before going into the background, now is the time
     * to restore it. This guarantees that the application looks the same as
     * when it was suspended.
     *
     * If you are using audio, you should use this method to resume any audio
     * paused before app suspension.
     */
    virtual void onResume()  override;
    
    
#pragma mark Application Loop
    /**
     * The method called to update the application data.
     *
     * This is your core loop and should be replaced with your custom implementation.
     * This method should contain any code that is not an OpenGL call.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    virtual void update(float timestep) override;
    
    /**
     * The method called to draw the application to the screen.
     *
     * This is your core loop and should be replaced with your custom implementation.
     * This method should OpenGL and related drawing calls.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     */
    virtual void draw() override;
};

#endif /* __LUMIA_APP_H__ */
