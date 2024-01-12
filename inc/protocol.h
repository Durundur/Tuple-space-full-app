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

#define PROTOCOL_TS_OUT_MESSAGE 2
#define PROTOCOL_TS_IN_MESSAGE 4
#define PROTOCOL_TS_INP_MESSAGE 6
#define PROTOCOL_TS_RD_MESSAGE 8
#define PROTOCOL_TS_RDP_MESSAGE 16

#define PROTOCOL_TS_SUCCESS 1
#define PROTOCOL_TS_FAILURE 3

int serialize_message(uint8_t *buff_dst, Tuple *tuple_src, int version, int message_type);
int deserialize_message(Tuple *tuple_dst, uint8_t *buff_src, int *version, int *message_type);

void set_version_and_payload_type(uint8_t *buff_dst, int version, int payload_type);
void set_payload_size(uint8_t *buff_dst, int payload_size);
void set_is_actual_and_field_type(uint8_t *buff_dst, int field_is_actual, int field_type);
int set_tuple_name_to_buff(uint8_t *buff_dst, char *tuple_name);
void set_field_size(uint8_t *buff_dst, int payload_size);
int set_fields_to_buff(uint8_t *buff_dst, Tuple *tuple_src);
int copy_data(void *dst, void *src, int data_size);

void get_version_and_payload_type(int *version_dst, int *payload_type_dst, const uint8_t *buff_src);
void get_payload_size(int *payload_size_dst, const uint8_t *buff_src);
void get_is_actual_and_field_type(int *field_is_actual_dst, int *field_type_dst, const uint8_t *buff_src);
int get_tuple_name_from_buff(Tuple *tuple_dst, uint8_t *buff_src);
int get_fields_from_buff(Tuple *tuple_dst, uint8_t *buff_src);

int serialize_short_message(uint8_t *buff_dst, int version, int message_type);

#endif
