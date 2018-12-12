#ifndef CLICK_ELLIGATOR_HH
#define CLICK_ELLIGATOR_HH

#include <click/element.hh>

CLICK_DECLS

class ElligatorElement : public Element { public:
  ElligatorElement() CLICK_COLD;
  const char *class_name() const { return "Elligator"; }
  const char *port_count() const { return "1/2"; }
  void push(int, Packet *);
};

CLICK_ENDDECLS
#endif
