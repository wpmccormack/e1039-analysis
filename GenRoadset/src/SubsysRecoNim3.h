#ifndef _SUBSYS_RECO_NIM3__H_
#define _SUBSYS_RECO_NIM3__H_
#include <vector>
#include <fun4all/SubsysReco.h>
#include "TreeData.h"
class TFile;
class TTree;
class TH1;
class SQEvent;
class SQHitVector;

class SubsysRecoNim3: public SubsysReco {
  /// Input
  SQEvent*     mi_evt;
  SQHitVector* mi_vec_hit;

  /// Output
  TFile* mo_file;
  TTree* mo_tree;
  BgData mo_bg;

  TH1* h1_evt_cnt;

 public:
  SubsysRecoNim3(const std::string &name="SubsysRecoNim3");
  virtual ~SubsysRecoNim3() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

 protected:
  void ExtractHits(const SQHitVector* hit_vec, const std::string det_name, std::vector<int>& list_ele);
  //void FindAllRoads(const SQHitVector* hv, const int tb, std::vector<int>& roads, int* n_hit);
};

#endif // _SUBSYS_RECO_NIM3__H_
