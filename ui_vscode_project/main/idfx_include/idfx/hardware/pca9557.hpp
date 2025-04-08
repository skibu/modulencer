/* PCA9557 is IO extender that is communicated with via I2C.
 * Documentation on the PCA9557 is at https://www.ti.com/lit/ds/symlink/pca9557.pdf
 *
 * This software should also work for a TCA9534 chip, though that one also has
 * interrupts so the software would need to be extended to handle that. In fact,
 * inspiration for this code came from https://github.com/hideakitai/TCA9534/tree/master .
 *
 * The polarity feature of the PCA9557 is not made available in this software. The polarity
 * for each bit is initialized to be 0 for each bit, and no functions are provided to change it.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "idfx/hardware/i2c.hpp"
#include "idfx/hardware/io.hpp"

using namespace idfx;

class PCA9557 : public IOExpander {
   public:
    /* Constructs the PCA9557 object and initializes it so that polarity is set to 0x00 instead
       of the hardware default of 0xF0 */
    PCA9557(const idfx::I2CAddress address, const idfx::I2CNumber i2c_number = I2CNumber::I2C0());
 
    ~PCA9557();

    /* Sets the specified config bit to 0, which indicates it is an output bit. */
    void configAsOutput(int io_bit) const;

    /* Sets the specified config bit to 1, which indicates it is an input bit. */
    void configAsInput(int io_bit) const;

    /* Sets the specified bit to on or off. */
    void setBit(int io_bit, bool on) const;

    /* Works for both input and output bits */
    uint8_t getBit(int io_bit) const;

   protected:
    const I2CDevice device_;

    // Constants for the PCA9557
    static const int CLOCK_SPEED = 400000;
    static const uint8_t CURRENT_VALUES_REGISTER = 0x00;
    static const uint8_t OUTPUT_PORT_REGISTER = 0x01;
    static const uint8_t POLARITY_REGISTER = 0x02;
    static const uint8_t CONFIG_REGISTER = 0x03;
};