#ifndef IOT_ESP32_FIRMWARE_DATA_STRUCTURES_H
#define IOT_ESP32_FIRMWARE_DATA_STRUCTURES_H

#include <Arduino.h>

namespace DATA_STRUCTURES {
    struct workload {
        String name;

        void (*callback)(void); // pointer to void!
    };
}

#endif //IOT_ESP32_FIRMWARE_DATA_STRUCTURES_H
