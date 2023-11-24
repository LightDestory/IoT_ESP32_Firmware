#ifndef IOT_ESP32_FIRMWARE_BL_COMMUNICATION_H
#define IOT_ESP32_FIRMWARE_BL_COMMUNICATION_H

#include <Arduino.h>
#include "../utils/data_structures.h"
#include "../utils/globals.h"

namespace BLE_COM {
    const String DEVICE_NAME = "CAPY-KING-ESP32";
    const uint8_t IMPLEMENTED_SUBS = 2;
    extern char local_addr[18];
    extern GLOBALS::SUB_ROUTINE_STATE state;
    extern int8_t selection_workload_cursor;
    extern int8_t selected_workload;
    extern DATA_STRUCTURES::workload sub_menus[IMPLEMENTED_SUBS];

    void init();

    void looper();

    void displayMacAddr();

    void MtoS();
}

#endif //IOT_ESP32_FIRMWARE_BL_COMMUNICATION_H
