//
//  LMInput.cpp
//
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#include "InputController.h"

using namespace cugl;

#define RANGE_CLAMP(x,y,z)  (x < y ? y : (x > z ? z : x))

#define X_ADJUST_FACTOR 34.0f
#define Y_ADJUST_FACTOR 16.0f

#pragma mark Input Constants

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
#define LISTENER_KEY    999


#pragma mark -
#pragma mark Input Controller
/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialzation.  It simply allocates the
 * object. This makes it safe to use this class without a pointer.
 */
InputController::InputController() :
_active(false),
_resetPressed(false),
_debugPressed(false),
_exitPressed(false),
_splitPressed(false),
_mergePressed(false),
_keyReset(false),
_keyDebug(false),
_keyExit(false),
_keySplit(false),
_keyMerge(false),
_switched(false),
_switchInputted(false),
_launched(false),
_launchInputted(false),
_dragged(false),
_dragging(false)
{
}

/**
 * Deactivates this input controller, releasing all listeners.
 *
 * This method will not dispose of the input controller. It can be reused
 * once it is reinitialized.
 */
void InputController::dispose() {
    if (_active) {
#ifndef CU_TOUCH_SCREEN
        Input::deactivate<Keyboard>();
        Mouse* mouse = Input::get<Mouse>();
        mouse->removePressListener(LISTENER_KEY);
        mouse->removeReleaseListener(LISTENER_KEY);
        mouse->removeDragListener(LISTENER_KEY);
#else
        Touchscreen* touch = Input::get<Touchscreen>();
        touch->removeBeginListener(LISTENER_KEY);
        touch->removeEndListener(LISTENER_KEY);
        touch->removeMotionListener(LISTENER_KEY);
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
bool InputController::init() {
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
    mouse->addDragListener(LISTENER_KEY, [=](const MouseEvent& event, const Vec2& previous, bool focus) {
        this->mouseDraggedCB(event, previous, focus);
    });
#else
    Touchscreen* touch = Input::get<Touchscreen>();
    touch->addBeginListener(LISTENER_KEY,[=](const TouchEvent& event, bool focus) {
        this->touchBeganCB(event,focus);
    });
    touch->addEndListener(LISTENER_KEY,[=](const TouchEvent& event, bool focus) {
        this->touchEndedCB(event,focus);
    });
    touch->addMotionListener(LISTENER_KEY, [=](const TouchEvent& event, const Vec2& previous, bool focus) {
        this->touchesDraggedCB(event, previous, focus);
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
void InputController::update(float dt) {
#ifndef CU_TOUCH_SCREEN
    // DESKTOP CONTROLS
    Keyboard* keys = Input::get<Keyboard>();

    // Map "keyboard" events to the current frame boundary
    _keyReset  = keys->keyPressed(RESET_KEY);
    _keyDebug  = keys->keyPressed(DEBUG_KEY);
    _keyExit   = keys->keyPressed(EXIT_KEY);
    _keyMerge  = keys->keyDown(MERGE_KEY);
    _keySplit  = keys->keyPressed(SPLIT_KEY);

    _resetPressed = _keyReset;
    _mergePressed = _keyMerge;

#endif
    
    _splitPressed = _keySplit;
    _debugPressed = _keyDebug;
    _exitPressed  = _keyExit;
    
    _switched = _switchInputted;
    _switchInputted = false;

    _launched = _launchInputted;
    _launchInputted = false;

// If it does not support keyboard, we must reset "virtual" keyboard
#ifdef CU_TOUCH_SCREEN
    _keyExit = false;
    _keyReset = false;
    _keyDebug = false;
    _keySplit = false;
    _keyMerge = false;
    _launchInputted = false;
    _switchInputted = false;
#endif
}

/**
 * Clears any buffered inputs so that we may start fresh.
 */
void InputController::clear() {
    _resetPressed = false;
    _debugPressed = false;
    _exitPressed  = false;
    _splitPressed = false;
    _mergePressed = false;
    _launched = false;
    _switched = false;
    _dragging = false;
    _dragged = false;

    _inputLaunch = Vec2::ZERO;
    _inputSwitch = Vec2::ZERO;
    _dclick = Vec2::ZERO;
    _plannedLaunch = Vec2::ZERO;
    _touchids.clear();
}

void InputController::clearAvatarStates() {
    _splitPressed = false;
    _mergePressed = false;
    _launched = false;
    _switched = false;
    _dragging = false;
    _dragged = false;

    _inputLaunch = Vec2::ZERO;
    _inputSwitch = Vec2::ZERO;
    _dclick = Vec2::ZERO;
    _plannedLaunch = Vec2::ZERO;
    _touchids.clear();
}

#pragma mark -
#pragma mark Touch and Mouse Callbacks

cugl::Vec2 InputController::calculateLaunch(cugl::Vec2 rawDrag) {
    rawDrag.x = -rawDrag.x / X_ADJUST_FACTOR;
    rawDrag.y = rawDrag.y / Y_ADJUST_FACTOR;

    // end position of touch was sufficiently far from start of touch, register as drag-and-release
    if (rawDrag.lengthSquared() > pow(MAXIMUM_LAUNCH_VELOCITY, 2)) {
        rawDrag = rawDrag.normalize().scale(MAXIMUM_LAUNCH_VELOCITY);
    }

    return rawDrag;
}

/**
 * Callback for the beginning of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void InputController::mousePressedCB(const MouseEvent& event, Uint8 clicks, bool focus) {
    // Update the touch location for later gestures
    _dclick.set(event.position);
}

/**
 * Callback for the end of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void InputController::mouseReleasedCB(const MouseEvent& event, Uint8 clicks, bool focus) {
    Vec2 finishDrag = event.position - _dclick;
    
    if (finishDrag.lengthSquared() < 625.0f) {
        // end position of touch was very close to start of touch, register as tap
        _inputSwitch = event.position;
        _switchInputted = true;
    } else {
        _dragged = false;

        finishDrag = calculateLaunch(finishDrag);

        _inputLaunch = _plannedLaunch;
        _launchInputted = true;
    }
}

/**
 * Callback for a touch moved event.
 *
 * @param event The associated event
 * @param previous The previous position of the touch
 * @param focus	Whether the listener currently has focus
 */
void InputController::mouseDraggedCB(const MouseEvent& event, const Vec2& previous, bool focus) {
    Vec2 currentDrag = event.position - _dclick;
    _dragDistance = currentDrag.x;
    // only register player as dragging if sufficiently far from initial click/touch
    if (currentDrag.lengthSquared() >= 625.0f) {
        _dragged = true;

        currentDrag = calculateLaunch(currentDrag);

        _plannedLaunch = currentDrag;
    }
}

/**
 * Callback for the beginning of a touch event
 *
 * @param event The associated event
 * @param focus	Whether the listener currently has focus
 */
void InputController::touchBeganCB(const TouchEvent& event, bool focus) {
    CULog("Touch began %lld", event.touch);
    _touchids.insert(event.touch);

    if (event.timestamp.ellapsedMillis(_clickTime) <= 250.0f) {
        _keySplit = true;
    }

    if (_touchids.size() == 1) {
        // only one finger on screen, input is drag-and-release or tap
        _dclick.set(event.position);
    } else if (_touchids.size() == 2) {
        // two fingers on screen, user is merging Lumia
        _keySplit = false;
        _mergePressed = true;
    } else if (_touchids.size() == 4) {
        _resetPressed = true;
    }
    else {
        // invalid input
        return;
    }
    
    _clickTime = event.timestamp;
}

 
/**
 * Callback for the end of a touch event
 *
 * @param event The associated event
 * @param focus	Whether the listener currently has focus
 */
void InputController::touchEndedCB(const TouchEvent& event, bool focus) {
    CULog("Touch ended %lld", event.touch);
    Vec2 finishDrag = event.position - _dclick;

    if (finishDrag.lengthSquared() < 625.0f) {
        // end position of touch was very close to start of touch, register as tap
        _inputSwitch = event.position;
        _switchInputted = true;
    } else if (_dragged){
        _dragged = false;

        finishDrag = calculateLaunch(finishDrag);

        _inputLaunch = finishDrag;
        _launchInputted = true;
    }

    // finger lifted off screen, remove from set of touch IDs
    _touchids.erase(event.touch);
    if (_touchids.size() != 2) {
        // doesn't have 2 fingers on screen, stop merging
        _mergePressed = false;
    }
    if (_touchids.size() != 4) {
        _resetPressed = false;
    }
}

/**
 * Callback for a touch moved event.
 *
 * @param event The associated event
 * @param previous The previous position of the touch
 * @param focus	Whether the listener currently has focus
 */
void InputController::touchesDraggedCB(const TouchEvent& event, const Vec2& previous, bool focus) {
    if (_touchids.size() == 1) {
        Vec2 currentDrag = event.position - _dclick;
        
        _dragDistance = currentDrag.x;

        // only register player as dragging if sufficiently far from initial click/touch
        if (currentDrag.lengthSquared() >= 625.0f) {
            _dragged = true;

            currentDrag = calculateLaunch(currentDrag);

            _plannedLaunch = currentDrag;
        }
    }
}
