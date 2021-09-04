//
// Created by jqt3o on 8/22/2021.
//

#ifndef PICO_EXAMPLES_ST25DV_H
#define PICO_EXAMPLES_ST25DV_H

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
    uint8_t * record_type;
    uint8_t ID;
    uint8_t * payload;
};

struct WellKnownPayload {

};

void st25dv_set_read_pointer(uint8_t addr[2]);
void st25dv_read_current_user(int length, uint8_t * data);
void st25dv_read_random_user(uint8_t addr[2], int length, uint8_t * data);
bool st25dv_read_record(struct NDEF_Record * data);
void st25dv_init(int sda, int scl);


#endif //PICO_EXAMPLES_ST25DV_H
