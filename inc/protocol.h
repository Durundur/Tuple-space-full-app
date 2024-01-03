#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "./common.h"
#include "./tuple_space.h"
#include <stdint.h>
#define MAX_NAME_SIZE 64

Tuple *deserialize_tuple(uint8_t *);
uint8_t *serialize_tuple(Tuple *, int);

void *copy_value_from_buff(void *, void *, int);
void *copy_value_to_buff(void *, void *, int);
uint8_t *copy_fields_from_buff(Tuple *tuple_dst, uint8_t *buff_src);
uint8_t *copy_field_from_buff(field_t *field_dst, uint8_t *buff_src);
uint8_t *copy_fields_to_buff(uint8_t *buff_dst, Tuple *tuple_src);
uint8_t *copy_field_to_buff(uint8_t *buff_dst, field_t *field_src);

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
