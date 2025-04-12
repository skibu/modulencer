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
 * Constructor for the ElecrowDisplay class. Initializes the display with the specified width and
 * height. The display is then initialized in the init() method.
 *
 * @param width The width of the display in pixels.
 * @param height The height of the display in pixels.
 */
ElecrowDisplay::ElecrowDisplay(uint32_t width, uint32_t height, ElecrowBoard& elecrow_board)
    : RGBDisplayBase(width, height),
      backlight_io_(GPIONum(BACKLIGHT_IO_BIT), "Backlight", &(elecrow_board.getIOExpander())) {
    INFO("Creating ElecrowDisplay object");

    // Initialize the configuration of the display

    // Set the GPIO pins for the display
    setHSyncGpioNum(GPIONum(GPIO_NUM_40));
    setVSyncGpioNum(GPIONum(GPIO_NUM_41));
    setDeGpioNum(GPIONum(GPIO_NUM_42));
    setPclkGpioNum(GPIONum(GPIO_NUM_39));
    // Not setting disp_gpio_num since not used by this display and so the default of GPIO_NUM_NC isi fine
    //setDispGpioNum(GPIONum(GPIO_NUM_NC));

    // Set the GPIO pins for the RGB data
    setRgbGpio(0, GPIONum(GPIO_NUM_21));
    setRgbGpio(1, GPIONum(GPIO_NUM_47));
    setRgbGpio(2, GPIONum(GPIO_NUM_48));
    setRgbGpio(3, GPIONum(GPIO_NUM_45));
    setRgbGpio(4, GPIONum(GPIO_NUM_38));
    setRgbGpio(5, GPIONum(GPIO_NUM_9));
    setRgbGpio(6, GPIONum(GPIO_NUM_10));
    setRgbGpio(7, GPIONum(GPIO_NUM_11));
    setRgbGpio(8, GPIONum(GPIO_NUM_12));
    setRgbGpio(9, GPIONum(GPIO_NUM_13));
    setRgbGpio(10, GPIONum(GPIO_NUM_14));
    setRgbGpio(11, GPIONum(GPIO_NUM_7));
    setRgbGpio(12, GPIONum(GPIO_NUM_17));
    setRgbGpio(13, GPIONum(GPIO_NUM_18));
    setRgbGpio(14, GPIONum(GPIO_NUM_3));
    setRgbGpio(15, GPIONum(GPIO_NUM_46));
}

ElecrowDisplay::~ElecrowDisplay() {
    ERROR("Destroying ElecrowDisplay object but should not destory hardware objects!");
}

void ElecrowDisplay::turnOnBacklight(bool on) const {
    backlight_io_.set(on);
}
