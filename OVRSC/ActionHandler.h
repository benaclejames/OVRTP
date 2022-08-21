#pragma once
#include <openvr.h>
#include <map>

#include "ActionCallback.h"


struct ActionHand
{
    // Create a map with a string key and ActionCallback handle
    std::map<const char*, ActionCallback*> actions = {
        {"atouch", nullptr},
        {"btouch", nullptr},
        {"trackpadtouch", nullptr},
        {"thumbsticktouch", nullptr}
    };

    void on_changed(bool newValue, void* priv_data);
    void update() const;
    ActionHand(std::string prefix);
};

struct ActionHandler
{
private:
    // Action Set Handle
    vr::VRActionSetHandle_t actionSet = vr::k_ulInvalidActionSetHandle;

    // Action Hand Touple
    std::tuple<ActionHand*, ActionHand*> actionHands;
public:
    ActionHandler(std::string actionPath, std::string actionSet);

    void update() const;
};
