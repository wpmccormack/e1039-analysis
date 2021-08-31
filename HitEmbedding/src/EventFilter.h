#ifndef _EVENT_FILTER__H_
#define _EVENT_FILTER__H_
#include <fun4all/SubsysReco.h>
class SQEvent;

/// An example class to filter events.
/** 
 * By default it accepts NIM3 (i.e. random RF) events.
 */
class EventFilter: public SubsysReco {
  SQEvent* mi_evt;

 public:
  EventFilter();
  virtual ~EventFilter() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);
};

#endif /* _EVENT_FILTER__H_ */
