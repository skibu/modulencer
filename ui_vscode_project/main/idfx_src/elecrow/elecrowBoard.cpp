/**
 * @brief Implementation of the ElecrowBoard class.
 *
 * SPDX-License-Identifier: MIT
 */

#include "idfx/elecrow/elecrowBoard.hpp"

#include <chrono>

#include "esp-idf-cxx/gpio_cxx.hpp"
#include "idfx/elecrow/interfaces.hpp"
#include "idfx/hardware/i2c.hpp"
#include "idfx/hardware/pca9557.hpp"
#include "idfx/utils/log.hpp"
#include "idfx/utils/time.hpp"

using namespace idfx;

ElecrowBoard::ElecrowBoard(int width, int height)
    : i2c_master_bus_(I2CMasterBus::getBus(I2CNumber(I2C_MASTER_NUM), SCL_GPIO(I2C_MASTER_SCL_IO),
                                           SDA_GPIO(I2C_MASTER_SDA_IO))),
      io_expander_(I2CAddress(IO_EXPANDER_I2C_ADDR)),
      display_(width, height, *this) {
    INFO("Creating ElecrowBoard object");

    // FIXME This should be moved to initialization of the GT911 

    // The initialization process was determined by looking at the setup() in
    // the Elecrow factory code software at
    // https://github.com/Elecrow-RD/CrowPanel-Advance-HMI-ESP32-AI-Display/blob/master/4.3/factory_code/factory_code.ino#L1252
    // Initialization for other displays will be completely different.

    // Sets IO1_TP_INT pin 1 to low, then sets IO expander bit 2 to LOW, delays 20msec, and then
    // back to HIGH, and delays for 100ms. And then it sets pin 1 to input, which I guess in a sense
    // releases it. Pin 2 of extender is PCA_IO2_TP_RST. And pin 1 is IO1_TP_INT, the interrupt pin
    // for the touch panel. First, set the touch panel interrupt pin to low
    INFO("Resetting touch panel");
    DEBUG("Setting TP_INT IO bit %d to an output bit and to low", TP_INT);
    const OutputBit touch_panel_interrupt_gpio(GPIONum(TP_INT), "TP_INT");
    touch_panel_interrupt_gpio.setOff();

    // Toggle PCA_IO2_TP_RST low and then high to reset the touch panel
    DEBUG("Toggling TP_RST expander IO bit %d to low and then high", TP_RST);
    const OutputBit touch_panel_reset_gpio(GPIONum(TP_RST), "TP_RST", &io_expander_);
    touch_panel_reset_gpio.setOff();
    sleep(20ms);
    touch_panel_reset_gpio.setOn();

    DEBUG("Setting TP_INT IO bit %d back to an input bit so that interrupts can be handled",
          TP_INT);
    sleep(100ms);
    const InputBit touch_panel_interrupt_gpio_input(GPIONum(TP_INT), "TP_INT");

    INFO("Done resetting touch panel");

    // Turn on backlight. FIXME This should be done after LVGL driver fully created
    display_.turnOnBacklight(true);
}

ElecrowBoard ::~ElecrowBoard() {
    ERROR("Destroying ElecrowBoard object but should not destory hardware objects!");
}
