#include <stdlib.h>
#include <stdio.h>
#include "../inc/header.h"
#include "../inc/protocol.h"

int main(void)
{
	printf("%s \n", "start client");
	// start_udp_client();
	uint8_t buff[MAX_BUFF];

	field_t template[2];
	template[0].is_actual = TS_NO;
	template[0].type = TS_INT;
	template[1].is_actual = TS_NO;
	template[1].type = TS_INT;
	template[2].is_actual = TS_YES;
	template[2].type = TS_FLOAT;
	template[2].data.float_field = 11.11;
	template[3].is_actual = TS_YES;
	template[3].type = TS_STRING;
	template[3].data.string_field = "totototojest str";
	Tuple tuple = {"nice_constants", template, 4};
	uint8_t *tuple_buff = NULL;
	tuple_buff = serialize_tuple(&tuple, 3);
	
	Tuple *tuple_new = deserialize_tuple(tuple_buff);
	t_print(tuple_new);
}