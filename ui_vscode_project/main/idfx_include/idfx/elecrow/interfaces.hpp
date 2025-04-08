/*
 * Provides access to the global interfaces, such as I2C, for the Elecrow board.
 *
 * SPDX-License-Identifier: MIT
 */


//using namespace idf;

// I2C frequency constants
#define I2C_STANDARD_MODE_HZ 100000
#define I2C_FAST_MODE_HZ 400000
#define I2C_FAST_MODE_PLUS_HZ 1000000
#define I2C_HIGH_SPEED_MODE_HZ 3400000
#define I2C_ULTRA_FAST_MODE_HZ 5000000

// Configuration of the I2C bus
#define I2C_MASTER_NUM I2CNumber::I2C0()
#define I2C_MASTER_SCL_IO 16
#define I2C_MASTER_SDA_IO 15
#define I2C_FREQUENCY I2C_FAST_MODE_HZ

// Configuration of the IO expander
#define IO_EXPANDER_I2C_ADDR 0x18
#define BACKLIGHT_IO_BIT 1

// IO specifiecations for the touch panel
#define TP_INT 1
#define TP_RST 2
