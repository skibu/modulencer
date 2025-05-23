/**
 * @file elecrowBoard.hpp
 * @author Michael Smith
 * @brief Manages all the hardware on the Elecrow board.
 * @details This includes the display, touch panel, and IO expander.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <idfx/elecrow/elecrowDisplay.hpp>
#include <idfx/hardware/pca9557.hpp>

namespace idfx {

class ElecrowBoard {
   public:
    ElecrowBoard(int width = 800, int height = 480);

    ~ElecrowBoard();

    // Delete copy constructor and copy assignment operator.
    // This is important because this object represents a piece of hardware and
    // should not be copied or assigned since then it would be wrongly destructed.
    ElecrowBoard(const ElecrowBoard&) = delete;
    ElecrowBoard& operator=(const ElecrowBoard&) = delete;

    const PCA9557& getIOExpander() const {
        return io_expander_;
    }

    const ElecrowDisplay& getDisplay() {
        return display_;
    }

   private:
    // Note: order of these members is important because it sets the order of initialization, and
    // the display needs to be initialized after the IO expander, and the IO expander needs to be
    // initialized after the I2C bus.
    const I2CMasterBus i2c_master_bus_;
    const PCA9557 io_expander_;
    const ElecrowDisplay display_;
};

}  // namespace idfx