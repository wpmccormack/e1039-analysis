#ifndef _SUBSYS_RECO_FULL_BG__H_
#define _SUBSYS_RECO_FULL_BG__H_
#include <vector>
#include <fun4all/SubsysReco.h>
#include "TreeData.h"
class TFile;
class TTree;
class TH1;
class SQEvent;
class SQHitVector;

class SubsysRecoFullBG: public SubsysReco {
  /// Input
  SQEvent*     mi_evt;
  SQHitVector* mi_vec_hit;

  /// Output
  TFile* mo_file;
  TTree* mo_tree;
  BgData mo_bg;

  TH1*   h1_evt_cnt;

 public:
  SubsysRecoFullBG(const std::string &name="SubsysRecoFullBG");
  virtual ~SubsysRecoFullBG() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

 protected:
  void ExtractHits(const SQHitVector* hit_vec, const std::string det_name, std::vector<int>& list_ele);
};

#endif // _SUBSYS_RECO_FULL_BG__H_
