#include "ActionHandler.h"
#include "openvr.h"
#include "OSC.h"

vr::IVRSystem* pVRSystem;
OSC osc;

int main(int argc, char* argv[])
{
    // Init openvr
    vr::EVRInitError eError = vr::VRInitError_None;
    pVRSystem = VR_Init(&eError, vr::VRApplication_Background);
    if (eError != vr::VRInitError_None)
        return 1;

    ActionHandler handler = ActionHandler("C:\\Users\\Ben\\RiderProjects\\OVRSC\\OVRSC\\manifests\\action_manifest.json", "/actions/default");
    while (true)
    {
        handler.update();
    }
    return 0;
}
