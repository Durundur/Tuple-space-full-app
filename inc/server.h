#ifndef SERVER_H
#define SERVER_H
#include "./header.h"
#include "./tuple_space.h"

void thread_task(void *args);
int process_request(Tuple *tuple, uint8_t *buff, int message_type);

typedef struct
{
	int s;
	struct sockaddr_in *c;
	unsigned char *buff;
} thread_task_args;

#endif
