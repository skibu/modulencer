/* Driver for Elecrow Advance HMI displays of size 4.3", 5.0", or 7". All
 * three of those displays are exactly the same, except for their physical size.
 *
 * SPDX-License-Identifier: MIT
 */

#include "idfx/display/displayDriverBase.hpp"

namespace idfx {

class ElecrowDisplay : public DisplayDriverBase {
   public:
    ElecrowDisplay(int width, int height);

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
    lv_disp_t *disp;
 };

} // End of namespace idf