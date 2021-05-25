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
    
    shared_ptr<Texture> prologueTex11 = assets->get<Texture>("prologue1.1");
    _prologueAnimation11 = scene2::AnimationNode::alloc(prologueTex11, 20, 1, 20);
    _curAnim = 0;
    _prologueAnimation11->setPosition(dimen.width / 2, dimen.height /2);
    float scale = dimen.height / (prologueTex11->getHeight()/20);
    _prologueAnimation11->setScale(scale);
    _prologueAnimation11->setVisible(true);
    _prologueAnimation11->setFrame(0);
    shared_ptr<Texture> prologueTex12 = assets->get<Texture>("prologue1.2");
    _prologueAnimation12 = scene2::AnimationNode::alloc(prologueTex12, 22, 1, 22);
    _prologueAnimation12->setPosition(dimen.width / 2, dimen.height /2);
    scale = dimen.height / (prologueTex12->getHeight()/22);
    _prologueAnimation12->setScale(scale);
    _prologueAnimation12->setVisible(false);
    _prologueAnimation12->setFrame(0);
    shared_ptr<Texture> prologueTex13 = assets->get<Texture>("prologue1.3");
    _prologueAnimation13 = scene2::AnimationNode::alloc(prologueTex13, 8, 1, 8);
    _prologueAnimation13->setPosition(dimen.width / 2, dimen.height /2);
    scale = dimen.height / (prologueTex13->getHeight()/8);
    _prologueAnimation13->setScale(scale);
    _prologueAnimation13->setVisible(false);
    _prologueAnimation13->setFrame(0);
    shared_ptr<Texture> prologueTex21 = assets->get<Texture>("prologue2.1");
    _prologueAnimation21 = scene2::AnimationNode::alloc(prologueTex21, 25, 1, 25);
    _prologueAnimation21->setPosition(dimen.width / 2, dimen.height /2);
    scale = dimen.height / (prologueTex21->getHeight()/25);
    _prologueAnimation21->setScale(scale);
    _prologueAnimation21->setVisible(false);
    _prologueAnimation21->setFrame(0);
    shared_ptr<Texture> prologueTex22 = assets->get<Texture>("prologue2.2");
    _prologueAnimation22 = scene2::AnimationNode::alloc(prologueTex22, 17, 1, 17);
    _prologueAnimation22->setPosition(dimen.width / 2, dimen.height /2);
    scale = dimen.height / (prologueTex22->getHeight()/17);
    _prologueAnimation22->setScale(scale);
    _prologueAnimation22->setVisible(false);
    _prologueAnimation22->setFrame(0);
    shared_ptr<Texture> prologueTex23 = assets->get<Texture>("prologue2.3");
    _prologueAnimation23 = scene2::AnimationNode::alloc(prologueTex23, 8, 1, 8);
    _prologueAnimation23->setPosition(dimen.width / 2, dimen.height /2);
    scale = dimen.height / (prologueTex23->getHeight()/8);
    _prologueAnimation23->setScale(scale);
    _prologueAnimation23->setVisible(false);
    _prologueAnimation23->setFrame(0);
    addChild(_prologueAnimation11);
    addChild(_prologueAnimation12);
    addChild(_prologueAnimation13);
    addChild(_prologueAnimation21);
    addChild(_prologueAnimation22);
    addChild(_prologueAnimation23);
    _tapped = false;
    _animations[0] =_prologueAnimation11;
    _animations[1] =_prologueAnimation12;
    _animations[2] =_prologueAnimation13;
    _animations[3] =_prologueAnimation21;
    _animations[4] =_prologueAnimation22;
    _animations[5] =_prologueAnimation23;
    Application::get()->setClearColor(Color4f::BLACK);
    return true;
}

void Cutscene::update(float timestep) {
    _input->update(timestep);
    cugl::Scene2::update(timestep);
    _ticks += 1;
    _tapped = _input->didSwitch();
    if (_sceneType == "prologue") {
        auto anim = _animations[_curAnim];
        int frame = anim->getFrame();
        if (frame < anim->getSize()-1 && !_tapped && _ticks % ANIMATION_INTERVAL==0){
            anim->setFrame(frame + 1);
        }else if (frame < anim->getSize() - 1 && _tapped){
            anim->setFrame(anim->getSize() - 1);
        }else if (frame == anim->getSize() - 1){
            if (_curAnim == 3 || _curAnim == 4){
                anim->setVisible(false);
                _curAnim += 1;
                _animations[_curAnim]->setVisible(true);
            } else if (_tapped){
                if (_curAnim==5){
                    setActive(false);
                    _nextScene = "levelselect";
                }else{
                    anim->setVisible(false);
                    _curAnim += 1;
                    _animations[_curAnim]->setVisible(true);
                }
            }
        }
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

