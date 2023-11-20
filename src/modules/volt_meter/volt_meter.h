#ifndef VOLT_METER_H_
#define VOLT_METER_H_

#include <Arduino.h>

namespace VOLT_METER {
    /*
     * This function calculates the current Voltage on the PIN 35.
     * @return: The current voltage as a float;
     */
    float _calculateVoltage();

    /*
     * This function draws the current Voltage on the display.
     */
    void _drawVoltageInputToDisplay();
    /*
     * This function represents the main loop of the module.
     */
    void looper();
}
#endif