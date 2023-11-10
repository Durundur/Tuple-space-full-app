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

void start_udp_client();

#define SERVER_PORT "4000"
#define SERVER_ADDR "192.168.56.103"
#define MAX_BUFF 256

void start_udp_client()
{
	struct addrinfo h, c, *r = NULL;
	int s, c_len = sizeof(c);
	char message_buff[MAX_BUFF];
	memset(&h, 0, sizeof(struct addrinfo));
	h.ai_family = PF_INET;
	h.ai_socktype = SOCK_DGRAM;
	int pos;
	if (getaddrinfo(SERVER_ADDR, SERVER_PORT, &h, &r) != 0)
	{
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		exit(-1);
	}
	if ((s = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1)
	{

		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		exit(-1);
	}
	sprintf(message_buff, "hello soe, adas d aseqw");
	pos = sendto(s, message_buff, sizeof(message_buff), 0, r->ai_addr, r->ai_addrlen);
	if (pos < 0)
	{
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
	}
	for (;;)
	{
		if ((pos = recvfrom(s, &message_buff, MAX_BUFF, 0, (struct sockaddr *)&c, &c_len)) < 0)
		{
			printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
			continue;
		}
		message_buff[pos] = '\0';
		printf("Recv: %s\n", message_buff);
	}
	freeaddrinfo(r);
	close(s);
}
