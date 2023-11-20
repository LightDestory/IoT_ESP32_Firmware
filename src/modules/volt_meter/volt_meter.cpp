#include "volt_meter.h"
#include "../pin_configuration.h"
#include "../display/display.h"
#include "../display/images.h"

float VOLT_METER::_calculateVoltage() {
    float vBat = (float) (analogRead(PIN_CONFIGURATION::V_BAT_IN));
    vBat *= 2;    // we divided by 2, so multiply back
    vBat *= 3.3;  // Multiply by 3.3V, our reference voltage
    vBat *= 1.1; // Multiply by 1.1V, The voltage divider
    vBat /= 4095; // convert to voltage
    return vBat;
}

void VOLT_METER::_drawVoltageInputToDisplay() {
    float vBat = _calculateVoltage();
    String vBatString = "V " + String(vBat, 6);
    DISPLAY_ESP::drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::electric, vBatString, "Voltage Meter");
}

void VOLT_METER::looper() {
    _drawVoltageInputToDisplay();
    delay(300);
}