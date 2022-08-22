#include "ActionHandler.h"
#include <iostream>
#include <utility>

#include "OSC.h"

static OSC osc;

void ActionHand::on_changed(bool newValue, void* priv_data)
{
    // Convert priv_data to char*
    char* data = (char*)priv_data;
    std::cout << "ActionHand::on_changed: " << data << " " << newValue << std::endl;
    states[data] = newValue;

    // Now time to create a bit mask of the states
    int buttonMask = 0;
    int buttonCompat = 0;
    int index = 0;
    for (auto it = states.begin(); it != states.end(); ++it)
    {
        if (it->second)
        {
            buttonMask |= (1 << index);
            if (buttonCompat == 0)
                buttonCompat = index+1;
        }
        index++;
    }

    char* prefix = (char*)oscPrefix.c_str();
    osc_int_message thumbIndex(prefix, buttonCompat);
    auto maskPrefixStr = oscPrefix+"Mask";
    char* maskPrefix = (char*)maskPrefixStr.c_str();
    osc_int_message thumbMask(maskPrefix, buttonMask);

    osc.send(thumbIndex.message, thumbIndex.writerIndex);
    osc.send(thumbMask.message, thumbMask.writerIndex);
}

inline void actionhand_forwarder(void* ctx, bool newValue, void* priv_data)
{
    // Who would win?
    // Clean code syntax vs One spicy member function?
    static_cast<ActionHand*>(ctx)->on_changed(newValue, priv_data);
}

void ActionHand::update() const
{
    for (auto& action : actionCallbacks)
        action->Update();
}

ActionHand::ActionHand(std::string prefix, std::string osc_prefix)
{
    oscPrefix = osc_prefix;
    actionCallbacks.clear();
    
    // For every actionHandler in actions map
    for (auto it = states.begin(); it != states.end(); ++it)
    {
        vr::VRActionHandle_t action = vr::k_ulInvalidActionHandle;
        auto name = prefix+it->first;
        const char* actionHandleName = name.c_str();
        vr::VRInput()->GetActionHandle(actionHandleName, &action);
        
        actionCallbacks.push_back(new ActionCallback(action, this, actionhand_forwarder, (void*)it->first));
    }
}

ActionHandler::ActionHandler(std::string actionSet)
{
    // Create our error and assume we have no errors to begin with
    vr::EVRInputError error = vr::VRInputError_None;

    // Get a handle on our action set so we can refresh it
    error = vr::VRInput()->GetActionSetHandle(actionSet.c_str(), &this->actionSet);
    if (error != vr::VRInputError_None)
    {
        std::cout << "Error getting action set handle: " << actionSet << std::endl;
        return;
    }
    
    // Initialize both of our action hands
    std::get<0>(actionHands) = new ActionHand(actionSet+"/in/left", "/avatar/parameters/LeftThumb");
    std::get<1>(actionHands) = new ActionHand(actionSet+"/in/right", "/avatar/parameters/RightThumb");
}

void ActionHandler::update() const
{
    vr::VRActiveActionSet_t activeSet = { 0 };
    activeSet.ulActionSet = actionSet;

    vr::VRInput()->UpdateActionState( &activeSet, sizeof(activeSet), 1 );

    std::get<0>(actionHands)->update();
    std::get<1>(actionHands)->update();
}
