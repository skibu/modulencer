/**
 * @brief Implementation of the ElecrowBoard class.
 *
 * SPDX-License-Identifier: MIT
 */

#include "idfx/elecrow/elecrowBoard.hpp"

#include "idfx/hardware/i2c.hpp"
#include "idfx/hardware/pca9557.hpp"
#include "idfx/elecrow/interfaces.hpp"
#include "idfx/utils/log.hpp"
#include "esp-idf-cxx/gpio_cxx.hpp"

idfx::ElecrowBoard::ElecrowBoard(int width, int height)
    : i2c_master_bus_(I2CMasterBus::getBus(I2CNumber(I2C_MASTER_NUM), SCL_GPIO(I2C_MASTER_SCL_IO),
                                           SDA_GPIO(I2C_MASTER_SDA_IO))),
      io_expander_(I2CAddress(IO_EXPANDER_I2C_ADDR)),
      display_(width, height, *this) {
    INFO("Creating ElecrowBoard object");
}

idfx::ElecrowBoard::~ElecrowBoard() {
    // Using ERROR logging so that stack trace provided so can figure out why being called
    ERROR("ElecrowBoard destructor called, but should probably not be destructing hardware objects");
}