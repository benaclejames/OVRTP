OutFile "Installer.exe"

InstallDir "$PROGRAMFILES64\OVRThumbParams"

Section

    SetOutPath $INSTDIR
    
    ; Copy all files from our Release Folder
    File ".\x64\Release\"
    
    ; Write our uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    ; Run our OVRTP.exe to register our exe with openvr
    ExecWait '"$INSTDIR\OVRTP.exe"'

SectionEnd

Section "Uninstall"

    Delete "$INSTDIR\*.*"
    Delete "$INSTDIR\Uninstall.exe"
    RMDir /r "$INSTDIR"
    
SectionEnd

