//
//  LMApp.h
//  This file will be used as reference for building the root class of the game Lumia
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21

#include "LumiaApp.h"

using namespace cugl;


#pragma mark -
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
void LumiaApp::onStartup() {
    _assets = AssetManager::alloc();
    _batch  = SpriteBatch::alloc();
    
    // Start-up basic input
#ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
    Input::get<Mouse>()->setPointerAwareness(Mouse::PointerAwareness::DRAG);
#endif
    
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    _assets->attach<WidgetValue>(WidgetLoader::alloc()->getHook());
    _assets->attach<scene2::SceneNode>(Scene2Loader::alloc()->getHook());
    _assets->attach<LevelModel>(GenericLoader<LevelModel>::alloc()->getHook());

    // Create a "loading" screen
    _loaded = false;
    _loading.init(_assets);
    
    // Que up the other assets
    AudioEngine::start();
    _assets->loadDirectoryAsync("json/assets.json",nullptr);
    //load in the json file
    _assets->loadAsync<LevelModel>("json/level1.json", "json/level1.json", nullptr);
    _assets->loadAsync<LevelModel>("json/level2.json", "json/level2.json", nullptr);
    
    Application::onStartup(); // YOU MUST END with call to parent
}

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
void LumiaApp::onShutdown() {
    _loading.dispose();
    _gameplay.dispose();
    _assets = nullptr;
    _batch = nullptr;
    
    // Shutdown input
#ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif
    
    AudioEngine::stop();
    Application::onShutdown();  // YOU MUST END with call to parent
}

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
void LumiaApp::onSuspend() {
    AudioEngine::get()->pause();
}

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
void LumiaApp::onResume() {
    AudioEngine::get()->resume();
}


#pragma mark -
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
void LumiaApp::update(float timestep) {
    if (!_loaded && _loading.isActive()) {
        _loading.update(0.01f);
    } else if (!_loaded) {
        _loading.dispose(); // Disables the input listeners in this mode

        _gameplay.init(_assets, "json/level1.json");
        _gameplay.setActive(false);
        _levelSelect.init(_assets);
        _levelSelect.setActive(false);
        _settings.init(_assets);
        _settings.setActive(false);
        
        // we have finished loading assets, go to main menu
        _mainMenu.init(_assets);
        _mainMenu.setActive(true);
        _currentScene = &_mainMenu;
        _loaded = true;
    } else {
        // Screens:
        // Main menu -> settings/level select/exit
        // Settings -> main menu/gameplay
        // Level select -> main menu/gameplay
        // Gameplay / gameoverlay -> mainmenu
        // Pause -> gameplay
        // Loading -> mainmenu
        // Game Over -> gameplay/mainmenu

        if (!_currentScene->isActive()) {
            string nextScene;
            if (_currentScene->getName() == "mainmenu") {
                _mainMenu.setActive(false);
                nextScene = dynamic_cast<MainMenuScene*>(_currentScene)->getNextScene();
                if (nextScene == "levelselect") {
                    _levelSelect.setActive(true);
                    _currentScene = &_levelSelect;
                }
            } else if (_currentScene->getName() == "levelselect") {
                _levelSelect.setActive(false);
                nextScene = dynamic_cast<LevelSelectScene*>(_currentScene)->getNextScene();
                if (nextScene == "game") {
                    _gameplay.init(_assets, dynamic_cast<LevelSelectScene*>(_currentScene)->getSelectedLevel());
                    _gameplay.setMusicVolume(_settings.getMusicVolume() / 100.0f);
                    _gameplay.setEffectVolume(_settings.getEffectVolume() / 100.0f);
                    _currentScene = &_gameplay;
                } else if (nextScene == "settings") {
                    _settings.setActive(true);
                    _currentScene = &_settings;
                }
            } else if(_currentScene->getName() == "settings") {
                _settings.setActive(false);
                nextScene = dynamic_cast<SettingsScene*>(_currentScene)->getNextScene();
                if (nextScene == "levelselect") {
                    _levelSelect.setActive(true);
                    _currentScene = &_levelSelect;
                }
            }
        } else {
            _currentScene->update(timestep);
        }
    }
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void LumiaApp::draw() {
    if (!_loaded) {
        _loading.render(_batch);
    } else {
        _currentScene->render(_batch);
    }
}

