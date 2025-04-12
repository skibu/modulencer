/**
 * @file touchInputBase.hpp
 * @brief Base class for touch input
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "idfx/utils/log.hpp"

namespace idfx {

class TouchInputBase {
   public:
    TouchInputBase() {
        INFO("TouchInputBase constructor called");
    }

    virtual ~TouchInputBase() {
        ERROR("Destroying RGBDisplayBase object but should not destory hardware objects!");
    }

    // Delete copy constructor and copy assignment operator.
    // This is important because this object represents a piece of hardware and
    // should not be copied or assigned since then it would be wrongly destructed.
    TouchInputBase(const TouchInputBase&) = delete;
    TouchInputBase& operator=(const TouchInputBase&) = delete;

    void foo();
    
    /**
     * @brief Initialize the touch input
     *
     * @return true if initialization was successful
     * @return false if initialization failed
     */
    virtual bool init() = 0;

    /**
     * @brief Read the touch input
     *
     * @return true if touch input was read successfully
     * @return false if touch input read failed
     */
    virtual bool read() = 0;
};

}  // namespace idfx