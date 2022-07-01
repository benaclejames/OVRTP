#include "OSC.h"

#include <cstring>

#include "ButtonState.h"

int OSC::create_osc_int_message(char* message, const char* address, int value)
{
    int len = strlen(address)+1;
    int paddedLen = len + 4 - len % 4;
    memcpy(message, address, paddedLen);

    message[paddedLen++] = ',';
    message[paddedLen++] = 'i'; 
    message[paddedLen++] = '\0';
    message[paddedLen++] = '\0';

    // Convert value from big endian to little endian and copy to message
    char* valuePtr = (char*)&value;
    message[paddedLen++] = valuePtr[3];
    message[paddedLen++] = valuePtr[2];
    message[paddedLen++] = valuePtr[1];
    message[paddedLen++] = valuePtr[0];

    return paddedLen;
}

OSC::OSC()
{
    WSAStartup(MAKEWORD(2,2), &data);

    addr.sin_family = AF_INET;
    InetPton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    addr.sin_port = htons(9000);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

void OSC::HandleControllerStateChange(ButtonState* state, bool isLeftController)
{
    int newState = state->CalculateButtonsInt();
    char message[100];
    int msgLen;
    if (isLeftController)
    {
        msgLen = create_osc_int_message(message, "/avatar/parameters/LeftThumb", newState);
    }
    else
    {
        msgLen = create_osc_int_message(message, "/avatar/parameters/RightThumb", newState);
    }
    sendto(sock, message, msgLen, 0, (sockaddr*)&addr, sizeof(addr));
}
