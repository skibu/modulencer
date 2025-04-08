/*
 * SPDX-License-Identifier: MIT
 */
#include "idfx/elecrow/elecrowDisplay.hpp"

#include <thread>

#include "esp-idf-cxx/gpio_cxx.hpp"
#include "idfx/elecrow/elecrowBoard.hpp"
#include "idfx/elecrow/interfaces.hpp"
#include "idfx/hardware/pca9557.hpp"
#include "idfx/utils/log.hpp"
#include "idfx/utils/time.hpp"

using namespace idfx;

/**
 * Constructor for the ElecrowDisplay class. Initializes the display with the specified width and height.
 * The display is then initialized in the init() method.
 *
 * @param width The width of the display in pixels.
 * @param height The height of the display in pixels.
 */
ElecrowDisplay::ElecrowDisplay(int width, int height, ElecrowBoard &board)
    : DisplayDriverBase(width, height), board_(board) {
        // Initialize the display so that it can be used
        init();
}

/**
 * The initialization process was determined by looking at the setup() in
 * the Elecrow factory code software at 
 * https://github.com/Elecrow-RD/CrowPanel-Advance-HMI-ESP32-AI-Display/blob/master/4.3/factory_code/factory_code.ino#L1252 
 * Initialization for other displays will be completely different.
 */
void ElecrowDisplay::init() {
    INFO("Initializing Elecrow display...");

    const PCA9557* expander_ptr = &board_.getIOExpander();

        // Enable the backlight, which is on the PCA9557 IO extender chip
    OutputBit backlight_io(GPIONum(BACKLIGHT_IO_BIT), "Backlight", expander_ptr);
    backlight_io.setOn();

    // Sets IO1_TP_INT pin 1 to low, then sets IO expander bit 2 to LOW, delays 20msec, and then back to HIGH, 
    // and delays for 100ms. And then it sets pin 1 to input, which I guess in a sense releases it.
    // Pin 2 of extender is PCA_IO2_TP_RST. And pin 1 is IO1_TP_INT, the interrupt pin for the 
    // touch panel.
    // First, set the touch panel interrupt pin to low
    INFO("Resetting touch panel");
    DEBUG("Setting TP_INT IO bit %d to an output bit and to low", TP_INT);
    const OutputBit touch_panel_interrupt_gpio(GPIONum(TP_INT), "TP_INT");
    touch_panel_interrupt_gpio.setOff();

    // Toggle PCA_IO2_TP_RST low and then high to reset the touch panel
    DEBUG("Toggling TP_RST expander IO bit %d to low and then high", TP_RST);
    const OutputBit touch_panel_reset_gpio(GPIONum(TP_RST), "TP_RST", expander_ptr);
    touch_panel_reset_gpio.setOff();
    sleep(20ms);
    touch_panel_reset_gpio.setOn();

    DEBUG("Setting TP_INT IO bit %d back to an input bit so that interrupts can be handled", TP_INT);
    sleep(100ms);
    const InputBit touch_panel_interrupt_gpio_input(GPIONum(TP_INT), "TP_INT");

    INFO("Done resetting touch panel");
}

void ElecrowDisplay::flush() {
}