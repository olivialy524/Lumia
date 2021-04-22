//
//  LMInput.h
//
//  This file is based on the CS 4152 PlatformDemo by Walker White and Anthony Perello
//  Version: 3/5/21
//
#ifndef __INPUT_H__
#define __INPUT_H__
#include <cugl/cugl.h>
#include <unordered_set>
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
/**
 * This class represents player input in the Platform demo.
 *
 * This input handler uses the CUGL input API.  It uses the polling API for
 * keyboard, but the callback API for touch.  This demonstrates a mix of ways
 * to handle input, and the reason for hiding it behind an abstraction like
 * this class.
 *
 * In the case of touch devices, this implements the platformer controls
 * found in the popular game Oddmar.  The left side is a floating joystick
 * for left-right controls.  The right side is swipe up to jump and double
 * tap to shoot.
 *
 * Unlike CUGL input devices, this class is not a singleton.  It must be
 * allocated before use.  However, you will notice that we do not do any
 * input initialization in the constructor.  This allows us to allocate this
 * controller as a field without using pointers. We simply add the class to the
 * header file of its owner, and delay initialization (via the method init())
 * until later. This is one of the main reasons we like to avoid initialization
 * in the constructor.
 */
class InputController {
private:
    /** Whether or not this input is active */
    bool _active;
    // KEYBOARD EMULATION
    /** Whether the reset key is down */
    bool  _keyReset;
    /** Whether the debug key is down */
    bool  _keyDebug;
    /** Whether the exit key is down */
    bool  _keyExit;
    /** Whether the split key is down */
    bool  _keySplit;
    /** Whether the merge key is down */
    bool  _keyMerge;

    bool  _keyBack;
    // MOUSE + TOUCH SUPPORT
    /** Whether the mouse or finger was tapped */
    bool  _switchInputted;
    /** Whether the mouse or finger was drag and released */
    bool _launchInputted;
    /** The initial click or touch location for the current click-and-drag */
    cugl::Vec2 _dclick;
    /** The timestamp for a double tap */
    cugl::Timestamp _clickTime;
    /** The touch id(s) of fingers on the screen */
    std::unordered_set<Uint64> _touchids;
  
    /** Maximum allowed Lumia launch velocity */
    float MAXIMUM_LAUNCH_VELOCITY = 20.0f;
    
protected:
    // INPUT RESULTS
    /** Whether the reset action was chosen. */
    bool _resetPressed;
    /** Whether the debug toggle was chosen. */
    bool _debugPressed;
    /** Whether the exit action was chosen. */
    bool _exitPressed;
    
    bool _backPressed;
    /** Whether the split action was chosen. */
    bool _splitPressed;
    /** Whether the merge action was chosen. */
    bool _mergePressed;
    /** Whether player attempted to switch control of Lumia(s) */
    bool _switched;
    /** The tap/click location of the players attempt to switch control */
    cugl::Vec2 _inputSwitch;
    /** Whether Lumia was launched */
    bool _launched;
    /** The launch velocity produced by player input */
    cugl::Vec2 _inputLaunch;
    /** Whether the player is dragging */
    bool _dragging;
    /** Whether the player is dragging */
    bool _dragged;
    /** The planned launch velocity produced by player input drag */
    cugl::Vec2 _plannedLaunch;
  
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new input controller.
     *
     * This constructor does NOT do any initialzation.  It simply allocates the
     * object. This makes it safe to use this class without a pointer.
     */
    InputController(); // Don't initialize.  Allow stack based
    
    /**
     * Disposes of this input controller, releasing all listeners.
     */
    ~InputController() { dispose(); }
    
    /**
     * Deactivates this input controller, releasing all listeners.
     *
     * This method will not dispose of the input controller. It can be reused
     * once it is reinitialized.
     */
    void dispose();
    
    /**
     * Initializes the input control for the given bounds
     *
     * The bounds are the bounds of the scene graph.  This is necessary because
     * the bounds of the scene graph do not match the bounds of the display.
     * This allows the input device to do the proper conversion for us.
     *
     * @return true if the controller was initialized successfully
     */
    bool init();
    
#pragma mark -
#pragma mark Input Detection
    /**
     * Returns true if the input handler is currently active
     *
     * @return true if the input handler is currently active
     */
    bool isActive( ) const { return _active; }

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
    void  update(float dt);

