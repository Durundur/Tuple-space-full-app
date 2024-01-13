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

int last_pin_state = 0;

void setup()
{
  ZsutEthernet.begin(mac);
  Serial.println(ZsutEthernet.localIP());
  Udp.begin(APP2_SENSOR_2_PORT);
  ts_init_client(buff, &Udp, &server_ip);
  ZsutPinMode(ZSUT_PIN_D7, INPUT);
}

void loop()
{
  int new_pin_state = read_pin_D7();
  if(last_pin_state != new_pin_state){
    field_t templ[1];
    templ[0].is_actual = TS_YES;
    templ[0].type = TS_STRING;
    if(new_pin_state == 1){
      templ[0].data.string_field = "0->1";
    }else{
      templ[0].data.string_field = "1->0";
    }
    Tuple tuple = {"sensor_change", templ, 1};
    if(ts_out(tuple.name, tuple.fields, tuple.fields_size) == TS_FAILURE){
      Serial.println("Error: ts_out execution failed"); 
    }
    last_pin_state = new_pin_state;
  }
}

int read_pin_D7(){
  int a = ZsutDigitalRead();
  if((a &= 0x80) == 128) return 1;
  return 0; 
}
