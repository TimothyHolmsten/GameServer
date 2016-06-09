//
// Created by Timothy Friberg Holmsten on 08/06/16.
//

#include "headers/packet.h"

Packet packet_nullify(Packet packet) {
    for (int i = 0; i < PACKET_LENGTH; i++)
    {
        packet.data[i] = 0;
    }
    return packet;
}

