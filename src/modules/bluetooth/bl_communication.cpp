#include "bl_communication.h"
#include  "modules/utils/serial_logger/serial_logger.h"
#include "../display/display.h"
#include "../display/images.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

GLOBALS::SUB_ROUTINE_STATE BLE_COM::state = GLOBALS::UN_SETUP;
int8_t BLE_COM::selection_workload_cursor = 0;
int8_t BLE_COM::selected_workload = -1;
char BLE_COM::local_addr[18] = "";
DATA_STRUCTURES::workload BLE_COM::sub_menus[BLE_COM::IMPLEMENTED_SUBS] = {
        {"Start Master to Slave", BLE_COM::MtoS},
        {"Display BL Mac Addr",   BLE_COM::displayMacAddr}
};

void BLE_COM::MtoS() {
    SERIAL_LOGGER::log("Starting Master to Slave");
}

void BLE_COM::displayMacAddr() {
    DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::bluetooth, "Your Mac Addr", local_addr);
}

void BLE_COM::init() {
    // Enable Bluetooth
    if (!btStart()) {
        DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error", "Failed to start Bluetooth" );
        SERIAL_LOGGER::log("Failed to start Bluetooth");
        state = GLOBALS::FAILED_INIT;
        return;
    }
    if (esp_bluedroid_init() != ESP_OK) {
        DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error", "Failed to init Bluetooth" );
        SERIAL_LOGGER::log("Failed to init Bluetooth");
        state = GLOBALS::FAILED_INIT;
        return;
    }
    if (esp_bluedroid_enable() != ESP_OK) {
        DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::error, "Error", "Failed to enable Bluetooth" );
        SERIAL_LOGGER::log("Failed to enable Bluetooth");
        state = GLOBALS::FAILED_INIT;
        return;
    }
    // Get local address to string
    const uint8_t *addr = esp_bt_dev_get_address();
    sprintf(local_addr, "%02X:%02X:%02X:%02X:%02X:%02X", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    state = GLOBALS::READY;
}

void BLE_COM::looper() {
    if (GLOBALS::interrupt_flag) {
        selected_workload = -1;
        state = GLOBALS::READY;
        GLOBALS::interrupt_flag = false;
    }
    if (state == GLOBALS::UN_SETUP)
        init();
    if (state == GLOBALS::FAILED_INIT)
        return;
    if (BLE_COM::selected_workload == -1) {
        DISPLAY_ESP::requestSelectionFromMenu("BL Menu", sub_menus, &IMPLEMENTED_SUBS, &selection_workload_cursor,
                                              &selected_workload);
    } else {
        sub_menus[BLE_COM::selected_workload].callback();
    }
}