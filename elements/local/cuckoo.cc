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
  __uint128_t t = p->ip_header()->ip_src.s_addr;
  t = (t << 32) | p->ip_header()->ip_dst.s_addr;
  t = (t << 32) | p->tcp_header()->th_sport;
  t = (t << 32) | p->tcp_header()->th_dport;

  switch(i)
  {
    case 0:
      if(filter.Contain(t) == cuckoofilter::Ok) output(0).push(p);
      else output(1).push(p);
      break;

    case 1:
      filter.Add(t);
      output(1).push(p);
      break;

    case 2:
      filter.Delete(t);
      output(1).push(p);
      break;
  }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(CuckooElement)
