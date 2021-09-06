//
// Created by jqt3o on 9/5/2021.
//

#ifndef PICO_EXAMPLES_MAZER_H
#define PICO_EXAMPLES_MAZER_H


//The struct might be initialized with 0.
struct OutputPayload {
    uint8_t output_count;

    //Trying to avoid dynamic memory in the structs, makes parsing easier
    //16 bit pwm outputs. Period is dictated by the algorithm.
    uint16_t outputs[8];
};

//The struct might be initialized with 0.
struct ReadingPayload {
    uint8_t reading_count;
    //Trying to avoid dynamic memory in the structs, makes parsing easier
    //16 bit readings, depends on the sensor. But might be an ADC, K-type thermocouple, ds18b20
    uint16_t readings[8];
};

enum AlgorithmType {
    None,
    AutoPID,
    PID,
    Thermostatic
};

//The struct might be initialized with 0.
struct AlgorithmPayload {
    enum AlgorithmType type : 8;
    uint8_t input_count;
    uint8_t input_numbers[8];
    float set_point;
    unsigned int output_heat : 4;
    unsigned int output_cool : 4;
    float pwm_period;

    //For PID, if all three params are 0, then we're auto tuning
    float param1; //Kp, dTh;
    float param2; //Ki, dTl;
    float param3; //Kd, reserved;
};

struct MazerConfiguration {
    struct OutputPayload _outputs;
    struct ReadingPayload _readings;
    int _algorithmCount;
    struct AlgorithmPayload  _algorithms[5];
};

void read_configuration(struct MazerConfiguration * config);
void write_configuration(struct MazerConfiguration * config);
void print_configuration(struct MazerConfiguration * config);

#endif //PICO_EXAMPLES_MAZER_H
