#ifndef CLICK_TLSSWITCH_HH
#define CLICK_TLSSWITCH_HH

#include <click/element.hh>

CLICK_DECLS

class TLSSwitchElement : public Element { public:
  TLSSwitchElement() CLICK_COLD;
  const char *class_name() const { return "TLSSwitch"; }
  const char *port_count() const { return "1/3"; }
  void push(int, Packet *);
};

CLICK_ENDDECLS
#endif
