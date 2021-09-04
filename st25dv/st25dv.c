//
// Created by jqt3o on 8/22/2021.
//

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "st25dv.h"
#include <stdlib.h>

#define DEVICE_SELECT_BYTE(e2) 0x50 | ((e2&0x1)<<2) | 0x3
void st25dv_init(int sda, int scl)
{
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);
}
void st25dv_set_read_pointer(uint8_t addr[2])
{
    uint8_t device_select = DEVICE_SELECT_BYTE(0);
    i2c_write_blocking(i2c_default, device_select, addr, 2, true);
}
void st25dv_read_current_user(int length, uint8_t * data)
{
    uint8_t device_select = DEVICE_SELECT_BYTE(0);
    i2c_read_blocking(i2c_default, device_select, data, length, false);
}

void st25dv_read_random_user(uint8_t addr[2], int length, uint8_t * data)
{
    st25dv_set_read_pointer(addr);
    st25dv_read_current_user(length, data);
}

bool st25dv_read_record(struct NDEF_Record * data) {
    uint8_t header = 0;
    st25dv_read_current_user(1, &header);
    data->header = *((struct NDEF_Header *) &header);
    printf("MB: %d ME: %d CF: %d SR: %d IL: %d TNF: %d\n",
            data->header.MB,
            data->header.ME,
            data->header.CF,
            data->header.SR,
            data->header.IL,
            data->header.TNF);
    //Type Length
    st25dv_read_current_user(1, &data->type_length);
    printf("type_length: %d\n", data->type_length);
    //Payload Length
    st25dv_read_current_user(data->header.SR ? 1 : 4, &data->payload_length);
    printf("payload_length: %d\n", data->payload_length);
    //ID Length
    if (data->header.IL) {
        st25dv_read_current_user(1, &data->ID_length);
        printf("ID_length: %d\n", data->ID_length);
    }
    //Record Type
    data->record_type = (uint8_t *) calloc(1, data->type_length);
    st25dv_read_current_user(data->type_length, &data->record_type);
    printf("record_type: %d\n", data->record_type);
    //Record ID
    if (data->header.IL) {
        st25dv_read_current_user(1, &data->ID);
        printf("ID: %d\n", data->ID);
    }
    //Payload
    data->payload = (uint8_t *) calloc(1, data->payload_length);
    st25dv_read_current_user(data->payload_length, data->payload);
    printf("Payload: %s\n", data->payload);

    //TODO: follow chunked records
    return true;
}


