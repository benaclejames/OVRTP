#pragma once
#include <openvr.h>
#include <map>
#include <vector>

#include "ActionCallback.h"


struct ActionHand
{
    // Create a map with a string key and ActionCallback handle
    std::map<const char*, bool> states = {
        {"atouch", false},
        {"btouch", false},
        {"trackpadtouch", false},
        {"thumbsticktouch", false}
    };

    std::vector<ActionCallback*> actionCallbacks;

    std::string oscPrefix;
    
    void on_changed(bool newValue, void* priv_data);
    void update() const;
    ActionHand(std::string prefix, std::string oscPrefix);
};

struct ActionHandler
{
private:
    // Action Set Handle
    vr::VRActionSetHandle_t actionSet = vr::k_ulInvalidActionSetHandle;

    // Action Hand Touple
    std::tuple<ActionHand*, ActionHand*> actionHands;
public:
    ActionHandler(std::string actionSet);

    void update() const;
};
