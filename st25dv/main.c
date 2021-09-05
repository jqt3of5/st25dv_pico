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

    uint8_t addr[2] = {0x00,0x00};
    uint8_t buffer[16] = {0};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (true) {
	    gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(250);
	    gpio_put(PICO_DEFAULT_LED_PIN, 0);

        st25dv_read_random_user(addr, 6, buffer);
        for (int i = 0; i < 6; ++i)
        {
            printf("0x%X ", buffer[i]);
        }
        printf("\n");

        struct NDEF_Record *record;
        do {
            record = (struct NDEF_Record *) calloc(sizeof(struct NDEF_Record), 1);
            st25dv_read_record(record);
            //Parse payload into record types.




        } while(!record->header.ME);
        sleep_ms(5000);
    }
#pragma clang diagnostic pop
    return 0;
}
