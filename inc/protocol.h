#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "./common.h"
#include "./tuple_space.h"
#include <stdint.h>

#define MAX_NAME_SIZE 64

// ts_out  011 3
// ts_in   100 4
// ts_inp  101 5
// ts_rd   110 6
// ts_rdp  111 7
// int - 4bajty
// float - 8bajtow

uint8_t *serialize_tuple(Tuple *, int);
Tuple *deserialize_tuple(uint8_t *);
uint8_t *copy_str_to_buff(uint8_t *, char *);
void *copy_value_to_buff(void *, void *, int);
void *copy_value_from_buff(void *, void *, int );
uint8_t *copy_fields(uint8_t *, Tuple *);
uint8_t *copy_field_type_and_data(uint8_t *, field_t *);

typedef struct
{
    int name_length;
    char *name;
    int fields_size;
    field_t *fields;
} data_prot_t;

typedef struct
{
    int message_type;
} header_prot_t;

typedef struct
{
    header_prot_t *header;
    data_prot_t *data;
} tuple_protocol_t;
#endif
