#include <stdlib.h>
#include <stdio.h>
#include "../inc/header.h"
#include "../inc/protocol.h"
#include <string.h>

int main(void)
{
	printf("%s \n", "start client");
	uint8_t buff[MAX_BUFF];
	field_t template[4];
	template[0].is_actual = TS_NO;
	template[0].type = TS_INT;
	template[1].is_actual = TS_NO;
	template[1].type = TS_INT;
	template[2].is_actual = TS_YES;
	template[2].type = TS_FLOAT;
	template[2].data.float_field = 11.11;
	template[3].is_actual = TS_YES;
	template[3].type = TS_STRING;
	template[3].data.string_field = "witam co to za lwpre";
	Tuple tuple = {"nice_constants", template, 4};
	int res;
	res = serialize_tuple(buff, &tuple, 3);
	if (res != 0)
	{
		Tuple new_tuple;
		memset(&new_tuple, 0, sizeof(Tuple));
		int message_type = 0;
		if (deserialize_tuple(&new_tuple, buff, &message_type) != 0)
		{
			t_print(&new_tuple);
		}
	}
}