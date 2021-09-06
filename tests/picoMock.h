//
// Created by jqt3o on 9/5/2021.
//

#ifndef PICO_EXAMPLES_PICOMOCK_H
#define PICO_EXAMPLES_PICOMOCK_H

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned short

#define true 1
#define false 0
#define i2c_default 0

void i2c_write_blocking(int i2c, uint8_t device_select, uint8_t * data, int len, int nostop);
void i2c_read_blocking(int i2c, uint8_t device_select, uint8_t * data, int len, int nostop);
#endif //PICO_EXAMPLES_PICOMOCK_H
