#include <iostream>
#include <ostream>
#include <tuple>

#include "ButtonState.h"
#include "openvr.h"
#include "OSC.h"

using namespace vr;

IVRSystem* pVRSystem;
OSC osc;

std::tuple<TrackedDeviceIndex_t, TrackedDeviceIndex_t> GetLeftRightControllerIndexes(IVRSystem* system)
{
    TrackedDeviceIndex_t leftController = k_unTrackedDeviceIndexInvalid;
    TrackedDeviceIndex_t rightController = k_unTrackedDeviceIndexInvalid;
    for (int i = 1; i < k_unMaxTrackedDeviceCount; i++)
    {
        ETrackedDeviceClass deviceClass = system->GetTrackedDeviceClass((TrackedDeviceIndex_t)i);
        if (deviceClass == TrackedDeviceClass_Controller)
        {
            ETrackedControllerRole role = system->GetControllerRoleForTrackedDeviceIndex(i);
            if (leftController == k_unTrackedDeviceIndexInvalid && role == TrackedControllerRole_LeftHand)
                leftController = (TrackedDeviceIndex_t)i;
            else if (rightController == k_unTrackedDeviceIndexInvalid && role == TrackedControllerRole_RightHand)
                rightController = (TrackedDeviceIndex_t)i;
        }
    }
    return std::make_tuple(leftController, rightController);
}

int main(int argc, char* argv[])
{
    // Init openvr
    EVRInitError eError = VRInitError_None;
    pVRSystem = VR_Init(&eError, VRApplication_Background);
    if (eError != VRInitError_None)
        return 1;

    auto [leftControllerId, rightControllerId] = GetLeftRightControllerIndexes(pVRSystem);

    ButtonState leftButtonStates{}, rightButtonStates{};
    VRControllerState_t leftControllerState, rightControllerState;
    while (true)
    {
        pVRSystem->GetControllerState(leftControllerId, &leftControllerState, sizeof(leftControllerState));
        if (leftButtonStates.Update(&leftControllerState))
            osc.HandleControllerStateChange(&leftButtonStates, true);
        
        pVRSystem->GetControllerState(rightControllerId, &rightControllerState, sizeof(rightControllerState));
        if (rightButtonStates.Update(&rightControllerState))
            osc.HandleControllerStateChange(&rightButtonStates, false);
    }
    return 0;
}
