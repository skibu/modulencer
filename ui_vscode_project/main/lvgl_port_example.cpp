/*
 * Initial code for setting up lvgl port for the Elecrow board
 * This code is based on the example code from the ESP-IDF LVGL port
 * and the ESP-IDF RGB panel driver example.
 */

#include "driver/i2c_master.h"  // Note: this specifies that using v2 of i2c driver (i2c.h is for v1)
#include "esp_check.h"
#include "esp_err.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_touch_gt911.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "idfx/hardware/i2c.hpp"
#include "idfx/elecrow/elecrowBoard.hpp"
#include "lv_demos.h"

#define ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS (0x5D)

/* LCD size DONE */
#define EXAMPLE_LCD_H_RES (800)
#define EXAMPLE_LCD_V_RES (480)

/* LCD settings DONE just using values from the original example */
#define EXAMPLE_LCD_LVGL_FULL_REFRESH (0)
#define EXAMPLE_LCD_LVGL_DIRECT_MODE (1)
#define EXAMPLE_LCD_LVGL_AVOID_TEAR (1)
#define EXAMPLE_LCD_RGB_BOUNCE_BUFFER_MODE (1)
#define EXAMPLE_LCD_DRAW_BUFF_DOUBLE (0)
#define EXAMPLE_LCD_DRAW_BUFF_HEIGHT (100)
#define EXAMPLE_LCD_RGB_BUFFER_NUMS (2)
#define EXAMPLE_LCD_RGB_BOUNCE_BUFFER_HEIGHT (10)

#define USE_DMA_FOR_BUFFERS (false)

/* LCD pins */
#define EXAMPLE_LCD_GPIO_VSYNC (GPIO_NUM_41 /*3*/)
#define EXAMPLE_LCD_GPIO_HSYNC (GPIO_NUM_40 /*46*/)
#define EXAMPLE_LCD_GPIO_DE (GPIO_NUM_42 /*17*/)
#define EXAMPLE_LCD_GPIO_PCLK (GPIO_NUM_39 /*9*/)
#define EXAMPLE_LCD_GPIO_DISP (GPIO_NUM_NC)

/* LCD data pins DONE */
#define EXAMPLE_LCD_GPIO_DATA0 (GPIO_NUM_21)
#define EXAMPLE_LCD_GPIO_DATA1 (GPIO_NUM_47)
#define EXAMPLE_LCD_GPIO_DATA2 (GPIO_NUM_48)
#define EXAMPLE_LCD_GPIO_DATA3 (GPIO_NUM_45)
#define EXAMPLE_LCD_GPIO_DATA4 (GPIO_NUM_38)
#define EXAMPLE_LCD_GPIO_DATA5 (GPIO_NUM_9)
#define EXAMPLE_LCD_GPIO_DATA6 (GPIO_NUM_10)
#define EXAMPLE_LCD_GPIO_DATA7 (GPIO_NUM_11)
#define EXAMPLE_LCD_GPIO_DATA8 (GPIO_NUM_12)
#define EXAMPLE_LCD_GPIO_DATA9 (GPIO_NUM_13)
#define EXAMPLE_LCD_GPIO_DATA10 (GPIO_NUM_14)
#define EXAMPLE_LCD_GPIO_DATA11 (GPIO_NUM_7)
#define EXAMPLE_LCD_GPIO_DATA12 (GPIO_NUM_17)
#define EXAMPLE_LCD_GPIO_DATA13 (GPIO_NUM_18)
#define EXAMPLE_LCD_GPIO_DATA14 (GPIO_NUM_3)
#define EXAMPLE_LCD_GPIO_DATA15 (GPIO_NUM_46)

