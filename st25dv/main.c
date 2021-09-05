/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "st25dv.h"
#include <stdlib.h>

int main() {
    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    i2c_init(i2c_default, 400*1000);
    st25dv_init(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN);

    for(int i = 0; i < 10; ++i)
    {
        printf(".");
        sleep_ms(1000);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    struct NDEF_Record * records[10] = {0};
    int j = 0;
    while (true) {
	    gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(250);
	    gpio_put(PICO_DEFAULT_LED_PIN, 0);

	    int count = st25dv_read_all_records(records);
	    printf("records: %d", count);
	    for(int i = 0; i < count; ++i)
        {
	        printf("0x%X ", records[i]->payload[0]);
	        records[i]->payload[0] = j;
	        j += 1;
        }

	    st25dv_write_records(count, records);
        sleep_ms(5000);
    }
#pragma clang diagnostic pop
    return 0;
}
