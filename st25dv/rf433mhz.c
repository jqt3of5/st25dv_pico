//
// Created by jqt3o on 9/2/2021.
//
#include <pico.h>
#include <hardware/gpio.h>
#include <hardware/timer.h>
#include "rf433mhz.h"
enum Preamble {
   ZigZag,
   Blinds
};

void rf433_init(uint pin)
{

}
void rf433_transmit(uint pin, char * data, int length, int periodms, enum Preamble preamble)
{
    int low_us = periodms * 1000 / 3.0;
    int high_us = periodms * 1000 - low_us;

    switch (preamble)
    {
        case ZigZag:
            for (int i = 0; i < 16; ++i)
            {
                gpio_put(pin, true);
                busy_wait_us(periodms*1000);
                gpio_put(pin, false);
                busy_wait_us(periodms*1000);
            }
            break;
        case Blinds:
            gpio_put(pin, true);
            busy_wait_us(5000);
            gpio_put(pin, false);
            busy_wait_us(2500);
            gpio_put(pin, true);
            busy_wait_us(1000);
            break;
    }

    for (int i = 0; i < 8*length; ++i)
    {
        bool value = data[i/8] & (1 << i%8);
        gpio_put(pin, true);
        if (value)
        {
            busy_wait_us(high_us);
        } else {
            busy_wait_us(low_us);
        }

        gpio_put(pin, false);
        if (value)
        {
            busy_wait_us(low_us);
        } else {
            busy_wait_us(high_us);
        }
    }
}

int rf433_receive(char * data)
{

}
