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

void start_udp_server();
void thread_task(void *);

#define SERVER_PORT "4000"
#define MAX_BUFF 256
#define MAX_THREADS 100

typedef struct
{
	int s;
	struct sockaddr_in *c;
	char recv_buff[MAX_BUFF];
} thread_task_args;

void thread_task(void *args)
{
	printf("%s\n", "new thread");
	thread_task_args *a = (thread_task_args *)args;
	printf("recv: %s \n", a->recv_buff);
	int pos = sendto(a->s, a->recv_buff, strlen(a->recv_buff), 0, (const struct sockaddr *)a->c, sizeof(*(a->c)));
	if (pos < 0)
	{
		printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
	}
	free(a->c);
	free(a);
	pthread_exit(0);
}

void start_udp_server()
{
	char recv_buff[MAX_BUFF];
	pthread_t thread_id;
	struct addrinfo h, *r = NULL;
	struct sockaddr_in c;
	int s, c_len = sizeof(c);
	memset(&h, 0, sizeof(struct addrinfo));
	h.ai_family = PF_INET;
	h.ai_socktype = SOCK_DGRAM;
	h.ai_flags = AI_PASSIVE;
	int pos;
	if (getaddrinfo(NULL, SERVER_PORT, &h, &r) != 0)
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
		if ((pos = recvfrom(s, &recv_buff, MAX_BUFF, 0, (struct sockaddr *)&c, &c_len)) < 0)
		{
			printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
			continue;
		}
		recv_buff[pos] = '\0';
		thread_task_args *args = malloc(sizeof(thread_task_args));
		args->s = s;
		args->c = malloc(sizeof(struct sockaddr_in));
		memcpy(args->c, &c, sizeof(struct sockaddr_in));
		strcpy(args->recv_buff, recv_buff);
		if (pthread_create(&thread_id, NULL, thread_task, args) != 0)
		{
			printf("ERROR: %s (%s:%d)\n", strerror(errno), __FILE__, __LINE__);
		}
	}
	freeaddrinfo(r);
	close(s);
}