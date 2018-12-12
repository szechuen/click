#include <click/config.h>

#include <cstring>
extern "C"
{
  #include <tapdance/elligator2.h>
}

#include "elligator.hh"
CLICK_DECLS

ElligatorElement::ElligatorElement()
{
}

void ElligatorElement::push(int i, Packet *p)
{
  if(p->length() < 256)
  {
    output(1).push(p);
    return;
  }

  unsigned char trailer[240];
  std::strncpy((char *)&trailer, (char *)&p->data()[p->length()-256], 240);

  unsigned char tag[180];

  uint32_t word;
  for(int i=0; i<60; i++)
  {
    word = (trailer[i*4] & 0x3f) << 18;
    word |= (trailer[i*4+1] & 0x3f) << 12;
    word |= (trailer[i*4+2] & 0x3f) << 6;
    word |= trailer[i*4+3] & 0x3f;

    tag[i*3] = (word >> 16) & 0xff;
    tag[i*3+1] = (word >> 8) & 0xff;
    tag[i*3+2] = word & 0xff;
  }

  tag[31] &= 0x7f;

  unsigned char station_privkey[32];
  char out[132];
  unsigned char out_aes[32];

  size_t len = get_payload_from_tag((unsigned char *)&station_privkey, (unsigned char *)&tag, 180, (char *)&out, 132, (unsigned char *)&out_aes);

  if(len) output(0).push(p);
  else output(1).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(ElligatorElement)
ELEMENT_LIBS(-ltapdance -lgmp -L/usr/local/opt/openssl/lib -lssl -lcrypto)
