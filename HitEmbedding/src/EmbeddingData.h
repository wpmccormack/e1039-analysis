#ifndef _EMBEDDING_DATA__H_
#define _EMBEDDING_DATA__H_
#include <phool/PHObject.h>
#include <TLorentzVector.h>

struct EmbEventData {
  int   run_id;
  int   spill_id;
  int   event_id;
  short trig_bits;
  int   turn_id;
  int   rf_id;
  int   rf_inte[33];
  EmbEventData();
  virtual ~EmbEventData() {;}
  ClassDef(EmbEventData, 1);
};

struct EmbHitData {
  int    hit_id;
  int    det_id;
  int    ele_id;
  double tdc_time;
  double drift_dist;
  double pos;
  EmbHitData();
  virtual ~EmbHitData() {;}
  ClassDef(EmbHitData, 1);
};

struct EmbSimEventData {
  double weight;
  EmbSimEventData();
  virtual ~EmbSimEventData() {;}
  ClassDef(EmbSimEventData, 1);
};

struct EmbSimTrackData {
  int            trk_id;
  int            charge;
  TVector3       pos_vtx;
  TVector3       pos_st1;
  TVector3       pos_st3;
  TLorentzVector mom_vtx;
  TLorentzVector mom_st1;
  TLorentzVector mom_st3;
  EmbSimTrackData();
  virtual ~EmbSimTrackData() {;}
  ClassDef(EmbSimTrackData, 1);
};

struct EmbSimDimuonData {
  int            dim_id;
  int            trk_pos_id;
  int            trk_neg_id;
  TVector3       pos;
  TLorentzVector mom_pos;
  TLorentzVector mom_neg;
  EmbSimDimuonData();
  virtual ~EmbSimDimuonData() {;}
  ClassDef(EmbSimDimuonData, 1);
};

typedef std::vector<EmbHitData      > EmbHitList;
typedef std::vector<EmbSimTrackData > EmbSimTrackList;
typedef std::vector<EmbSimDimuonData> EmbSimDimuonList;

#endif // _EMBEDDING_DATA__H_