// Here are the original values from the driver example
// #define EXAMPLE_LCD_GPIO_DATA0 (GPIO_NUM_10)
// #define EXAMPLE_LCD_GPIO_DATA1 (GPIO_NUM_11)
// #define EXAMPLE_LCD_GPIO_DATA2 (GPIO_NUM_12)
// #define EXAMPLE_LCD_GPIO_DATA3 (GPIO_NUM_13)
// #define EXAMPLE_LCD_GPIO_DATA4 (GPIO_NUM_14)
// #define EXAMPLE_LCD_GPIO_DATA5 (GPIO_NUM_21)
// #define EXAMPLE_LCD_GPIO_DATA6 (GPIO_NUM_47)
// #define EXAMPLE_LCD_GPIO_DATA7 (GPIO_NUM_48)
// #define EXAMPLE_LCD_GPIO_DATA8 (GPIO_NUM_45)
// #define EXAMPLE_LCD_GPIO_DATA9 (GPIO_NUM_38)
// #define EXAMPLE_LCD_GPIO_DATA10 (GPIO_NUM_39)
// #define EXAMPLE_LCD_GPIO_DATA11 (GPIO_NUM_40)
// #define EXAMPLE_LCD_GPIO_DATA12 (GPIO_NUM_41)
// #define EXAMPLE_LCD_GPIO_DATA13 (GPIO_NUM_42)
// #define EXAMPLE_LCD_GPIO_DATA14 (GPIO_NUM_2)
// #define EXAMPLE_LCD_GPIO_DATA15 (GPIO_NUM_1)

/* Touch settings DONE */
// #define EXAMPLE_TOUCH_I2C_NUM (0)
#define EXAMPLE_TOUCH_I2C_CLK_HZ (400000)
// Note: the default address is 0x5D so only need to change this if the address is different
#define ESP_LCD_TOUCH_IO_I2C_ADDRESS (0x5D)

/* LCD touch pins DONE */
#define EXAMPLE_TOUCH_I2C_SCL (16)
#define EXAMPLE_TOUCH_I2C_SDA (15)

/* LCD timing DONE though don't know why this was called 35HZ timing */
#define EXAMPLE_LCD_PANEL_35HZ_RGB_TIMING() \
    {                                       \
        .pclk_hz = 21 /*18*/ * 1000 * 1000, \
        .h_res = EXAMPLE_LCD_H_RES,         \
        .v_res = EXAMPLE_LCD_V_RES,         \
        .hsync_pulse_width = 4 /*40*/,      \
        .hsync_back_porch = 8 /*40*/,       \
        .hsync_front_porch = 8 /*48*/,      \
        .vsync_pulse_width = 4 /*23*/,      \
        .vsync_back_porch = 8 /*32*/,       \
        .vsync_front_porch = 8 /*13*/,      \
        .flags.pclk_active_neg = true,      \
    }

static const char *TAG = "EXAMPLE";

// LVGL image declare
LV_IMG_DECLARE(esp_logo)

/* LCD IO and panel */
static esp_lcd_panel_handle_t lcd_panel = NULL;
static esp_lcd_touch_handle_t touch_handle = NULL;

/* LVGL display and touch */
static lv_display_t *lvgl_disp = NULL;
static lv_indev_t *lvgl_touch_indev = NULL;

