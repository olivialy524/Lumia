//
//  UIButtonScene.cpp
//  UI Demo
//
//  This module shows off a basic button.  The button is backed by a NinePatch
//  so it can resize dynamically.  It changes the text on a click.
//
//  Author: Walker White
//  Version: 1/20/18
//
#include "Cutscene.h"

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
bool Cutscene::init(const std::shared_ptr<AssetManager>& assets, string sceneType) {
    setName("cutscene");

    _input = InputController::getInstance();
    _input->init();
    _sceneType = sceneType;
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    _assets = assets;
    auto layer = assets->get<scene2::SceneNode>("cutscene");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    addChild(layer);
    
    _line1 = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("cutscene_line1"));
    _line2 = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("cutscene_line2"));
    _break1 = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("cutscene_break1"));
    _line3 = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("cutscene_line3"));
    _line4 = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("cutscene_line4"));
    _line5 = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("cutscene_line5"));
    _break2 = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("cutscene_break2"));
    _line6 = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("cutscene_line6"));
    _line7 = std::dynamic_pointer_cast<scene2::Label>(assets->get<scene2::SceneNode>("cutscene_line7"));
    _prologueAnimation = scene2::AnimationNode::alloc(assets->get<Texture>("level_complete"), 1, 1);
    _prologueAnimation->setPosition(dimen.width / 2, dimen.height - (dimen.height / 4));
    _prologueAnimation->setVisible(false);

    _line1Index = 0;
    _line2Index = 0;
    _break1Index = 0;
    _line3Index = 0;
    _line4Index = 0;
    _line5Index = 0;
    _break2Index = 0;
    _line6Index = 0;
    _line7Index = 0;
    _line8Index = 0;
    _line9Index = 0;
    _tapped = 0;

    _line2->setVisible(false);
    _break1->setVisible(false);
    _line3->setVisible(false);
    _line4->setVisible(false);
    _line5->setVisible(false);
    _break2->setVisible(false);
    _line6->setVisible(false);
    _line7->setVisible(false);

    _line1->setPosition(dimen.width / 2, dimen.height - (dimen.height / 4));
    _line2->setPosition(dimen.width / 2, dimen.height - (dimen.height / 4) - 40);
    _break1->setPosition(dimen.width / 2, dimen.height - (dimen.height / 4) - 70);

    _line3->setPosition(dimen.width / 2, dimen.height - (dimen.height / 2) + 40);
    _line4->setPosition(dimen.width / 2, dimen.height - (dimen.height / 2));
    _line5->setPosition(dimen.width / 2, dimen.height - (dimen.height / 2) - 40);
    _break2->setPosition(dimen.width / 2, dimen.height - (dimen.height / 2) - 70);

    _line6->setPosition(dimen.width / 2, (dimen.height / 4) + 40);
    _line7->setPosition(dimen.width / 2, dimen.height / 4);
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    return true;
}

void Cutscene::update(float timestep) {
    _input->update(timestep);
    cugl::Scene2::update(timestep);

    if (_input->didSwitch()) {
        _tapped += 1;
    }

    if (_sceneType == "prologue") {
        if (_line1Index < string(LINE1).length()) {
            _line1Index += 1;
            _line1->setText(string(LINE1).substr(0, _line1Index));
        } else if (_line1Index == string(LINE1).length() && _line2Index < string(LINE2).length()) {
            _line2->setVisible(true);
            _line2Index += 1;
            _line2->setText(string(LINE2).substr(0, _line2Index));
        } else if (_line2Index == string(LINE2).length() && _break1Index < string(BREAK).length()) {
            _break1->setVisible(true);
            _break1Index += 1;
            _break1->setText(string(BREAK).substr(0, _break1Index));
        } else if (_break1Index == string(BREAK).length() && _line3Index < string(LINE3).length() && _tapped >= 1) {
            _line3->setVisible(true);
            _line3Index += 1;
            _line3->setText(string(LINE3).substr(0, _line3Index));
        } else if (_line3Index == string(LINE3).length() && _line4Index < string(LINE4).length() && _tapped >= 1) {
            _line4->setVisible(true);
            _line4Index += 1;
            _line4->setText(string(LINE4).substr(0, _line4Index));
        } else if (_line4Index == string(LINE4).length() && _line5Index < string(LINE5).length() && _tapped >= 1) {
            _line5->setVisible(true);
            _line5Index += 1;
            _line5->setText(string(LINE5).substr(0, _line5Index));
        } else if (_line5Index == string(LINE5).length() && _break2Index < string(BREAK).length() && _tapped >= 1) {
            _break2->setVisible(true);
            _break2Index += 1;
            _break2->setText(string(BREAK).substr(0, _break2Index));
        } else if (_break2Index == string(BREAK).length() && _line6Index < string(LINE6).length() && _tapped >= 2) {
            _line6->setVisible(true);
            _line6Index += 1;
            _line6->setText(string(LINE6).substr(0, _line6Index));
        } else if (_line6Index == string(LINE6).length() && _line7Index < string(LINE7).length() && _tapped >= 2) {
            _line7->setVisible(true);
            _line7Index += 1;
            _line7->setText(string(LINE7).substr(0, _line7Index));
        } else if (_line7Index == string(LINE7).length() && !_prologueAnimation->isVisible() && _tapped >= 3) {
            _line1->setVisible(false);
            _line2->setVisible(false);
            _break1->setVisible(false);
            _line3->setVisible(false);
            _line4->setVisible(false);
            _line5->setVisible(false);
            _break2->setVisible(false);
            _line6->setVisible(false);
            _line7->setVisible(false);
            _prologueAnimation->setVisible(true);
            CULog("lasdlfaslfjk");
        } else if (_prologueAnimation->isVisible() && _line8Index < string(LINE8).length() && _tapped >= 3) {
            _line3->setVisible(true);
            _line8Index += 1;
            _line3->setText(string(LINE8).substr(0, _line8Index));
        } else if (_line8Index == string(LINE8).length() && _line9Index < string(LINE9).length() && _tapped >= 3) {
            _line4->setVisible(true);
            _line9Index += 1;
            _line4->setText(string(LINE9).substr(0, _line9Index));
        } else if (_line9Index == string(LINE9).length() && _tapped >= 4) {
            setActive(false);
            _nextScene = "levelselect";
        }
    } else if (_sceneType == "epilogue") {

    }
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void Cutscene::dispose() {
    _assets = nullptr;
    Scene2::dispose();
}

/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void Cutscene::setActive(bool value) {
    _active = value;
}

