#pragma once
#include <openvr.h>

static bool IsButtonTouched(vr::VRControllerState_t* controllerState, vr::EVRButtonId button)
{
    return controllerState->ulButtonTouched & ButtonMaskFromId(button);
}

struct ButtonState
{
    bool A;
    bool B;
    bool Touchpad;
    bool Joystick;

    // Make a function to take in controller state and update all the button states, and if any have changed, call a function to handle the change.
    bool Update(vr::VRControllerState_t* controllerState);
    int CalculateButtonsInt() const;
};
