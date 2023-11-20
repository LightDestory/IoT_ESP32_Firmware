#ifndef IOT_ESP32_FIRMWARE_LED_BUTTONS_H
#define IOT_ESP32_FIRMWARE_LED_BUTTONS_H

namespace LED_BUTTONS {
    // Functions
    /*
     * This function checks the state of the buttons and turns on/off the LEDs.
     */
    void _button_check();
    /*
     * This function displays the information on the display about the LEDs usage.
     */
    void _display_info();
    /*
     * This function represents the main loop of the module.
     */
    void looper();
}

#endif //IOT_ESP32_FIRMWARE_LED_BUTTONS_H
