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
BLEScan *BLE_COM::scanner = nullptr;
DATA_STRUCTURES::workload BLE_COM::sub_menus[BLE_COM::IMPLEMENTED_SUBS] = {
        {"GATT DeviceName",     BLE_COM::GATT_Client_DeviceName},
        {"GATT BatteryValue",   BLE_COM::GATT_Client_BatteryLevel},
        {"Display BL Mac Addr", BLE_COM::displayMacAddr}
};

BLE_COM::BLE_scanCallback *callback = new BLE_COM::BLE_scanCallback();

void BLE_COM::GATT_Client_DeviceName() {
    GATT_Client("Device Name", "00001800-0000-1000-8000-00805F9B34FB", "00002A00-0000-1000-8000-00805F9B34FB");
}

void BLE_COM::GATT_Client_BatteryLevel() {
    GATT_Client("Battery Level", "0000180F-0000-1000-8000-00805F9B34FB", "00002A19-0000-1000-8000-00805F9B34FB");
}

void BLE_COM::GATT_Client(String title, String ServiceUUID, String CharacteristicUUID) {
    if (found_devices.empty() && !scan_device_executed) {
        DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::radar, "Scanning", "Wait 6 secs");
        scanner = BLEDevice::getScan();
        client = BLEDevice::createClient();
        scanner->setAdvertisedDeviceCallbacks(callback);
        scanner->setActiveScan(true);
        scanner->start(5);
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
        if (!client->isConnected()) {
            SERIAL_LOGGER::log("Connecting to device..." + found_devices.at(selected_device).name);
            DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::bluetooth, "Connecting",
                                                        "Status");
            if (!client->connect(&found_devices.at(selected_device).descriptor)) {
                SERIAL_LOGGER::log("Failed to connect to device");
                DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error",
                                                            "Failed to connect to device");
                state = GLOBALS::STATIC;
                return;
            }
        } else {
            SERIAL_LOGGER::log("Connected to device..." + found_devices.at(selected_device).name);
            BLERemoteService *service = client->getService(ServiceUUID.c_str());
            if (service == nullptr) {
                DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error",
                                                            "Failed to find Service");
            } else {
                SERIAL_LOGGER::log("Found Service!");
                BLERemoteCharacteristic *feature = service->getCharacteristic(CharacteristicUUID.c_str());
                if (feature == nullptr) {
                    DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error",
                                                                "Failed to find Feature");
                } else {
                    String value = feature->readValue().c_str();
                    SERIAL_LOGGER::log("Value: " + value);
                    DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::bluetooth, title,
                                                                value);
                }
            }
            state = GLOBALS::STATIC;
            client->disconnect();
            scanner->stop();
            scanner->clearResults();
            return;
        }
    }
}

void BLE_COM::displayMacAddr() {
    if (state != GLOBALS::STATIC)
        state = GLOBALS::STATIC;
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
    if (state == GLOBALS::FAILED_INIT or state == GLOBALS::STATIC)
        return;
    if (BLE_COM::selected_workload == -1) {
        DISPLAY_ESP::requestSubroutineFromMenu("BLE Menu", sub_menus, &IMPLEMENTED_SUBS, &selection_workload_cursor,
                                               &selected_workload);
    } else {
        sub_menus[BLE_COM::selected_workload].callback();
    }
}