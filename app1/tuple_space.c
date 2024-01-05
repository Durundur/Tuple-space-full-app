#include "tuple_space.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "protocol.h"
#include "header.h"
#include <Zsut.h>
#include <ZsutClient.h>
#include <ZsutDhcp.h>
#include <ZsutDns.h>
#include <ZsutEthernet.h>
#include <ZsutEthernetClient.h>
#include <ZsutEthernetServer.h>
#include <ZsutEthernetUdp.h>
#include <ZsutFeatures.h>
#include <ZsutIPAddress.h>
#include <ZsutUdp.h>

byte mac[] = {0xaa, 0xbb, 0xcc, 0x12, 0x34, 0x56};
ZsutEthernetUDP Udp;
ZsutIPAddress server_ip = ZsutIPAddress(192, 168, 56, 103);

int ts_client_init(){
	ZsutEthernet.begin(mac);
	Serial.print(F("USED IP:"));
	Serial.println(ZsutEthernet.localIP());
	Udp.begin(APP_1_WORKER_PORT);
}


// Function to add a tuple to the tuple space
int ts_out(char *name, field_t *fields, int fields_size)
{
	
}

// Function to retrieve and remains a tuple with a matching template, no blocking
int ts_rdp(char *name, field_t *fields, int fields_size)
{
	
}

// Function to retrieve and remove a tuple with a matching template, no blocking
int ts_inp(char *name, field_t *fields, int fields_size)
{
	
}

// Function to retrieve and remove a tuple with a matching template, blocking
int ts_in(char *name, field_t *fields, int fields_size)
{
	Tuple tuple = {name, fields, fields_size};
	uint8_t buff = NULL;
	buff = serialize_tuple(&tuple, 3);
	Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
	int r = Udp.write(sendBuffer, len);
	Udp.endPacket();
}

// Function to retrieve and remains a tuple with a matching template, blocking
int ts_rd(char *name, field_t *fields, int fields_size)
{

}


