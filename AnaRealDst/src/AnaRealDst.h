#ifndef _ANA_REAL_DST__H_
#define _ANA_REAL_DST__H_
#include <fun4all/SubsysReco.h>
class TFile;
class TNtuple;
class TH1;
class TH2;

class AnaRealDst: public SubsysReco {
  TFile* f_out;
  TNtuple* nt;
  TH1* h1_nhit_h1x;
  TH1* h1_nhit_h1y;
  TH1* h1_nhit_h2x;
  TH1* h1_nhit_h2y;

 public:
  AnaRealDst() {;}
  virtual ~AnaRealDst();
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);
};

#endif /* _ANA_REAL_DST__H_ */
