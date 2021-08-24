#ifndef _TREE_DATA__H_
#define _TREE_DATA__H_
#include <phool/PHObject.h>
#include <TLorentzVector.h>

struct EventData {
  int event_id;
  int trig_bits;
  int rec_stat;
  double weight;
  int n_h1x;
  int n_h2x;
  int n_h3x;
  int n_h4x;

  EventData();
  virtual ~EventData() {;}

  ClassDef(EventData, 1);
};

struct TrackData {
  int            charge;
  TVector3       pos_vtx;
  TLorentzVector mom_vtx;

  TrackData();
  virtual ~TrackData() {;}

  ClassDef(TrackData, 1);
};

struct DimuonData {
  TVector3       pos;
  TLorentzVector mom;
  TLorentzVector mom_pos;
  TLorentzVector mom_neg;

  DimuonData();
  virtual ~DimuonData() {;}

  ClassDef(DimuonData, 1);
};

typedef std::vector<TrackData > TrackList;
typedef std::vector<DimuonData> DimuonList;

#endif /* _TREE_DATA__H_ */
