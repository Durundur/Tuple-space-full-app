#include "../inc/protocol.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "../inc/header.h"
#include "../inc/tuple_space.h"

// buff
//  3 | e 0 | 6e 69 63 65 5f 63 6f 6e 73 74 61 6e 74 73 | 3 | 0 | 4 | 64 0 0 0 | 1 | 8 | 8f c2 31 41 0 0 0 0 | 1 | b | 74 6f 20 6a 65 73 74 20 73 74 72
//  1.| 2.  | 3.										   | 4.| 5.| 6.| 7.		  | 8.| 9.| 10.	                |11.|12.| 13.
// 1. typ wiadomosci - 1bajt
// 2. dlugosc nazwy - 2bajty
// 3. nazwa
// 4. liczba fieldów - 1bajt
// 5. field is_actual - 1bajt
// 6. dlugosc pola danych w field - 1bajt
// 7. pole danych
// 8. field is_actual - 1bajt
//  ...
//  zalozenie ze jak jest string to nie moze miec 4 lub 8 bajtow dlugosci, bo nie ma jak tego odroznic czy to int czy float

// copy from serialized buff to Tuple
Tuple *deserialize_tuple(uint8_t *buff_src)
{
	Tuple *tuple = malloc(sizeof(Tuple));
	int message_type = 0;
	buff_src = copy_value_from_buff(&message_type, buff_src, 1);
	int tuple_name_len = 0;
	buff_src = copy_value_from_buff(&tuple_name_len, buff_src, 2);
	tuple->name = malloc(tuple_name_len);
	buff_src = copy_value_from_buff(tuple->name, buff_src, tuple_name_len);
	buff_src = copy_value_from_buff(&(tuple->fields_size), buff_src, 1);
	buff_src = copy_fields_from_buff(tuple, buff_src);
	return tuple;
}

// copy from Tuple to buff
uint8_t *serialize_tuple(Tuple *tuple, int message_type)
{
	uint8_t *buff = malloc(MAX_BUFF * sizeof(uint8_t));
	uint8_t *buff_ptr = buff;
	buff_ptr = copy_value_to_buff(buff_ptr, &message_type, 1);
	int tuple_name_len = strlen(tuple->name);
	buff_ptr = copy_value_to_buff(buff_ptr, &tuple_name_len, 2);
	buff_ptr = copy_value_to_buff(buff_ptr, tuple->name, tuple_name_len);
	buff_ptr = copy_value_to_buff(buff_ptr, &(tuple->fields_size), 1);
	buff_ptr = copy_fields_to_buff(buff_ptr, tuple);
	printf("serialized size %d \n", buff_ptr - buff);
	return buff;
}

void *copy_value_from_buff(void *dst, void *src, int size)
{
	memcpy(dst, src, size);
	return src + size;
}

void *copy_value_to_buff(void *dst, void *src, int size)
{
	memcpy(dst, src, size);
	return dst + size;
}

uint8_t *copy_fields_from_buff(Tuple *tuple_dst, uint8_t *buff_src)
{
	tuple_dst->fields = malloc(tuple_dst->fields_size * sizeof(field_t));

	for (int i = 0; i < tuple_dst->fields_size; i++)
	{
		field_t *field = &(tuple_dst->fields[i]);
		buff_src = copy_field_from_buff(field, buff_src);
	}
	return buff_src;
}

uint8_t *copy_field_from_buff(field_t *field_dst, uint8_t *buff_src)
{
	buff_src = copy_value_from_buff(&(field_dst->is_actual), buff_src, 1);
	int data_field_len = 0;
	buff_src = copy_value_from_buff(&data_field_len, buff_src, 1);
	switch (data_field_len)
	{
	case TS_INT_SIZE:
		field_dst->type = TS_INT;
		buff_src = copy_value_from_buff(&(field_dst->data.int_field), buff_src, data_field_len);
		break;
	case TS_FLOAT_SIZE:
		field_dst->type = TS_FLOAT;
		buff_src = copy_value_from_buff(&(field_dst->data.float_field), buff_src, data_field_len);
		break;
	default:
		field_dst->type = TS_STRING;
		field_dst->data.string_field = malloc(data_field_len);
		buff_src = copy_value_from_buff(field_dst->data.string_field, buff_src, data_field_len);
		break;
	}
}

uint8_t *copy_fields_to_buff(uint8_t *buff_dst, Tuple *tuple_src)
{
	for (int i = 0; i < tuple_src->fields_size; i++)
	{
		field_t *field = &(tuple_src->fields)[i];
		buff_dst = copy_field_to_buff(buff_dst, field);
	}
	return buff_dst;
}

uint8_t *copy_field_to_buff(uint8_t *buff_dst, field_t *field_src)
{
	buff_dst = copy_value_to_buff(buff_dst, &(field_src->is_actual), 1);
	switch (field_src->type)
	{
	case TS_INT:
		int int_size = 4;
		buff_dst = copy_value_to_buff(buff_dst, &int_size, 1);
		buff_dst = copy_value_to_buff(buff_dst, &(field_src->data.int_field), int_size);
		return buff_dst;
	case TS_FLOAT:
		int float_size = 8;
		buff_dst = copy_value_to_buff(buff_dst, &float_size, 1);
		buff_dst = copy_value_to_buff(buff_dst, &(field_src->data.float_field), float_size);
		return buff_dst;
	case TS_STRING:
		int string_len = strlen(field_src->data.string_field);
		buff_dst = copy_value_to_buff(buff_dst, &string_len, 1);
		buff_dst = copy_value_to_buff(buff_dst, field_src->data.string_field, string_len);
		return buff_dst;
	}
}
