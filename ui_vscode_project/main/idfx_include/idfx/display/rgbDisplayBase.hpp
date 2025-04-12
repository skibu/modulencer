/**
 * @file rgbDisplay.hpp
 * @brief Base class for RGB displays
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_rgb.h>

#include "esp-idf-cxx/gpio_cxx.hpp"
#include "idfx/utils/log.hpp"

namespace idfx {

/**
 * @brief Base class for 16-bit RGB656 SPI displays
 *
 * 16-bit per pixel RGB565 SPI displays are very different from the more common 1-bit displays.
 * A 16-bit display transfers 16 parallel bits of data at a time, meaning that the color data can be
 * transmitted 16 times as fast, enabling higher refresh rates and smoother animations. This is
 * especially important for displays that are 800px x 480px or larger, where the amount of data
 * to be transferred is quite large.
 *
 * ESP provides a special driver for RGB displays that is different from the SPI driver. This class
 * is a wrapper around the ESP driver to make it easier to use with LVGL. It provides a common
 * interface for all RGB displays, and handles the initialization and configuration of the display.
 * This particular driver is accessed by this class using esp_lcd_new_rgb_panel(). This ESP driver
 * actually does most of the work.
 *
 * The RGB565 specification is a 16-bit color format that uses 5 bits for red, 6 bits for green, and
 * 5 bits for blue. This format is commonly used in displays because it provides a good balance
 * between color depth and memory usage. The RGB565 format is also compatible with the LVGL library,
 * which uses a 16-bit color format for its graphics.
 */
class RGBDisplayBase {
   public:
    /**
     * @brief Construct a new RGBDisplayBase object
     * Initializes the panel configuration structure to default values. But the super
     * class still needs to set the GPIO pins for the display and a few other parameters.
     * @param width of display in pixels
     * @param height  of display in pixels
     */
    RGBDisplayBase(uint32_t width, uint32_t height);

    /**
     * @brief Destroy the RGBDisplayBase object
     * Should not be called since should not destroy hardware objects.
     */
    ~RGBDisplayBase() {
        ERROR("Destroying RGBDisplayBase object but should not destory hardware objects!");
    }

    // Delete copy constructor and copy assignment operator.
    // This is important because this object represents a piece of hardware and
    // should not be copied or assigned since then it would be wrongly destructed.
    RGBDisplayBase(const RGBDisplayBase&) = delete;
    RGBDisplayBase& operator=(const RGBDisplayBase&) = delete;

    /**
     * @brief Get the Panel object
     * The panel object is one of the components needed to create an LVGL display driver.
     * @note function cannot be const since it initializes member panel_handle_ if it hasn't been
     * initialized yet.
     * @return esp_lcd_panel_handle_t the panel object
     */
    esp_lcd_panel_handle_t getPanel() const;

    /**
     * @brief Turns on or off the display's backlight
     * Virtual function that must be implemented by the derived class
     */
    virtual void turnOnBacklight(bool on) const = 0;

    /**
     * @brief Get the Width of the display
     *
     * @return int the width of the display in pixels
     */
    int getWidth() const {
        return width_;
    }

    /**
     * @brief Get the Height of the display
     *
     * @return int the height of the display in pixels
     */
    int getHeight() const {
        return height_;
    }

    // For chaining setting of configuration
   protected:
    /**
     * @brief Optional. Set the timing pclk_hz if default of 21Mhz not correct
     *
     * @param pclk_hz
     */
    void setPclkHz(uint32_t pclk_hz) {
        panel_conf_.timings.pclk_hz = pclk_hz;
    }

    /**
     * @brief Optional. Set the timing hsync_pulse_width if default of 4 not correct
     *
     * @param pulse_width
     */
    void setHSyncPulseWidth(uint32_t pulse_width) {
        panel_conf_.timings.hsync_pulse_width = pulse_width;
    }

    /**
     * @brief Optional. Set the timing hsync_back_porch if default of 8 not correct
     *
     * @param back_porch
     */
    void setHSyncBackPorch(uint32_t back_porch) {
        panel_conf_.timings.hsync_back_porch = back_porch;
    }

    /**
     * @brief Optional. Set the timing hsync_front_porch if default of 8 not correct
     *
     * @param front_porch
     */
    void setHSyncFrontPorch(uint32_t front_porch) {
        panel_conf_.timings.hsync_front_porch = front_porch;
    }

    /**
     * @brief Optional. Set the timing vsync_pulse_width if default of 4 not correct
     *
     * @param pulse_width
     */
    void setVSyncPulseWidth(uint32_t pulse_width) {
        panel_conf_.timings.vsync_pulse_width = pulse_width;
    }

    /**
     * @brief Optional. Set the timing vsync_back_porch if default of 8 not correct
     *
     * @param back_porch
     */
    void setVSyncBackPorch(uint32_t back_porch) {
        panel_conf_.timings.vsync_back_porch = back_porch;
    }

