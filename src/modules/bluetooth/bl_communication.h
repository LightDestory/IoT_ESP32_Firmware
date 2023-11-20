#ifndef IOT_ESP32_FIRMWARE_BL_COMMUNICATION_H
#define IOT_ESP32_FIRMWARE_BL_COMMUNICATION_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include "../utils/data_structures.h"

namespace BL_COM {
    const String DEVICE_NAME = "CAPY-KING-ESP32";
    const uint8_t IMPLEMENTED_SUBS = 2;
    enum BL_STATE {
        UNSETUP, READY, LOOPING
    };
    extern BL_STATE state;
    extern BluetoothSerial serialBT;
    extern int8_t selection_workload_cursor;
    extern int8_t selected_workload;
    extern DATA_STRUCTURES::workload sub_menus[IMPLEMENTED_SUBS];

    void connectionCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

    void init();

    void looper();

    void displayMacAddr();

    void MtoS();
}

#endif //IOT_ESP32_FIRMWARE_BL_COMMUNICATION_H
