#include "bl_communication.h"
#include "../display/display.h"
#include "../display/images.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include <BLEDevice.h>


GLOBALS::SUB_ROUTINE_STATE BLE_COM::state = GLOBALS::UN_SETUP;
int8_t BLE_COM::selection_workload_cursor = 0;
int8_t BLE_COM::selected_workload = -1;
int8_t BLE_COM::selection_device_cursor = 0;
int8_t BLE_COM::selected_device = -1;
std::vector<DATA_STRUCTURES::ble_device_descriptor> BLE_COM::found_devices = {};
char BLE_COM::local_addr[18] = "";
bool BLE_COM::scan_device_executed = false;
BLEClient *BLE_COM::client = nullptr;
DATA_STRUCTURES::workload BLE_COM::sub_menus[BLE_COM::IMPLEMENTED_SUBS] = {
        {"BLE GATT Client DeviceName", BLE_COM::GATT_Client},
        {"Display BL Mac Addr",        BLE_COM::displayMacAddr}
};

BLE_COM::BLE_scanCallback *callback = new BLE_COM::BLE_scanCallback();

void BLE_COM::GATT_Client() {
    if (found_devices.empty() && !scan_device_executed) {
        DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::radar, "Scanning", "Wait 6 secs");
        BLEScan *pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(callback);
        pBLEScan->setActiveScan(true);
        pBLEScan->start(3);
        delay(4000);
        scan_device_executed = true;
    }
    if (found_devices.empty() && scan_device_executed) {
        DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error", "No devices found");
        SERIAL_LOGGER::log("No devices found");
        state = GLOBALS::FAILED_INIT;
        return;
    }
    if (selected_device == -1) {
        uint8_t quantity = (uint8_t) found_devices.size();
        DISPLAY_ESP::requestBLEDeviceFromMenu("Select Device", &found_devices, &quantity,
                                              &selection_device_cursor, &selected_device);
    } else {
        if (!client) {
            SERIAL_LOGGER::log("Connecting to device..." + found_devices.at(selected_device).name);
            client = BLEDevice::createClient();
            client->connect(&found_devices.at(selected_device).descriptor);
        }
        if (client->isConnected()) {
            SERIAL_LOGGER::log("Connected to device..." + found_devices.at(selected_device).name);
            BLERemoteService *pRemoteService = client->getService("00001800-0000-1000-8000-00805F9B34FB");
            if (pRemoteService == nullptr) {
                DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error",
                                                            "Failed to find Device Info Service");
                state = GLOBALS::FAILED_INIT;
                return;
            }
            SERIAL_LOGGER::log("Found Device Info Service!");
            BLERemoteCharacteristic *pRemoteCharacteristic = pRemoteService->getCharacteristic(
                    "00002A00-0000-1000-8000-00805F9B34FB");
            if (pRemoteCharacteristic == nullptr) {
                DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error",
                                                            "Failed to find Device Name Service");
                state = GLOBALS::FAILED_INIT;
                return;
            }
            DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::bluetooth, "Device Name",
                                                        pRemoteCharacteristic->readValue().c_str());
        }
        client->disconnect();
    }
}

void BLE_COM::displayMacAddr() {
    if (state != GLOBALS::LOOPING)
        state = GLOBALS::LOOPING;
    DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::bluetooth, "Your Mac Addr", local_addr);
}

void BLE_COM::init() {
    // Enable Bluetooth
    if (!btStart()) {
        DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error", "Failed to start Bluetooth");
        SERIAL_LOGGER::log("Failed to start Bluetooth");
        state = GLOBALS::FAILED_INIT;
        return;
    }
    if (esp_bluedroid_init() != ESP_OK) {
        DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error", "Failed to init Bluetooth");
        SERIAL_LOGGER::log("Failed to init Bluetooth");
        state = GLOBALS::FAILED_INIT;
        return;
    }
    if (esp_bluedroid_enable() != ESP_OK) {
        DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error", "Failed to enable Bluetooth");
        SERIAL_LOGGER::log("Failed to enable Bluetooth");
        state = GLOBALS::FAILED_INIT;
        return;
    }
    BLEDevice::init(DEVICE_NAME.c_str());
    // Get local address to string
    const uint8_t *addr = esp_bt_dev_get_address();
    sprintf(local_addr, "%02X:%02X:%02X:%02X:%02X:%02X", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    state = GLOBALS::READY;
}

void BLE_COM::looper() {
    if (GLOBALS::interrupt_flag) {
        selected_workload = -1;
        selected_device = -1;
        selection_device_cursor = 0;
        selection_workload_cursor = 0;
        state = GLOBALS::READY;
        GLOBALS::interrupt_flag = false;
        found_devices.clear();
        scan_device_executed = false;
        delete client;
        client = nullptr;
    }
    if (state == GLOBALS::UN_SETUP)
        init();
    if (state == GLOBALS::FAILED_INIT)
        return;
    if (BLE_COM::selected_workload == -1) {
        DISPLAY_ESP::requestSubroutineFromMenu("BL Menu", sub_menus, &IMPLEMENTED_SUBS, &selection_workload_cursor,
                                               &selected_workload);
    } else {
        sub_menus[BLE_COM::selected_workload].callback();
    }
}