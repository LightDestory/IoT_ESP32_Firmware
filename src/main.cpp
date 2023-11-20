#include <Arduino.h>
#include "modules/utils/data_structures.h"
#include "modules/display/display.h"
#include "modules/pin_configuration.h"
#include "modules/led_buttons/led_buttons.h"
#include "modules/utils/serial_logger.h"
#include "modules/volt_meter/volt_meter.h"
#include "modules/bluetooth/bl_communication.h"

const uint8_t IMPLEMENTED_PROGRAMS = 3;
DATA_STRUCTURES::workload programs[IMPLEMENTED_PROGRAMS] = {
        {"Volt Meter",  VOLT_METER::looper},
        {"LED Buttons", LED_BUTTONS::looper},
        {"BL_COM",      BL_COM::looper}
};

int8_t selection_workload_cursor = 0;
int8_t selected_workload = -1;
uint8_t interrupt_watcher = 0;

/*
 * This function is used to check if the current executed program should be interrupted and reset to the main menu.
 * The interrupt is triggered by a long press of BUTTON_1.
 */
void checkProgramInterrupt() {
    if (digitalRead(PIN_CONFIGURATION::BUTTON_1) == HIGH && interrupt_watcher != 0) {
        interrupt_watcher = 0;
        return;
    }
    if (digitalRead(PIN_CONFIGURATION::BUTTON_1) == LOW) {
        if (++interrupt_watcher == 10) {
            interrupt_watcher = 0;
            selected_workload = -1;
            PIN_CONFIGURATION::resetOutputs();
            return;
        }
    }

}

void setup() {
    Serial.begin(115200);
    SERIAL_LOGGER::log("Booting Capybara King...");
    if (!DISPLAY_ESP::display_hw.init()) {
        SERIAL_LOGGER::log("Failed to initialize display!");
        while (true) {}
    }
    DISPLAY_ESP::showBootAnimation();
    PIN_CONFIGURATION::initPinMode();
    DISPLAY_ESP::updateBootAnimationProgressBar(25);
    delay(400);
    PIN_CONFIGURATION::resetOutputs();
    delay(400);
    DISPLAY_ESP::updateBootAnimationProgressBar(100);
}

void loop() {
    checkProgramInterrupt();
    if (selected_workload == -1) {
        DISPLAY_ESP::requestSelectionFromMenu("Select Program", programs, &IMPLEMENTED_PROGRAMS,
                                              &selection_workload_cursor, &selected_workload);
    } else {
        programs[selected_workload].callback();
    }
    delay(150);
}