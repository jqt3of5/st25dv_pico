//
// Created by jqt3o on 9/5/2021.
//
#include "picoMock.h"
#include <stdlib.h>
#include <memory.h>
uint16_t _address = 0;
uint8_t _memory[1024] = {0};

void i2c_write_blocking(int i2c, uint8_t device_select, uint8_t * data, int len, int nostop)
{
    _address = (data[0] << 8) | data[1];
    if (len > 2)
        memcpy(_memory + _address, data + 2, len-2);
}
void i2c_read_blocking(int i2c, uint8_t device_select, uint8_t * data, int len, int nostop)
{
    memcpy(data, _memory + _address, len);
    _address = _address + len;
}