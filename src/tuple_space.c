#include "../inc/tuple_space.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static TupleSpace tuple_space;

int ts_out(char *name, field_t *fields, int fields_size)
{
	
}

int ts_rdp(char *name, field_t *fields, int fields_size)
{
	
}

int ts_inp(char *name, field_t *fields, int fields_size)
{
	
}


int ts_in(char *name, field_t *fields, int fields_size)
{
	
}


int ts_rd(char *name, field_t *fields, int fields_size)
{
	
}

int ts_add(char *name, field_t *fields, int fields_size)
{
	if (fields_size > 0 && tuple_space.size < MAX_TUPLE_SPACE)
	{

		Tuple *new_tuple = &tuple_space.tuples[tuple_space.size];
		new_tuple->name = name;
		// if (new_tuple->name == NULL)
		// {
		// 	printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		// 	exit(-1);
		// }
		new_tuple->fields = fields;
		// if (new_tuple->fields == NULL)
		// {
		// 	printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		// 	exit(-1);
		// }
		// memcpy(new_tuple->fields, fields, fields_size * sizeof(field_t));
		new_tuple->fields_size = fields_size;
		tuple_space.size++;
		return TS_SUCCESS;
	}
	return TS_FAILURE;
}

int ts_get_tuple(char *name, field_t *fields, int fields_size){
	int found_index;
	if (find_matched_tuple(name, fields, fields_size, &found_index) == TS_SUCCESS)
	{
		copy_tuple_to_template(name, fields, fields_size, found_index);
		return TS_SUCCESS;
	}
	return TS_FAILURE;
}
int ts_get_tuple_and_remove(char *name, field_t *fields, int fields_size){
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
	printf("  Name: %s\n", t->name);
	printf("  Fields:\n");
	for (int j = 0; j < t->fields_size; ++j)
	{
		printf("    Field %d:\n", j + 1);
		printf("      Is Actual: %s\n", t->fields[j].is_actual ? "YES" : "NO");
		printf("      Type: %d\n", t->fields[j].type);
		if (t->fields[j].type == TS_INT)
		{
			printf("      Data (Int): %d\n", t->fields[j].data.int_field);
		}
		else if (t->fields[j].type == TS_FLOAT)
		{
			printf("      Data (Float): %f\n", t->fields[j].data.float_field);
		}
		else if (t->fields[j].type == TS_STRING)
		{
			printf("      Data (String): %s\n", t->fields[j].data.string_field);
		}
	}
}

void ts_print()
{
	printf("Tuple Space:\n");
	for (int i = 0; i < tuple_space.size; ++i)
	{
		Tuple *t = &tuple_space.tuples[i];
		printf("Tuple %d:\n", i + 1);
		t_print(t);
		printf("\n");
	}
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
			}
			if ((fields + i)->type == TS_FLOAT)
			{
				(fields + i)->data.float_field = ((t->fields) + i)->data.float_field;
			}
			if ((fields + i)->type == TS_STRING)
			{
				strcpy((fields + i)->data.string_field, ((t->fields) + i)->data.string_field);
			}
		}
	}
}

int fields_match(field_t *template, field_t *field)
{
	if (template->type == field->type)
	{
		if (template->is_actual == TS_YES && field->is_actual == TS_YES)
		{
			if (template->type == TS_INT)
			{
				if (template->data.int_field == field->data.int_field)
				{
					return TS_SUCCESS;
				}
				return TS_FAILURE;
			}
			else if (template->type == TS_FLOAT)
			{
				if (template->data.float_field == field->data.float_field)
				{
					return TS_SUCCESS;
				}
				return TS_FAILURE;
			}
			else if (template->type == TS_STRING)
			{
				if (strcmp(template->data.string_field, field->data.string_field) == 0)
				{
					return TS_SUCCESS;
				}
				return TS_FAILURE;
			}
		}
		else if (template->is_actual == TS_FAILURE)
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
		int compare_sum = 0;
		for (int i = 0; i < fields_size; i++)
		{
			compare_sum += fields_match(fields + i, (t->fields) + i);
		}
		if (compare_sum == fields_size)
		{
			return TS_SUCCESS;
		}
		else
		{
			return TS_FAILURE;
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

int delete_tupe_from_tuple_space(int tuple_index){
	if(tuple_index < tuple_space.size){
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
