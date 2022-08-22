#include <iostream>
#include <ostream>

#include "ActionHandler.h"
#include "openvr.h"
#include "OSC.h"

vr::IVRSystem* pVRSystem;

char* GetRelativePath(char* fileName)
{
    // Get exe path
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    char* lastSlash = strrchr(path, '\\');
    *lastSlash = '\0';

    // Concat the trailing slash and filename
    strcat_s(path, "\\");
    strcat_s(path, fileName);

    // Resize the buffer to fit the full path
    char* result = new char[strlen(path) + 1];
    strcpy_s(result, strlen(path) + 1, path);
    
    return result;
}

int WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR cmdLine, int showCmd)
{
    // Init openvr
    vr::EVRInitError eError = vr::VRInitError_None;
    pVRSystem = VR_Init(&eError, vr::VRApplication_Overlay);
    if (eError != vr::VRInitError_None)
    {
        std::cout << "Failed to init openvr" << std::endl;
        return -1;
    }

    vr::EVRApplicationError aError = vr::VRApplications()->AddApplicationManifest(GetRelativePath((char*)"app.vrmanifest"));
    if (aError != vr::VRApplicationError_None)
    {
        std::cout << "Failed to register app manifest" << std::endl;
        return -1;
    }

    if (!vr::VRApplications()->GetApplicationAutoLaunch("benaclejames.ovrtp"))
    {
        aError = vr::VRApplications()->SetApplicationAutoLaunch("benaclejames.ovrtp", true);
        if (aError != vr::VRApplicationError_None)
        {
            std::cout << "Failed to set autolaunch" << std::endl;
            return -1;
        }
    }
    

    ActionHandler handler = ActionHandler("/actions/default");
    while (true)
    {
        handler.update();
    }
    return 0;
}
