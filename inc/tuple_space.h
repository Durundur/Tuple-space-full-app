#ifndef TUPLE_SPACE_H
#define TUPLE_SPACE_H
//
#define TS_YES 1
#define TS_NO 0
//
#define TS_INT 0
#define TS_FLOAT 1
#define TS_STRING 2
//
#define TS_SUCCESS 1
#define TS_FAILURE 0

#define MAX_TUPLE_SPACE 56

typedef struct
{
	int is_actual;
	int type;
	union
	{
		int int_field;
		float float_field;
		char *string_field;
	} data;
} field_t;

typedef struct
{
	char *name;
	field_t *fields;
	int fields_size;
} Tuple;

typedef struct
{
	Tuple *tuples;
	int size;
} TupleSpace;

/* API */
/* these functions return TS_SUCCESS or TS_FAILURE */
/* parameters: tuple name, other fields, no. of other fields */

// Function to add a tuple to the tuple space
int ts_out(char *, field_t *, int);

// Function to retrieve and remove a tuple with a matching template, blocking
int ts_in(char *, field_t *, int);

// Function to retrieve and remove a tuple with a matching template, no blocking
int ts_inp(char *, field_t *, int);

// Function to retrieve and remains a tuple with a matching template, blocking
int ts_rd(char *, field_t *, int);

// Function to retrieve and remains a tuple with a matching template, no blocking
int ts_rdp(char *, field_t *, int);

int ts_add(char *name, field_t *fields, int fields_size);
int ts_get_tuple(char *name, field_t *fields, int fields_size);
int ts_get_tuple_and_remove(char *name, field_t *fields, int fields_size);

void ts_print();
void t_print(Tuple *t);
void ts_init();
int copy_tuple_to_template(char *, field_t *, int, int);
int fields_match(field_t *, field_t *);
int tuple_matches(Tuple *, char *, field_t *, int);
int find_matched_tuple(char *, field_t *, int, int *);
int delete_tupe_from_tuple_space(int);
#endif
