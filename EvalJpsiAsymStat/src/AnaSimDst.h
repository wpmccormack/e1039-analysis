#ifndef _ANA_SIM_DST__H_
#define _ANA_SIM_DST__H_
#include <map>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <fun4all/SubsysReco.h>
#include "TreeData.h"
class TFile;
class TTree;
class SQEvent;
class SRecEvent;
class SQMCEvent;
class SQTrackVector;
class SQDimuonVector;

/// An example class to analyze the simulated uDST file.
class AnaSimDst: public SubsysReco {
  /// Input
  SQEvent       * mi_evt;
  SRecEvent     * mi_srec;
  SQMCEvent     * mi_evt_true;
  SQTrackVector * mi_vec_trk;
  SQDimuonVector* mi_vec_dim;

  /// Output
  TFile* file;
  TTree* tree;
  EventData  mo_evt;
  TrackList  mo_trk_true;
  TrackList  mo_trk_reco;
  DimuonList mo_dim_true;
  DimuonList mo_dim_reco;

  std::map<int, int> m_proc_id_cnt;
  std::map<int, int> m_part_id_1_cnt;
  std::map<int, int> m_part_id_2_cnt;
  std::map<int, int> m_part_id_3_cnt;
  std::map<int, int> m_part_id_4_cnt;
  std::map<std::string, int> m_part_id_12_cnt;

 public:
  AnaSimDst();
  virtual ~AnaSimDst() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

 private:
  int GetNodes(PHCompositeNode *topNode);
  void MakeTree();

  typedef std::map<int, int> IdMap_t; // For now the key is not ID but index.
  void FindTrackRelation (IdMap_t& id_map);
  void FindDimuonRelation(IdMap_t& id_map);

  void CalcAngle(const TLorentzVector& mu0, const TLorentzVector& mu1, const int pol_sign, double& xb, double& xt, double& phi_s_tf);
};

#endif /* _ANA_SIM_DST__H_ */
