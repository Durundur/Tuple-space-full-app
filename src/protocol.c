#include "../inc/protocol.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../inc/header.h"
#include "../inc/tuple_space.h"
#include <errno.h>

// from Tuple to serialized buff
int serialize_message(uint8_t *buff_dst, Tuple *tuple_src, int version, int message_type)
{
	uint8_t *buff_start_ptr = buff_dst;
	memset(buff_dst, 0, MAX_BUFF);
	set_version_and_payload_type(buff_dst, version, message_type);
	buff_dst++;
	set_payload_size(buff_dst, tuple_src->fields_size + 1);
	buff_dst++;
	buff_dst += set_tuple_name_to_buff(buff_dst, tuple_src->name);
	buff_dst += set_fields_to_buff(buff_dst, tuple_src);
	return buff_dst - buff_start_ptr;
}

// from serialized buff to Tuple
int deserialize_message(Tuple *tuple_dst, uint8_t *buff_src, int *version, int *message_type)
{
	uint8_t *buff_start_ptr = buff_src;
	get_version_and_payload_type(version, message_type, buff_src);
	buff_src++;
	int field_size = 0;
	get_payload_size(&(field_size), buff_src);
	if (field_size == 0)
	{
		return TS_SUCCESS;
	}
	tuple_dst->fields_size = field_size - 1;
	buff_src++;
	buff_src += get_tuple_name_from_buff(tuple_dst, buff_src);
	buff_src += get_fields_from_buff(tuple_dst, buff_src);
	return buff_src - buff_start_ptr;
}

int serialize_short_message(uint8_t *buff_dst, int version, int message_type)
{
	uint8_t *buff_start_ptr = buff_dst;
	memset(buff_dst, 0, MAX_BUFF);
	set_version_and_payload_type(buff_dst, version, message_type);
	buff_dst++;
	set_payload_size(buff_dst, 0);
	buff_dst++;
	return buff_dst - buff_start_ptr;
}

int set_fields_to_buff(uint8_t *buff_dst, Tuple *tuple_src)
{
	uint8_t *buff_start_ptr = buff_dst;
	for (size_t i = 0; i < tuple_src->fields_size; i++)
	{
		set_is_actual_and_field_type(buff_dst, tuple_src->fields[i].is_actual, tuple_src->fields[i].type);
		buff_dst++;
		if (tuple_src->fields[i].is_actual == TS_NO)
		{
			continue;
		}
		switch (tuple_src->fields[i].type)
		{
		case TS_INT:
			set_field_size(buff_dst, PROTOCOL_INT_SIZE);
			buff_dst++;
			buff_dst += copy_data(buff_dst, &(tuple_src->fields[i].data.int_field), PROTOCOL_INT_SIZE);
			break;
		case TS_FLOAT:
			set_field_size(buff_dst, PROTOCOL_FLOAT_SIZE);
			buff_dst++;
			buff_dst += copy_data(buff_dst, &(tuple_src->fields[i].data.float_field), PROTOCOL_FLOAT_SIZE);
			break;
		case TS_STRING:
			set_field_size(buff_dst, strlen(tuple_src->fields[i].data.string_field));
			buff_dst++;
			buff_dst += copy_data(buff_dst, tuple_src->fields[i].data.string_field, strlen(tuple_src->fields[i].data.string_field));
			break;
		}
	}
	return buff_dst - buff_start_ptr;
}

int set_tuple_name_to_buff(uint8_t *buff_dst, char *tuple_name)
{
	uint8_t *buff_start_ptr = buff_dst;
	set_is_actual_and_field_type(buff_dst, TS_YES, TS_STRING);
	buff_dst++;
	set_field_size(buff_dst, strlen(tuple_name));
	buff_dst++;
	buff_dst += copy_data(buff_dst, tuple_name, strlen(tuple_name));
	return buff_dst - buff_start_ptr;
}

int copy_data(void *dst, void *src, int data_size)
{
	memcpy(dst, src, data_size);
	return data_size;
}

void set_is_actual_and_field_type(uint8_t *buff_dst, int field_is_actual, int field_type)
{
	field_is_actual &= 0x01;
	field_type &= 0x7F;
	*buff_dst |= field_is_actual << 7;
	*buff_dst |= field_type;
}

void set_field_size(uint8_t *buff_dst, int payload_size)
{
	payload_size &= 0xFF;
	*buff_dst |= payload_size;
}

