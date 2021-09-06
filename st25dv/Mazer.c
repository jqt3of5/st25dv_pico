//
// Created by jqt3o on 9/5/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "st25dv.h"
#include "Mazer.h"

const char * OutputRTD= "urn:nfc:ext:substantive.tech:output";
const char * ReadingRTD= "urn:nfc:ext:substantive.tech:reading";
const char * PID_RTD= "urn:nfc:ext:substantive.tech:pid";
const char * ThermostaticRTD= "urn:nfc:ext:substantive.tech:thermostatic";
const char * UnknownRTD= "urn:nfc:ext:substantive.tech:unknown";

void write_configuration(struct MazerConfiguration * config)
{
    struct NDEF_Record * records[10] = {0};

    records[0] = (struct NDEF_Record *)calloc(sizeof(struct NDEF_Record), 1);
//    records[0]->header.IL = 0;
//    records[0]->header.CF = 0;
//    records[0]->header.ME = 0;
    records[0]->header.SR = 1;
    records[0]->header.MB = 1;
    records[0]->header.TNF = External_Record;

    int payload_type_length = strlen(ReadingRTD);
    records[0]->record_type = (char *)calloc(payload_type_length + 1, 1);
    memcpy(records[0]->record_type, ReadingRTD, payload_type_length);
    records[0]->type_length = payload_type_length;

    //readings payload
    //TODO: We're copying the whole struct, even though not every element holds data
    records[0]->payload_length = sizeof(struct ReadingPayload);
    records[0]->payload = (uint8_t*)calloc(sizeof(struct ReadingPayload), 1);
    memcpy(records[0]->payload, &config->_readings, sizeof(struct ReadingPayload));

    records[1] = (struct NDEF_Record *)calloc(sizeof(struct NDEF_Record), 1);

    records[1]->header.SR = 1;
    records[1]->header.TNF = External_Record;

    payload_type_length = strlen(OutputRTD);
    records[1]->record_type = (char *)calloc(payload_type_length + 1, 1);
    memcpy(records[1]->record_type, OutputRTD, payload_type_length);
    records[1]->type_length = payload_type_length;

    //outputs payload
    //TODO: We're copying the whole struct, even though not every element holds data
    records[1]->payload_length = sizeof(struct OutputPayload);
    records[1]->payload = (uint8_t*)calloc(sizeof(struct OutputPayload), 1);
    memcpy(records[1]->payload, &config->_outputs, sizeof(struct OutputPayload));

    int i = 0;
    for (i = 0; i < config->_algorithmCount; ++i)
    {
        struct AlgorithmPayload * algPayload = &config->_algorithms[i];

        struct NDEF_Record * record = (struct NDEF_Record *)calloc(sizeof(struct NDEF_Record), 1);
        records[i + 2] = record;

        record->header.SR = 1;
        record->header.TNF = External_Record;

        switch(algPayload->type)
        {
            case None:
                payload_type_length = strlen(UnknownRTD);
                record->record_type = (char *)calloc(payload_type_length + 1, 1);
                memcpy(record->record_type, UnknownRTD, payload_type_length);
                record->type_length = payload_type_length;
                continue;
            case AutoPID:
            case PID:
                payload_type_length = strlen(PID_RTD);
                record->record_type = (char *)calloc(payload_type_length+1, 1);
                memcpy(record->record_type, PID_RTD, payload_type_length);
                record->type_length = payload_type_length;
                break;
            case Thermostatic:
                payload_type_length = strlen(ThermostaticRTD);
                record->record_type = (char *)calloc(payload_type_length+1, 1);
                memcpy(record->record_type, ThermostaticRTD, payload_type_length);
                record->type_length = payload_type_length;
                break;
        }

        //TODO: We're copying thewhole struct, even though not every element holds data
        record->payload_length = sizeof(struct AlgorithmPayload);
        record->payload = (uint8_t*)calloc(sizeof(struct AlgorithmPayload), 1);
        memcpy(record->payload, algPayload, sizeof(struct AlgorithmPayload));
    }

    //the last record should have message end flag set
    records[config->_algorithmCount + 2 - 1]->header.ME = 1;

    st25dv_write_records(config->_algorithmCount + 2, records);
}

void read_configuration(struct MazerConfiguration * config)
{
    //Read configuration/state
    struct NDEF_Record * records[10] = {0};
    int count = st25dv_read_all_records(records);
    printf("records: %d", count);

    int algorithmCount = 0;
    for (int i = 0; i < count; ++i)
    {
        if (records[i]->header.TNF == External_Record)
        {
            if (!strncmp(records[i]->record_type, ReadingRTD, records[i]->type_length))
            {
                memcpy(&config->_readings,records[i]->payload, records[i]->payload_length);
            }
            if (!strncmp(records[i]->record_type, OutputRTD, records[i]->type_length))
            {
                memcpy(&config->_outputs,records[i]->payload, records[i]->payload_length);
            }
            if (!strncmp(records[i]->record_type, PID_RTD, records[i]->type_length))
            {
                memcpy(&config->_algorithms[algorithmCount],records[i]->payload, records[i]->payload_length);
                algorithmCount += 1;
            }
            if (!strncmp(records[i]->record_type, ThermostaticRTD, records[i]->type_length))
            {
                memcpy(&config->_algorithms[algorithmCount],records[i]->payload, records[i]->payload_length);
                algorithmCount += 1;
            }
        }
    }
    config->_algorithmCount = algorithmCount;
}

void print_configuration(struct MazerConfiguration * config)
{
    printf("Algorithms: %d\n", config->_algorithmCount);
    printf("Configuration: \n");

    for(int i = 0; i < config->_algorithmCount; ++i)
    {
        switch (config->_algorithms[i].type)
        {
            case None:
                printf("None - setpoint: %f Kp: %f Ki: %f Kd: %f\n",
                        config->_algorithms[i].set_point,
                        config->_algorithms[i].param1,
                        config->_algorithms[i].param2,
                        config->_algorithms[i].param3);
                break;
            case PID:
                printf("PID - setpoint: %f Kp: %f Ki: %f Kd: %f\n",
                        config->_algorithms[i].set_point,
                        config->_algorithms[i].param1,
                        config->_algorithms[i].param2,
                        config->_algorithms[i].param3);
                break;
            case AutoPID:
                printf("AutoPID - setpoint: %f\n", config->_algorithms[i].set_point);
                break;
            case Thermostatic:
                printf("Thermostatic - setpoint: %f dTh: %f dTl: %f Reserved: %f\n",
                        config->_algorithms[i].set_point,
                        config->_algorithms[i].param1,
                        config->_algorithms[i].param2,
                        config->_algorithms[i].param3);
                break;
        }
    }
}

