#ifndef _ANA_REAL_DST__H_
#define _ANA_REAL_DST__H_
#include <fun4all/SubsysReco.h>
class TFile;
class TTree;
class TH1;
class TH2;

/// An example class to analyze the E1039 DST file.
class AnaRealDst: public SubsysReco {
  static const std::vector<std::string> list_det_name;
  std::vector<int> list_det_id;

  TFile* f_out;
  TTree* tree;
  char   b_det_name[16];
  int    b_det;
  int    b_ele;
  double b_time;
  TH1*   h1_ele[99];

 public:
  AnaRealDst() {;}
  virtual ~AnaRealDst() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);
};

#endif /* _ANA_REAL_DST__H_ */
