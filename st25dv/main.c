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

//Holds the GPIO pins for our devices. The index in these arrays are what is used to reference them from the payloads

//ktypes use SPI, so the pins are the CS pins
int kType[] = {14};
int ds18b20[] = {12,13};
int relays[] = {25};

struct MazerConfiguration _configuration = {0};
void initialize()
{
    read_configuration(&_configuration);

    if (_configuration._outputs.output_count == 0)
    {

    }
    if (_configuration._readings.reading_count == 0)
    {

    }

    //if (any changes)
    write_configuration(&_configuration);

#if DEBUG
    print_configuration(&_configuration);
#endif
}
int main() {
    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    i2c_init(i2c_default, 400*1000);
    st25dv_init(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN);

    //Pause to startup
    for(int i = 0; i < 10; ++i)
    {
        printf(".");
        sleep_ms(1000);
    }


    blink(3, true);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
        //TODO: Run the loop
        //Read the inputs
        //Run through the algorithms
        //Set the output PWMs
        //Read the NFC configuration

        sleep_ms(5000);
        blink(4, true);
    }
#pragma clang diagnostic pop
    return 0;
}
