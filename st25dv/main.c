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
#include <string.h>


//Holds the GPIO pins for our devices. The index in these arrays are what is used to reference them from the payloads
//ktypes use SPI, so the pins are the CS pins
int kType[] = {14};
int ds18b20[] = {12,13};
int outputs[] = {25};

struct OutputPayload _outputs;
struct ReadingPayload _readings;
int _algorithmCount = 0;
struct AlgorithmPayload  _algorithms[5];

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

    //Read configuration/state
    struct NDEF_Record * records[10] = {0};
    int count = st25dv_read_all_records(records);
    printf("records: %d", count);

    int algorithmCount = 0;
    for (int i = 0; i < count; ++i)
    {
        if (records[i]->header.TNF == External_Record)
        {
            if (!strncmp(records[i]->record_type, ReadingRTD, records[i]->type_length))
            {
                memcpy(&_readings,records[i]->payload, records[i]->payload_length);
            }
            if (!strncmp(records[i]->record_type, OutputRTD, records[i]->type_length))
            {
                memcpy(&_outputs,records[i]->payload, records[i]->payload_length);
            }
            if (!strncmp(records[i]->record_type, PID_RTD, records[i]->type_length))
            {
                memcpy(&_algorithms[algorithmCount],records[i]->payload, records[i]->payload_length);
                algorithmCount += 1;
            }
            if (!strncmp(records[i]->record_type, ThermostaticRTD, records[i]->type_length))
            {
                memcpy(&_algorithms[algorithmCount],records[i]->payload, records[i]->payload_length);
                algorithmCount += 1;
            }
        }
    }
    _algorithmCount = algorithmCount;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
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
