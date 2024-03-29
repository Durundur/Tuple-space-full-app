#include "../inc/tuple_space.h"
#include "../inc/protocol.h"
#include "../inc/header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static TupleSpace tuple_space;

#ifdef ARDUINO
unsigned char *buff_ptr = NULL;
ZsutEthernetUDP *Udp_ptr = NULL;
ZsutIPAddress *server_ip_ptr = NULL;

int ts_init_client(unsigned char *buff_dst, ZsutEthernetUDP *Udp, ZsutIPAddress *server_ip)
{
	buff_ptr = buff_dst;
	Udp_ptr = Udp;
	server_ip_ptr = server_ip;
}

int ts_send_and_receive(uint8_t *buff, int buff_len, Tuple *tuple_dst)
{
	Udp_ptr->beginPacket(*server_ip_ptr, SERVER_PORT_INT);
	Udp_ptr->write(buff_ptr, buff_len);
	Udp_ptr->endPacket();
	while (1)
	{
		int response_packet = Udp_ptr->parsePacket();
		if (response_packet)
		{
			int response_len = Udp_ptr->read(buff_ptr, MAX_BUFF);
			int message_type = 0;
			int version = 0;
			if (deserialize_message(tuple_dst, buff_ptr, &version, &message_type) > 0)
			{
				if (message_type == PROTOCOL_TS_SUCCESS)
				{
					return TS_SUCCESS;
				}
			}
			return TS_FAILURE;
		}
	}
}

int ts_out(char *name, field_t *fields, int fields_size)
{
	Tuple tuple = {name, fields, fields_size};
	int buff_len = serialize_message(buff_ptr, &tuple, VERSION_NUMBER, PROTOCOL_TS_OUT_MESSAGE);
	if (buff_len > 0)
	{
		if (ts_send_and_receive(buff_ptr, buff_len, &tuple) == TS_SUCCESS)
		{
			return TS_SUCCESS;
		}
	}
	return TS_FAILURE;
}

int ts_rdp(char *name, field_t *fields, int fields_size)
{
	Tuple tuple = {name, fields, fields_size};
	int buff_len = serialize_message(buff_ptr, &tuple, VERSION_NUMBER, PROTOCOL_TS_RDP_MESSAGE);
	if (buff_len > 0)
	{
		if (ts_send_and_receive(buff_ptr, buff_len, &tuple) == TS_SUCCESS)
		{
			memcpy(fields, tuple.fields, tuple.fields_size * sizeof(field_t));
			free(tuple.name);
			free(tuple.fields);
			return TS_SUCCESS;
		}
	}
	return TS_FAILURE;
}

int ts_rd(char *name, field_t *fields, int fields_size)
{
	Tuple tuple = {name, fields, fields_size};
	int buff_len = serialize_message(buff_ptr, &tuple, VERSION_NUMBER, PROTOCOL_TS_RD_MESSAGE);
	if (buff_len > 0)
	{
		if (ts_send_and_receive(buff_ptr, buff_len, &tuple) == TS_SUCCESS)
		{
			memcpy(fields, tuple.fields, tuple.fields_size * sizeof(field_t));
			free(tuple.name);
			free(tuple.fields);
			return TS_SUCCESS;
		}
	}
	return TS_FAILURE;
}

int ts_inp(char *name, field_t *fields, int fields_size)
{
	Tuple tuple = {name, fields, fields_size};
	int buff_len = serialize_message(buff_ptr, &tuple, VERSION_NUMBER, PROTOCOL_TS_INP_MESSAGE);
	if (buff_len > 0)
	{
		if (ts_send_and_receive(buff_ptr, buff_len, &tuple) == TS_SUCCESS)
		{
			memcpy(fields, tuple.fields, tuple.fields_size * sizeof(field_t));
			free(tuple.name);
			free(tuple.fields);
			return TS_SUCCESS;
		}
	}
	return TS_FAILURE;
}

