//
// Created by jqt3o on 9/7/2021.
//

#ifndef PICO_EXAMPLES_CONFIG_H
#define PICO_EXAMPLES_CONFIG_H

#include <pico.h>

#define I2C_PORT i2c_default
#define I2C_SDA PICO_DEFAULT_I2C_SDA_PIN
#define I2C_SCL PICO_DEFAULT_I2C_SCL_PIN

//Holds the GPIO pins for our devices. The index in these arrays are what is used to reference them from the payloads

//ktypes use SPI, so the pins are the CS pins. Count as two, due to cold compensator
extern int kTypeCount;
extern uint kTypePins[];
extern int ds18b20Count;
extern uint ds18b20Pins[];
extern int relayCount;
extern uint relayPins[];

#endif //PICO_EXAMPLES_CONFIG_H