    /**
     * Clears any buffered inputs so that we may start fresh.
     */
    void clear();
    
#pragma mark -
#pragma mark Input Results
    /**
     * Returns the current input launch.
     *
     * The launch is determined by the angle and length of the player's drag-and-release.
     *
     * @return The input launch
     */
    const cugl::Vec2& getLaunch() { return _inputLaunch; }

    /**
     * Returns the current planned input launch.
     *
     * The planned launch is determined by the angle and length of the player's drag, without release.
     *
     * @return The planned input launch
     */
    cugl::Vec2& getPlannedLaunch() { return _plannedLaunch; }
    
    /** Converts raw drag input to launch velocity */
    cugl::Vec2 calculateLaunch(cugl::Vec2 rawDrag);

    /**
     * Returns whether or not the player is dragging their mouse/finger.
     *
     * @return whether or not the player is dragging their mouse/finger
     */
    bool isDragging() { return _dragged; }

    /**
     * Returns the attempted input switch tap/click location.
     *
     * @return The inputted tap/click location
     */
    const cugl::Vec2& getSwitch() { return _inputSwitch; }

    /**
     * Returns true if the player had split Lumia.
     *
     * @return true if the player had split Lumia.
     */
    float didSplit() const { return _splitPressed; }
    
    /**
     * Returns true if the player had merged Lumia.
     *
     * @return true if the player had merged Lumia.
     */
    float didMerge() const { return _mergePressed; }
    
    /**
     * Returns true if the player had switched control of Lumia.
     *
     * @return true if the player had switched control of Lumia.
     */
    float didSwitch() const { return _switched; }
    /**
     * Returns true if the player had drag and released.
     *
     * @return true if the player had drag and released.
     */
    float didLaunch() const { return _launched; }

    /**
     * Returns true if the reset button was pressed.
     *
     * @return true if the reset button was pressed.
     */
	bool didReset() const { return _resetPressed; }

    bool didGoBack() const { return _backPressed; }
    /**
     * Returns true if the player wants to go toggle the debug mode.
     *
     * @return true if the player wants to go toggle the debug mode.
     */
	bool didDebug() const { return _debugPressed; }

	/**
	 * Returns true if the exit button was pressed.
	 *
	 * @return true if the exit button was pressed.
	 */
	bool didExit() const { return _exitPressed; }
    
    float getMaximumLaunchVelocity(){
        return MAXIMUM_LAUNCH_VELOCITY;
    }

#pragma mark -
#pragma mark Touch and Mouse Callbacks
    /**
     * Callback for the beginning of a mouse event
     *
     * @param event The associated event
     * @param clicks The number of recent clicks, including this one
     * @param focus	Whether the listener currently has focus
     * f
     */
    void mousePressedCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);

    /**
     * Callback for the end of a mouse event
     *
     * @param event The associated event
     * @param clicks The number of recent clicks, including this one
     * @param focus	Whether the listener currently has focus
     */
    void mouseReleasedCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);

    /**
     * Callback for a mouse release event.
     *
     * @param event The associated event
     * @param previous The previous position of the touch
     * @param focus	Whether the listener currently has focus
     */
    void mouseDraggedCB(const cugl::MouseEvent& event, const cugl::Vec2& previous, bool focus);

    /**
     * Callback for the beginning of a touch event
     *
     * @param event The associated event
     * @param focus	Whether the listener currently has focus
     * f
     */
    void touchBeganCB(const cugl::TouchEvent& event, bool focus);

    /**
     * Callback for the end of a touch event
     *
     * @param event The associated event
     * @param focus	Whether the listener currently has focus
     */
    void touchEndedCB(const cugl::TouchEvent& event, bool focus);

    /**
     * Callback for a mouse release event.
     *
     * @param event The associated event
     * @param previous The previous position of the touch
     * @param focus	Whether the listener currently has focus
     */
    void touchesDraggedCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);
  
};

#endif /* __INPUT_H__ */
