//
//  LMInput.cpp
//
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#include "LMInput.h"

using namespace cugl;

#define RANGE_CLAMP(x,y,z)  (x < y ? y : (x > z ? z : x))

#define X_ADJUST_FACTOR 20.0f
#define Y_ADJUST_FACTOR 10.0f

#pragma mark Input Constants

/** Maximum allowed Lumia launch velocity */
#define MAXIMUM_LAUNCH_VELOCITY 70.0f
/** The key to use for reseting the game */
#define RESET_KEY KeyCode::R
/** The key for toggling the debug display */
#define DEBUG_KEY KeyCode::D
/** The key for exitting the game */
#define EXIT_KEY  KeyCode::ESCAPE

/** The key for splitting Lumia into two */
#define SPLIT_KEY KeyCode::S
/** The key for merging nearby Lumias together */
#define MERGE_KEY KeyCode::M
/** The key for switching control of the Lumia body to another */
#define SWITCH_KEY KeyCode::C

/** How close we need to be for a multi touch */
#define NEAR_TOUCH      100
/** The key for the event handlers */
#define LISTENER_KEY      1

/** How fast a double click must be in milliseconds */
#define DOUBLE_CLICK    400

#pragma mark -
#pragma mark Input Controller
/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialzation.  It simply allocates the
 * object. This makes it safe to use this class without a pointer.
 */
LumiaInput::LumiaInput() :
_active(false),
_resetPressed(false),
_debugPressed(false),
_exitPressed(false),
_splitPressed(false),
_mergePressed(false),
_switchPressed(false),
_keyJump(false),
_keyFire(false),
_keyReset(false),
_keyDebug(false),
_keyExit(false),
_keySplit(false),
_keyMerge(false),
_keySwitch(false),
_launched(false),
_launchInputted(false)
{
}

/**
 * Deactivates this input controller, releasing all listeners.
 *
 * This method will not dispose of the input controller. It can be reused
 * once it is reinitialized.
 */
void LumiaInput::dispose() {
    if (_active) {
#ifndef CU_TOUCH_SCREEN
        Input::deactivate<Keyboard>();
        Mouse* mouse = Input::get<Mouse>();
        mouse->removePressListener(LISTENER_KEY);
        mouse->removeReleaseListener(LISTENER_KEY);
#else
        Touchscreen* touch = Input::get<Touchscreen>();
        touch->removeBeginListener(LISTENER_KEY);
        touch->removeEndListener(LISTENER_KEY);
#endif
        _active = false;
    }
}

/**
 * Initializes the input control for the given bounds
 *
 * The bounds are the bounds of the scene graph.  This is necessary because
 * the bounds of the scene graph do not match the bounds of the display.
 * This allows the input device to do the proper conversion for us.
 *
 * @param bounds    the scene graph bounds
 *
 * @return true if the controller was initialized successfully
 */
bool LumiaInput::init(const Rect bounds) {
    bool success = true;
    
#ifndef CU_TOUCH_SCREEN
    success = Input::activate<Keyboard>();
    Mouse* mouse = Input::get<Mouse>();
    mouse->addPressListener(LISTENER_KEY, [=](const MouseEvent& event, Uint8 clicks, bool focus) {
        this->mousePressedCB(event, clicks, focus);
    });
    mouse->addReleaseListener(LISTENER_KEY, [=](const MouseEvent& event, Uint8 clicks, bool focus) {
        this->mouseReleasedCB(event, clicks, focus);
    });
#else
    Touchscreen* touch = Input::get<Touchscreen>();
    touch->addBeginListener(LISTENER_KEY,[=](const TouchEvent& event, bool focus) {
        this->touchBeganCB(event,focus);
    });
    touch->addEndListener(LISTENER_KEY,[=](const TouchEvent& event, bool focus) {
        this->touchEndedCB(event,focus);
    });
    touch->addMotionListener(LISTENER_KEY,[=](const TouchEvent& event, const Vec2& previous, bool focus) {
        this->touchesMovedCB(event, previous, focus);
    });
	
#endif
    _active = success;
    return success;
}


/**
 * Processes the currently cached inputs.
 *
 * This method is used to to poll the current input state.  This will poll the
 * keyboad and accelerometer.
 *
 * This method also gathers the delta difference in the touches. Depending on
 * the OS, we may see multiple updates of the same touch in a single animation
 * frame, so we need to accumulate all of the data together.
 */
