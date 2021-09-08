//
// Created by jqt3o on 9/6/2021.
//

#ifndef PICO_EXAMPLES_MAX31855_H
#define PICO_EXAMPLES_MAX31855_H

enum Max31855_Result {
    SuccessfulRead,
    SCV, //thermocouple shorted to VCC
    SCG, //thermocouple shorted to GND
    OC, //thermocouple open
    TransferFailed // the spi transfer failed to read all bytes
};

struct max31855_data {
   char OC : 1;
   char SCG: 1;
   char SCV : 1;
   char __reserved__: 1;
   int coldTemp : 12;
   char fault : 1;
   char __reserved1__: 1;
   int thermocoupleTemp : 14;
};

void max31855_init(int count, uint csPins[]);
enum Max31855_Result max31855_read(int csPin, int16_t * probeTemp, int16_t * coldTemp);


#endif //PICO_EXAMPLES_MAX31855_H
