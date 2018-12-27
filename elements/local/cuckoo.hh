#ifndef CLICK_CUCKOO_HH
#define CLICK_CUCKOO_HH

#include <click/element.hh>
#include <cuckoofilter/cuckoofilter.h>

CLICK_DECLS

class CuckooElement : public Element { public:
  cuckoofilter::CuckooFilter<uint64_t, 12> filter{1000000};

  CuckooElement() CLICK_COLD;
  const char *class_name() const { return "Cuckoo"; }
  const char *port_count() const { return "3/3"; }
  void push(int, Packet *);
};

CLICK_ENDDECLS
#endif
