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

byte mac[] = {0x01, 0x11, 0xaa, 0x12, 0x44, 0x56};
unsigned char buff[MAX_BUFF];
ZsutEthernetUDP Udp;
ZsutIPAddress server_ip = ZsutIPAddress(192, 168, 56, 103);

void setup()
{
  ZsutEthernet.begin(mac);
  Serial.println(ZsutEthernet.localIP());
  Udp.begin(APP1_WORKER_PORT);
  ts_init_client(buff, &Udp, &server_ip);
}


void loop()
{
  field_t templ[1];
  templ[0].is_actual = TS_NO;
  templ[0].type = TS_INT;
  Tuple tuple = {"check_if_prime", templ, 1};
  if (ts_in(tuple.name, tuple.fields, tuple.fields_size) == TS_SUCCESS)
  {    
    if (is_prime(tuple.fields[0].data.int_field) == True)
    {
      tuple.name = "is_prime";
    }
    else
    {
      tuple.name = "is_not_prime";
    }
    if(ts_out(tuple.name, tuple.fields, tuple.fields_size) == TS_FAILURE){
        Serial.println("TS_OUT Error");
    }
  }
  else
  {
    Serial.println("TS_IN Error");
  }
  delay(3000);
}


Boolean is_prime(int n)
{
  if (n == 2 || n == 3)
    return True;

  if (n <= 1 || n % 2 == 0 || n % 3 == 0)
    return False;

  for (int i = 5; i * i <= n; i += 6)
  {
    if (n % i == 0 || n % (i + 2) == 0)
      return False;
  }
  return True;
}
