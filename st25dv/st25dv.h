//
// Created by jqt3o on 8/22/2021.
//

#ifndef PICO_EXAMPLES_ST25DV_H
#define PICO_EXAMPLES_ST25DV_H

#if UNIT_TESTS
#include "../tests/picoMock.h"
#endif

#define ST25DV_DEVICE_SELECT_BYTE(e2) 0x50 | ((e2&0x1)<<2) | 0x3
#define ST25DV_USER_MEMORY_START 0x06

enum TNF {
    Empty = 0x00,
    WellKnown = 0x01,
    MIME_Type = 0x02,
    Absolute_URI = 0x03,
    External_Record = 0x04,
    Unknown_Record = 0x05,
    Unchanged_Record = 0x06
};

struct NDEF_Header {
    enum TNF TNF : 3;
    unsigned int IL : 1;
    unsigned int SR : 1;
    unsigned int CF : 1;
    unsigned int ME : 1;
    unsigned int MB : 1;
};

struct NDEF_Record {
    struct NDEF_Header header;
    uint8_t type_length;
    uint32_t payload_length;
    uint8_t ID_length;
    char* record_type;
    uint8_t ID;
    uint8_t * payload;
};

struct WellKnownPayload {

};

//Initializes the specified pins
void st25dv_init(int sda, int scl);

//Sets the read/write pointer to an address in user space
void st25dv_set_read_pointer(uint16_t addr);

void st25dv_write_records(int count, struct NDEF_Record * records[]);
void st25dv_write_record(struct NDEF_Record * record);
//Reads all the records in the first message of the NFC tag.
//Returns the number of records
int st25dv_read_all_records(struct NDEF_Record * records[]);
//Reads from the current location in user memory
void st25dv_read_current_user(int length, uint8_t * data);
//Reads from the specified location in user memory
void st25dv_read_random_user(uint16_t addr, int length, uint8_t * data);
//Reads a single record from the current location in user memory
int st25dv_read_record(struct NDEF_Record * data);




#endif //PICO_EXAMPLES_ST25DV_H