static esp_err_t app_lcd_init(void) {
    esp_err_t ret = ESP_OK;

    /* LCD initialization */
    ESP_LOGI(TAG, "Initialize RGB panel");
    esp_lcd_rgb_panel_config_t panel_conf = {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .timings =
            {
                .pclk_hz = 21 /*18*/ * 1000 * 1000,
                .h_res = EXAMPLE_LCD_H_RES,
                .v_res = EXAMPLE_LCD_V_RES,
                .hsync_pulse_width = 4 /*40*/,
                .hsync_back_porch = 8 /*40*/,
                .hsync_front_porch = 8 /*48*/,
                .vsync_pulse_width = 4 /*23*/,
                .vsync_back_porch = 8 /*32*/,
                .vsync_front_porch = 8 /*13*/,
                .flags =
                    {
                        .hsync_idle_low = false,
                        .vsync_idle_low = false,
                        .de_idle_high = false,
                        .pclk_active_neg = true,
                        .pclk_idle_high = false,
                    },
            },
        .data_width = 16,
        .bits_per_pixel = 16,
        .num_fbs = EXAMPLE_LCD_RGB_BUFFER_NUMS,
        .bounce_buffer_size_px = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_RGB_BOUNCE_BUFFER_HEIGHT,
        .sram_trans_align = 0,    // deprecated
        .psram_trans_align = 64,  // deprecated
        .hsync_gpio_num = EXAMPLE_LCD_GPIO_HSYNC,
        .vsync_gpio_num = EXAMPLE_LCD_GPIO_VSYNC,
        .de_gpio_num = EXAMPLE_LCD_GPIO_DE,
        .pclk_gpio_num = EXAMPLE_LCD_GPIO_PCLK,
        .disp_gpio_num = EXAMPLE_LCD_GPIO_DISP,
        .data_gpio_nums =
            {
                EXAMPLE_LCD_GPIO_DATA0,
                EXAMPLE_LCD_GPIO_DATA1,
                EXAMPLE_LCD_GPIO_DATA2,
                EXAMPLE_LCD_GPIO_DATA3,
                EXAMPLE_LCD_GPIO_DATA4,
                EXAMPLE_LCD_GPIO_DATA5,
                EXAMPLE_LCD_GPIO_DATA6,
                EXAMPLE_LCD_GPIO_DATA7,
                EXAMPLE_LCD_GPIO_DATA8,
                EXAMPLE_LCD_GPIO_DATA9,
                EXAMPLE_LCD_GPIO_DATA10,
                EXAMPLE_LCD_GPIO_DATA11,
                EXAMPLE_LCD_GPIO_DATA12,
                EXAMPLE_LCD_GPIO_DATA13,
                EXAMPLE_LCD_GPIO_DATA14,
                EXAMPLE_LCD_GPIO_DATA15,
            },
        .flags =
            {
                .disp_active_low = false, /* If this flag is enabled, a low level of display control
                                         signal can turn the screen on; vice versa */
                .refresh_on_demand = false, /* If this flag is enabled, the host only refresh the
                                           frame buffer in `esp_lcd_panel_draw_bitmap` and
                                           `esp_lcd_rgb_panel_refresh`. */
                .fb_in_psram = true, /* If this flag is enabled, the frame buffer will be allocated
                                     from PSRAM, preferentially */
                .double_fb = false,  /* If this flag is enabled, the driver will allocate two screen
                                    sized frame buffer, same as num_fbs=2 */
                .no_fb = false, /* If this flag is enabled, the driver won't allocate frame buffer.
                               Instead, user should fill in the bounce buffer manually in the
                               on_bounce_empty` callback */
                .bb_invalidate_cache = false, /* If this flag is enabled, in bounce back mode we'll
                         do a cache invalidate on the read data, freeing the cache. Can be
                         dangerous if data is written from other core(s). */

            },
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_rgb_panel(&panel_conf, &lcd_panel), err, TAG, "RGB init failed");
    ESP_GOTO_ON_ERROR(esp_lcd_panel_init(lcd_panel), err, TAG, "LCD init failed");

    return ret;

err:
    if (lcd_panel) {
        esp_lcd_panel_del(lcd_panel);
    }
    return ret;
}

/**
 * Calls esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle) to setup the gt911 touch
 * controller. Sets up touch_handle global so it can be used for LVGL.
 *
 * @return esp_err_t
 */
