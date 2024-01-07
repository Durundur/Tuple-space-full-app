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

byte mac[] = {0x01, 0xff, 0xaa, 0x12, 0x34, 0x56};
unsigned char buff[MAX_BUFF];
ZsutEthernetUDP Udp;
ZsutIPAddress server_ip = ZsutIPAddress(192, 168, 56, 103);

int get_random_num()
{
  return ZsutMillis() % 17;
}

void setup()
{
  Serial.begin(115200);
  ZsutEthernet.begin(mac);
  Serial.println(ZsutEthernet.localIP());
  Udp.begin(APP1_MANAGER_PORT);
  ts_init_client(buff, &Udp, &server_ip);
}

void loop()
{
  field_t templ[2];
  templ[0].is_actual = TS_YES;
  templ[0].type = TS_INT;
  templ[0].data.int_field = get_random_num();
  ts_out("check_if_prime\0", templ, 1);
  Serial.println("SENT");
  delay(5000);
}
