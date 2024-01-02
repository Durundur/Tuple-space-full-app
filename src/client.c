#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include "../inc/header.h"
#include "../inc/tuple_space.h"
#include "../inc/protocol.h"

void start_udp_client()
{
	struct addrinfo hints, client_addr_info, *server_addr_info = NULL;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	int s, c_addr_len = sizeof(client_addr_info);
	int pos;
	if (getaddrinfo(SERVER_ADDR, SERVER_PORT, &hints, &server_addr_info) != 0)
	{
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		exit(-1);
	}
	if ((s = socket(server_addr_info->ai_family, server_addr_info->ai_socktype, server_addr_info->ai_protocol)) == -1)
	{
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		exit(-1);
	}

	unsigned char buff[MAX_BUFF];

	// tuple
	field_t template[2];
	template[0].is_actual = TS_NO;
	template[0].type = TS_INT;
	template[0].data.int_field = 100;
	template[1].is_actual = TS_YES;
	template[1].type = TS_FLOAT;
	template[1].data.float_field = 11.11;
	template[2].is_actual = TS_YES;
	template[2].type = TS_STRING;
	template[2].data.string_field = "totototojest str";
	Tuple tuple = {"nice_constants", template, 3};
	uint8_t *tuple_buff = NULL;
	tuple_buff = serialize_tuple(&tuple, 3);

	// for (size_t i = 0; i < 50; i++)
	// {
	// 	printf("%x \n", tuple_buff[i]);
	// }

	Tuple *tuple_new = deserialize_tuple(tuple_buff);
	t_print(tuple_new);
	// printf(" %s \n", tuple_new->name);
	// printf(" %d \n", tuple_new->fields_size);

	pos = sendto(s, (void *)buff, MAX_BUFF, 0, server_addr_info->ai_addr, server_addr_info->ai_addrlen);

	if (pos < 0)
	{
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
	}
	for (;;)
	{
		if ((pos = recvfrom(s, (void *)buff, MAX_BUFF, 0, (struct sockaddr *)&client_addr_info, &c_addr_len)) < 0)
		{
			printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
			continue;
		}
		// message_buff[pos] = '\0';
		// printf("Recv: %s\n", message_buff);
	}
	freeaddrinfo(server_addr_info);
	close(s);
}
