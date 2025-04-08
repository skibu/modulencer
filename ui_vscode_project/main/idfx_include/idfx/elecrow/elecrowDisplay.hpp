/* Driver for Elecrow Advance HMI displays of size 4.3", 5.0", or 7". All
 * three of those displays are exactly the same, except for their physical size.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "idfx/display/displayDriverBase.hpp"

namespace idfx {

// Forward declaration of ElecrowBoard since we have a circular dependency
// between ElecrowBoard and ElecrowDisplay.
class ElecrowBoard;

class ElecrowDisplay : public DisplayDriverBase {
   public:
    ElecrowDisplay(int width, int height, ElecrowBoard &board);

   protected:
    void init() override;

    void flush() override;
    lv_disp_t *getDisp() override {
        return nullptr;
    };
    size_t getDispBufSize() override {
        return 0;
    };

   private:
    const ElecrowBoard &board_;
    lv_disp_t *disp;
};

}  // namespace idfx