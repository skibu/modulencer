/*
 * SPDX-License-Identifier: MIT
 */

#include "idfx/hardware/pca9557.hpp"

#include "idfx/hardware/i2c.hpp"
#include "idfx/utils/log.hpp"

using namespace idfx;

PCA9557::PCA9557(const I2CAddress address, const I2CNumber i2c_number)
    : device_(address, "PCA9557 IO Extender", i2c_number, Frequency::Hz(CLOCK_SPEED)) {
    DEBUG("Creating PCA9557 I2C based IO expander object");

    auto result = device_.writeAndRead(POLARITY_REGISTER, 1);
    VERBOSE("Polarity register initially was: 0x%02X", result[0]);

    // Clear the polarity since the hardware default is a very odd 0xF0
    const uint8_t CLEAR = 0x00;
    result = device_.writeAndRead({POLARITY_REGISTER, CLEAR}, 1);
    if (result[0] != CLEAR) {
        ERROR("Failed to set polarity register to CLEAR 0x%02X. Instead, result was 0x%02X", CLEAR,
              result[0]);
    }
}

PCA9557::~PCA9557() {
    WARN("Hardware device %s being destructed, which is probably noot intended.",
         device_.toName().c_str());
}

void PCA9557::configAsOutput(int io_bit) const {
    DEBUG("Configuring PCA9557 IO bit %d as output", io_bit);

    // Read the current config. First need to write to the register address (I think)
    auto result = device_.writeAndRead(CONFIG_REGISTER, 1);
    VERBOSE("Originally config was: 0x%02X", result[0]);

    uint8_t new_value = result[0] & ~(1 << io_bit);
    VERBOSE("Config will be set to: 0x%02X", new_value);
    result = device_.writeAndRead({CONFIG_REGISTER, new_value}, 1);
    VERBOSE("After setting it the config returned is: 0x%02X", result[0]);
}

void PCA9557::configAsInput(int io_bit) const {
    DEBUG("Configuring PCA9557 IO bit %d as input", io_bit);

    // Read the current config. First need to write to the register address (I think)
    auto result = device_.writeAndRead(CONFIG_REGISTER, 1);
    VERBOSE("Originally config is: 0x%02X", result[0]);

    uint8_t new_value = result[0] | (1 << io_bit);
    VERBOSE("Config will be set to: 0x%02X", new_value);
    result = device_.writeAndRead({CONFIG_REGISTER, new_value}, 1);
    VERBOSE("After setting it the config returned is: 0x%02X\n", result[0]);
}

void PCA9557::setBit(int io_bit, bool on) const {
    DEBUG("Setting IO bit %d on the pca9557 to %d", io_bit, on);

    // Get current state
    auto result = device_.writeAndRead({CURRENT_VALUES_REGISTER}, 1);
    VERBOSE("Initial value was: 0x%02X", result[0]);

    // Set the specified bit to the specified value
    volatile uint8_t new_value = result[0];
    if (on) {
        new_value |= (1 << io_bit);
    } else {
        new_value &= ~(1 << io_bit);
    }
    VERBOSE("New value will be: 0x%02X", new_value);
    result = device_.writeAndRead({OUTPUT_PORT_REGISTER, new_value}, 1);
    DEBUG("Set PCA9557 expander output IO bit %d to %s the value returned is: 0x%02X", io_bit,
          on ? "on" : "off", result[0]);
}

uint8_t PCA9557::getBit(int io_bit) const {
    DEBUG("Getting the current value of IO bit %d of the PCA9557", io_bit);
    auto result = device_.writeAndRead(CURRENT_VALUES_REGISTER, 1);
    bool bit_set = (result[0] & (1 << io_bit)) != 0;
    DEBUG("On PCA9557 expander bit %d is currently %d\n", io_bit, bit_set);
    return bit_set;
}