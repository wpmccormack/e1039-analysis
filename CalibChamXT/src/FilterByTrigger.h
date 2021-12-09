#ifndef _FILTER_BY_TRIGGER__H_
#define _FILTER_BY_TRIGGER__H_
#include <fun4all/SubsysReco.h>
class TFile;
class TH1;
class SQEvent;

/// A SubsysReco module to filter events by trigger.
class FilterByTrigger: public SubsysReco {
  unsigned short m_trig_bits;
  std::string m_fn_out;
  TFile* m_file_out;
  TH1* m_h1_evt_cnt;
  SQEvent* m_evt;

 public:
  FilterByTrigger();
  virtual ~FilterByTrigger();
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

  void SetTriggerBits(const unsigned short trig_bits) { m_trig_bits = trig_bits; }
  void EnableOutput(const std::string fn_out) { m_fn_out = fn_out; }
};

#endif // _FILTER_BY_TRIGGER__H_
