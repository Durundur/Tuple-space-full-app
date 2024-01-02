#ifndef SERVER_H
#define SERVER_H

#include "header.h"

typedef struct
{
	int s;
	struct sockaddr_in *c;
	unsigned char recv_buff[MAX_BUFF];
} thread_task_args;

void start_udp_server();
void thread_task(void *);
#endif


