#include "OSC.h"

#include <cstring>
#include <iostream>
#include <ostream>

OSC::OSC()
{
    WSAStartup(MAKEWORD(2,2), &data);

    addr.sin_family = AF_INET;
    InetPton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    addr.sin_port = htons(9000);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

osc_message::osc_message(char* address, char type) : address(address), type(type)
{
    int len = strlen(address)+1;
    writerIndex = quantize(len, 4);
    memcpy(message, address, len);

    message[writerIndex++] = ',';
    message[writerIndex++] = type;
    message[writerIndex++] = '\0';
    message[writerIndex++] = '\0';
}

osc_message::osc_message(char* data, int size)
{
    memcpy(message, data, size);

    // Read the address string
    int len = strlen(message)+1;
    address = new char[len];
    memcpy(address, message, len);
    // Offset the writerIndex by the address length quantized to 4 bytes
    writerIndex = quantize(len, 4);
    // Ensure the next 2 bytes are a comma and the type
    if (message[writerIndex++] != ',') {
        std::cout << "HALP" << std::endl;
    }

    type = message[writerIndex++];
    writerIndex += 2; // Skip the two null bytes
}

osc_int_message::osc_int_message(char* address, int value) : osc_message(address, 'i') {
    char* bytes = static_cast<char*>(static_cast<void*>(&value));
    swap_endianness(bytes, sizeof value);
    memcpy(&message[writerIndex], bytes, sizeof value);
    writerIndex += sizeof value;
}

int osc_int_message::parse() {
    int value;
    memcpy(&value, &message[writerIndex], sizeof value);
    swap_endianness((char*)&value, sizeof value);
    return value;
}
