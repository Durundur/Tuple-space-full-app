#include <stdlib.h>
#include <stdio.h>
#include "../inc/header.h"
#include "../inc/protocol.h"
#include <string.h>

int main(void)
{
	ts_init();
	printf("%s \n", "start client");
	uint8_t buff[MAX_BUFF];
	field_t template[2];
	template[0].is_actual = TS_YES;
	template[0].type = TS_INT;
	template[0].data.int_field = 100;
	template[1].is_actual = TS_YES;
	template[1].type = TS_FLOAT;
	template[1].data.float_field = 1.11;
	ts_add("is prime", template, 2);
	template[0].is_actual = TS_YES;
	template[0].type = TS_INT;
	template[0].data.int_field = 44;
	template[1].is_actual = TS_YES;
	template[1].type = TS_FLOAT;
	template[1].data.float_field = 41.41;
	ts_add("is prime", template, 2);

	template[0].is_actual = TS_NO;
	template[0].type = TS_INT;
	template[1].is_actual = TS_NO;
	template[1].type = TS_FLOAT;
	Tuple t = {"is prime", template, 2};
	ts_print();
	ts_get_tuple_and_remove(t.name, t.fields, t.fields_size);
	t_print(&t);
	ts_print();
}

// Tuple tuple = {"nice_constants", template, 2};
// int res;
// res = serialize_tuple(buff, &tuple, 3);
// if (res > 0)
// {
// 	printf("serialize: \n");
// 	for (int i = 0; i < res; i++)
// 	{
// 		printf(" %d \n", buff[i]);
// 	}
// 	Tuple new_tuple;
// 	memset(&new_tuple, 0, sizeof(Tuple));
// 	int message_type = 0;
// 	int res2 = deserialize_tuple(&new_tuple, buff, &message_type);
// 	if (res2 > 0)
// 	{
// 		printf("deserialize: \n");
// 		for (int i = 0; i < res2; i++)
// 		{
// 			printf(" %d \n", buff[i]);
// 		}
// 		t_print(&new_tuple);
// 	}
// }