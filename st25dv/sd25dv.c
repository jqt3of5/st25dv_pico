/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdlib.h>

static int addr1 = 0x2D;
static int addr2 = 0x53;
static int addr3 = 0x57;

#define SDA_PIN 4
#define SCL_PIN 5

#define DEVICE_SELECT_BYTE(e2) 0x50 | ((e2&0x1)<<2) | 0x3 


void st25dv_set_read_pointer(uint8_t addr[2])
{
    uint8_t device_select = DEVICE_SELECT_BYTE(0);
    i2c_write_blocking(i2c_default, device_select, addr, 2, true);
}
static void st25dv_read_current_user(int length, uint8_t * data)
{
    uint8_t device_select = DEVICE_SELECT_BYTE(0);
    i2c_read_blocking(i2c_default, device_select, data, length, false);
}

static void st25dv_read_random_user(uint8_t addr[2], int length, uint8_t * data)
{
    st25dv_set_read_pointer(addr);
    st25dv_read_current_user(length, data);
}

enum TNF {
    Empty = 0x00,
    WellKnown = 0x01,
    MIME_Type = 0x02,
    Absolute_URI = 0x03,
    External_Record = 0x04,
    Unknown_Record = 0x05,
    Unchanged_Record = 0x06
};

struct NDEF_Header {
    enum TNF TNF : 3;
    unsigned int IL : 1;
    unsigned int SR : 1;
    unsigned int CF : 1;
    unsigned int ME : 1;
    unsigned int MB : 1;
};

struct NDEF_Record {
   struct NDEF_Header header;
   uint8_t type_length;
   uint32_t payload_length;
   uint8_t ID_length;
   uint8_t record_type;
   uint8_t ID;
   uint8_t * payload;
};

bool st25dv_read_record(struct NDEF_Record * data)
{
    uint8_t header = 0;
    st25dv_read_current_user(1, &header);
    data->header = *((struct NDEF_Header*) &header);
    printf("MB: %d ME: %d CF: %d SR: %d IL: %d TNF: %d\n", data->header.MB, data->header.ME, data->header.CF, data->header.SR, data->header.IL, data->header.TNF);
    //Type Length
    st25dv_read_current_user(1, &data->type_length);
    printf("type_length: %d\n", data->type_length);
    //Payload Length
    st25dv_read_current_user(data->header.SR ? 1 : 4, &data->payload_length);
    printf("payload_length: %d\n", data->payload_length);
    //ID Length
    if (data->header.IL)
    {
        st25dv_read_current_user(1, &data->ID_length);
        printf("ID_length: %d\n", data->ID_length);
    }
    //Record Type
    st25dv_read_current_user(1, &data->record_type);
    printf("record_type: %d\n", data->record_type);
    //Record ID
    if (data->header.IL)
    {
        st25dv_read_current_user(1, &data->ID);
        printf("ID: %d\n", data->ID);
    }
    //Payload
    data->payload = (uint8_t*)calloc(1, data->payload_length);
    st25dv_read_current_user(data->payload_length, data->payload);
    printf("Payload: %s\n", data->payload);

    //TODO: follow chunked records
    return true;
}

int main() {
    stdio_init_all();

    i2c_init(i2c_default, 400*1000);
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);

    for (int j = 0; j < 15; ++j)
    {
        sleep_ms(1000);
        printf("time: %d", j);
    }
    printf("Device Configured, starting process \n i2c pins: %x %x", PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN);

    uint8_t addr[2] = {0x00,0x00};
    uint8_t buffer[16] = {0};


    while (true) {
	    gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(250);
	    gpio_put(PICO_DEFAULT_LED_PIN, 0);

        uint8_t addr[2] = {0x00,0x00};

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
        } while(!record->header.ME);
        sleep_ms(5000);
    }
    return 0;
}
