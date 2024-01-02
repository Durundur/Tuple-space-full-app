#include "../inc/tuple_space.h"
#include <stdio.h>
#include <stdlib.h>
#include "../inc/server.h"

int main(void)
{
	// ts_init();
	// int nice_power;
	// double temp, pi;
	// field_t my_tuple[2];
	// field_t my_template[2];
	// my_tuple[0].is_actual = TS_YES;
	// my_tuple[0].type = TS_INT;
	// my_tuple[0].data.int_field = 120;
	// my_tuple[1].is_actual = TS_YES;
	// my_tuple[1].type = TS_FLOAT;
	// my_tuple[1].data.float_field = 3.14;
	// ts_out("nice_constants", my_tuple, 2);
	// my_tuple[0].is_actual = TS_YES;
	// my_tuple[0].type = TS_FLOAT;
	// my_tuple[0].data.float_field = 1.100000;
	// my_tuple[1].is_actual = TS_YES;
	// my_tuple[1].type = TS_FLOAT;
	// my_tuple[1].data.float_field = 333.4;
	// ts_out("nice_constants", my_tuple, 2);
	// my_tuple[0].is_actual = TS_YES;
	// my_tuple[0].type = TS_STRING;
	// my_tuple[0].data.string_field = "to jest str";
	// my_tuple[1].is_actual = TS_YES;
	// my_tuple[1].type = TS_FLOAT;
	// my_tuple[1].data.float_field = 0.4;
	// ts_out("nice_constants", my_tuple, 2);
	// my_tuple[0].is_actual = TS_YES;
	// my_tuple[0].type = TS_INT;
	// my_tuple[0].data.int_field = 128;
	// my_tuple[1].is_actual = TS_YES;
	// my_tuple[1].type = TS_FLOAT;
	// my_tuple[1].data.float_field = 49.99;
	// ts_out("nice_constants", my_tuple, 2);
	// my_tuple[0].is_actual = TS_YES;
	// my_tuple[0].type = TS_INT;
	// my_tuple[0].data.int_field = 33;
	// my_tuple[1].is_actual = TS_YES;
	// my_tuple[1].type = TS_INT;
	// my_tuple[1].data.int_field = 3;
	// ts_out("nice_constants", my_tuple, 2);
	// ts_print();

	// my_template[0].is_actual = TS_NO;
	// my_template[0].type = TS_STRING;
	// my_template[0].data.string_field = malloc(MAX_STRING_SIZE * sizeof(char));
	// my_template[1].is_actual = TS_NO;
	// my_template[1].type = TS_FLOAT;
	// my_template[1].data.float_field = 0.4;
	// Tuple my_t = {"nice_constants", my_template, 2};

	// if (ts_rdp(my_t.name, my_t.fields, my_t.fields_size) == 1)
	// {
	// 	printf("FOUND: \n");
	// 	t_print(&my_t);
	// }

	// if (ts_inp(my_t.name, my_t.fields, my_t.fields_size) == 1)
	// {
	// 	printf("FOUND: \n");
	// 	t_print(&my_t);
	// }
	// ts_print();

	start_udp_server();
	return 0;
}