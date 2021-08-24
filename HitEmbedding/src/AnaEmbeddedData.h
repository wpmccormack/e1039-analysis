#ifndef _ANA_EMBEDDED_DATA__H_
#define _ANA_EMBEDDED_DATA__H_
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
class SQHitVector;
class SQTrackVector;
class SQDimuonVector;

/// An example class to analyze the simulated uDST file.
class AnaEmbeddedData: public SubsysReco {
  /// Input
  SQEvent       * mi_evt;
  SQEvent       * mi_evt_emb;
  SRecEvent     * mi_srec;
  SQMCEvent     * mi_evt_mc;
  SQHitVector   * mi_vec_hit;
  SQTrackVector * mi_vec_trk;
  SQDimuonVector* mi_vec_dim;

  /// Output
  TFile*     mo_file;
  TTree*     mo_tree;
  EventData  mo_evt;
  TrackList  mo_trk_true;
  TrackList  mo_trk_reco;
  DimuonList mo_dim_true;
  DimuonList mo_dim_reco;

 public:
  AnaEmbeddedData(const std::string name="AnaEmbeddedData");
  virtual ~AnaEmbeddedData() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

 private:
  void SeparateHits(const SQHitVector* vec_in, std::map<int, SQHitVector*>& vec_sep, const bool in_time=false);

  typedef std::map<int, int> IdMap_t; // For now the key is not ID but index.
  void FindTrackRelation (IdMap_t& id_map);
  void FindDimuonRelation(IdMap_t& id_map);
};

#endif /* _ANA_EMBEDDED_DATA__H_ */
