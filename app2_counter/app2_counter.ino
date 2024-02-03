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

#define INTERVAL 7000

byte mac[] = {0x99, 0x11, 0xaa, 0x12, 0x44, 0x56};
unsigned char buff[MAX_BUFF];
ZsutEthernetUDP Udp;
ZsutIPAddress server_ip = ZsutIPAddress(192, 168, 56, 103);

int state = 0;
int from_0_to_1_counter = 0;
int from_1_to_0_counter = 0;
long last_print_time = 0;

void setup()
{
  ZsutEthernet.begin(mac);
  Serial.println(ZsutEthernet.localIP());
  Udp.begin(APP2_COUNTER_PORT);
  ts_init_client(buff, &Udp, &server_ip);
  last_print_time = ZsutMillis();
}

void loop()
{
  delay(2000);
  field_t templ[1];
  if (state == 0)
  {
    templ[0].is_actual = TS_NO;
    templ[0].type = TS_STRING;
    Tuple tuple = {"sensor_change", templ, 1};
    if (ts_inp(tuple.name, tuple.fields, tuple.fields_size) == TS_SUCCESS)
   {
      if (strcmp(tuple.fields[0].data.string_field, "0->1") == 0)
      {
        from_0_to_1_counter++;
      }
      if (strcmp(tuple.fields[0].data.string_field, "1->0") == 0)
      {
        from_1_to_0_counter++;
      }
    }
    if (ZsutMillis() - last_print_time >= INTERVAL)
    {
      last_print_time = ZsutMillis();
      state = 1;
    }
  }

  if (state == 1)
  {
    Serial.print("0->1 changes count: ");
    Serial.println(from_0_to_1_counter);
    Serial.print("1->0 changes count: ");
    Serial.println(from_1_to_0_counter);
    state = 0;
  }
}