static esp_err_t app_touch_init(void) {
    // NOTE: driver already installed via the I2CBusManager class

    // /* Initilize I2C */
    // const i2c_config_t i2c_conf = {.mode = I2C_MODE_MASTER,
    //                                .sda_io_num = EXAMPLE_TOUCH_I2C_SDA,
    //                                .sda_pullup_en = GPIO_PULLUP_DISABLE,
    //                                .scl_io_num = EXAMPLE_TOUCH_I2C_SCL,
    //                                .scl_pullup_en = GPIO_PULLUP_DISABLE,
    //                                .master.clk_speed = EXAMPLE_TOUCH_I2C_CLK_HZ};
    // ESP_RETURN_ON_ERROR(i2c_param_config(EXAMPLE_TOUCH_I2C_NUM, &i2c_conf), TAG,
    //                     "I2C configuration failed");
    // // ESP_RETURN_ON_ERROR(i2c_driver_install(EXAMPLE_TOUCH_I2C_NUM, i2c_conf.mode, 0, 0, 0),
    // TAG,
    // //                     "I2C initialization failed");
    // // If I2C driver is already installed, i2c_driver_install() will return an error
    // esp_err_t result = i2c_driver_install(EXAMPLE_TOUCH_I2C_NUM, i2c_conf.mode, 0, 0, 0);
    // if (result != ESP_OK) {
    //     ESP_LOGE(TAG, "I2C driver installation failed result=%d: %s", result,
    //     esp_err_to_name(result)); ESP_LOGE(TAG, "But continuing anyway");
    // }

    /* Initialize touch HW */
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_H_RES,
        .y_max = EXAMPLE_LCD_V_RES,
        .rst_gpio_num = GPIO_NUM_NC,
        .int_gpio_num = GPIO_NUM_1,
        .levels =
            {
                .reset = 0,
                .interrupt = 0,
            },
        .flags =
            {
                .swap_xy = 0,
                .mirror_x = 0,
                .mirror_y = 0,
            },
        .process_coordinates = nullptr,
        .interrupt_callback = nullptr,
        .user_data = nullptr,
        .driver_data = nullptr,
    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = {
        .dev_addr = ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS, /* I2C device address */
        .on_color_trans_done = nullptr, /* Callback invoked when color data transfer has finished */
        .user_ctx =
            nullptr, /* User private data, passed directly to on_color_trans_done's user_ctx */
        .control_phase_bytes = 1, /* I2C LCD panel will encode control information (e.g. D/C
           selection) into control phase, in several bytes */
        .dc_bit_offset = 0,       /* Offset of the D/C selection bit in control phase */
        .lcd_cmd_bits = 16,       /* Bit-width of LCD command */
        .lcd_param_bits = 0,      /* Bit-width of LCD parameter */
        .flags =
            {
                .dc_low_on_data = false, /* If this flag is enabled, DC line = 0 means transfer
                                        data, DC line = 1 means transfer command; vice versa */
                .disable_control_phase =
                    true, /* If this flag is enabled, the control phase isn't used */
            },
        .scl_speed_hz = idfx::I2CDevice::DEFAULT_I2C_FREQUENCY,
    };

    // NOTE: the default is 0x5D
    tp_io_config.dev_addr = ESP_LCD_TOUCH_IO_I2C_ADDRESS;
    ESP_LOGI(TAG, "In app_touch_init(). About to get bus handle via getBusHandle()");
    i2c_master_bus_handle_t bus_handle = idfx::I2CMasterBus::getBusHandle(idfx::I2CNumber::I2C0());
    ESP_LOGI(TAG, "In app_touch_init() bus_handle=%p", bus_handle);
    ESP_LOGI(TAG, "In app_touch_init() about to call esp_lcd_new_panel_io_i2c()...");
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_i2c(bus_handle, &tp_io_config, &tp_io_handle), TAG,
                        "");

    ESP_LOGI(TAG, "In app_touch_init() about to call esp_lcd_touch_new_i2c_gt911()...");
    return esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle);
}

static esp_err_t app_lvgl_init(void) {
    /* Initialize LVGL */
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,       /* LVGL task priority */
        .task_stack = 6144,       /* LVGL task stack size */
        .task_affinity = -1,      /* LVGL task pinned to core (-1 is no affinity) */
        .task_max_sleep_ms = 500, /* Maximum sleep in LVGL task */
        .timer_period_ms = 5      /* LVGL timer tick period in ms */
    };
    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

    uint32_t buff_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT;
#if EXAMPLE_LCD_LVGL_FULL_REFRESH || EXAMPLE_LCD_LVGL_DIRECT_MODE
    buff_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES;
