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

#define MAX_TUPLE_SPACE 128

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

#ifdef ARDUINO
/* API */
/* these functions return TS_SUCCESS or TS_FAILURE */
/* parameters: tuple name, other fields, no. of other fields */

// Function to add a tuple to the tuple space
int ts_out(char *name, field_t *fields, int fields_size);

// Function to retrieve and remove a tuple with a matching template, blocking
int ts_in(char *name, field_t *fields, int fields_size);

// Function to retrieve and remove a tuple with a matching template, no blocking
int ts_inp(char *name, field_t *fields, int fields_size);

// Function to retrieve and remains a tuple with a matching template, blocking
int ts_rd(char *name, field_t *fields, int fields_size);

// Function to retrieve and remains a tuple with a matching template, no blocking
int ts_rdp(char *name, field_t *fields, int fields_size);

int ts_init_client(unsigned char *buff_dst, ZsutEthernetUDP *Udp, ZsutIPAddress *server_ip);
#endif
const char *get_type_name(int type);

//--------SERVER----------//

int ts_add(char *name, field_t *fields, int fields_size);
int ts_get_tuple(char *name, field_t *fields, int fields_size);
int ts_get_tuple_and_remove(char *name, field_t *fields, int fields_size);
void ts_print(void);
void t_print(Tuple *tuple);
void ts_init(void);
int copy_tuple_to_template(char *name, field_t *fields, int fields_size, int tuple_index_in_tuple_space);
int fields_match(field_t *field1, field_t *field2);
int tuple_matches(Tuple *tuple, char *name, field_t *fields, int fields_size);
int find_matched_tuple(char *name, field_t *fields, int fields_size, int *found_index);
int delete_tupe_from_tuple_space(int tuple_index);
int ts_get_number_of_tuples();
#endif