void LumiaInput::update(float dt) {
#ifndef CU_TOUCH_SCREEN
    // DESKTOP CONTROLS
    Keyboard* keys = Input::get<Keyboard>();

    // Map "keyboard" events to the current frame boundary
    _keyReset  = keys->keyPressed(RESET_KEY);
    _keyDebug  = keys->keyPressed(DEBUG_KEY);
    _keyExit   = keys->keyPressed(EXIT_KEY);
    _keyMerge   = keys->keyDown(MERGE_KEY);
    _keySplit   = keys->keyPressed(SPLIT_KEY);
    _keySwitch   = keys->keyPressed(SWITCH_KEY);

#endif

    _resetPressed = _keyReset;
    _debugPressed = _keyDebug;
    _exitPressed  = _keyExit;
    
    _splitPressed = _keySplit;
    _mergePressed = _keyMerge;
    _switchPressed = _keySwitch;

    _launched = _launchInputted;
    _launchInputted = false;

// If it does not support keyboard, we must reset "virtual" keyboard
#ifdef CU_TOUCH_SCREEN
    _keyExit = false;
    _keyReset = false;
    _keyDebug = false;
    _keyJump  = false;
    _keyFire  = false;
    _keySplit = false;
    _keySwitch = false;
    _keyMerge = false;
    _launchInputted = false;
#endif
}

/**
 * Clears any buffered inputs so that we may start fresh.
 */
void LumiaInput::clear() {
    _resetPressed = false;
    _debugPressed = false;
    _exitPressed  = false;
    _splitPressed = false;
    _switchPressed = false;
    _mergePressed = false;
    _launched = false;

    _inputLaunch = Vec2::ZERO;
    _dclick = Vec2::ZERO;
    _timestamp.mark();
}

#pragma mark -
#pragma mark Touch Controls

/**
 * Populates the initial values of the input TouchInstance
 */
void LumiaInput::clearTouchInstance(TouchInstance& touchInstance) {
    touchInstance.touchids.clear();
    touchInstance.position = Vec2::ZERO;
}

#pragma mark -
#pragma mark Touch and Mouse Callbacks

/**
 * Callback for the beginning of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void LumiaInput::mousePressedCB(const MouseEvent& event, Uint8 clicks, bool focus) {
    // Update the touch location for later gestures
    _dclick.set(event.position);
}

/**
 * Callback for the end of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void LumiaInput::mouseReleasedCB(const MouseEvent& event, Uint8 clicks, bool focus) {
    // Check for a double tap.
    //_keyReset = event.timestamp.ellapsedMillis(_timestamp) <= EVENT_DOUBLE_CLICK;
    _timestamp = event.timestamp;

    // If we reset, do not record the thrust
    /*if (_keyReset) {
        return;
    }*/

    Vec2 finishDrag = event.position - _dclick;

    finishDrag.x = RANGE_CLAMP(-finishDrag.x, -MAXIMUM_LAUNCH_VELOCITY, MAXIMUM_LAUNCH_VELOCITY);
    finishDrag.y = RANGE_CLAMP(finishDrag.y, -MAXIMUM_LAUNCH_VELOCITY, MAXIMUM_LAUNCH_VELOCITY);

    finishDrag.x = finishDrag.x / X_ADJUST_FACTOR;
    finishDrag.y = finishDrag.y / Y_ADJUST_FACTOR;

//    char print[64];
//    snprintf(print, sizeof print, "%f %f", finishDrag.x, finishDrag.y);
//    CULog(print);
    _inputLaunch = finishDrag;
    _launchInputted = true;
}

/**
 * Callback for the beginning of a touch event
 *
 * @param event The associated event
 * @param focus	Whether the listener currently has focus
 */
void LumiaInput::touchBeganCB(const TouchEvent& event, bool focus) {
    //CULog("Touch began %lld", event.touch);
    Vec2 pos = event.position;
}

 
/**
 * Callback for the end of a touch event
 *
 * @param event The associated event
 * @param focus	Whether the listener currently has focus
 */
void LumiaInput::touchEndedCB(const TouchEvent& event, bool focus) {
    // Reset all keys that might have been set
    Vec2 pos = event.position;
}
