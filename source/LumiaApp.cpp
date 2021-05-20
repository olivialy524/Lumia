//
//  LMApp.h
//  This file will be used as reference for building the root class of the game Lumia
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21

#include "LumiaApp.h"
#include <fstream>

using namespace cugl;


#pragma mark -
#pragma mark Application State

#define DEFAULT_SAVE "{\"level_saves\":[{\"name\":\"Level 1\",\"unlocked\":true,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level1.json\"},{\"name\":\"Level 2\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level2.json\"},{\"name\":\"Level 3\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level3.json\"},{\"name\":\"Level 4\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level4.json\"},{\"name\":\"Level 5\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level4.json\"},{\"name\":\"Level 6\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level6.json\"},{\"name\":\"Level 7\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level7.json\"},{\"name\":\"Level 8\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level8.json\"},{\"name\":\"Level 9\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level9.json\"},{\"name\":\"Level 10\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level10.json\"},{\"name\":\"Level 11\",\"unlocked\":false,\"completed\":false,\"stars\":-1,\"score\":-1,\"path\":\"json/level11.json\"}],\"musicVolume\":1,\"effectVolume\":1}"
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
    _UIbatch = SpriteBatch::alloc();
    
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
    _assets->attach<TileDataModel>(GenericLoader<TileDataModel>::alloc()->getHook());
    // Create a "loading" screen
    _loading.init(_assets);
    
    // Que up the other assets
    AudioEngine::start();
    _assets->loadDirectoryAsync("json/assets.json",nullptr);
    // load in the tiles json file
    _assets->loadAsync<TileDataModel>("json/tiles.json", "json/tiles.json", nullptr);
    
    if (!cugl::filetool::file_exists(Application::getSaveDirectory() + "save.json")) {
        // no save file exists yet, so create it
        std::shared_ptr<cugl::JsonValue> saveJson = cugl::JsonValue::allocWithJson(DEFAULT_SAVE);
        _saveFile = saveJson;
        std::shared_ptr<cugl::JsonWriter> writer = cugl::JsonWriter::alloc(Application::getSaveDirectory() + "save.json");
        writer->writeJson(_saveFile);
    } else {
        // save file already exists, so read from it
        std::shared_ptr<cugl::JsonReader> reader = cugl::JsonReader::alloc(Application::getSaveDirectory() + "save.json");
        _saveFile = reader->readJson();
    }

    // load level json files
    std::shared_ptr<cugl::JsonValue> levels = _saveFile->get("level_saves");
    for (int i = 0; i < levels->size(); i++) {
        CULog(levels->get(i)->getString("path").c_str());
        string levelPath = levels->get(i)->getString("path");
        _assets->loadAsync<LevelModel>(levelPath, levelPath, nullptr);
    }

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
    _settings.dispose();
    _levelSelect.dispose();
    _mainMenu.dispose();
    _pause.dispose();
    _win.dispose();
    _assets = nullptr;
    _batch = nullptr;
    _UIbatch = nullptr;
    
    // Shutdown input
#ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif
    
    std::shared_ptr<cugl::JsonWriter> writer = cugl::JsonWriter::alloc(Application::getSaveDirectory() + "save.json");
    _saveFile->get("musicVolume")->set(_settings.getMusicVolume());
    _saveFile->get("effectVolume")->set(_settings.getEffectVolume());
    writer->writeJson(_saveFile);
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
    std::shared_ptr<cugl::JsonWriter> writer = cugl::JsonWriter::alloc(Application::getSaveDirectory() + "save.json");
    _saveFile->get("musicVolume")->set(_settings.getMusicVolume());
    _saveFile->get("effectVolume")->set(_settings.getEffectVolume());
    writer->writeJson(_saveFile);
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
    std::shared_ptr<cugl::JsonReader> reader = cugl::JsonReader::alloc(Application::getSaveDirectory() + "save.json");
    _saveFile = reader->readJson();
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
    switch (_scene){
        case Loading:{
            if (_loading.isActive()){
                _loading.update(0.01f);
            } else {
                _scene = Main;
                _loading.setActive(false);
                _loading.dispose();
                _mainMenu.init(_assets);
                _mainMenu.setActive(true);
                _settings.init(_assets);
                _settings.setActive(false);
                _pause.init(_assets);
                _pause.setActive(false);
                _win.init(_assets);
                _win.setActive(false);
                _gameplay.init(_assets, "json/level1.json");
                _gameplay.dispose();
                _settings.setMusicVolume(_saveFile->getFloat("musicVolume"));
                _settings.setEffectVolume(_saveFile->getFloat("effectVolume"));
                std::shared_ptr<Sound> source = _assets->get<Sound>("ui");
                AudioEngine::get()->getMusicQueue()->play(source, true, _settings.getMusicVolume());
            }
            return;
        }
        case Main:{
            if (_mainMenu.isActive()){
                _mainMenu.update(timestep);
            } else {
                _mainMenu.setActive(false);
                _mainMenu.dispose();
                string nextScene = _mainMenu.getNextScene(); // TODO: change this to integer code
                if (nextScene ==  "levelselect"){
                    _scene = LevelSelect;
                    _levelSelect.init(_assets, _saveFile);
                    _levelSelect.setActive(true, _saveFile);
                }
            }
            return;
            
        }
        case LevelSelect:{
            if (_levelSelect.isActive()){
                _levelSelect.update(timestep);
            } else {
                _levelSelect.setActive(false, _saveFile);
                string nextScene = _levelSelect.getNextScene();
                if (nextScene == "game"){
                    _scene = Game;
                    _gameplay.init(_assets, _levelSelect.getSelectedLevel());
                    _gameplay.setMusicVolume(_settings.getMusicVolume());
                    _gameplay.setEffectVolume(_settings.getEffectVolume());
                    _gameplay.setActive(true);
                } else if (nextScene == "settings") {
                    _scene = Settings;
                    _settings.setNextScene("levelselect");
                    _settings.setActive(true);
                    _settings.setMusicVolume(_saveFile->getFloat("musicVolume"));
                    _settings.setEffectVolume(_saveFile->getFloat("effectVolume"));
                }
            }
            return;
        }
        case Game:{
            if (_gameplay.isActive()){
                _gameplay.update(timestep);
            } else {
                _gameplay.setActive(false);
                string nextScene = _gameplay.getNextScene();
                if (nextScene == "pause") {
                    _scene = Pause;
                    string levelFile = _gameplay.getCurrentLevel();

                    if (levelFile.find("level") != string::npos) {
                        int startIdx = levelFile.find("level") + 5;
                        int endIdx = levelFile.find(".json");
                        string levelNumber = levelFile.substr(startIdx, endIdx - startIdx);

                        _pause.setLevelNumber(_assets, levelNumber);
                        _pause.setDetailsLabel(_assets, _gameplay.getPlantProgress());
                    } else {
                        int startIdx = levelFile.find("tutorial") + 8;
                        int endIdx = levelFile.find(".json");
                        string levelNumber = levelFile.substr(startIdx, endIdx - startIdx);

                        _pause.setLevelNumber(_assets, "T" + levelNumber);
                        _pause.setDetailsLabel(_assets, _gameplay.getPlantProgress());
                    }
                    _pause.setActive(true);
                } else if (nextScene == "win") {
                    _scene = Win;
                    string levelFile = _gameplay.getCurrentLevel();

                    // update save file after completing a level
                    std::shared_ptr<cugl::JsonValue> levels = _saveFile->get("level_saves");
                    for (int i = 0; i < levels->size(); i++) {
                        std::shared_ptr<cugl::JsonValue> level = levels->get(i);
                        if (level->getString("path") == levelFile) {
                            level->get("unlocked")->set(true);
                            level->get("completed")->set(true);
                            // only save highscores
                            if (level->getInt("score") < _gameplay.getRemainingSize()) {
                                level->get("stars")->set((double)_gameplay.getStars());
                                level->get("score")->set((double)_gameplay.getRemainingSize());
                            }
                            
                            if (i < levels->size() - 1) {
                                std::shared_ptr<cugl::JsonValue> nextLevel = levels->get(i + 1);
                                nextLevel->get("unlocked")->set(true);
                            }
                            break;
                        }
                    }

                    if (levelFile.find("level") != string::npos) {
                        int startIdx = levelFile.find("level") + 5;
                        int endIdx = levelFile.find(".json");
                        string levelNumber = levelFile.substr(startIdx, endIdx - startIdx);

                        _win.setLevelNumber(_assets, levelNumber);
                        _win.setWinLabel(_assets, "Level " + levelNumber + " completed!");
                    } else {
                        int startIdx = levelFile.find("tutorial") + 8;
                        int endIdx = levelFile.find(".json");
                        string levelNumber = levelFile.substr(startIdx, endIdx - startIdx);

                        _win.setLevelNumber(_assets, "T" + levelNumber);
                        _win.setWinLabel(_assets, "Tutorial " + levelNumber + " completed!");
                    }
                    _win.setStars(_assets, _gameplay.getStars());
                    _win.setDetailsLabel(_assets, to_string(_gameplay.getRemainingSize()));
                    _win.setActive(true);
                }
            }
            return;
        }
        case Pause: {
            if (_pause.isActive()) {
                _pause.update(timestep);
            } else {
                _pause.setActive(false);
                string nextScene = _pause.getNextScene();
                if (nextScene == "game-continue") {
                    _scene = Game;
                    _gameplay.setPlaying();
                    _gameplay.setActive(true);
                } else if (nextScene == "game-restart") {
                    _scene = Game;
                    _gameplay.reset();
                    _gameplay.setPlaying();
                    _gameplay.setActive(true);
                } else if (nextScene == "levelselect") {
                    _gameplay.dispose();
                    _scene = LevelSelect;
                    _levelSelect.setActive(true, _saveFile);
                    std::shared_ptr<Sound> source = _assets->get<Sound>("ui");
                    AudioEngine::get()->getMusicQueue()->play(source, true, _settings.getMusicVolume());
                } else if (nextScene == "settings") {
                    _scene = Settings;
                    _settings.setNextScene("pause");
                    _settings.setActive(true);
                }
            }
            return;
        }
        case Win: {
            if (_win.isActive()) {
                _win.update(timestep);
            } else {
                _win.setActive(false);
                string nextScene = _win.getNextScene();
                if (nextScene == "win-continue") {
                    _scene = Game;
                    string levelFile = _gameplay.getCurrentLevel();
                    _gameplay.dispose();

                    if (levelFile.find("level") != string::npos) {
                        int startIdx = levelFile.find("level") + 5;
                        int endIdx = levelFile.find(".json");
                        string levelNumber = std::to_string(stoi(levelFile.substr(startIdx, endIdx - startIdx)) + 1);
                        // TODO: update this with eventual number of levels in the game
                        if (levelNumber == "12") {
                            _scene = LevelSelect;
                            _levelSelect.setActive(true, _saveFile);
                            std::shared_ptr<Sound> source = _assets->get<Sound>("ui");
                            AudioEngine::get()->getMusicQueue()->play(source, true, _settings.getMusicVolume());
                        } else {
                            _gameplay.init(_assets, "json/level" + levelNumber + ".json");
                        }
                    } else {
                        int startIdx = levelFile.find("tutorial") + 8;
                        int endIdx = levelFile.find(".json");
                        string levelNumber = std::to_string(stoi(levelFile.substr(startIdx, endIdx - startIdx)) + 1);
                        // TODO: update this with eventual number of tutorials in the game
                        if (levelNumber == "4") {
                            _gameplay.init(_assets, "json/level1.json");
                        } else {
                            _gameplay.init(_assets, "json/tutorial" + levelNumber + ".json");
                        }
                    }
                    _gameplay.setMusicVolume(_settings.getMusicVolume());
                    _gameplay.setEffectVolume(_settings.getEffectVolume());
                    _gameplay.setActive(true);
                } else if (nextScene == "win-replay") {
                    _scene = Game;
                    _gameplay.reset();
                    _gameplay.setPlaying();
                    _gameplay.setActive(true);
                } else if (nextScene == "levelselect") {
                    _gameplay.dispose();
                    _scene = LevelSelect;
                    _levelSelect.setActive(true, _saveFile);
                    std::shared_ptr<Sound> source = _assets->get<Sound>("ui");
                    AudioEngine::get()->getMusicQueue()->play(source, true, _settings.getMusicVolume());
                } else if (nextScene == "settings") {
                    _scene = Settings;
                    _settings.setNextScene("win");
                    _settings.setActive(true);
                }
            }
            return;
        }
        case Settings:{
            if (_settings.isActive()){
                _settings.update(timestep);
            } else {
                _saveFile->get("musicVolume")->set(_settings.getMusicVolume());
                _saveFile->get("effectVolume")->set(_settings.getEffectVolume());
                _settings.setActive(false);
                _gameplay.setMusicVolume(_settings.getMusicVolume());
                _gameplay.setEffectVolume(_settings.getEffectVolume());
                string nextScene = _settings.getNextScene();
                if (nextScene == "levelselect") {
                    std::shared_ptr<Sound> source = _assets->get<Sound>("ui");
                    AudioEngine::get()->getMusicQueue()->play(source, true, _settings.getMusicVolume());
                    _scene = LevelSelect;
                    _levelSelect.setActive(true, _saveFile);
                } else if (nextScene == "pause") {
                    std::shared_ptr<Sound> source = _assets->get<Sound>("game");
                    AudioEngine::get()->getMusicQueue()->play(source, true, _settings.getMusicVolume());
                    _scene = Pause;
                    _pause.setActive(true);
                } else if (nextScene == "win") {
                    std::shared_ptr<Sound> source = _assets->get<Sound>("win");
                    AudioEngine::get()->getMusicQueue()->play(source, false, _settings.getMusicVolume());
                    _scene = Win;
                    _win.setActive(true);
                }
            }
            return;
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
    switch (_scene){
        case Loading:{
            _loading.render(_batch);
            break;
        }
        case Main:{
            _mainMenu.render(_batch);
            break;
        }
        case LevelSelect:{
            _levelSelect.render(_batch);
            break;
        }
        case Settings:{
            _settings.render(_batch);
            break;
        }
        case Game:{
            _gameplay.render_game(_batch, _UIbatch);
            break;
        }
        case Pause: {
            _pause.render(_batch);
            break;
        }
        case Win: {
            _win.render(_batch);
            break;
        }
    }
        
}

