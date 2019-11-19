#ifndef _ANA_SIM_DST__H_
#define _ANA_SIM_DST__H_
#include <TVector3.h>
#include <TLorentzVector.h>
#include <fun4all/SubsysReco.h>
class TFile;
class TTree;
class SQEvent;
class SQHitVector;
class PHG4TruthInfoContainer;
class SRecEvent;

struct SQTrack {
  int id;
  int charge;
  int vtx_id;
  int n_hits;
  TVector3 pos_vtx;
  TLorentzVector mom_vtx;
  SQTrack() : id(0), charge(0), vtx_id(0), n_hits(0), pos_vtx(0, 0, 0), mom_vtx(0, 0, 0, 0) {;}
};
typedef std::vector<SQTrack> SQTrackList;

struct SQDimuon {
  int id;
  TVector3 pos;
  TLorentzVector mom;
  TLorentzVector mom_pos;
  TLorentzVector mom_neg;
  int track_idx_pos;
  int track_idx_neg;
  SQDimuon() : id(0), track_idx_pos(0), track_idx_neg(0) {;}
};
typedef std::vector<SQDimuon> SQDimuonList;

/// An example class to analyze the E1039 simulated DST file.
class AnaSimDst: public SubsysReco {
  SQTrackList  m_list_trk;
  SQDimuonList m_list_dim;

  SQEvent* sqevt;
  SQHitVector* sqhit;
  PHG4TruthInfoContainer* g4true;
  SRecEvent  * srec;

  TFile* file;
  TTree* tree;

 public:
  AnaSimDst() {;}
  virtual ~AnaSimDst() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

  void EvalX1X2(const SQDimuon* dim, float& x1, float& x2);

 private:
  /// Variables for tree branch
  int trigger;
  int rec_stat;

  int   trk_n;
  int   trk_ch[99];
  float trk_x [99];
  float trk_y [99];
  float trk_z [99];
  float trk_px[99];
  float trk_py[99];
  float trk_pz[99];
  int   trk_reco_id[99];
  float trk_reco_x [99];
  float trk_reco_y [99];
  float trk_reco_z [99];
  float trk_reco_px[99];
  float trk_reco_py[99];
  float trk_reco_pz[99];

  int   dim_n;
  float dim_x   [99];
  float dim_y   [99];
  float dim_z   [99];
  float dim_px  [99];
  float dim_py  [99];
  float dim_pz  [99];
  float dim_mass[99];
  float dim_eta [99];
  float dim_phi [99];
  float dim_x1  [99];
  float dim_x2  [99];
  int   dim_reco_id[99];
  float dim_reco_x   [99];
  float dim_reco_y   [99];
  float dim_reco_z   [99];
  float dim_reco_px  [99];
  float dim_reco_py  [99];
  float dim_reco_pz  [99];
  float dim_reco_mass[99];
  float dim_reco_eta [99];
  float dim_reco_phi [99];
  float dim_reco_x1  [99];
  float dim_reco_x2  [99];
};

#endif /* _ANA_SIM_DST__H_ */
