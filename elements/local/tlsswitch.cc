#include <click/config.h>

#include <clicknet/tcp.h>

#include "tlsswitch.hh"
CLICK_DECLS

TLSSwitchElement::TLSSwitchElement()
{
}

void TLSSwitchElement::push(int, Packet *p)
{
  const click_tcp *tcp = p->tcp_header();
  uint8_t *tcp_data = (uint8_t *)tcp + (tcp->th_off<<2);

  if(tcp_data >= p->end_data())
  {
    output(2).push(p);
    return;
  }

  switch(*tcp_data)
  {
    case 0x16:
      output(0).push(p);
      break;

    case 0x17:
      output(1).push(p);
      break;

    default:
      output(2).push(p);
  }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TLSSwitchElement)
