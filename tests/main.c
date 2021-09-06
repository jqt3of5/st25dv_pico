//
// Created by jqt3o on 9/5/2021.
//
#include "picoMock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../st25dv/st25dv.h"
#include "../st25dv/Mazer.h"

extern uint8_t _memory[1024];
int main()
{
    struct MazerConfiguration config = {0};
    config._readings.reading_count = 2;
    config._readings.readings[0] = 400;
    config._readings.readings[1] = 500;

    config._outputs.output_count = 1;
    config._outputs.outputs[0] = 255;

    config._algorithmCount = 3;
    config._algorithms[0].type = AutoPID;
    config._algorithms[0].param1 = 1.234f;
    config._algorithms[0].param2 = 2.345f;
    config._algorithms[0].param3 = 100;
    config._algorithms[0].set_point = 100;
    config._algorithms[0].input_count = 2;
    config._algorithms[0].input_numbers[0] = 123;
    config._algorithms[0].input_numbers[1] = 255;
    config._algorithms[0].output_cool = 1;
    config._algorithms[0].output_heat = 2;
    config._algorithms[0].pwm_period = 0.5f;

    config._algorithms[1].type = PID;
    config._algorithms[1].param1 = 1.234f;
    config._algorithms[1].param2 = 2.345f;
    config._algorithms[1].param3 = 100;
    config._algorithms[1].set_point = 100;
    config._algorithms[1].input_count = 2;
    config._algorithms[1].input_numbers[0] = 123;
    config._algorithms[1].input_numbers[1] = 255;
    config._algorithms[1].output_cool = 1;
    config._algorithms[1].output_heat = 2;
    config._algorithms[1].pwm_period = 0.5f;

    config._algorithms[2].type = Thermostatic;
    config._algorithms[2].param1 = 1.234f;
    config._algorithms[2].param2 = 2.345f;
    config._algorithms[2].param3 = 100;
    config._algorithms[2].set_point = 100;
    config._algorithms[2].input_count = 2;
    config._algorithms[2].input_numbers[0] = 123;
    config._algorithms[2].input_numbers[1] = 255;
    config._algorithms[2].output_cool = 1;
    config._algorithms[2].output_heat = 2;
    config._algorithms[2].pwm_period = 0.5f;

    write_configuration(&config);

    struct MazerConfiguration config2 = {0};

    read_configuration(&config2);

    int result = memcmp(&config, &config2, sizeof(struct MazerConfiguration));

    printf("memory comparison: %d\n", result);

    print_configuration(&config2) ;

}
