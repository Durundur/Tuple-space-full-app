#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../inc/header.h"
#include "../inc/server.h"
#include "../inc/protocol.h"

void thread_task(void *args)
{
	thread_task_args *a = (thread_task_args *)args;
	Tuple tuple;
	memset(&tuple, 0, sizeof(Tuple));
	int message_type = 0;
	int version = 0;
	int request_len = deserialize_message(&tuple, a->buff, &version, &message_type);
	if (request_len > 0)
	{
		t_print(&tuple);
		int response_len = process_request(&tuple, a->buff, message_type);
		if (response_len > 0)
		{
			int pos = sendto(a->s, a->buff, response_len, 0, (const struct sockaddr *)a->c, sizeof(*(a->c)));
			if (pos < 0)
			{
				printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
			}
		}
	}
	free(a->c);
	free(a->buff);
	free(a);
	free(tuple.fields);
	free(tuple.name);
	pthread_exit(0);
}

int process_request(Tuple *tuple, uint8_t *buff, int message_type)
{
	switch (message_type)
	{
	case PROTOCOL_TS_OUT_MESSAGE:
		if (ts_add(tuple->name, tuple->fields, tuple->fields_size) == TS_SUCCESS)
		{
			ts_print();
			return serialize_short_message(buff, 1, PROTOCOL_TS_SUCCESS);
		}
		else
		{
			printf("ERROR while adding new tuple (%s:%d)\n", __FILE__, __LINE__);
			return serialize_short_message(buff, 1, PROTOCOL_TS_FAILURE);
		}
	case PROTOCOL_TS_IN_MESSAGE:
		while (1)
		{
			if (ts_get_tuple_and_remove(tuple->name, tuple->fields, tuple->fields_size) == TS_SUCCESS)
			{
				int serialize_len = serialize_message(buff, tuple, 1, PROTOCOL_TS_SUCCESS);
				if (serialize_len > 0)
				{
					ts_print();
					return serialize_len;
				}
				return serialize_short_message(buff, 1, PROTOCOL_TS_FAILURE);
			}
		}

	case PROTOCOL_TS_INP_MESSAGE:
		if (ts_get_tuple_and_remove(tuple->name, tuple->fields, tuple->fields_size) == TS_SUCCESS)
		{
			int serialize_len = serialize_message(buff, tuple, 1, PROTOCOL_TS_SUCCESS);
			if (serialize_len > 0)
			{
				ts_print();
				return serialize_len;
			}
		}
		return serialize_short_message(buff, 1, PROTOCOL_TS_FAILURE);

	case PROTOCOL_TS_RD_MESSAGE:
		while (1)
		{
			if (ts_get_tuple(tuple->name, tuple->fields, tuple->fields_size) == TS_SUCCESS)
			{
				int serialize_len = serialize_message(buff, tuple, 1, PROTOCOL_TS_SUCCESS);
				if (serialize_len > 0)
				{
					ts_print();
					return serialize_len;
				}
				return serialize_short_message(buff, 1, PROTOCOL_TS_FAILURE);
			}
		}

	case PROTOCOL_TS_RDP_MESSAGE:
		if (ts_get_tuple(tuple->name, tuple->fields, tuple->fields_size) == TS_SUCCESS)
		{
			int serialize_len = serialize_message(buff, tuple, 1, PROTOCOL_TS_SUCCESS);
			if (serialize_len > 0)
			{
				ts_print();
				return serialize_len;
			}
		}
		return serialize_short_message(buff, 1, PROTOCOL_TS_FAILURE);
	}
}

int main()
{
	ts_init();
	unsigned char recv_buff[MAX_BUFF];
	pthread_t thread_id;
	struct addrinfo hints, *r = NULL;
	struct sockaddr_in c;
	int s, c_len = sizeof(c);
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	int pos;
	if (getaddrinfo(NULL, SERVER_PORT, &hints, &r) != 0)
	{
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		exit(-1);
	}
	if ((s = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1)
	{
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		exit(-2);
	}
	if (bind(s, r->ai_addr, r->ai_addrlen) != 0)
	{
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		exit(-3);
	}
	for (;;)
	{
		if ((pos = recvfrom(s, recv_buff, MAX_BUFF, 0, (struct sockaddr *)&c, &c_len)) < 0)
		{
			printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
			continue;
		}
		printf("Received new request (%d bytes) \n", pos);
		thread_task_args *args = malloc(sizeof(thread_task_args));
		args->s = s;
		args->c = malloc(sizeof(struct sockaddr_in));
		memcpy(args->c, &c, sizeof(struct sockaddr_in));
		args->buff = calloc(MAX_BUFF, sizeof(uint8_t));
		memcpy(args->buff, recv_buff, pos);
		if (pthread_create(&thread_id, NULL, (void *)thread_task, args) != 0)
		{
			printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
			continue;
		}
		memset(recv_buff, 0, MAX_BUFF);
	}
	freeaddrinfo(r);
	close(s);
}