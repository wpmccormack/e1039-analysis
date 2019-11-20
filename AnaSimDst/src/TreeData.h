#ifndef _TREE_DATA__H_
#define _TREE_DATA__H_
#include <phool/PHObject.h>
#include <TLorentzVector.h>

struct EventData {
  int proc_id;
  int par_id[4]; // 2 -> 2
  TLorentzVector par_mom[4];
  int trig_bits;
  int rec_stat;
  int n_dim_reco;

  EventData();
  virtual ~EventData() {;}

  ClassDef(EventData, 1);
};

struct TrackData {
  int   charge;
  float x ;
  float y ;
  float z ;
  float px;
  float py;
  float pz;

  TrackData();
  virtual ~TrackData() {;}

  ClassDef(TrackData, 1);
};

struct DimuonData {
  float x   ;
  float y   ;
  float z   ;
  float px  ;
  float py  ;
  float pz  ;
  float mass;
  float eta ;
  float phi ;
  float x1  ;
  float x2  ;

  float pos_px;
  float pos_py;
  float pos_pz;
  float neg_px;
  float neg_py;
  float neg_pz;

  DimuonData();
  virtual ~DimuonData() {;}

  ClassDef(DimuonData, 1);
};

typedef std::vector<TrackData > TrackList;
typedef std::vector<DimuonData> DimuonList;

#endif /* _TREE_DATA__H_ */
