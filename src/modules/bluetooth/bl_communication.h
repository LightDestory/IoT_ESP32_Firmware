#ifndef IOT_ESP32_FIRMWARE_BL_COMMUNICATION_H
#define IOT_ESP32_FIRMWARE_BL_COMMUNICATION_H

#include <Arduino.h>
#include  "modules/utils/serial_logger/serial_logger.h"
#include "../utils/globals.h"
#include <vector>
#include <BLEClient.h>

namespace BLE_COM {

    // BLE Stuff

    const String DEVICE_NAME = "CAPY-KING-ESP32";
    extern char local_addr[18];

    // Subroutines stuff
    const uint8_t IMPLEMENTED_SUBS = 2;
    extern GLOBALS::SUB_ROUTINE_STATE state;
    extern int8_t selection_workload_cursor;
    extern int8_t selected_workload;
    extern int8_t selection_device_cursor;
    extern int8_t selected_device;
    extern bool scan_device_executed;
    extern BLEClient *client;
    extern DATA_STRUCTURES::workload sub_menus[IMPLEMENTED_SUBS];
    extern std::vector<DATA_STRUCTURES::ble_device_descriptor> found_devices;

    class BLE_scanCallback : public BLEAdvertisedDeviceCallbacks {
        void onResult(BLEAdvertisedDevice advertisedDevice) {
            String log_message = "BLE Device found: \nAddress: ";
            String tmp = advertisedDevice.getAddress().toString().c_str();
            tmp.toUpperCase();
            log_message += tmp;
            if (advertisedDevice.haveName()) {
                log_message += "\nName: ";
                tmp = advertisedDevice.getName().c_str();
                log_message += tmp;
            }
            SERIAL_LOGGER::log(log_message);
            BLE_COM::found_devices.push_back({tmp, advertisedDevice});
        }
    };

    void init();

    void looper();

    void displayMacAddr();

    void GATT_Client();
}

#endif //IOT_ESP32_FIRMWARE_BL_COMMUNICATION_H
