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
#include "header.h"
#include "tuple_space.h"
#include "protocol.h"
#include "protocol.c"


int get_random_num()
{
  return ZsutMillis() % 17;
}

void setup()
{
  Serial.begin(115200);
  ts_client_init()
}

void loop()
{
  field_t templ[2];
  templ[0].is_actual = TS_NO;
  templ[0].type = TS_INT;
  templ[1].is_actual = TS_NO;
  templ[1].type = TS_INT;
  Tuple tuple = {"check_if_prime", templ, 2};
  
  delay(3000);
}
