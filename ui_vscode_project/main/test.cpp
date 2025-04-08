#include "test.hpp"

#include "idfx/elecrow/elecrowBoard.hpp"
#include "idfx/elecrow/interfaces.hpp"
#include "idfx/hardware/interrupts.hpp"
#include "idfx/hardware/io.hpp"
#include "idfx/hardware/pca9557.hpp"
#include "idfx/utils/log.hpp"
#include "idfx/utils/time.hpp"


void individual_isr_function(int num) {
    INFO("====================== in individual_isr_function() for bit %d", num);
}

void buzz(idfx::OutputPWM& buzzer_pwm) {
    INFO("Setting buzzer frequency to 500Hz");
    buzzer_pwm.setFrequency(500);
    idfx::sleep(1000ms);
    INFO("Setting buzzer frequency to 1000Hz");
    buzzer_pwm.setFrequency(1000);
    idfx::sleep(1000ms);
}
extern "C" void lvgl_port_app_main(void);

void testNew(void) {
    INFO("=============== Starting Test Program ===============\n");

    // Create Elecrow board, which for now turns on backlight. Create it using new
    // so that it doesn't get destructed when going out of scope.
    auto board_ptr = new idfx::ElecrowBoard(800, 480);

    // Run the lvgl port application to see if can display anything
    lvgl_port_app_main();

    if (true) return;

    // // Play with buzzer
    // auto buzzer = idf::OutputBit(GPIONum(GPIO_NUM_8), "buzzer");
    // buzzer.setOn();
    // idf::sleep(1000ms);
    // buzzer.setOff();

    // Configure interrupt handler on pin 3
    auto GPIO_BIT = GPIO_NUM_3;
    idfx::GpioInterrupteHandler(GPIONum(GPIO_BIT), individual_isr_function);

    for (;;) {
        INFO("========= Setting up buzzer OutputPWM for GPIO_NUM_8 ==========");
        idfx::OutputPWM buzzer_pwm(GPIO_NUM_8);

        INFO("Testing buzzer at duty cycle of 50, 5, and 0");
        buzzer_pwm.setDuty(50);
        buzz(buzzer_pwm);
        buzzer_pwm.setDuty(5);
        buzz(buzzer_pwm);
        buzzer_pwm.setDuty(0);
        buzz(buzzer_pwm);

        // buzzer.setOn();
        // idf::sleep(1000ms);
        // buzzer.setOff();

        INFO("Still in main loop. Tripping the test ISR...");
        gpio_set_level(GPIO_BIT, 0);
        idfx::sleep(1ms);
        gpio_set_level(GPIO_BIT, 1);
        INFO("bit %d value now is %d", GPIO_BIT, gpio_get_level(GPIO_BIT));
        idfx::sleep(10s);
    };
}