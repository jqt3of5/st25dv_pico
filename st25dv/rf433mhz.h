//
// Created by jqt3o on 9/2/2021.
//

#ifndef PICO_EXAMPLES_RF433MHZ_H
#define PICO_EXAMPLES_RF433MHZ_H

void transmit(int pin, char * data, int length, int periodms);
int receive(int pin, char * data);

#endif //PICO_EXAMPLES_RF433MHZ_H
