#include <Arduino.h>
#include "display.h"
#include "images.h"
#include "modules/utils/serial_logger/serial_logger.h"

SSD1306 DISPLAY_ESP::display_hw = SSD1306(0x3c, PIN_CONFIGURATION::DISPLAY_SDA, PIN_CONFIGURATION::DISPLAY_SCL);
uint8_t DISPLAY_ESP::currentProgressBarPercentage = 0;

void DISPLAY_ESP::drawCenteredImageTitleSubtitle(const unsigned char image[], String title, String subData) {
    // Clear Display
    display_hw.clear();
    // Draw Image
    display_hw.drawXbm(48, 0, DISPLAY_IMAGE_SIZE, DISPLAY_IMAGE_SIZE, image);
    // Draw Titles
    display_hw.setFont(ArialMT_Plain_16);
    display_hw.setTextAlignment(TEXT_ALIGN_CENTER);
    display_hw.drawString(64, 35, title);
    if (subData != "") {
        display_hw.setFont(ArialMT_Plain_10);
        display_hw.drawString(64, 51, subData);
    }
    display_hw.display();
}

void DISPLAY_ESP::drawCenteredTitleSubtitle(String title, String subData) {
    // Clear Display
    display_hw.clear();
    // Draw Titles
    display_hw.setFont(ArialMT_Plain_24);
    display_hw.setTextAlignment(TEXT_ALIGN_CENTER);
    display_hw.drawString(64, 10, title);
    if (subData != "") {
        display_hw.setFont(ArialMT_Plain_16);
        display_hw.drawString(64, 40, subData);
    }
    display_hw.display();
}

void DISPLAY_ESP::showBootAnimation() {
    currentProgressBarPercentage = 0;
    drawCenteredImageTitleSubtitle(DISPLAY_IMAGES::capybara_head, "Capybara King");
    // Wait for seconds
    delay(BOOT_FIRST_PHASE_DURATION);
    // Display Second Phase with progress bar
    // Clear Display
    display_hw.clear();
    updateBootAnimationProgressBar(currentProgressBarPercentage);
}

uint8_t DISPLAY_ESP::_progressBarValidation(uint8_t targetPercentage) {
    if (targetPercentage < currentProgressBarPercentage) {
        return currentProgressBarPercentage;
    } else if (targetPercentage > 100) {
        return 100;
    } else if (targetPercentage < 0) {
        return 0;
    }
    return targetPercentage;
}

void DISPLAY_ESP::updateBootAnimationProgressBar(uint8_t targetPercentage) {
    targetPercentage = _progressBarValidation(targetPercentage);
    for (uint8_t i = currentProgressBarPercentage; i <= targetPercentage; i++) {
        // Display Progress Bar
        display_hw.drawXbm(48, 0, DISPLAY_IMAGE_SIZE, DISPLAY_IMAGE_SIZE, DISPLAY_IMAGES::capybara_body);
        display_hw.drawProgressBar(4, 48, 120, 10, i);
        display_hw.setFont(ArialMT_Plain_10);
        display_hw.setTextAlignment(TEXT_ALIGN_CENTER);
        display_hw.drawString(64, 32, "Loading...");
        display_hw.display();
        delay(75);
    }
    currentProgressBarPercentage = targetPercentage;
    if (currentProgressBarPercentage == 100) {
        delay(BOOT_SECOND_PHASE_CLEAR_TIME);
        // Clear Display
        display_hw.clear();
        display_hw.display();
    }
}

void
DISPLAY_ESP::requestSubroutineFromMenu(String title, DATA_STRUCTURES::workload *menu_entries, const uint8_t *quantity,
                                       int8_t *selection_cursor, int8_t *selection) {
    if (digitalRead(PIN_CONFIGURATION::BUTTON_2) == LOW) {
        *selection = *selection_cursor;
        return;
    }
    if (digitalRead(PIN_CONFIGURATION::BUTTON_1) == LOW) {
        *selection_cursor = (*selection_cursor + 1) % *quantity;
    }
    display_hw.clear();
    display_hw.setFont(ArialMT_Plain_10);
    display_hw.setTextAlignment(TEXT_ALIGN_CENTER);
    display_hw.drawString(64, 0, title);
    display_hw.setTextAlignment(TEXT_ALIGN_LEFT);
    uint8_t min = *quantity < 5 ? *quantity : 5;
    for (uint8_t i = 0; i < min; ++i) {
        uint8_t sel = (*selection_cursor + i) % *quantity;
        display_hw.drawString(0, 11 * (i + 1),
                              i == 0 ? (menu_entries + sel)->name + " <--" : (menu_entries + sel)->name);
    }
    display_hw.display();
}

void DISPLAY_ESP::requestBLEDeviceFromMenu(String title, std::vector<DATA_STRUCTURES::ble_device_descriptor> *devices,
                                           uint8_t *quantity,
                                           int8_t *selection_cursor, int8_t *selection) {
    if (digitalRead(PIN_CONFIGURATION::BUTTON_2) == LOW) {
        *selection = *selection_cursor;
        return;
    }
    if (digitalRead(PIN_CONFIGURATION::BUTTON_1) == LOW) {
        *selection_cursor = (*selection_cursor + 1) % *quantity;
    }
    display_hw.clear();
    display_hw.setFont(ArialMT_Plain_10);
    display_hw.setTextAlignment(TEXT_ALIGN_CENTER);
    display_hw.drawString(64, 0, title);
    display_hw.setTextAlignment(TEXT_ALIGN_LEFT);
    uint8_t min = *quantity < 5 ? *quantity : 5;
    for (uint8_t i = 0; i < min; ++i) {
        uint8_t sel = (*selection_cursor + i) % *quantity;
        display_hw.drawString(0, 11 * (i + 1),
                              i == 0 ? devices->at(sel).name + " <--" : devices->at(sel).name);
    }
    display_hw.display();
}