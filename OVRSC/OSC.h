#pragma once

#include <WS2tcpip.h>
#include <WinSock2.h>

struct ButtonState;

class OSC
{
    WSADATA data{};
    SOCKADDR_IN addr{};
    SOCKET sock;
    
    int create_osc_int_message(char* message, const char* address, int value);
public:
    OSC();
    void HandleControllerStateChange(ButtonState* state, bool isLeftController);
};
