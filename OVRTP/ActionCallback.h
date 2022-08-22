#pragma once

typedef void (*onValueChangeCallback)(void*, bool newValue, void* userData);

class ActionCallback
{
    // Delegate to be called with a bool when the value of a button changes
    onValueChangeCallback onChangedCallback;
    
    // Store a pointer to the object that owns the member function
    void* callingMember;
    
    // Data to be included with the callback
    void* userData;

    // Handle to check for changes on
    vr::VRActionHandle_t actionHandle = vr::k_ulInvalidActionHandle;
    
    
public:
    ActionCallback(vr::VRActionHandle_t actionHandle, void* ctx, onValueChangeCallback onChangedCallback, void* userData)
    {
        callingMember = ctx;
        this->onChangedCallback = onChangedCallback;
        this->userData = userData;
        this->actionHandle = actionHandle;
    }

    void Update() const
    {
        vr::InputDigitalActionData_t actionData;
        vr::VRInput()->GetDigitalActionData(actionHandle, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);

        if (actionData.bChanged)
            onChangedCallback(callingMember, actionData.bState, userData);
    }
};
