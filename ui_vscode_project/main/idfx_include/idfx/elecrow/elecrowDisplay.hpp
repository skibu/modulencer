/* Driver for Elecrow Advance HMI displays of size 4.3", 5.0", or 7". All
 * three of those displays are exactly the same, except for their physical size.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "idfx/display/rgbDisplayBase.hpp"
// #include "idfx/elecrow/elecrowBoard.hpp"
#include "idfx/hardware/io.hpp"
#include "idfx/utils/log.hpp"

namespace idfx {

// Forward declaration of ElecrowBoard class. Needed due to circular reference.
class ElecrowBoard;

class ElecrowDisplay : public RGBDisplayBase {
   public:
    ElecrowDisplay(uint32_t width, uint32_t height, ElecrowBoard& elecrow_board);

    ~ElecrowDisplay();

    // Delete copy constructor and copy assignment operator.
    // This is important because this object represents a piece of hardware and
    // should not be copied or assigned since then it would be wrongly destructed.
    ElecrowDisplay(const ElecrowDisplay&) = delete;
    ElecrowDisplay& operator=(const ElecrowDisplay&) = delete;

    /**
     * @brief Turns on or off the display's backlight
     * Virtual function that must be implemented by the derived class
     */
    void turnOnBacklight(bool on) const;

   private:
    const OutputBit backlight_io_;
};

}  // namespace idfx