#include <click/config.h>

#include <clicknet/tcp.h>
#include <cuckoofilter/cuckoofilter.h>

#include "cuckoo.hh"
CLICK_DECLS

CuckooElement::CuckooElement()
{
}

void CuckooElement::push(int i, Packet *p)
{
  uint64_t ip = p->ip_header()->ip_src.s_addr;
  ip = (ip << 32) | p->ip_header()->ip_dst.s_addr;

  uint64_t tcp = p->tcp_header()->th_sport;
  tcp = (tcp << 32) | p->tcp_header()->th_dport;

  tcp = tcp << 16;
  uint64_t t = ip ^ tcp;

  switch(i)
  {
    case 0:
      if(filter.Contain(t) == cuckoofilter::Ok) output(0).push(p);
      else output(1).push(p);
      break;

    case 1:
      filter.Add(t);
      output(2).push(p);
      break;

    case 2:
      filter.Delete(t);
      output(2).push(p);
      break;
  }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CuckooElement)