#endif

    /* Add LCD screen */
    ESP_LOGD(TAG, "Add LCD screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = nullptr,
        .panel_handle = lcd_panel,
        .control_handle = nullptr,
        .buffer_size = buff_size,
        .double_buffer = EXAMPLE_LCD_DRAW_BUFF_DOUBLE,
        .trans_size = 0,
        .hres = EXAMPLE_LCD_H_RES,
        .vres = EXAMPLE_LCD_V_RES,
        .monochrome = false,
        .rotation =
            {
                .swap_xy = false,
                .mirror_x = false,
                .mirror_y = false,
            },
        .color_format = LV_COLOR_FORMAT_RGB565,
        .flags =
            {
                .buff_dma = USE_DMA_FOR_BUFFERS,
                .buff_spiram = false,
                .sw_rotate = true,
                .swap_bytes = false,
#if EXAMPLE_LCD_LVGL_FULL_REFRESH
                .full_refresh = true,
                .direct_mode = false,
#elif EXAMPLE_LCD_LVGL_DIRECT_MODE
                .full_refresh = false,
                .direct_mode = true,
#endif
            },
    };
    const lvgl_port_display_rgb_cfg_t rgb_cfg = {.flags = {
#if EXAMPLE_LCD_RGB_BOUNCE_BUFFER_MODE
                                                     .bb_mode = true,
#else
                                                     .bb_mode = false,
#endif
#if EXAMPLE_LCD_LVGL_AVOID_TEAR
                                                     .avoid_tearing = true,
#else
                                                     .avoid_tearing = false,
#endif
                                                 }};
    lvgl_disp = lvgl_port_add_disp_rgb(&disp_cfg, &rgb_cfg);

    /* Add touch input (for selected screen) */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = lvgl_disp,
        .handle = touch_handle,
    };
    lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);

    return ESP_OK;
}

/**
 * @brief callback for when button is clicked
 *
 * Note: cannot log within a callback function. Also, calling lv_display_set_rotation()
 * is not reliable. Both will pretty quickly cause an abort.
 *
 * @param e
 */
static void _app_button_cb(lv_event_t *e) {
    //ESP_LOGI(TAG, "============================ Button clicked");
    lv_disp_rotation_t rotation = lv_disp_get_rotation(lvgl_disp);
    int rotation_value = static_cast<int>(rotation);
    rotation_value++;
    if (rotation_value > LV_DISPLAY_ROTATION_270) {
        rotation_value = LV_DISPLAY_ROTATION_0;
    }

    /* LCD HW rotation */
    //ESP_LOGI(TAG, "============================ Setting LCD rotation %d", rotation);
    //lv_display_set_rotation(lvgl_disp, static_cast<lv_disp_rotation_t>(rotation_value));
    //ESP_LOGI(TAG, "============================ Done setting LCD rotation %d", rotation);
}

static void app_main_display(void) {
    lv_obj_t *scr = lv_scr_act();

    /* Your LVGL objects code here .... */

    /* Create image */
    lv_obj_t *img_logo = lv_img_create(scr);
    lv_img_set_src(img_logo, &esp_logo);
    lv_obj_align(img_logo, LV_ALIGN_TOP_MID, 0, 20);

    /* Label */
    lv_obj_t *label = lv_label_create(scr);
    lv_obj_set_width(label, EXAMPLE_LCD_H_RES);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
#if LVGL_VERSION_MAJOR == 8
    lv_label_set_recolor(label, true);
    lv_label_set_text(label,
                      "#FF0000 " LV_SYMBOL_BELL " Hello world Espressif and LVGL " LV_SYMBOL_BELL
                      "#\n#FF9400 " LV_SYMBOL_WARNING
                      " For simplier initialization, use BSP " LV_SYMBOL_WARNING " #");
#else
    lv_label_set_text(label, LV_SYMBOL_BELL
                      " Hello world Espressif and LVGL " LV_SYMBOL_BELL "\n " LV_SYMBOL_WARNING
                      " For simplier initialization, use BSP " LV_SYMBOL_WARNING);
#endif
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 20);

    /* Button */
    lv_obj_t *btn = lv_btn_create(scr);
    label = lv_label_create(btn);
    lv_label_set_text_static(label, "Rotate screen");
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_add_event_cb(btn, _app_button_cb, LV_EVENT_CLICKED, NULL);
}

// forward declaration
void lv_example_chart_1(void);

extern "C" void lvgl_port_app_main(idfx::ElecrowBoard *board_ptr) {
    /* LCD HW initialization */

    // Determine lcd_panel
    //ESP_ERROR_CHECK(app_lcd_init());
    lcd_panel = board_ptr->getDisplay().getPanel();

    //const auto lcd_panel = board_ptr->getDisplay().getPanel();

    /* Touch initialization */
    ESP_ERROR_CHECK(app_touch_init());

    /* LVGL initialization */
    ESP_ERROR_CHECK(app_lvgl_init());

    /* Show LVGL objects */
    lvgl_port_lock(0);
    app_main_display();
    // lv_example_chart_1();
    lvgl_port_unlock();
}