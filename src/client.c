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
	uint8_t buff[MAX_BUFF];
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
		buff[pos] = '\0';
		printf("Recv: %s\n", buff);
	}
	freeaddrinfo(server_addr_info);
	close(s);
}
