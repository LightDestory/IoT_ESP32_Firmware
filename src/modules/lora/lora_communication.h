//
// Created by apb23 on 13/12/2023.
//

#ifndef IOT_ESP32_FIRMWARE_LORA_COMMUNICATION_H
#define IOT_ESP32_FIRMWARE_LORA_COMMUNICATION_H
#include <LoRa.h>
#include "../utils/globals.h"

namespace LORA {
    extern GLOBALS::SUB_ROUTINE_STATE state;
    /*
     * This function represents the main loop of the module.
     */

    void init();
    void looper();
}

#endif //IOT_ESP32_FIRMWARE_LORA_COMMUNICATION_H
