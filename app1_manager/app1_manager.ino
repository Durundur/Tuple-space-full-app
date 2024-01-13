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
#include "D:\PSIR\proj\inc\tuple_space.h"
#include "D:\PSIR\proj\src\tuple_space.c"
#include "D:\PSIR\proj\inc\protocol.h"
#include "D:\PSIR\proj\src\protocol.c"
#include "D:\PSIR\proj\inc\header.h"
#include "D:\PSIR\proj\inc\common.h"

byte mac[] = {0x99, 0x11, 0xaa, 0x12, 0x44, 0x56};
unsigned char buff[MAX_BUFF];
ZsutEthernetUDP Udp;
ZsutIPAddress server_ip = ZsutIPAddress(192, 168, 56, 103);

int N = 20;
int state = 0;
int output_counter = 2;
int input_counter = 0;

void setup()
{
  ZsutEthernet.begin(mac);
  Serial.println(ZsutEthernet.localIP());
  Udp.begin(APP1_MANAGER_PORT);
  ts_init_client(buff, &Udp, &server_ip);
}


void loop()
{
  field_t templ[1];
  if(state == 0){
    if(output_counter == N){
      state = 1;
    }
    templ[0].is_actual = TS_YES;
    templ[0].type = TS_INT;
    templ[0].data.int_field = output_counter;
    Tuple tuple = {"check_if_prime", templ, 1};
    if (ts_out(tuple.name, tuple.fields, tuple.fields_size) == TS_FAILURE)
    {    
      Serial.println("TS_OUT Error");
    }
    output_counter++;
  }
  
  if(state == 1){
    if(input_counter == N-1){
      Serial.println("Collected " + String(N-1) + " tuples, closing the process"); 
      exit(1);
    }
    templ[0].is_actual = TS_NO;
    templ[0].type = TS_INT;
    Tuple tuple = {"is_prime", templ, 1};
    if(ts_inp(tuple.name, tuple.fields, tuple.fields_size) == TS_FAILURE){
      Serial.println("TS_INP Error, can't find tuple");
    }
    else{
      t_print(&tuple);
      input_counter++;
    }
    state = 2;
  }

  if(state == 2){
    if(input_counter == N-1){
      Serial.println("Collected " + String(N-1) + " tuples, closing the process"); 
      exit(1);
    }
    templ[0].is_actual = TS_NO;
    templ[0].type = TS_INT;
    Tuple tuple = {"is_not_prime", templ, 1};
    if(ts_inp(tuple.name, tuple.fields, tuple.fields_size) == TS_FAILURE){
      Serial.println("TS_INP Error, can't find tuple");
    }
    else{
      t_print(&tuple);
      input_counter++;
    }
    state = 1;
  }
  delay(3000);
}
