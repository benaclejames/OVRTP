#include "ButtonState.h"

bool ButtonState::Update(vr::VRControllerState_t* controllerState)
{
    bool a = IsButtonTouched(controllerState, vr::k_EButton_IndexController_A);
    bool b = IsButtonTouched(controllerState, vr::k_EButton_IndexController_B);
    bool touchpad = IsButtonTouched(controllerState, vr::k_EButton_SteamVR_Touchpad);
    bool joystick = IsButtonTouched(controllerState, vr::k_EButton_IndexController_JoyStick);

    // If any of the buttons have changed, call a function to handle the change.
    bool changed = false;
    if (a != A || b != B || touchpad != Touchpad || joystick != Joystick)
    {
        changed = true;
    }

    if (!changed)
        return false;
    
    // Then set the button states to the new values.
    A = a;
    B = b;
    Touchpad = touchpad;
    Joystick = joystick;

    return true;
}

int ButtonState::CalculateButtonsInt() const
{
    if (A)
        return 1;
    if (B)
        return 2;
    if (Touchpad)
        return 3;
    if (Joystick)
        return 4;
    return 0;
}