    /**
     * @brief Optional. Set the timing vsync_front_porch if default of 8 not correct
     *
     * @param front_porch
     */
    void setVSyncFrontPorch(uint32_t front_porch) {
        panel_conf_.timings.vsync_front_porch = front_porch;
    }

    /**
     * @brief Optional. Set the timing flag hsync_idle_low if default of false not correct
     *
     * @param idle_low
     */
    void setHsyncIdleLow(bool idle_low) {
        panel_conf_.timings.flags.hsync_idle_low = idle_low;
    }

    /**
     * @brief Optional. Set the timing flag vsync_idle_low if default of false not correct
     *
     * @param idle_low
     */
    void setVsyncIdleLow(bool idle_low) {
        panel_conf_.timings.flags.vsync_idle_low = idle_low;
    }

    /**
     * @brief Optional. Set the timing flag de_idle_high if default of false not correct
     *
     * @param idle_high
     */
    void setDeIdleHigh(bool idle_high) {
        panel_conf_.timings.flags.de_idle_high = idle_high;
    }

    /**
     * @brief Optional. Set the timing flag pclk_active_neg if default of true not correct
     *
     * @param active_neg
     */
    void setPclkActiveNeg(bool active_neg) {
        panel_conf_.timings.flags.pclk_active_neg = active_neg;
    }

    /**
     * @brief Optional. Set the timing flag pclk_idle_high if default of false not correct
     *
     * @param idle_high
     */
    void setPclkIdleHigh(bool idle_high) {
        panel_conf_.timings.flags.pclk_idle_high = idle_high;
    }

    /**
     * @brief Optional. Set num_fbs number of frame buffers to value different from default of 2
     *
     * @param num_fbs Can be 1 to 3. The default of 2 is recommended
     */
    void setNumFbs(size_t num_fbs) {
        if (num_fbs < 1 || num_fbs > 3) {
            ERROR("Invalid number of frame buffers: %d. Must be 1 to 3", num_fbs);
            return;
        }
        panel_conf_.num_fbs = num_fbs;
    }

    /**
     * @brief Optional. Set the flags.disp_active_low if default of false is not correct
     *
     * If this flag is enabled, a low level of display control signal can turn the screen on.
     * This param can be modified.
     *
     * @param value
     */
    void setFlagDispActiveLow(bool value) {
        panel_conf_.flags.disp_active_low = value;
    }

    /**
     * @brief Optional. Set the Bounce Buffer Height in pixels
     * The recommended value is 1/10 of the height of the display, which is the default.
     * Setting to a higher or lower value may improve or decrease performance.
     * @param height_px in pixels
     */
    void setBounceBufferHeight(size_t height_px) {
        panel_conf_.bounce_buffer_size_px = width_ * height_px;
    }

    /**
     * @brief Mandatory. Set the hsync_gpio_num GPIO number for the display
     *
     * @param gpio_num
     */
    void setHSyncGpioNum(GPIONum gpio_num) {
        panel_conf_.hsync_gpio_num = gpio_num.get_value();
    }

    /**
     * @brief Mandatory. Set the vsync_gpio_num GPIO number for the display
     *
     * @param gpio_num
     */
    void setVSyncGpioNum(GPIONum gpio_num) {
        panel_conf_.vsync_gpio_num = gpio_num.get_value();
    }

    /**
     * @brief Mandatory. Set the de_gpio_num GPIO number for the display
     *
     * @param gpio_num
     */
    void setDeGpioNum(GPIONum gpio_num) {
        panel_conf_.de_gpio_num = gpio_num.get_value();
    }

    /**
     * @brief Mandatory. Set the pclk_gpio_num GPIO number for the display
     *
     * @param gpio_num
     */
    void setPclkGpioNum(GPIONum gpio_num) {
        panel_conf_.pclk_gpio_num = gpio_num.get_value();
    }

    /**
     * @brief Optional. Set the disp_gpio_num GPIO number for the display
     * Many displays don't use this pin so the default of GPIO_NUM_NC is usually adequate.
     * @param gpio_num
     */
    void setDispGpioNum(GPIONum gpio_num) {
        panel_conf_.disp_gpio_num = gpio_num.get_value();
    }

    /**
     * @brief Mandetory. Set the RGB GPIO bit
     *
     * Must be used by derived display class for all 16 GPIO pins.
     *
     * @param index which one to set
     * @param gpio_num the GPIO number to set it to
     */
    void setRgbGpio(uint32_t index, GPIONum gpio_num) {
        panel_conf_.data_gpio_nums[index] = gpio_num.get_value();
    }

   protected:
    const int width_;
    const int height_;
    mutable esp_lcd_panel_handle_t panel_handle_ = nullptr;  // mutable since just a cache value
    esp_lcd_rgb_panel_config_t panel_conf_ = {};
};

}  // namespace idfx