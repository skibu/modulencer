/*
 * Contains C++ classes for a Master I2C device to write commands to a recipient.
 * The classes are based on v2 of the ESP-IDF I2C driver. This means that you must
 * set CONFIG_I2C_ENABLE_SLAVE_DRIVER_VERSION_2 in Kconfig, as described at
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig.html#config-i2c-enable-slave-driver-version-2
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifndef __cpp_exceptions
#error I2C class can only be used when __cpp_exceptions is enabled. Enable CONFIG_COMPILER_CXX_EXCEPTIONS in Kconfig
#endif

#include <string>
#include <vector>

#include "driver/i2c_master.h"
#include "esp-idf-cxx/esp_exception.hpp"
#include "esp-idf-cxx/gpio_cxx.hpp"

namespace idfx {

struct I2CException : public ESPException {
    I2CException(esp_err_t error);
};

/**
 * @brief Represents a valid SDA signal pin number.
 */
class SDA_type;
using SDA_GPIO = GPIONumBase<class SDA_type>;

/**
 * @brief Represents a valid SCL signal pin number.
 */
class SCL_type;
using SCL_GPIO = GPIONumBase<class SCL_type>;

/**
 * @brief Valid representation of I2C number.
 *
 * A chip can have multiple I2C interfaces, each identified by a bus number, subsequently called I2C
 * number. Instances of this class are guaranteed to always contain a valid I2C number.
 */
class I2CNumber : public StrongValueComparable<uint32_t> {
    /**
     * Construct a valid representation of the I2C number.
     *
     * This constructor is private because the it can only be accessed but the static creation
     * methods below. This guarantees that an instance of I2CNumber always carries a valid number.
     */
    constexpr explicit I2CNumber(uint32_t number) : StrongValueComparable<uint32_t>(number) {}

   public:
    /**
     * @brief create an I2C number representing the first I2C bus of the chip.
     */
    constexpr static I2CNumber I2C0() {
        return I2CNumber(0);
    }

#if CONFIG_SOC_I2C_NUM == 2
    /**
     * @brief create an I2C number representing the second I2C bus of the chip.
     */
    constexpr static I2CNumber I2C1() {
        return I2CNumber(1);
    }
#endif
};

/**
 * @brief Valid representation of I2C address.
 *
 * Instances of this class are guaranteed to always contain a valid I2C address.
 */
class I2CAddress : public StrongValueComparable<uint8_t> {
   public:
    explicit I2CAddress(const uint8_t addr);
};

class I2CMasterBus {
   public:
    /**
     * @brief Get the Bus object for the specified port.
     *
     * Creates the bus if it doesn't yet exist
     *
     * @param i2c_port
     * @param scl_gpio
     * @param sda_gpio
     * @param enable_internal_pullups
     * @return I2CMasterBus&
     */
    static I2CMasterBus& getBus(const I2CNumber i2c_port, const SCL_GPIO scl_gpio,
                                const SDA_GPIO sda_gpio, const bool enable_internal_pullups = true);

    /**
     * @brief Get the Bus Handle object
     *
     * Assumes the bus has already been configured for the specified port. If
     * it hasn't then throws an exception.
     *
     * @param i2c_port Port number of the I2C bus. Either I2CNumber::I2C0() or I2CNumber::I2C1()
     * @return i2c_master_bus_handle_t&
     * @throws I2CException with ESP_ERR_INVALID_STATE value if bus not configured for that port
     */
    static i2c_master_bus_handle_t& getBusHandle(I2CNumber const i2c_port);

    ~I2CMasterBus() {};

    void reset();

   private:
    I2CMasterBus(const I2CNumber i2c_number, const SCL_GPIO scl_gpio, const SDA_GPIO sda_gpio,
                 const bool enable_internal_pullups);

   private:
    i2c_master_bus_config_t conf_;
    i2c_master_bus_handle_t bus_handle_;

    static I2CMasterBus* bus_handle_port0_ptr_;
    static I2CMasterBus* bus_handle_port1_ptr_;
};

class I2CDevice {
   public:
    /**
     * @brief Construct a new I2CDevice object
     *
     * @param i2c_address address of the device. This is a 7 bit address, so the
     * address should be in the range 0x03 to 0x77. The address is shifted left
     * by 1 bit to get the actual address used by the I2C driver.
     * @param name for logging
     * @param i2c_port which I2C port to use, I2C0 or I2C1. Default is I2C0
     * @param clock_speed I2C frequency. Default is 400kHz
     * @throws I2CException with ESP_ERR_INVALID_STATE if the bus was not created yet
     * @throws I2CException with ESP_ERR_INVALID_ARG if the address is invalid
     */
    I2CDevice(const I2CAddress i2c_address, const std::string name,
              const I2CNumber i2c_port = I2CNumber::I2C0(),
              const Frequency clock_speed = Frequency(DEFAULT_I2C_FREQUENCY));

    ~I2CDevice();

    static constexpr int DEFAULT_I2C_FREQUENCY = 400000;

    /**
     * @brief Convert this object to string
     * Useful for logging
     * @return std::string
     */
    std::string toName() const;

    /**
     * @brief write data to the device
     *
     * Writes the data to the device. Does not wait for command to be
     * completed.
     * @param write_data the data to write. Easiest way to pass in command is
     * to use a struct like {BYTE0, BYTE1}
     * @return true if the write was successful
     */
    bool asyncWrite(const std::vector<uint8_t>& commands) const;

    bool asyncWrite(const uint8_t command) const;

    /**
     * @brief write command and read data from the device
     *
     * @param command the command to write
     * @param read_size the number of bytes to read from the device
     * @return std::vector<uint8_t> the data read from the device
     */
    std::vector<uint8_t> writeAndRead(const uint8_t command, const size_t read_size) const;
    /**
     * @brief write commands and read data from the device
     *
     * Sends command to the I2C device and then reads and returns the data.
     * @param commands the data to write
     * @param read_size the number of bytes to read from the device
     * @return std::vector<uint8_t> the data read from the device
     */
    std::vector<uint8_t> writeAndRead(const std::vector<uint8_t>& commands,
                                      const size_t read_size) const;

    /**
     * @brief verifies that can communicate with the device
     *
     * @return true if can communicate with the device
     */
    bool probe();

    /**
     * @brief Provides the I2C address of the device
     *
     * @return uint8_t the I2C address of the device
     */
    uint8_t getI2CAddress() const {
        return i2c_address_.get_value();
    }

    /**
     * @brief Provides the I2C port number of the device
     *
     * @return uint32_t the I2C port number of the device
     */
    uint32_t getI2CPortNumber() const {
        return i2c_port_.get_value();
    }

   private:
    const I2CNumber i2c_port_;
    const I2CAddress i2c_address_;
    const std::string name_;

    i2c_master_dev_handle_t device_handle_;

    const int TIMEOUT_MS = 100;
};

}  // namespace idfx
