#include "ActionHandler.h"
#include <iostream>

void ActionHand::on_changed(bool newValue, void* priv_data)
{
    // Convert priv_data to char*
    char* data = (char*)priv_data;
    std::cout << "changed "<< data << " to " << (newValue ? "true" : "false") << std::endl;
}

inline void actionhand_forwarder(void* ctx, bool newValue, void* priv_data)
{
    static_cast<ActionHand*>(ctx)->on_changed(newValue, priv_data);
}

void ActionHand::update() const
{
    for (auto& action : actions)
        action.second->Update();
}

ActionHand::ActionHand(std::string prefix)
{
    // For every actionHandler in actions map
    for (auto it = actions.begin(); it != actions.end(); ++it)
    {
        vr::VRActionHandle_t action = vr::k_ulInvalidActionHandle;
        vr::VRInput()->GetActionHandle((prefix+it->first).c_str(), &action);

        it->second = new ActionCallback(action, this, actionhand_forwarder, (void*)it->first);
    }
}

ActionHandler::ActionHandler(std::string actionPath, std::string actionSet)
{
    // Create our error and assume we have no errors to begin with
    vr::EVRInputError error = vr::VRInputError_None;

    // Tell OpenVR about our action manifest
    error = vr::VRInput()->SetActionManifestPath(actionPath.c_str());
    if (error != vr::VRInputError_None)
    {
        std::cout << "Error setting action manifest path: " << actionPath << std::endl;
        return;
    }

    // Get a handle on our action set so we can refresh it
    error = vr::VRInput()->GetActionSetHandle(actionSet.c_str(), &this->actionSet);
    if (error != vr::VRInputError_None)
    {
        std::cout << "Error getting action set handle: " << actionSet << std::endl;
        return;
    }
    
    // Initialize both of our action hands
    std::get<0>(actionHands) = new ActionHand(actionSet+"/in/left");
    std::get<1>(actionHands) = new ActionHand(actionSet+"/in/right");
}

void ActionHandler::update() const
{
    vr::VRActiveActionSet_t activeSet = { 0 };
    activeSet.ulActionSet = actionSet;

    vr::VRInput()->UpdateActionState( &activeSet, sizeof(activeSet), 1 );

    std::get<0>(actionHands)->update();
    std::get<1>(actionHands)->update();
}