void set_version_and_payload_type(uint8_t *buff_dst, int version, int payload_type)
{
	version &= 0x03;
	payload_type &= 0x3F;
	*buff_dst |= version << 6;
	*buff_dst |= payload_type;
}

void set_payload_size(uint8_t *buff_dst, int payload_size)
{
	payload_size &= 0xFF;
	*buff_dst |= payload_size;
}

void get_version_and_payload_type(int *version_dst, int *payload_type_dst, const uint8_t *buff_src)
{
	*version_dst = (*buff_src >> 6) & 0x03;
	*payload_type_dst = *buff_src & 0x3F;
}

void get_payload_size(int *payload_size_dst, const uint8_t *buff_src)
{
	*payload_size_dst = (*buff_src & 0xFF);
}

void get_is_actual_and_field_type(int *field_is_actual_dst, int *field_type_dst, const uint8_t *buff_src)
{
	*field_is_actual_dst = (*buff_src >> 7) & 0x01;
	*field_type_dst = *buff_src & 0x7F;
}

void get_field_size(int *payload_size_dst, const uint8_t *buff_src)
{
	*payload_size_dst = *buff_src & 0xFF;
}

int get_field_data(void *field_data_dst, uint8_t *buff_src, int field_data_size)
{
	memcpy(field_data_dst, buff_src, field_data_size);
	return field_data_size;
}

int get_tuple_name_from_buff(Tuple *tuple_dst, uint8_t *buff_src)
{
	uint8_t *buff_start_ptr = buff_src;
	int is_actual = 0, field_type = 0, name_len = 0;
	get_is_actual_and_field_type(&is_actual, &field_type, buff_src);
	buff_src++;
	get_field_size(&name_len, buff_src);
	buff_src++;
	tuple_dst->name = calloc(name_len, sizeof(unsigned char));
	if (tuple_dst->name == NULL)
	{
#ifdef ARDUINO
		Serial.print("Memory alocation error");
		Serial.print(__LINE__);
		Serial.print(__FILE__);
		Serial.print("\n");
#endif
#ifndef ARDUINO
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
#endif
	}
	buff_src += copy_data(tuple_dst->name, buff_src, name_len);
	return buff_src - buff_start_ptr;
}

int get_fields_from_buff(Tuple *tuple_dst, uint8_t *buff_src)
{
	uint8_t *buff_start_ptr = buff_src;
	tuple_dst->fields = calloc(tuple_dst->fields_size, sizeof(field_t));
	if (tuple_dst->fields == NULL)
	{
#ifdef ARDUINO
		Serial.print("Memory alocation error");
		Serial.print(__LINE__);
		Serial.print(__FILE__);
		Serial.print("\n");
#endif
#ifndef ARDUINO
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
#endif
	}
	for (size_t i = 0; i < tuple_dst->fields_size; i++)
	{
		get_is_actual_and_field_type(&(tuple_dst->fields[i].is_actual), &(tuple_dst->fields[i].type), buff_src);
		buff_src++;
		if (tuple_dst->fields[i].is_actual == TS_NO)
		{
			continue;
		}
		int field_size = 0;
		switch (tuple_dst->fields[i].type)
		{
		case TS_INT:
			get_field_size(&field_size, buff_src);
			buff_src++;
			buff_src += copy_data(&(tuple_dst->fields[i].data.int_field), buff_src, field_size);
			break;
		case TS_FLOAT:
			get_field_size(&field_size, buff_src);
			buff_src++;
			buff_src += copy_data(&(tuple_dst->fields[i].data.float_field), buff_src, field_size);
			break;
		case TS_STRING:
			get_field_size(&field_size, buff_src);
			buff_src++;
			tuple_dst->fields[i].data.string_field = calloc(field_size, sizeof(unsigned char));
			if (tuple_dst->fields[i].data.string_field == NULL)
			{
#ifdef ARDUINO
				Serial.print("Memory alocation error");
				Serial.print(__LINE__);
				Serial.print(__FILE__);
				Serial.print("\n");
#endif
#ifndef ARDUINO
				printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
#endif
			}
			buff_src += copy_data(tuple_dst->fields[i].data.string_field, buff_src, field_size);
			break;
		default:
#ifdef ARDUINO
			Serial.print("Unknown field type");
			Serial.print(__LINE__);
			Serial.print(__FILE__);
			Serial.print("\n");
#endif
#ifndef ARDUINO
			printf("Unknown field type \n");
#endif
		}
	}
	return buff_src - buff_start_ptr;
}
