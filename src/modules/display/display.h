#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "SSD1306.h"
#include "../pin_configuration.h"
#include "../utils/data_structures.h"

namespace DISPLAY_ESP {
    extern SSD1306 display_hw;
    const uint16_t BOOT_FIRST_PHASE_DURATION = 2500;
    const uint16_t BOOT_SECOND_PHASE_CLEAR_TIME = 500;
    extern uint8_t currentProgressBarPercentage;

    /*
     * This function is used to validate the target percentage of the progress bar.
     * @param targetPercentage: The target percentage of the progress bar.
     * @return: A validated target percentage.
     */
    uint8_t _progressBarValidation(uint8_t targetPercentage);
    /*
     * This function is used to show and update the progress bar of the boot animation.
     * It will update the progress bar only if the target percentage is valid.
     * @param targetPercentage: The target percentage of the progress bar.
     */
    void updateBootAnimationProgressBar(uint8_t targetPercentage);
    /*
     * This function is used to display the boot animation followed by the progress bar.
     */
    void showBootAnimation();
    /*
     * This function is used to draw a centered image with a title and a subtitle on the SSD1306 display.
     * @param image: The image to be drawn.
     * @param title: The title to be drawn on the bottom of the image.
     * @param subData: The subtitle to be drawn on the bottom of the title.
     */
    void drawCenteredImageTitleSubtitle(const unsigned char image[], String title, String subData = "");
    /*
     * This function is used to draw a centered title and a subtitle on the SSD1306 display.
     * @param title: The title to be drawn.
     * @param subData: The subtitle to be drawn on the bottom of the title.
     */
    void drawCenteredTitleSubtitle(String title, String subData = "");

    /*
     * This function is used to draw a selection menu with infinite scrolling effect.
     * It can show up to 5-ish entries at a time.
     * It updates the selection cursor and the selection variable according to the user input.
     * @param title: The title of the menu.
     * @param menu_entries: The entries of the menu.
     * @param quantity: The quantity of the entries.
     * @param selection_cursor: The current selected entry from the menu.
     * @param selection: The selected program to execute.
     */
    void requestSelectionFromMenu(String title, DATA_STRUCTURES::workload *menu_entries, const uint8_t *quantity,
                                  int8_t *selection_cursor, int8_t *selection);
}
#endif