int ts_in(char *name, field_t *fields, int fields_size)
{
	Tuple tuple = {name, fields, fields_size};
	int buff_len = serialize_message(buff_ptr, &tuple, VERSION_NUMBER, PROTOCOL_TS_IN_MESSAGE);
	if (buff_len > 0)
	{
		if (ts_send_and_receive(buff_ptr, buff_len, &tuple) == TS_SUCCESS)
		{
			memcpy(fields, tuple.fields, tuple.fields_size * sizeof(field_t));
			free(tuple.name);
			free(tuple.fields);
			return TS_SUCCESS;
		}
	}
	return TS_FAILURE;
}

#endif

int ts_add(char *name, field_t *fields, int fields_size)
{
	if (fields_size > 0 && tuple_space.size < MAX_TUPLE_SPACE)
	{
		Tuple *new_tuple = &tuple_space.tuples[tuple_space.size];
		new_tuple->name = strdup(name);
		if (new_tuple->name == NULL)
		{
			printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		}
		new_tuple->fields = (field_t *)malloc(fields_size * sizeof(field_t));
		if (new_tuple->fields == NULL)
		{
			printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		}
		memcpy(new_tuple->fields, fields, fields_size * sizeof(field_t));
		new_tuple->fields_size = fields_size;
		tuple_space.size++;
		return TS_SUCCESS;
	}
	return TS_FAILURE;
}

int ts_get_tuple(char *name, field_t *fields, int fields_size)
{
	int found_index;
	if (find_matched_tuple(name, fields, fields_size, &found_index) == TS_SUCCESS)
	{
		copy_tuple_to_template(name, fields, fields_size, found_index);
		return TS_SUCCESS;
	}
	return TS_FAILURE;
}
int ts_get_tuple_and_remove(char *name, field_t *fields, int fields_size)
{
	int found_index;
	if (find_matched_tuple(name, fields, fields_size, &found_index) == TS_SUCCESS)
	{
		copy_tuple_to_template(name, fields, fields_size, found_index);
		delete_tupe_from_tuple_space(found_index);
		return TS_SUCCESS;
	}
	return TS_FAILURE;
}

void t_print(Tuple *t)
{
#ifdef ARDUINO
	Serial.print("Name: " + String(t->name));
	for (int j = 0; j < t->fields_size; ++j)
	{
		Serial.print(" Field " + String(j + 1) + ":");
		Serial.print(" Is Actual: " + String(t->fields[j].is_actual ? "YES" : "NO"));
		Serial.print(" Type: " + String(get_type_name(t->fields[j].type)));
		if (t->fields[j].type == TS_INT && t->fields[j].is_actual)
		{
			Serial.print(" Data: " + String(t->fields[j].data.int_field));
		}
		else if (t->fields[j].type == TS_FLOAT && t->fields[j].is_actual)
		{
			Serial.print(" Data: " + String(t->fields[j].data.float_field));
		}
		else if (t->fields[j].type == TS_STRING && t->fields[j].is_actual)
		{
			Serial.print(" Data: " + String(t->fields[j].data.string_field));
		}
	}
	Serial.println("");
#else
	printf("Name: %s", t->name);
	for (int j = 0; j < t->fields_size; ++j)
	{
		printf(" Field %d:", j + 1);
		printf(" Is Actual: %s", t->fields[j].is_actual ? "YES" : "NO");
		printf(" Type: %s", get_type_name(t->fields[j].type));
		if (t->fields[j].type == TS_INT && t->fields[j].is_actual)
		{
			printf(" Data: %d", t->fields[j].data.int_field);
		}
		else if (t->fields[j].type == TS_FLOAT && t->fields[j].is_actual)
		{
			printf(" Data: %f", t->fields[j].data.float_field);
		}
		else if (t->fields[j].type == TS_STRING && t->fields[j].is_actual)
		{
			printf(" Data: %s", t->fields[j].data.string_field);
		}
	}
	printf("\n");
#endif
}

const char *get_type_name(int type)
{
	switch (type)
	{
	case TS_INT:
		return "TS_INT";
	case TS_FLOAT:
		return "TS_FLOAT";
	case TS_STRING:
		return "TS_STRING";
	default:
		return "Unknown Type";
	}
}

