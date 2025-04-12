/**
 *
 * SPDX-License-Identifier: MIT
 */
#include "idfx/display/rgbDisplayBase.hpp"

#include "idfx/utils/log.hpp"
#include "esp_lcd_panel_ops.h"  // Include the header for esp_lcd_panel_init

using namespace idfx;

RGBDisplayBase::RGBDisplayBase(uint32_t width, uint32_t height) : width_(width), height_(height) {
    DEBUG("RGBDisplayBase constructor called");

    // Initialize config structure to defaults. Then then super class
    // only needs to set a subset of the fields.
    panel_conf_.clk_src = LCD_CLK_SRC_PLL160M;

    esp_lcd_rgb_timing_t default_timings = {
        .pclk_hz = 21 /*18*/ * 1000 * 1000,  // Example code used 18MHz
        .h_res = width,
        .v_res = height,
        .hsync_pulse_width = 4,  // Example code used 40
        .hsync_back_porch = 8,   // Example code used 40
        .hsync_front_porch = 8,  // Example code used 48
        .vsync_pulse_width = 4,  // Example code used 23
        .vsync_back_porch = 8,   // Example code used 32
        .vsync_front_porch = 8,  // Example code used 13
        .flags =
            {
                .hsync_idle_low = false,
                .vsync_idle_low = false,
                .de_idle_high = false,
                .pclk_active_neg = true,
                .pclk_idle_high = false,
            },
    };
    panel_conf_.timings = default_timings;

    // RGB display class is only for RGB565 where color data is 16 bits
    panel_conf_.data_width = 16;
    panel_conf_.bits_per_pixel = 16;

    // Best results with 2 frame buffers, though can be set to 1
    panel_conf_.num_fbs = 2;  // Can be set to 1 to 3

    // Bounce buffer size is recommended to be set to 1/10 of the height of the display.
    // It will be created in onboard RAM, which is precious, so use it wisely.
    const size_t BOUNCE_BUFFER_HEIGHT = static_cast<size_t>(height / 10);
    panel_conf_.bounce_buffer_size_px = width * BOUNCE_BUFFER_HEIGHT;

    // sram_trans_align & psram_trans_align are marked as deprecated
    // panel_conf_.sram_trans_align = 0;
    // panel_conf_.psram_trans_align = 64;

    // Note: Super class MUST set the following gpio pins to the proper values
    panel_conf_.hsync_gpio_num = GPIO_NUM_NC;
    panel_conf_.vsync_gpio_num = GPIO_NUM_NC;
    panel_conf_.de_gpio_num = GPIO_NUM_NC;
    panel_conf_.pclk_gpio_num = GPIO_NUM_NC;
    // But disp_gpio_num does not need to be set
    panel_conf_.disp_gpio_num = GPIO_NUM_NC;

    // Initialize the RGB GPIO values to a bad value to force super class to set them
    for (int i = 0; i < 16; ++i) {
        panel_conf_.data_gpio_nums[i] = GPIO_NUM_NC;
    }

    // Set default flags
    // If this flag is enabled, a low level of display control signal can turn the screen on.
    // This param can be modified.
    panel_conf_.flags.disp_active_low = false;
    // If this flag is enabled, the host only refresh the frame buffer in
    // `esp_lcd_panel_draw_bitmap`and esp_lcd_rgb_panel_refresh`
    panel_conf_.flags.refresh_on_demand = false;
    // Framebuffer needs to be in PSRAM since it is too large for internal RAM
    panel_conf_.flags.fb_in_psram = true;
    // If this flag is enabled, the driver will allocate two screen sized frame buffer, same as
    // num_fbs = 2
    panel_conf_.flags.double_fb = false;
    // If this flag is enabled, the driver won't allocate frame buffer. Instead, user should fill in
    // the bounce buffer manually in the on_bounce_empty` callback
    panel_conf_.flags.no_fb = false;
    // If this flag is enabled, in bounce back mode we'll do a cache invalidate on the read data,
    // freeing the cache. Can be dangerous if data is written from other core(s).
    panel_conf_.flags.bb_invalidate_cache = false;
}

esp_lcd_panel_handle_t RGBDisplayBase::getPanel() const{
    // Initialize panel if haven't done so yet
    if (panel_handle_ == nullptr) {
        DEBUG("Initializing esp_lcd_panel_handle_t panel using the specified configuration...");

        // Check that all the GPIO pins have been set. If not then abort app since display cannot work.
        ASSERT(panel_conf_.hsync_gpio_num != GPIO_NUM_NC);
        ASSERT(panel_conf_.vsync_gpio_num != GPIO_NUM_NC);
        ASSERT(panel_conf_.de_gpio_num != GPIO_NUM_NC);
        ASSERT(panel_conf_.pclk_gpio_num != GPIO_NUM_NC);
        for (int i = 0; i < 16; ++i) {
            ASSERT(panel_conf_.data_gpio_nums[i] != GPIO_NUM_NC);
        }

        // Create the panel
        auto result = esp_lcd_new_rgb_panel(&panel_conf_, &panel_handle_);
        if (result != ESP_OK) {
            ERROR("Failed to create RGB panel: %s", esp_err_to_name(result));
            abort();
        }

        // Initialize the panel
        result = esp_lcd_panel_init(panel_handle_);
        if (result != ESP_OK) {
            ERROR("Failed to initialize RGB panel: %s", esp_err_to_name(result));
            // Free the resource, though this is minor since program definitely will not work
            esp_lcd_panel_del(panel_handle_);
            abort();
        }
    }

    return panel_handle_;
}
