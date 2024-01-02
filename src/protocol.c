#include "../inc/protocol.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../inc/tuple_space.h"

uint8_t *copy_str_to_buff(uint8_t *dst, char *src)
{
	int str_len = strlen(src);
	for (int i = 0; i < str_len; i++)
	{
		*dst = *src;
		++dst;
		++src;
	}
	return dst;
}

void *copy_value_to_buff(void *dst, void *src, int size)
{
	memcpy(dst, src, size);
	return dst + size;
}

void *copy_value_from_buff(void *dst, void *src, int size)
{
	memcpy(dst, src, size);
	return src + size;
}

uint8_t *copy_fields_to_buff(uint8_t *dst, Tuple *tuple)
{
	uint8_t *buff_ptr = dst;
	for (size_t i = 0; i < tuple->fields_size; i++)
	{
		field_t *current_field = &(tuple->fields)[i];
		buff_ptr = copy_value_to_buff(buff_ptr, &(current_field->is_actual), 1);
		buff_ptr = copy_field_type_and_data(buff_ptr, current_field);
	}
	return buff_ptr;
}

uint8_t *copy_field_type_and_data(uint8_t *dst, field_t *field)
{
	switch (field->type)
	{
	case TS_INT:
		int int_size = 4;
		dst = copy_value_to_buff(dst, &int_size, 1);
		dst = copy_value_to_buff(dst, &(field->data.int_field), int_size);
		return dst;
	case TS_FLOAT:
		int float_size = 8;
		dst = copy_value_to_buff(dst, &float_size, 1);
		dst = copy_value_to_buff(dst, &(field->data.float_field), float_size);
		return dst;
	case TS_STRING:
		int string_len = strlen(field->data.string_field);
		dst = copy_value_to_buff(dst, &string_len, 1);
		dst = copy_str_to_buff(dst, field->data.string_field);
		return dst;
	}
}

uint8_t *serialize_tuple(Tuple *tuple, int message_type)
{
	uint8_t *buff = malloc(256 * sizeof(uint8_t));
	uint8_t *buff_ptr = buff;
	buff_ptr = copy_value_to_buff(buff_ptr, &message_type, 1);
	int tuple_name_len = strlen(tuple->name);
	buff_ptr = copy_value_to_buff(buff_ptr, &tuple_name_len, 2);
	buff_ptr = copy_str_to_buff(buff_ptr, tuple->name);
	buff_ptr = copy_value_to_buff(buff_ptr, &(tuple->fields_size), 1);
	buff_ptr = copy_fields_to_buff(buff_ptr, tuple);
	printf("serialized size %d \n", buff_ptr - buff);
	return buff;
}

uint8_t *copy_fields_from_buff(Tuple *tuple_dst, uint8_t *buff_src)
{
	uint8_t *buff_ptr = buff_src;
	tuple_dst->fields = malloc(tuple_dst->fields_size * sizeof(field_t));

	for (int i = 0; i < tuple_dst->fields_size; i++)
	{
		field_t *field = &(tuple_dst->fields[i]);

		buff_ptr = copy_value_from_buff(&(field->is_actual), buff_ptr, 1);
		int data_field_len = 0;
		buff_ptr = copy_value_from_buff(&data_field_len, buff_ptr, 1);

		switch (data_field_len)
		{
		case 4:
			field->type = TS_INT;
			buff_ptr = copy_value_from_buff(&(field->data.int_field), buff_ptr, data_field_len);
			break;
		case 8:
			field->type = TS_FLOAT;
			buff_ptr = copy_value_from_buff(&(field->data.float_field), buff_ptr, data_field_len);
			break;
		default:
			field->type = TS_STRING;
			field->data.string_field = malloc(data_field_len);
			buff_ptr = copy_value_from_buff(field->data.string_field, buff_ptr, data_field_len);
			break;
		}
	}
	return buff_ptr;
}

Tuple *deserialize_tuple(uint8_t *buff_src)
{
	uint8_t *buff_ptr = buff_src;
	Tuple *tuple = malloc(sizeof(Tuple));
	int message_type = 0;
	buff_ptr = copy_value_from_buff(&message_type, buff_ptr, 1);
	int tuple_name_len = 0;
	buff_ptr = copy_value_from_buff(&tuple_name_len, buff_ptr, 2);
	tuple->name = malloc(tuple_name_len);
	buff_ptr = copy_value_from_buff(tuple->name, buff_ptr, tuple_name_len);
	buff_ptr = copy_value_from_buff(&(tuple->fields_size), buff_ptr, 1);
	buff_ptr = copy_fields_from_buff(tuple, buff_ptr);
	return tuple;
}

// 3 | e 0 | 6e 69 63 65 5f 63 6f 6e 73 74 61 6e 74 73 | 3 | 0 | 4 | 64 0 0 0 | 1 | 8 | 8f c2 31 41 0 0 0 0 | 1 | b | 74 6f 20 6a 65 73 74 20 73 74 72
// zalozenie ze jak jest string to nie moze miec 4 lub 8 bajtow dlugosci, bo nie ma jak tego odroznic czy to int czy float
