#ifndef IOT_ESP32_FIRMWARE_GLOBALS_H
#define IOT_ESP32_FIRMWARE_GLOBALS_H

#include <Arduino.h>
#include "BLEAdvertisedDevice.h"

namespace GLOBALS {
    extern bool interrupt_flag;
    enum SUB_ROUTINE_STATE {
        UN_SETUP, FAILED_INIT, READY, LOOPING
    };
}

namespace DATA_STRUCTURES {
    struct workload {
        String name;

        void (*callback)(); // pointer to void!
    };

    struct ble_device_descriptor {
        String name;
        BLEAdvertisedDevice descriptor;
    };
}

#endif //IOT_ESP32_FIRMWARE_GLOBALS_H
