#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "./common.h"
#include "./tuple_space.h"
#include <stdint.h>

#define PROTOCOL_INT_SIZE 4
#define PROTOCOL_FLOAT_SIZE 8

#define PROTOCOL_INT_FIELD TS_INT
#define PROTOCOL_FLOAT_FIELD TS_FLOAT
#define PROTOCOL_STRING_FIELD TS_STRING

#define PROTOCOL_TS_OUT_MESSAGE 7
#define PROTOCOL_TS_IN_MESSAGE 8
#define PROTOCOL_TS_INP_MESSAGE 9
#define PROTOCOL_TS_RD_MESSAGE 10
#define PROTOCOL_TS_RDP_MESSAGE 11
#define PROTOCOL_TS_SUCCESS_RESPONSE 17
#define PROTOCOL_TS_FAILURE_RESPONSE 18

int deserialize_tuple(Tuple *tuple_dst, uint8_t *buff_src, int *message_type);
int serialize_tuple(uint8_t *buff_dst, Tuple *tuple_src, int message_type);

int construct_response(uint8_t *buff_dst, int flag);
void *copy_value_from_buff(void *dst, void *src, int size);
void *copy_value_to_buff(void *dst, void *src, int size);
uint8_t *copy_fields_from_buff(Tuple *tuple_dst, uint8_t *buff_src);
uint8_t *copy_field_from_buff(field_t *field_dst, uint8_t *buff_src);
uint8_t *copy_fields_to_buff(uint8_t *buff_dst, Tuple *tuple_src);
uint8_t *copy_field_to_buff(uint8_t *buff_dst, field_t *field_src);

#endif
