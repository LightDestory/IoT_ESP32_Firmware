#include "led_buttons.h"
#include "../pin_configuration.h"
#include "../display/display.h"
#include "../utils/serial_logger.h"
#include <Arduino.h>


void LED_BUTTONS::_button_check() {
    if (digitalRead(PIN_CONFIGURATION::BUTTON_1) == LOW) {
        SERIAL_LOGGER::log("Button 1 has been pressed");
        digitalWrite(PIN_CONFIGURATION::RED_LED, HIGH);
        digitalWrite(PIN_CONFIGURATION::GREEN_LED, HIGH);
        digitalWrite(PIN_CONFIGURATION::YELLOW_LED, HIGH);
        digitalWrite(PIN_CONFIGURATION::BLUE_LED, HIGH);
        digitalWrite(PIN_CONFIGURATION::WHITE_LED, HIGH);
    }
    if (digitalRead(PIN_CONFIGURATION::BUTTON_2) == LOW) {
        SERIAL_LOGGER::log("Button 2 has been pressed");
        digitalWrite(PIN_CONFIGURATION::RED_LED, LOW);
        digitalWrite(PIN_CONFIGURATION::GREEN_LED, LOW);
        digitalWrite(PIN_CONFIGURATION::YELLOW_LED, LOW);
        digitalWrite(PIN_CONFIGURATION::BLUE_LED, LOW);
        digitalWrite(PIN_CONFIGURATION::WHITE_LED, LOW);
    }
}

void LED_BUTTONS::_display_info() {
    DISPLAY_ESP::display_hw.clear();
    DISPLAY_ESP::display_hw.setFont(ArialMT_Plain_10);
    DISPLAY_ESP::display_hw.setTextAlignment(TEXT_ALIGN_LEFT);
    DISPLAY_ESP::display_hw.drawString(0, 0, "PRESS TO POWER OFF ->");
    DISPLAY_ESP::display_hw.drawString(0, 54, "PRESS TO POWER ON ->");
    DISPLAY_ESP::display_hw.display();
}


void LED_BUTTONS::looper() {
    _display_info();
    _button_check();
}