/*
 * This is similar to i2c_cxx.cpp, except that uses the new I2C driver.
 *
 * SPDX-License-Identifier: MIT
 */

#ifdef __cpp_exceptions

#include "idfx/hardware/i2c.hpp"

#include <cstdio>

#include "driver/i2c_master.h"
#include "idfx/hardware/i2c.hpp"
#include "idfx/utils/log.hpp"

using namespace std;

namespace idfx {

/**
 * Convenience macro to help converting IDFX error codes into a child of ESPException.
 */
#undef CHECK_THROW_SPECIFIC
#define CHECK_THROW_SPECIFIC(error_, exception_type_)              \
    do {                                                           \
        esp_err_t result = (error_);                               \
        if (result != ESP_OK) throw idfx::exception_type_(result); \
    } while (0)

#define I2C_CHECK_THROW(err) CHECK_THROW_SPECIFIC((err), I2CException)

/**
 * I2C bus are defined in the header files, let's check that the values are correct
 */
#if SOC_I2C_NUM >= 2
static_assert(I2C_NUM_1 == 1, "I2C_NUM_1 must be equal to 1");
#endif  // SOC_I2C_NUM >= 2

I2CException::I2CException(esp_err_t error) : ESPException(error) {}

esp_err_t check_i2c_num(uint32_t i2c_num) noexcept {
    if (i2c_num >= I2C_NUM_MAX) {
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

esp_err_t check_i2c_addr(uint32_t addr) noexcept {
    // maximum I2C address currently supported in the C++ classes is 127
    if (addr > 0x7f) {
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

I2CAddress::I2CAddress(const uint8_t addr) : StrongValueComparable<uint8_t>(addr) {
    esp_err_t error = check_i2c_addr(addr);
    if (error != ESP_OK) {
        throw I2CException(error);
    }
}

// Declare static variables for class I2CMasterBus
I2CMasterBus *I2CMasterBus::bus_handle_port0_ptr_;
I2CMasterBus *I2CMasterBus::bus_handle_port1_ptr_;

I2CMasterBus &I2CMasterBus::getBus(const I2CNumber i2c_port, const SCL_GPIO scl_gpio,
                                   const SDA_GPIO sda_gpio, const bool enable_internal_pullups) {
    // If already have a bus for the port, return it
    if (i2c_port == I2CNumber::I2C0()) {
        if (I2CMasterBus::bus_handle_port0_ptr_ != nullptr) {
            return *I2CMasterBus::bus_handle_port0_ptr_;
        }
    } else if (i2c_port == I2CNumber::I2C1()) {
        if (I2CMasterBus::bus_handle_port1_ptr_ != nullptr) {
            return *I2CMasterBus::bus_handle_port1_ptr_;
        }
    }

    // There wasn't one created yet so create it now
    I2CMasterBus *bus_ptr = new I2CMasterBus(i2c_port, scl_gpio, sda_gpio, enable_internal_pullups);

    if (i2c_port == I2CNumber::I2C0()) {
        I2CMasterBus::bus_handle_port0_ptr_ = bus_ptr;
    } else if (i2c_port == I2CNumber::I2C1()) {
        I2CMasterBus::bus_handle_port1_ptr_ = bus_ptr;
    }
    return *bus_ptr;
}

i2c_master_bus_handle_t &I2CMasterBus::getBusHandle(const I2CNumber i2c_port) {
    if (i2c_port == I2CNumber::I2C0()) {
        if (bus_handle_port0_ptr_ != nullptr) {
            return bus_handle_port0_ptr_->bus_handle_;
        }
    } else if (i2c_port == I2CNumber::I2C1()) {
        if (bus_handle_port1_ptr_ != nullptr) {
            return bus_handle_port1_ptr_->bus_handle_;
        }
    }

    // If we get here, then the bus was not created yet. Can't handle that so throw error
    throw I2CException(ESP_ERR_INVALID_STATE);
}

I2CMasterBus::I2CMasterBus(const I2CNumber i2c_port, const SCL_GPIO scl_gpio,
                           const SDA_GPIO sda_gpio, const bool enable_internal_pullups = true) {
    // Configure the bus for the specified i2c port.
    // Will throw exception if port already configured.
    conf_ = {
        .i2c_port = (int)i2c_port.get_value(),
        .sda_io_num = (gpio_num_t)sda_gpio.get_value(),
        .scl_io_num = (gpio_num_t)scl_gpio.get_value(),
        .clk_source = i2c_clock_source_t::I2C_CLK_SRC_XTAL,
        .glitch_ignore_cnt = 7,
        .intr_priority = 1,
        .trans_queue_depth = 10,  // queue for async writes
        .flags = {.enable_internal_pullup = (uint32_t)enable_internal_pullups, .allow_pd = 0},
    };
    I2C_CHECK_THROW(i2c_new_master_bus(&conf_, &bus_handle_));
}

void I2CMasterBus::reset() {
    I2C_CHECK_THROW(i2c_master_bus_reset(bus_handle_));
}

// This callback only exists so that writes can be asynchronous. It doesn't actually do do anything.
static bool masterEventCallback(i2c_master_dev_handle_t i2c_dev,
                                const i2c_master_event_data_t *evt_data, void *arg) {
    VERBOSE("I2C write finished callback called. %s", ((I2CDevice *)arg)->toName().c_str());

    // Return that did not wake up higher priority task
    return false;
}

I2CDevice::I2CDevice(const I2CAddress i2c_address, const std::string name, const I2CNumber i2c_port,
                     const Frequency clock_speed)
    : i2c_port_(i2c_port), i2c_address_(i2c_address), name_(name) {
    // Get already created bus handle for the port.
    i2c_master_bus_handle_t bus_handle = I2CMasterBus::getBusHandle(i2c_port);

    // Configure the I2C device
    i2c_device_config_t dev_conf = {.dev_addr_length = i2c_addr_bit_len_t::I2C_ADDR_BIT_LEN_7,
                                    .device_address = i2c_address.get_value(),
                                    .scl_speed_hz = clock_speed.get_value(),
                                    .scl_wait_us = 0,
                                    .flags = {.disable_ack_check = 0}};
    I2C_CHECK_THROW(i2c_master_bus_add_device(bus_handle, &dev_conf, &device_handle_));

    // Register an event callback so that writes can be asynchronous. Pass in pointer to this
    // I2CDevice instance so that we can access the device handle in the callback.
    INFO("FIXME - Not registering I2C callback for now");
    // i2c_master_event_callbacks_t callbacks = {.on_trans_done = masterEventCallback};
    // INFO("Registering I2C event callback for device=%s", toName().c_str());
    // I2C_CHECK_THROW(i2c_master_register_event_callbacks(device_handle_, &callbacks, this));
}

I2CDevice::~I2CDevice() {
    // Delete the device info if it exists
    if (device_handle_ != nullptr) {
        // Degister the event callback
        i2c_master_register_event_callbacks(device_handle_, nullptr, nullptr);

        // Remove the device from the bus
        i2c_master_bus_rm_device(device_handle_);
        device_handle_ = nullptr;
    }
}

std::string I2CDevice::toName() const {
    // Using sprintf() instead of stringstream since I think it is more readable
    char buffer[200];
    int num_chars = sprintf(buffer, "I2CDevice %s(port I2C%lu, address 0x%02X)", name_.c_str(),
                            i2c_port_.get_value(), i2c_address_.get_value());
    if (num_chars >= 0)
        return std::string(buffer, num_chars);
    else
        return "error";
}

bool I2CDevice::asyncWrite(const uint8_t command) const {
    auto result = i2c_master_transmit(device_handle_, &command, 1, TIMEOUT_MS);

    return ESP_OK == result;
}

bool I2CDevice::asyncWrite(const std::vector<uint8_t> &commands) const {
    // Convert commands data vector to a uint8_t array buffer
    const std::size_t commands_size = commands.size();
    const uint8_t *commands_buffer = commands.data();

    // Actually transmit the data
    I2C_CHECK_THROW(
        i2c_master_transmit(device_handle_, commands_buffer, commands_size, TIMEOUT_MS));

    return true;
}

std::vector<uint8_t> I2CDevice::writeAndRead(const uint8_t command, const size_t read_size) const {
    // Convert commands data vector to a uint8_t array buffer
    const size_t commands_size = 1;
    const uint8_t *commands_buffer = &command;

    // Actually send the command and read the data
    uint8_t read_buffer[read_size];
    VERBOSE("About to send command=0x%02X, read_size=%zu, read_buffer[0]=0x%02X", command,
            read_size, read_buffer[0]);
    I2C_CHECK_THROW(i2c_master_transmit_receive(device_handle_, commands_buffer, commands_size,
                                                read_buffer, read_size, TIMEOUT_MS));
    VERBOSE("After read the read_buffer[0]=0x%02X", read_buffer[0]);

    // Convert the read buffer to a vector and return it
    std::vector<uint8_t> result(read_buffer, read_buffer + read_size);
    return result;
}

std::vector<uint8_t> I2CDevice::writeAndRead(const std::vector<uint8_t> &commands,
                                             const size_t read_size) const {
    // Convert commands data vector to a uint8_t array buffer
    const size_t commands_size = commands.size();
    const uint8_t *commands_buffer = commands.data();

    // Actually send the command and read the data
    uint8_t read_buffer[read_size];
    VERBOSE(
        "Sending cmd cmds_size=%zu, commands_buf[0]=0x%02X, commands_buf[1]=0x%02X, read_size=%zu",
        commands_size, commands_buffer[0], commands_buffer[1], read_size);
    I2C_CHECK_THROW(i2c_master_transmit_receive(device_handle_, commands_buffer, commands_size,
                                                read_buffer, read_size, TIMEOUT_MS));
    VERBOSE("After read the read_buffer[0]=0x%02X", read_buffer[0]);

    // Convert the read buffer to a vector and return it
    std::vector<uint8_t> result(read_buffer, read_buffer + read_size);
    return result;
}

bool I2CDevice::probe() {
    // Get already created bus handle for the port
    i2c_master_bus_handle_t bus_handle = I2CMasterBus::getBusHandle(i2c_port_);

    // Probe the I2C address
    esp_err_t err = i2c_master_probe(bus_handle, i2c_address_.get_value(), TIMEOUT_MS);

    // Log and return the result
    if (err != ESP_OK) {
        ERROR("I2C probe for address 0x%02X failed: %s", i2c_address_.get_value(),
              esp_err_to_name(err));
        return false;
    } else {
        ERROR("I2C probe for address 0x%02X succeeded", i2c_address_.get_value());
        return true;
    }
}

}  // namespace idfx

#endif  // __cpp_exceptions