void ts_print()
{
	printf("Tuple Space:\n");
	for (int i = 0; i < tuple_space.size; ++i)
	{
		Tuple *t = &tuple_space.tuples[i];
		printf("Tuple %d:", i + 1);
		t_print(t);
	}
}

int ts_get_number_of_tuples()
{
	return tuple_space.size;
}

void ts_init()
{
	tuple_space.tuples = malloc(MAX_TUPLE_SPACE * sizeof(Tuple));
	tuple_space.size = 0;
}

int copy_tuple_to_template(char *name, field_t *fields, int fields_size, int tuple_index_in_tuple_space)
{
	Tuple *t = &tuple_space.tuples[tuple_index_in_tuple_space];
	for (int i = 0; i < fields_size; i++)
	{
		if ((fields + i)->is_actual == TS_NO)
		{
			if ((fields + i)->type == TS_INT)
			{
				(fields + i)->data.int_field = ((t->fields) + i)->data.int_field;
				(fields + i)->is_actual = TS_YES;
			}
			if ((fields + i)->type == TS_FLOAT)
			{
				(fields + i)->data.float_field = ((t->fields) + i)->data.float_field;
				(fields + i)->is_actual = TS_YES;
			}
			if ((fields + i)->type == TS_STRING)
			{
				free((fields + i)->data.string_field);
				(fields + i)->data.string_field = strdup(((t->fields) + i)->data.string_field);
				(fields + i)->is_actual = TS_YES;
			}
		}
	}
}

int fields_match(field_t *field1, field_t *field2)
{
	if (field1->type == field2->type)
	{
		if (field1->is_actual == TS_YES && field2->is_actual == TS_YES)
		{
			if (field1->type == TS_INT)
			{
				if (field1->data.int_field == field2->data.int_field)
				{
					return TS_SUCCESS;
				}
				return TS_FAILURE;
			}
			else if (field1->type == TS_FLOAT)
			{
				if (field1->data.float_field == field2->data.float_field)
				{
					return TS_SUCCESS;
				}
				return TS_FAILURE;
			}
			else if (field1->type == TS_STRING)
			{
				if (strcmp(field1->data.string_field, field2->data.string_field) == 0)
				{
					return TS_SUCCESS;
				}
				return TS_FAILURE;
			}
		}
		else if (field1->is_actual == TS_NO && field2->is_actual == TS_YES || field1->is_actual == TS_YES && field2->is_actual == TS_NO)
		{
			return TS_SUCCESS;
		}
	}
	return TS_FAILURE;
}

int tuple_matches(Tuple *t, char *name, field_t *fields, int fields_size)
{
	if (strcmp(name, t->name) == 0 && t->fields_size == fields_size)
	{
		for (int i = 0; i < fields_size; i++)
		{
			if (fields_match(fields + i, (t->fields) + i) == 0)
			{
				return TS_FAILURE;
			}
			return TS_SUCCESS;
		}
	}
	return TS_FAILURE;
}

int find_matched_tuple(char *name, field_t *fields, int fields_size, int *found_index)
{
	for (int index = 0; index < tuple_space.size; index++)
	{
		Tuple *t = &tuple_space.tuples[index];
		if (tuple_matches(t, name, fields, fields_size) == TS_SUCCESS)
		{
			*found_index = index;
			return TS_SUCCESS;
		}
	}
	return TS_FAILURE;
}

int delete_tupe_from_tuple_space(int tuple_index)
{
	if (tuple_index < tuple_space.size)
	{
		Tuple *t = &tuple_space.tuples[tuple_index];
		free(t->fields);
		free(t->name);
		if (tuple_index < tuple_space.size - 1)
		{
			tuple_space.tuples[tuple_index] = tuple_space.tuples[tuple_space.size - 1];
		}
		tuple_space.size--;
		return TS_SUCCESS;
	}
	return TS_FAILURE;
}
