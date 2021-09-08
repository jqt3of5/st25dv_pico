/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "st25dv.h"
#include "Mazer.h"
#include <stdlib.h>
#include <string.h>
#include "max31855.h"
#include "Config.h"


void blink(int count, bool fast)
{
    for (int i = 0; i < count; ++i)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(fast ? 200 : 500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(fast ? 200 : 500);
    }
}

struct MazerConfiguration _configuration = {0};
void initialize_config()
{
    uint8_t device_select = ST25DV_DEVICE_SELECT_BYTE(0);
    uint8_t addr[2] = {0x00, 0x06};
    uint8_t data[128] = {0};
    i2c_write_blocking(I2C_PORT,device_select, addr, 2, true );
    i2c_read_blocking(I2C_PORT, device_select, data, 128, false);

    for (int i = 0; i < 128; ++i)
    {
        printf("0x%x ", data[i]);
    }
    //TODO: use a repo layer to cache state, and sync with nfc
//    read_configuration(&_configuration);

    //if (any changes)
    //TODO: write the config temporarily
    _configuration._readings.reading_count = 2;
    _configuration._readings.readings[0] = 100;

    _configuration._outputs.output_count = 4;
    _configuration._outputs.outputs[0] = 255;
    _configuration._outputs.outputs[1] = 100;

    _configuration._algorithmCount = 1;
    _configuration._algorithms[0].input_count = 1;
    _configuration._algorithms[0].input_numbers[0] = 0;
    _configuration._algorithms[0].pwm_period = 1;
    _configuration._algorithms[0].output_heat = 0;
    _configuration._algorithms[0].output_cool = 1;
    _configuration._algorithms[0].set_point = 123;
    _configuration._algorithms[0].type = PID;

    write_configuration(&_configuration);
    //turns out we need a slight delay after writing
    sleep_ms(1000);
    read_configuration(&_configuration);

#if DEBUG
    print_configuration(&_configuration);
#endif

}
int main() {
    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    //Pause to startup
    for(int i = 0; i < 10; ++i)
    {
        printf(".");
        sleep_ms(1000);
    }

    i2c_init(I2C_PORT, 400*1000);
    st25dv_init();



//    max31855_init(1, kTypePins);


    initialize_config();

    blink(3, true);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        //TODO: Run the loop
        //Read the inputs
//        int16_t probeTemp, coldTemp;
//        enum Max31855_Result max_result = max31855_read(kTypePins[0], &probeTemp, &coldTemp);
//        _configuration._readings.reading_count = 2;
//        switch(max_result)
//        {
//            case SuccessfulRead:
//                _configuration._readings.readings[0] = probeTemp;
//                _configuration._readings.readings[1] = coldTemp;
//                break;
//            case SCV:
//                _configuration._readings.readings[1] = coldTemp;
//                break;
//            case SCG:
//                _configuration._readings.readings[1] = coldTemp;
//                break;
//            case OC:
//                _configuration._readings.readings[1] = coldTemp;
//                break;
//            case TransferFailed:
//                break;
//        }

        //Run through the algorithms
        //Set the output PWMs
        //Read the NFC configuration

        sleep_ms(1000);
        blink(4, true);
    }
#pragma clang diagnostic pop
    return 0;
}
