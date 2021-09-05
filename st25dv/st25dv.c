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
void st25dv_set_read_pointer(uint16_t addr)
{
    uint8_t device_select = DEVICE_SELECT_BYTE(0);
    i2c_write_blocking(i2c_default, device_select, (uint8_t*)&addr, 2, true);
}
void st25dv_read_current_user(int length, uint8_t * data)
{
    uint8_t device_select = DEVICE_SELECT_BYTE(0);
    i2c_read_blocking(i2c_default, device_select, data, length, false);
}

void st25dv_read_random_user(uint16_t addr, int length, uint8_t * data)
{
    st25dv_set_read_pointer(addr);
    st25dv_read_current_user(length, data);
}

int st25dv_count_records() {
    return 0;
}

int st25dv_skip_record() {
    return 0;
}

int st25dv_record_byte_count(struct NDEF_Record * record)
{
    int byteCount = 0;
    //header
    byteCount += 1;
    //type_length;
    byteCount += 1;
    //payload_length
    byteCount += record->header.SR ? 1 : 4;
    //ID Length
    byteCount += record->header.IL ? 1 : 0;
    //Record Type
    byteCount += record->type_length;
    //Record ID
    byteCount += record->header.IL ? record->ID_length : 0;
    //Payload
    byteCount += record->payload_length;

    return byteCount;
}

int st25dv_record_bytes(struct NDEF_Record * record, uint8_t ** data)
{
    int count = st25dv_record_byte_count(record);
    uint8_t * bytes= (uint8_t*)calloc(count + 2, 1);
    *data = bytes;

    int currentByte = 0;

    //Skip two bytes for the address
    currentByte = 2;
    bytes[currentByte] = *((uint8_t*)&record->header);
    currentByte += 1;

    bytes[currentByte] = *((uint8_t*)&record->type_length);
    currentByte += 1;

    uint8_t * payload_length = ((uint8_t*)&record->payload_length);
    if (!record->header.SR)
    {
        bytes[currentByte] = payload_length[3];
        currentByte += 1;
        bytes[currentByte] = payload_length[2];
        currentByte += 1;
        bytes[currentByte] = payload_length[1];
        currentByte += 1;
    }
    bytes[currentByte] = payload_length[0];
    currentByte += 1;

    if (record->header.IL)
    {
        bytes[currentByte] = *((uint8_t*)&record->ID_length);
        currentByte += 1;
    }

    uint8_t  * recordType = ((uint8_t*)&record->record_type);
    for (int j = record->type_length-1; j >= 0; --j)
    {
        bytes[currentByte] = recordType[j];
        currentByte += 1;
    }

    if (record->header.IL)
    {
        bytes[currentByte] = record->ID;
        currentByte += 1;
    }

    //TODO: memcopy?
    for (int j = 0; j < record->payload_length; ++j)
    {
        bytes[currentByte] = record->payload[j];
        currentByte += 1;
    }

    return count;
}

void st25dv_write_records(int count, struct NDEF_Record * records[])
{
    uint8_t device_select = DEVICE_SELECT_BYTE(0);
    //skip the CC file
    uint16_t address = 0x06;
    for (int i = 0; i < count; ++i)
    {
        uint8_t * bytes;
        int byteCount = st25dv_record_bytes(records[i], &bytes);
        bytes[0] = (address >> 8) & 0xff;
        bytes[1] = address & 0xff;

        i2c_write_blocking(i2c_default, device_select, bytes, byteCount, false);
        address += byteCount;
    }
}

int st25dv_read_all_records(struct NDEF_Record* records[])
{
    //skip the CC file
    st25dv_set_read_pointer(0x0600);

    int i = 0;
    do {
        records[i] = (struct NDEF_Record *) calloc(sizeof(struct NDEF_Record), 1);
        st25dv_read_record(records[i]);
        i += 1;
    } while(!records[i-1]->header.ME);

    return i;
}

int st25dv_read_record(struct NDEF_Record * data) {
    uint8_t header = 0;
    st25dv_read_current_user(1, &header);
    data->header = *((struct NDEF_Header *) &header);

#if DEBUG
    printf("MB: %d ME: %d CF: %d SR: %d IL: %d TNF: %d\n",
            data->header.MB,
            data->header.ME,
            data->header.CF,
            data->header.SR,
            data->header.IL,
            data->header.TNF);
#endif
    //Type Length
    st25dv_read_current_user(1, &data->type_length);
#if DEBUG
    printf("type_length: %d\n", data->type_length);
#endif

    //Payload Length
    st25dv_read_current_user(data->header.SR ? 1 : 4, (uint8_t*)&data->payload_length);
#if DEBUG
    printf("payload_length: %d\n", data->payload_length);
#endif

    //ID Length
    if (data->header.IL) {
        st25dv_read_current_user(1, &data->ID_length);
#if DEBUG
        printf("ID_length: %d\n", data->ID_length);
#endif
    }
    //Record Type
    data->record_type = (uint8_t *) calloc(1, data->type_length);
    st25dv_read_current_user(data->type_length, data->record_type);
#if DEBUG
    printf("record_type: %d\n", data->record_type);
#endif

    //Record ID
    if (data->header.IL) {
        st25dv_read_current_user(1, &data->ID);
#if DEBUG
        printf("ID: %d\n", data->ID);
#endif
    }

    //Payload
    data->payload = (uint8_t *) calloc(1, data->payload_length);
    st25dv_read_current_user(data->payload_length, data->payload);
#if DEBUG
    printf("Payload: %s\n", data->payload);
#endif

    //TODO: follow chunked records. Combine Payloads together
    return data->payload_length;
}


